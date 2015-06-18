// generates random samples
// uses Park-Miller RNG
#ifndef NOISEGEN_HPP
#define NOISEGEN_HPP

#include "stdafx.h"
#include <numeric>
#include <complex>
#include <QThread>
#include <QMutex>
#include <QVector>
#include "jobmanager.hpp"

typedef std::complex<T_REAL> Complex;
typedef JobManager<FFTJob<Complex>*> FFTJobManager;

class NoiseGen : public QObject
{
    Q_OBJECT

private:
    long m_seed;
    FFTJobManager m_dataSource;
    QMutex m_bufferLock;
    int m_sleepTime;
    bool m_active, m_isFile;
    const QString m_filename;

public:
    NoiseGen(int sleepTime = 0, long seed = 1) :
        m_seed(seed),
        m_sleepTime(sleepTime),
        m_active(true),
        m_dataSource(),
        m_isFile(false)
    {
    }

    NoiseGen(int sleepTime, const QString& filename) :
        m_sleepTime(sleepTime),
        m_active(true),
        m_dataSource(),
        m_isFile(true),
        m_filename(filename)
    {
    }

    FFTJobManager* getDataSource()
    {
        return &m_dataSource;
    }

    void deactivate()
    {
        m_active = false;
    }

    Complex next()
    {
        double max = (long)-1;
        double re = nextInt() / max, im = nextInt() / max;
        return Complex(re, im);
    }

    ~NoiseGen()
    {
    }

public slots:
    void onStart()
    {
        qDebug("NoiseGen started");
        if(m_isFile)
            workFile();
        else
            workRng();

        if(!m_active)
            qDebug("NoiseGen stopped prematurely.");
        else
            qDebug("NoiseGen stopped");
        QThread::currentThread()->exit();
    }

private:
    void workFile()
    {
        const size_t size = 56384, cols = 35;
        Complex* buffer = new Complex[size];
        QFile file(m_filename);
        if(!file.open(QIODevice::ReadOnly))
            return;
        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);

        short sample[2];
        for (int i = 0; i < cols && m_active; ++i)
        {
            size_t j = 0;
            while(!stream.atEnd() && (j < size))
            {
                stream >> sample[0] >> sample[1];
                buffer[j++] = Complex(sample[0] / 32767.0, sample[1] / 32767.0);
            }

            double f = (i / ((double)cols - 1)) * (27 - 2) + 2;
            m_dataSource.enqueue(new FFTJob<Complex>(buffer, size, f));
            QThread::msleep(m_sleepTime);
        }
    }

    void workRng()
    {
        const size_t size = 56384, cols = 35;
        Complex* buffer = new Complex[size];
        for (int i = 0; i < cols && m_active; ++i)
        {
            for (int j = 0; j < size; ++j)
                buffer[j] = next();
            QThread::msleep(m_sleepTime);
            m_dataSource.lock();
            m_dataSource.enqueue(new FFTJob<Complex>(buffer, size, i * 10000));
            m_dataSource.unlock();
        }
    }

    unsigned long nextInt()
    {
        unsigned long k;

        k = m_seed / 12773;
        m_seed = 16807 * (m_seed - k * 12773) - 2836 * k;

        return m_seed;
    }
};
#endif
