#include "worker.h"
#include "Windows.h"

Worker::Worker(QThread* thread):
    m_thread(thread),
    m_active(true)
{
    moveToThread(thread);
}

void Worker::setTime(const int other)
{
    m_time = other;
    m_lastEffectiveTick = GetTickCount();
}

void Worker::deactivate()
{
    m_active = false;
    m_thread->wait();
}

void Worker::work()
{
    while(m_active)
    {
        int elapsed = GetTickCount() - m_lastEffectiveTick;
        if(elapsed < 1000)
        {
            m_thread->msleep(100);
            continue;
        }

        if(m_time > 0)
            emit tick(--m_time);
        else
            emit done();

        m_lastEffectiveTick += elapsed;
    }

    qDebug("timer stopped due to deactivation.\n");
    m_thread->exit();
}
