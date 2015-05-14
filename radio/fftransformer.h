#ifndef FFTRANSFORMER_H
#define FFTRANSFORMER_H

#include "stdafx.h"
#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QMutex>
#include "mainwindow.h"
#include "fftw3.h"
#include "workerfft.h"

class FFTransformer : public QObject
{
    Q_OBJECT

private:
    WorkerFFT** m_workers;
    QThread** m_threads;
    unsigned char m_workersCount, m_workersStopped;
    fftw_complex* m_complexSub;
    void fillComplexSub();
    size_t m_bufferSize;    

public:
    /**
     * @brief setSafeExit
     * quit instead of waiting on a job container.
     */
    void setSafeExit();
    explicit FFTransformer(QObject* parent, char workersCount = 1);
    void start();
    void setDataSource(FFTJobManager *dataSource);
    ~FFTransformer();

private slots:
    void workerFinished();

signals:
    void dataProcessed(FilterResult* data);
    void finished();
};

#endif
