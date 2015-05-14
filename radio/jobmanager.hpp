#ifndef JOBMANAGER_H
#define JOBMANAGER_H

#include "stdafx.h"
#include <QQueue>
#include <QWaitCondition>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>

template <typename T>
class JobManager
{
private:
    QQueue<T> m_queue;
    QMutex* m_lockMutex;
    size_t m_maxItems;

public:
    JobManager(size_t cap = (size_t)-1):
        m_maxItems(cap),
        m_lockMutex(new QMutex())
    {
        qDebug("JobManager created");
    }

    bool any()
    {
        return !m_queue.isEmpty();
    }

    bool isCapped()
    {
        return m_queue.size() == m_maxItems;
    }

    T dequeue()
    {
        T result = m_queue.dequeue();
        qDebug("dequeued (size = %s)", qPrintable(QString::number(m_queue.size())));
        return result;
    }

    void enqueue(T what)
    {
        if(m_queue.count() >= m_maxItems)
            throw QString("Queue cap reached");
        m_queue.enqueue(what);
        qDebug("enqueued (size = %s)", qPrintable(QString::number(m_queue.size())));
    }

    void clear()
    {
        m_queue.clear();
    }

    void lock()
    {
        m_lockMutex->lock();
    }

    void unlock()
    {
        m_lockMutex->unlock();
    }

    ~JobManager()
    {
        delete m_lockMutex;
        qDebug() << "JobManager stopped";
    }
};

template<typename C>
class FFTJob
{
private:
    C* m_buffer;
    size_t m_length;
    double m_frequency;
public:
    C* getBuffer()
    {
        return m_buffer;
    }

    FFTJob(C* buffer, size_t length, double frequency):
        m_length(length),
        m_frequency(frequency)
    {
        m_buffer = new C[length];
        memcpy(m_buffer, buffer, length * sizeof(C));
    }

    inline size_t frequency()const
    {
        return m_frequency;
    }

    inline size_t length()const
    {
        return m_length;
    }
};

typedef FFTJob<double> FilterResult;


#endif // JOBMANAGER_H
