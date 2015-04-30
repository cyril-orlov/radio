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
    char m_workersCount;
    fftw_complex* m_complexSub;
    void fillComplexSub();
    size_t m_bufferSize;

public:
    explicit FFTransformer(QObject* parent, char workersCount = 1);
    void start();
    void setDataSource(FFTJobManager *dataSource);
    ~FFTransformer();

signals:
    void dataProcessed(double* data, size_t column, size_t length);
};

#endif
