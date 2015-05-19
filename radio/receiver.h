#ifndef RECEIVER_H
#define RECEIVER_H

#include "stdafx.h"
#include <QObject>
#include <QDateTime>
#include "options.h"
#include "workerrx.h"
#include "uhd/device.hpp"
#include "uhd/stream.hpp"

// wrapper for usrp
class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent, QTime when, FFTJobManager *dataSource);
    virtual ~Receiver();

private:
    WorkerRx* m_worker;
    QThread* m_thread;
    uhd::usrp::multi_usrp::sptr m_device;

    WorkerRx::Config configure(const QTime &when, FFTJobManager *dataSource);

signals:
    void onError(const QString& message);
    void done();
};

#endif // RECEIVER_H
