#include "receiver.h"
#include "options.h"


Receiver::Receiver(QObject *parent) :
    QObject(parent)
{
 //  uhd::device_addr_t address(Options::getInstance()->getAddress().toStdString());
 //   m_device = uhd::usrp::multi_usrp::make(address);
}

void Receiver::onDataReceived()
{
    emit dataReceived();
}
