#include <QThread>
#include "timer.h"
#include "options.h"

Timer::Timer(QObject *parent) :
    m_thread(new QThread(parent))
{
    m_worker = new WorkerTime(m_thread);
    QObject::connect(m_worker, &WorkerTime::done, this, &Timer::done_internal);
    QObject::connect(m_worker, &WorkerTime::tick, this, &Timer::tick_internal);
}

void Timer::checkOptions()
{
    int time = Options::getInstance()->getTimeLeft();
    if(time > 0)
        setTime(time);
}

void Timer::done_internal()
{   
    emit done();
}

void Timer::tick_internal(int count)
{
    emit tick(count);
}

