#include "receiver.h"

#include "uhd/usrp/multi_usrp.hpp"

void showWarning(QWidget * parent, const QString &title, const QString &text);

Receiver::Receiver(QObject *parent) :
    QObject(parent),
    m_thread(nullptr),
    m_worker(nullptr)
{
}

void Receiver::connect()
{
    if(m_thread != nullptr)
    {
        m_thread->terminate();
        m_thread->wait(1000);
        delete m_thread;
    }
    if(m_worker != nullptr)
        delete m_worker;

    m_thread = new QThread();
    m_worker = new WorkerRx(m_thread);
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
    connect();
}

void Receiver::onOptionsUpdated()
{
    QString address = Options::getInstance()->getAddress();
    if(m_lastAddress.compare(address) != 0)
    {
        m_lastAddress = address;
        connect();
    }
}
