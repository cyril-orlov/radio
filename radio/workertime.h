#ifndef WORKERTIME_H
#define WORKERTIME_H

#include "worker.hpp"

class WorkerTime : public Worker
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
    WorkerTime(QThread* thread);
    virtual ~WorkerTime() {}

    void work() override;

signals:
    void tick(int count);

};

#endif // WORKERTIME_H
