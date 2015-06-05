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

typedef std::complex<T_REAL> Complex;
typedef JobManager<FFTJob<Complex>*> FFTJobManager;

class WorkerFFT : public Worker
{
    Q_OBJECT

public:
    WorkerFFT(QThread* thread, size_t bufferSize, size_t bufferStep);    
    void setSafeExit();
private:
    FFT_COMPLEX *m_sampleBuffer, *m_spectrumBuffer, *m_complexSub;
    FFT_PLAN m_plan, m_inversePlan;
    size_t m_bufferSize, m_bufferStep;
    QThread * m_thread;
    QMutex * m_accessMutex;
    FFTJobManager* m_dataSource;
    bool m_safeExit;
    void handleJob(FFTJob<Complex>* job);
    void afterDestroy();
#ifdef DUMP_FFT
    void DumpFFT(FilterResult* job);
#endif
#ifdef DUMP_RAW
    void DumpRaw(FFTJob<Complex> *job);
#endif

public:
    QMutex* mutex();
    virtual void work() override;
    void init(FFT_COMPLEX *complexSub);
    QQueue<Complex>* getData();
    void setDataSource(FFTJobManager* dataSource);

signals:
    void digested(FilterResult *r);
    void finished();
};

#endif // WORKERFFT_H
