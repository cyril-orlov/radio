#ifndef WORKERRX_H
#define WORKERRX_H

#include "worker.h"
#include <QObject>
#include <complex>
#include <QVector>

void showWarning(QWidget * parent, const QString &title, const QString &text);
typedef std::complex<double> Complex;
typedef QVector<Complex> Samples;

class WorkerRx : public Worker
{
    Q_OBJECT

public:
    static const int SAMPLE_SIZE = sizeof(Complex);

    explicit WorkerRx(QThread *thread = 0);

signals:
    void dataReceived(Samples* data);
    void error(const QString & message);

public slots:
    void work() override;
};

#endif // WORKERRX_H
