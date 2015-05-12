#ifndef WORKERFFT_H
#define WORKERFFT_H

#include "stdafx.h"
#include "worker.hpp"
#include "fftw3.h"
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <complex>
#include "options.h"
#include "jobmanager.hpp"

typedef std::complex<double> Complex;
typedef JobManager<FFTJob<Complex>*> FFTJobManager;

class WorkerFFT : public Worker
{
    Q_OBJECT

public:
    WorkerFFT(QThread* thread, size_t bufferSize, size_t bufferStep);    

private:
    fftw_complex *m_sampleBuffer, *m_spectrumBuffer, *m_complexSub;
    fftw_plan m_plan, m_inversePlan;
    size_t m_bufferSize, m_bufferStep;
    QThread * m_thread;
    QMutex * m_accessMutex;
    FFTJobManager* m_dataSource;
    void handleJob(FFTJob<Complex>* job);
    void afterDestroy();
#ifdef DUMP_FFT
    void DumpFFT(double* buffer, size_t steps);
#endif
#ifdef DUMP_RAW
    void DumpRaw(FFTJob<Complex> *job);
#endif

public:
    QMutex* mutex();
    virtual void work() override;
    void init(fftw_complex *complexSub);
    QQueue<Complex>* getData();
    void setDataSource(FFTJobManager* dataSource);

signals:
    void digested(double* data, size_t length, size_t column);
};

#endif // WORKERFFT_H
