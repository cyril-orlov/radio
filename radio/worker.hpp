#ifndef WORKER_H
#define WORKER_H

#include "stdafx.h"
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWeakPointer>

class Worker : public QObject
{
    Q_OBJECT

private:
    volatile bool m_active;
    QWeakPointer<QThread> m_thread;

protected:
    bool getActive()const  { return m_active; }
    void setActive(bool other) { m_active = other; }
    QThread* getThread()const { return m_thread.value; }

    virtual void work() = 0;
    Worker(){}

public:

    Worker(QThread* thread):
        m_active(true)
    {
        m_thread.assign(thread);
        QObject::connect(thread, &QThread::started, this, &Worker::work);
        moveToThread(thread);
    }

    virtual ~Worker()
    {
        deactivate();
        afterDestroy();
    }

    virtual void afterDestroy(){}

    void deactivate()
    {
        m_active = false;
        m_thread.value->quit();
        m_thread.value->wait();
        qDebug("Worker (%s) deactivated ", qPrintable(this->objectName()));
    }


signals:
    void done();

};

#endif // WORKER_H
