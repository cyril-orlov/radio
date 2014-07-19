#ifndef WORKERFFT_H
#define WORKERFFT_H

#include "worker.hpp"
#include "fftw3.h"
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <complex>

typedef std::complex<double> Complex;

class WorkerFFT : public Worker
{
    Q_OBJECT

public:
    WorkerFFT(QQueue<Complex>* queue, QThread* thread, size_t bufferSize, size_t bufferOverlap);
    ~WorkerFFT();

private:
    fftw_complex *m_inBuffer, *m_outBuffer;
    fftw_plan m_plan;
    size_t m_bufferSize, m_bufferOverlap;
    QThread * m_thread;
    QMutex * m_accessMutex;
    QQueue<Complex>* m_data;

public:
    QMutex* mutex();
    virtual void work() override;
    void init();

signals:
    void digested(QVector<double>& data);
};

#endif // WORKERFFT_H
