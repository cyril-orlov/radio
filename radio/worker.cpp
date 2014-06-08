#include "worker.h"

Worker::Worker(QThread* thread):
    m_thread(thread)
{
    moveToThread(thread);
}

void Worker::work()
{
    while(1)
    {
        if(m_time > 0)
            emit tick(--m_time);
        else
            emit done();
        m_thread->msleep(500);
    }
}
