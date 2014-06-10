#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include "options.h"
#include "workerrx.h"
#include "uhd/device.hpp"
#include "uhd/stream.hpp"

// wrapper for usrp
class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = 0);
    virtual ~Receiver(){}


private:

    WorkerRx* m_worker;
    QThread* m_thread;
    Address m_lastAddress;
    uhd::usrp::multi_usrp::sptr m_device;

    bool m_configured;

    long m_frequency;
    uhd::rx_streamer::sptr m_stream;

    void configure();
    void launch();

signals:
    void dataReceived(Samples* buffer);

public slots:
    void onDataReceived(Samples* buffer);
    void onOptionsUpdated();
    void onStarted();
    void onError(const QString& message);    
};

#endif // RECEIVER_H
