#include "receiver.h"

#include "uhd/usrp/multi_usrp.hpp"

void showWarning(QWidget * parent, const QString &title, const QString &text);

Receiver::Receiver(QObject *parent) :
    QObject(parent),
    m_thread(nullptr),
    m_worker(nullptr),
    m_configured(false)
{
}

void Receiver::configure()
{
    m_configured = false;
    if(m_thread != nullptr)
    {
        m_thread->terminate();
        m_thread->wait(1000);
        delete m_thread;
        m_thread = nullptr;
    }
    if(m_worker != nullptr)
    {
        delete m_worker;
        m_worker = nullptr;
    }

    std::string addr = Options::getInstance()->getAddress().toStdString();
    uhd::device_addr_t addressHint;
    addressHint.set(std::string("addr0"), addr);
    uhd::device_addrs_t found = uhd::device::find(addressHint);

    if(found.size() == 0)
    {
        onError(QString("По адресу <strong>") +
                Options::getInstance()->getAddress() +
                QString("</strong> устройств не найдено."));
        return;
    }

    long freq = Options::getInstance()->getFrequency();

    m_configured = true;
    m_device = uhd::usrp::multi_usrp::make(found[0]);
    m_device->set_rx_freq(freq);
    m_device->set_rx_rate(freq / 2);
    uhd::time_spec_t start();

    m_thread = new QThread();
    WorkerRx::Config config;
    config.device = m_device;
    config.frequency = freq;
    config.stream = m_device->get_rx_stream(uhd::stream_args_t("fc64", "sc16"));
    config.time = uhd::time_spec_t(Options::getInstance()->getTimeLeft() + 0.1);
    config.timeout = 0.1;

    m_worker = new WorkerRx(config, m_thread);
}

void Receiver::launch()
{
    if(!m_configured)
        return;

    QObject::connect(m_worker, &WorkerRx::error, this, &Receiver::onError);
    QObject::connect(m_worker, &WorkerRx::dataReceived, this, &Receiver::onDataReceived);
    m_thread->start();
}

void Receiver::onDataReceived(Samples* buffer)
{
    emit dataReceived(buffer);
}

void Receiver::onError(const QString& message)
{
    showWarning(nullptr, QString("Ошибка"), message);
}

void Receiver::onStarted()
{
    launch();
}

void Receiver::onOptionsUpdated()
{
    configure();
}
