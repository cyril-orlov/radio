#include "workerrx.h"
#include "uhd/stream.hpp"
#include "uhd/device.hpp"
#include "uhd/usrp/multi_usrp.hpp"
#include "options.h"
#include <QVector>

WorkerRx::WorkerRx(QThread *thread) :
    Worker(thread)
{
    QObject::connect(thread, &QThread::started, this, &WorkerRx::work);
    moveToThread(thread);
}

void WorkerRx::work()
{
    std::string addr = Options::getInstance()->getAddress().toStdString();

    try
    {
        long frequency = Options::getInstance()->getFrequency();
        uhd::device_addr_t addressHint;
        addressHint.set(std::string("addr0"), addr);
        uhd::device_addrs_t found = uhd::device::find(addressHint);

        if(found.size() == 0)
        {
            QString message = QString("По адресу <strong>") + Options::getInstance()->getAddress() + QString("</strong> устройств не найдено.");
            emit error(message);
            m_thread->exit(1);
            return;
        }

        uhd::usrp::multi_usrp::sptr device = uhd::usrp::multi_usrp::make(found[0]);

        int bufferSize = Options::getInstance()->getFrequency() / 2;
        device->set_rx_rate(frequency / 2);
        uhd::rx_streamer::sptr stream = device->get_rx_stream
                (uhd::stream_args_t("fc64", "sc16"));


        uhd::rx_metadata_t metadata;

        Samples* data = new Samples(bufferSize);
        QVector<void*> buffers = QVector<void*>();
        for(int i = 0; i != stream->get_num_channels(); i++)
            buffers.push_back(&data->front());
        while(m_active)
        {
            size_t received = stream->recv(buffers, bufferSize, metadata, true);
            if(received == 0)
                break;
            if(metadata.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT)
            {
                showWarning(nullptr, QString("Ошибка"), QString("Превышено время ожидания устройства"));
                break;
            }
            else
                if(metadata.error_code == uhd::rx_metadata_t::ERROR_CODE_OVERFLOW)
                {
                    showWarning(nullptr, QString("Ошибка"),
                                QString("Промежуточной пропускной способности недостаточно: требуется ") +
                                QString::number(device->get_rx_rate() * WorkerRx::SAMPLE_SIZE / 1e6) +
                                QString("МБ/с"));
                    break;
                }

        }

        delete data;
    }
    catch(uhd::exception & e)
    {
        qDebug(e.what());
    }

    m_thread->exit(0);
}

