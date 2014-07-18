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

    if(m_worker != nullptr)
    {
        m_worker->deactivate();
        delete m_worker;
        m_worker = nullptr;
    }
    if(m_thread != nullptr)
    {
        m_thread->terminate();
        m_thread->wait(1000);
        delete m_thread;
        m_thread = nullptr;
    }

    Address address = Options::getInstance()->getAddress();
    std::string addr = address.toString().toStdString();
    uhd::device_addr_t addressHint;
    addressHint.set(std::string("addr0"), addr);
    uhd::device_addrs_t found = uhd::device::find(addressHint);

    if(found.size() == 0)
    {
        if(m_lastAddress != address) // only nag about new addresses
        {
            onError(QString("По адресу <strong>") +
                    Options::getInstance()->getAddress().toString() +
                    QString("</strong> устройств не найдено."));
            m_lastAddress = address;
        }
        return;
    }

    long freq = Options::getInstance()->getFrequency();
    long band = Options::getInstance()->getBand();

    m_configured = true;
    m_device = uhd::usrp::multi_usrp::make(found[0]);
    m_device->set_rx_freq(freq);
    m_device->set_rx_rate(band);

    m_thread = new QThread();
    WorkerRx::Config config;

    config.device = m_device;
    config.band = band;
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

void Receiver::onDataReceived(Samples buffer, unsigned int count)
{
    emit dataReceived(buffer, count);
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


Receiver::~Receiver()
{
    if (m_worker)
    {
        m_worker->deactivate();
        delete m_worker;
        m_worker = nullptr;
    }
    if(m_thread)
    {
        delete m_thread;
        m_thread = nullptr;
    }
}
