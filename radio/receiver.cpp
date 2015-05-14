#include "receiver.h"

#include "uhd/usrp/multi_usrp.hpp"

void showWarning(QWidget * parent, const QString &title, const QString &text);

Receiver::Receiver(QObject* parent, QTime when, FFTJobManager* dataSource) :
    QObject(parent),
    m_thread(new QThread())
{
    m_worker = new WorkerRx(m_thread, configure(when, dataSource));

    QObject::connect(m_worker, &WorkerRx::error, this, &Receiver::onError);
    QObject::connect(m_worker, &WorkerRx::done, this, &Receiver::done);

    m_thread->start();
}

WorkerRx::Config Receiver::configure(const QTime& when, FFTJobManager* dataSource)
{
    auto o = Options::getInstance();
    WorkerRx::Config config;

    config.buffer = dataSource;
    config.startFrequency = o->getStartFrequency();
    config.endFrequency = o->getEndFrequency();
    config.actualBand = o->calculateActualBand(o->getActualBand(), o->getSignalSpeed());
    config.signalSpeed = o->getSignalSpeed();
    config.timeout = 10.0;

    config.samples = (size_t)
            (o->getBand() * (o->getActualBand() / config.signalSpeed + o->getExtraTicks() / 1000.0));

    Address address = o->getAddress();
    std::string addr = address.toString().toStdString();
    uhd::device_addr_t addressHint;
    addressHint.set(std::string("addr0"), addr);
    uhd::device_addrs_t found = uhd::device::find(addressHint);

    if(found.size() == 0)    
        throw QString("По адресу <strong>") +
                  o->getAddress().toString() +
                  QString("</strong> устройств не найдено.");

    uhd::device_addr_t a = found[0];
    m_device = uhd::usrp::multi_usrp::make(a);
    m_device->set_time_source(QString("external").toStdString());
    //m_device->set_clock_source(QString("external").toStdString());
    m_device->set_rx_rate(o->getBand());
    config.device = m_device;
    config.stream = m_device->get_rx_stream(uhd::stream_args_t("fc64", "sc16"));

    config.when = when;
    config.extraTicks = o->getExtraTicks();

    return config;
}

void Receiver::onError(const QString& message)
{
    qDebug() << "Receiver: " << message;
    throw message;
}

Receiver::~Receiver()
{
    delete m_worker;
    qDebug("Receiver destroyed");
}
