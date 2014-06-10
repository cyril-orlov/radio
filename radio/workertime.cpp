#include "workertime.h"
#include "Windows.h"

WorkerTime::WorkerTime(QThread* thread):
    Worker(thread),
    m_time(0)
{}

void WorkerTime::setTime(const int other)
{
    m_time = other;
    m_lastEffectiveTick = GetTickCount();
}


void WorkerTime::work()
{
    while(getActive())
    {
        int elapsed = GetTickCount() - m_lastEffectiveTick;
        if(elapsed < 1000)
        {
            getThread()->msleep(100);
            continue;
        }

        if(m_time > 0)
            emit tick(--m_time);
        else
            if(m_time == 0)
            {
                emit done();
                m_time = -1;
            }

        m_lastEffectiveTick += elapsed;
    }

    qDebug("timer stopped due to deactivation.\n");
    getThread()->exit(0);
}
