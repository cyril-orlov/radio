#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>

class Worker : public QObject
{
    Q_OBJECT

private:
    bool m_active;
    QThread * m_thread;

protected:
    QThread* getThread()const  { return m_thread; }
    void setThread(QThread* other) { m_thread = other; }

    bool getActive()const  { return m_active; }
    void setActive(bool other) { m_active = other; }

public:
    Worker(QThread* thread):
        m_thread(thread),
        m_active(true)
    {
        QObject::connect(thread, &QThread::started, this, &Worker::work);
        moveToThread(thread);
    }

    virtual ~Worker() {}

    virtual void work() = 0;

    void deactivate()
    {
        m_active = false;
        m_thread->wait();
    }

signals:
    void done();

};

#endif // WORKER_H
