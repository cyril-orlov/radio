#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "worker.h"


class Timer : public QObject
{   
    Q_OBJECT
public:
    explicit Timer(QObject *parent = 0);
    virtual ~Timer() { m_worker->deactivate(); }

private:
    Worker* m_worker;
    QThread* m_thread;

public:
    int getTime()const  { return m_worker->getTime(); }
    void setTime(const int other) { m_worker->setTime(other); }

    void work() { m_thread->start(); }

signals:
    void done();
    void tick(int count);

private slots:
    void done_internal();
    void tick_internal(int count);

public slots:
    void checkOptions();

};

#endif // TIMER_H
