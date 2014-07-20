#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H
#include <qwt_plot.h>
#include "datahelper.h"
#include <QWheelEvent>
#include <qwt_plot_curve.h>

class CustomPlot : public QwtPlot
{
private:
    int m_autoScaleCooldown;
    double m_customTop;

public:
    CustomPlot(QWidget* parent)
        : QwtPlot(parent),
          m_autoScaleCooldown(0),
          m_customTop(0)
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
        if(!m_autoScaleCooldown)
        {
            if(curveData->boundingRect().top() > m_customTop)
                setAxisScale(QwtPlot::yLeft, 0, m_customTop = curveData->boundingRect().top());
        }
        else
            m_autoScaleCooldown--;
        QwtPlot::replot();
        curveData->mutex()->unlock();
    }

    void wheelEvent(QWheelEvent * e) override
    {
        const int autoScaleCooldown = 300;
        m_autoScaleCooldown = autoScaleCooldown;

        const int deltaSteps = 120;
        int stepsTaken = e->delta() / deltaSteps;

        if (stepsTaken < 0)
            setAxisScale(QwtPlot::yLeft, 0, m_customTop /= (1.1 * -stepsTaken));
        else
            if (stepsTaken > 0)
                setAxisScale(QwtPlot::yLeft, 0, m_customTop *= (1.1 * stepsTaken));
        e->accept();
    }
};

#endif
