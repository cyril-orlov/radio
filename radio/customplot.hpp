#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H

#include "stdafx.h"
#include <qwt_plot.h>
#include "datahelper.h"
#include <QWheelEvent>
#include <qwt_plot_spectrogram.h>
#include <qwt_color_map.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_rescaler.h>

class CustomPlot : public QwtPlot
{
private:
    int m_autoScaleCooldown;
    double m_customTop;
    bool m_toScale;

public:
    CustomPlot(QWidget* parent)
        : QwtPlot(parent),
          m_autoScaleCooldown(0),
          m_customTop(0),
          m_toScale(false)
    {
        QwtPlotSpectrogram* spectrogram = new QwtPlotSpectrogram("Spectrum");
        QwtLinearColorMap* colorMap = fromPalette();
        spectrogram->setColorMap(colorMap);
        QList<double> contourLevels;
        for ( double level = 0; level <= 1; level += 0.25 )
            contourLevels += level;
        spectrogram->setContourLevels(contourLevels);
        setAutoReplot(true);
        spectrogram->setData(new DataHelper());
        spectrogram->attach(this);
        QSize delta = size() - canvas()->size();
    }

    QwtLinearColorMap* fromPalette()const
    {
        QFile palette(":/jet/jetpalette.txt");
        if(!palette.open(QIODevice::ReadOnly))
        {
            qDebug("can't open palette resource!");
            return new QwtLinearColorMap(Qt::black, Qt::red);
        }
        QTextStream stream(&palette);
        QList<QColor> colors;
        while(!stream.atEnd())
        {
            auto c = stream.readLine();
            QStringList s = c.split(QChar(0x20), QString::SplitBehavior::SkipEmptyParts);
            if(s.size() != 3)
                continue;
            int r, g, b;
            r = s[0].toInt();
            g = s[1].toInt();
            b = s[2].toInt();
            colors.push_back(QColor(r, g, b));
        }
        double interval = 1.0 / colors.size();
        QwtLinearColorMap* result = new QwtLinearColorMap();
        for (int i = 0; i < colors.size(); ++i)
            result->addColorStop(i * interval, colors[i]);

        return result;
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

    void mouseReleaseEvent(QMouseEvent* e)override
    {
        if(m_toScale = !m_toScale)
        {
            auto s = dynamic_cast<QwtPlotSpectrogram*>(itemList()[0]);
            auto d  = dynamic_cast<DataHelper*>(s->data());
            setMinimumWidth(d->columns());
            setMinimumHeight(d->rows());
            enableAxis(yLeft, false);
            enableAxis(xBottom, false);
        }
        else
        {
            setMinimumWidth(0);
            setMinimumHeight(0);
            enableAxis(yLeft, true);
            enableAxis(xBottom, true);
        }
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
