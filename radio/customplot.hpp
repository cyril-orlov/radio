#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H

#include "stdafx.h"
#include <qwt_plot.h>
#include "datahelper.h"
#include <QWheelEvent>
#include <qwt_plot_spectrogram.h>
#include <qwt_color_map.h>

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
        QwtPlotSpectrogram* spectrogram = new QwtPlotSpectrogram("Spectrum");
        QwtColorMap* colorMap = new QwtLinearColorMap(Qt::darkCyan, Qt::red);
        spectrogram->setColorMap(colorMap);
        QList<double> contourLevels;
        for ( double level = 0; level <= 1; level += 0.25 )
            contourLevels += level;
        spectrogram->setContourLevels(contourLevels);
        setAutoReplot(true);
        spectrogram->setData(new DataHelper());
        spectrogram->attach(this);
    }


  /*  void replot() override
    {
        QwtPlotSpectrogram* curve = dynamic_cast<QwtPlotSpectrogram*>(itemList()[0]);
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
*/
    void paintEvent(QPaintEvent* p)override
    {
        QPainter painter(this);
        QwtScaleMap map;
        QwtPlotSpectrogram* spectrogram = dynamic_cast<QwtPlotSpectrogram*>(itemList()[0]);
        spectrogram->draw(&painter, map, map, this->rect());
    }

    void wheelEvent(QWheelEvent * e) override
    {
      /*  const int autoScaleCooldown = 300;
        m_autoScaleCooldown = autoScaleCooldown;

        const int deltaSteps = 120;
        int stepsTaken = e->delta() / deltaSteps;

        if (stepsTaken < 0)
            setAxisScale(QwtPlot::yLeft, 0, m_customTop /= (1.1 * -stepsTaken));
        else
            if (stepsTaken > 0)
                setAxisScale(QwtPlot::yLeft, 0, m_customTop *= (1.1 * stepsTaken));
        e->accept();*/
    }
};

#endif
