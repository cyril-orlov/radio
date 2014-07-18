#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>

class Worker : public QObject
{
    Q_OBJECT

private:
    volatile bool m_active;
    QThread * m_thread;

protected:
    QThread* getThread()const  { return m_thread; }
    void setThread(QThread* other) { m_thread = other; }

    bool getActive()const  { return m_active; }
    void setActive(bool other) { m_active = other; }

    virtual void work() = 0;

public:
    Worker(QThread* thread):
        m_thread(thread),
        m_active(true)
    {
        QObject::connect(thread, &QThread::started, this, &Worker::work);
        moveToThread(thread);
    }

    virtual ~Worker() {}

    void deactivate()
    {
        m_active = false;
        m_thread->quit();
        m_thread->wait();
        qDebug("Worker (%s) deactivated ", qPrintable(this->objectName()));
    }


signals:
    void done();

};

#endif // WORKER_H
