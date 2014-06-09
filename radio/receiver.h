#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include "options.h"
#include "workerrx.h"
#include "uhd/device.hpp"


class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = 0);
    virtual ~Receiver(){}

private:

    WorkerRx* m_worker;
    QThread* m_thread;
    QString m_lastAddress;
    uhd::device::sptr m_device;
    void connect();

signals:
    void dataReceived(Samples* buffer);

public slots:
    void onDataReceived(Samples* buffer);
    void onOptionsUpdated();
    void onStarted();
    void onError(const QString& message);
};

#endif // RECEIVER_H
