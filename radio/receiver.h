#ifndef RECEIVER_H
#define RECEIVER_H

#include "uhd/usrp/multi_usrp.hpp"

#include <QObject>

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = 0);
    virtual ~Receiver(){}

private:
    uhd::usrp::multi_usrp::sptr m_device;

signals:
    void dataReceived();

public slots:
    void onDataReceived();
};

#endif // RECEIVER_H
