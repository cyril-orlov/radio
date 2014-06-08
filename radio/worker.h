#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>

#endif // WORKER_H
class Worker : public QObject
{
    Q_OBJECT

private:
    bool m_active;
    int m_time;
    long m_lastEffectiveTick;
    QThread * m_thread;
public:
    int getTime()const  { return m_time; }
    void setTime(const int other);

public:
    Worker(QThread* thread);

    virtual ~Worker() {}

    void work();
    void deactivate();

signals:
    void done();
    void tick(int count);
};
