#include "workerrx.h"
#include "uhd/stream.hpp"
#include "uhd/device.hpp"
#include "uhd/utils/thread_priority.hpp"
#include "options.h"
#include <QVector>

WorkerRx::WorkerRx(const Config& config, QThread *thread) :
    Worker(thread),
    m_config(config)
{
}

void WorkerRx::setCountdown(size_t bufferSize)
{
    uhd::stream_cmd_t cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    cmd.num_samps = bufferSize;
    cmd.stream_now = false;
    cmd.time_spec = m_config.time;
    m_config.stream->issue_stream_cmd(cmd);
}

bool WorkerRx::checkMetadataError(const uhd::rx_metadata_t& metadata)
{
    if(metadata.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT)
    {
        emit error(QString("Превышено время ожидания устройства"));
        return false;
    }
    else
        if(metadata.error_code == uhd::rx_metadata_t::ERROR_CODE_OVERFLOW)
        {
            emit error (QString("Промежуточной пропускной способности недостаточно: требуется ") +
                        QString::number(m_config.device->get_rx_rate() * WorkerRx::SAMPLE_SIZE / 1e6) +
                        QString("МБ/с"));
            return false;
        }

    return true;
}

void WorkerRx::work()
{
    try
    {
        uhd::set_thread_priority_safe();

        size_t bufferSize = m_config.frequency / 2;
        m_config.device->set_rx_rate(bufferSize);

        setCountdown(bufferSize);

        uhd::rx_metadata_t metadata;
        // emit data received every step
        while(getActive())
        {
            Samples* data = new Samples(bufferSize);
            QVector<void*> buffers = QVector<void*>();
            for(int i = 0; i != m_config.stream->get_num_channels(); i++)
                buffers.push_back(&data->front());

            size_t received = m_config.stream->recv(buffers, bufferSize, metadata, m_config.timeout, false);
            if(received == 0)
                break;
            if(checkMetadataError(metadata))
                break;

            emit dataReceived(data);
        }
    }
    catch(uhd::exception & e)
    {
        qDebug(e.what());
    }

    m_config.stream->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS);

    getThread()->exit(0);
}

