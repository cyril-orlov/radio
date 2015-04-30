#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimerEvent>

class Scheduler : public QObject
{
    Q_OBJECT

    static const int SLEEP_MS = 10;
    QTime m_time;
    bool m_fired;

    int checkSchedule();
    void timerEvent(QTimerEvent *e);
public:
    const QTime& when()const;
    explicit Scheduler(QObject *parent, const QTime& when);
    ~Scheduler();
signals:
    void launch();
};

#endif // SCHEDULER_H
