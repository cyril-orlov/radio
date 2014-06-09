#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>

class Worker : public QObject
{
    Q_OBJECT

protected:
    bool m_active;
    QThread * m_thread;

private:
    int m_time;
    long m_lastEffectiveTick;

public:
    int getTime()const  { return m_time; }
    void setTime(const int other);

public:
    Worker(QThread* thread);

    virtual ~Worker() {}

    virtual void work();
    void deactivate();

signals:
    void done();
    void tick(int count);

};

#endif // WORKER_H
