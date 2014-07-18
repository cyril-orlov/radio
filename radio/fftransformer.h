#ifndef FFTRANSFORMER_H
#define FFTRANSFORMER_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QMutex>
#include "mainwindow.h"
#include "fftw3.h"
#include "workerfft.h"

//todo: a worker, make sample queue, ttransform fixed count - 10000
class FFTransformer : public QObject
{
    Q_OBJECT

private:
    WorkerFFT* m_worker;
    QQueue<Complex> m_data;
    QThread *m_thread;

public:
    explicit FFTransformer(size_t bufferSize = 1 << 12);
    ~FFTransformer();

signals:
    void dataProcessed(QVector<double> data);

public slots:
    void onDataReceived(Complex * data, size_t count);
    void onDataProcessed(QVector<double> &data);

};

#endif
