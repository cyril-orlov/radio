#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>

#endif // WORKER_H
class Worker : public QObject
{
    Q_OBJECT

private:
    int m_time;
    QThread * m_thread;
public:
    int getTime()const  { return m_time; }
    void setTime(const int other) { m_time = other; }

public:
    Worker(QThread* thread);

    virtual ~Worker() {}

    void work();

signals:
    void done();
    void tick(int count);
};
