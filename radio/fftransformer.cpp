#include "fftransformer.h"

FFTransformer::FFTransformer(MainWindow *parent) :
    QObject(parent)
{

}

void FFTransformer::onDataReceived(QVector<Complex> * data)
{
    ((MainWindow*)parent())->onChartChanged(data);
}
