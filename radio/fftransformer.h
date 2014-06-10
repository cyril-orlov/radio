#ifndef FFTRANSFORMER_H
#define FFTRANSFORMER_H

#include <QObject>
#include "mainwindow.h"
#include "fftw3.h"

typedef std::complex<double> Complex;

class FFTransformer : public QObject
{
    Q_OBJECT

private:
    fftw_complex *m_inBuffer, *m_outBuffer;
    fftw_plan m_plan;
    size_t m_bufferSize;

public:
    explicit FFTransformer(size_t bufferSize, MainWindow *parent = 0);
    ~FFTransformer();

signals:

public slots:
    void onDataReceived(QVector<Complex> * data);
    void init();

};

#endif
