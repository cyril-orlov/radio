#include "scheduler.h"

Scheduler::Scheduler(QObject* parent, const QTime &when) :
    QObject(parent),
    m_time(when),
    m_fired(false)
{
    startTimer(SLEEP_MS, Qt::TimerType::PreciseTimer);
    qDebug() << "Scheduler created for " << when.toString(QString("hh:mm:ss:zzz"));
}

const QTime& Scheduler::when() const
{
    return m_time;
}

void Scheduler::timerEvent(QTimerEvent* e)
{
    auto now = QTime::currentTime();
    int msecToLaunch = now.msecsTo(m_time);
    if(msecToLaunch < SLEEP_MS + 7000)
    {
        emit launch();
        m_fired = true;
        killTimer(e->timerId());
    }
}

Scheduler::~Scheduler()
{
    if(!m_fired)
        qDebug() << "Scheduler stopped prematurely.";
}

/*void Scheduler::loadSchedule(const QString &filename)
{
    m_schedule.clear();
    QFile file(filename);
    if(!file.open(QFile::ReadOnly))
        throw new QString("Can't open schedule file.");
    QTextStream stream(&file);
    QString line;
    while((line = stream.readLine()) != QString::null)
    {
        auto splitted = line.split(QChar('|'));
        if(splitted.size() != 2)
        {
            qDebug() << "can't undestand schedule entry: " << line;
            continue;
        }
        ScheduleEntry entry;
        entry.name = splitted[0];

        const QString format = QString("hh:mm:ss:zzz");

        entry.time = QTime::fromString(splitted[1], format);
        if(!entry.time.isValid())
        {
            qDebug() << "incorrect time format: " << splitted[1] << " expected " << format;
            continue;
        }

        m_schedule.push_back(entry);
    }
    qDebug() << "schedule loaded from " << filename << ". total items " << m_schedule.size();
    file.close();
    emit scheduleLoaded(m_schedule);
}
*/
