#ifndef FFTRANSFORMER_H
#define FFTRANSFORMER_H

#include <QObject>
#include "mainwindow.h"

typedef std::complex<double> Complex;

class FFTransformer : public QObject
{
    Q_OBJECT
public:
    explicit FFTransformer(MainWindow *parent = 0);

signals:

public slots:
    void onDataReceived(QVector<Complex> * data);

};

#endif
