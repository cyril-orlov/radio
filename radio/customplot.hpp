#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H
#include <qwt_plot.h>
#include "datahelper.h"
#include <qwt_plot_curve.h>

class CustomPlot : public QwtPlot
{
public:
    CustomPlot(QWidget* parent)
        : QwtPlot(parent)
    {
        this->startTimer(10);
        QwtPlotCurve* curve = new QwtPlotCurve("Spectrum");
        curve->setPen(Qt::blue, 1);
        setAxisAutoScale(QwtPlot::Axis::yLeft, false);
        setAutoReplot(false);
        DataHelper * data = new DataHelper();
        curve->setData(data);
        curve->attach(this);
    }

    void timerEvent(QTimerEvent*)override
    {
        replot();
    }

    void replot() override
    {
        QwtPlotCurve* curve = dynamic_cast<QwtPlotCurve*>(itemList()[0]);
        DataHelper* curveData = static_cast<DataHelper*>(curve->data());
        curveData->mutex()->lock();
        setAxisScale(QwtPlot::yLeft, 0, curveData->boundingRect().top());
        QwtPlot::replot();
        curveData->mutex()->unlock();
    }
};

#endif
