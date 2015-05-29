#include "workerrx.h"
#include "uhd/stream.hpp"
#include "uhd/device.hpp"
#include "uhd/utils/thread_priority.hpp"
#include "options.h"
#include <vector>

WorkerRx::WorkerRx(QThread* thread, const Config& cfg) :
    Worker(thread),
    m_config(cfg)
{
    setObjectName(QString("RX"));
    if(cfg.endFrequency < cfg.startFrequency)
        throw QString("end frequency is less than start frequency");
    if(cfg.actualBand <= 0)
        throw QString("band must be > 0");
    if(cfg.signalSpeed <= 0)
        throw QString("signal speed must be > 0");
    if(cfg.stream == nullptr)
        throw QString("no stream to read from");
}


bool WorkerRx::checkMetadataError(const uhd::rx_metadata_t& metadata)
{
    switch(metadata.error_code)
    {
        case uhd::rx_metadata_t::ERROR_CODE_NONE:
           return false;

        case uhd::rx_metadata_t::ERROR_CODE_TIMEOUT:
            emit error(QString("Превышено время ожидания устройства"));
            return true;
        case uhd::rx_metadata_t::ERROR_CODE_OVERFLOW:
            emit error (QString("Промежуточной пропускной способности недостаточно: требуется ") +
                        QString::number(m_config.device->get_rx_rate() * WorkerRx::SAMPLE_SIZE / (1 << 31)) +
                        QString("МБ/с"));
            return true;
        case uhd::rx_metadata_t::ERROR_CODE_LATE_COMMAND:
            emit error (QString("Недостаточно времени на подготовку."));
            return true;
        default:
            emit error (QString("Ошибка") + QString::number(metadata.error_code));
            return true;
    }


    return false;
}

void WorkerRx::timeFix()
{
    while(QTime::currentTime().msec() > 500)
        QThread::msleep(100);

    int nextSecond = QTime::currentTime().msecsSinceStartOfDay() / 1000 + 1;
    m_config.device->set_time_unknown_pps(uhd::time_spec_t::from_ticks(nextSecond, 1));

    QThread::msleep(1100);

    qDebug() << "T " << QTime::currentTime().toString("hh:mm:ss:zzz");
    auto t = QTime::fromMSecsSinceStartOfDay(m_config.device->get_time_now().to_ticks(1000));
    qDebug() << "TU" << t.toString(QString("hh:mm:ss:zzz"));
}

uhd::time_spec_t WorkerRx::doFirstRx()
{  
    timeFix();

    m_config.device->set_rx_freq(m_config.startFrequency);
    uhd::stream_cmd_t cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    cmd.num_samps = m_config.samples;
    cmd.stream_now = false;
    cmd.time_spec = uhd::time_spec_t::from_ticks(m_config.when.msecsSinceStartOfDay() - m_config.extraTicks, 1000);
    m_config.stream->issue_stream_cmd(cmd);

    return cmd.time_spec;
}

uhd::time_spec_t WorkerRx::doRx(double freq, double sec, const uhd::time_spec_t& time)
{
    m_config.device->set_rx_freq(freq);
    uhd::stream_cmd_t cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    cmd.num_samps = m_config.samples;
    cmd.stream_now = false;

    cmd.time_spec = time + uhd::time_spec_t(0, sec);
    m_config.stream->issue_stream_cmd(cmd);

    return cmd.time_spec;
}

void WorkerRx::work()
{
    uhd::set_thread_priority_safe();
    uhd::rx_metadata_t metadata;

    std::vector<void*> buffers = std::vector<void*>(1);
    Samples buffer = new Complex[m_config.samples];
    buffers[0] = buffer;

    double frequency = m_config.startFrequency;
    const double extra = 0.1;
    bool first = true;
    uhd::time_spec_t lastLaunch, newTime;
    while(getActive() && frequency < m_config.endFrequency)
    {
        if(first)
        {
            lastLaunch = doFirstRx();
            first = false;
        }
        else
            lastLaunch = doRx(frequency, extra, newTime);


        auto c = m_config.stream->recv(buffers, m_config.samples, metadata, m_config.timeout, false);

        if(checkMetadataError(metadata) || !c)
            break;

        m_config.buffer->lock();
        m_config.buffer->enqueue(new FFTJob<Complex>(buffer, m_config.samples, frequency));
        m_config.buffer->unlock();

        auto t = QTime::fromMSecsSinceStartOfDay(lastLaunch.to_ticks(1000));
        qDebug() << t.toString(QString("hh:mm:ss:zzz")) << " " << frequency;

        newTime = m_config.device->get_time_now();
        uhd::time_spec_t frequencyFix = newTime - lastLaunch;
        frequency += (frequencyFix.get_real_secs() + extra) * m_config.signalSpeed;
    }

    emit done();
    getThread()->exit(0);
    delete[] buffer;
}

