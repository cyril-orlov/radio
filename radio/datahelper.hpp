#ifndef DATAHELPER_H
#define DATAHELPER_H
#include "qwt_series_data.h"
#include <complex>
#include "options.h"

typedef std::complex<double> Complex;

class DataHelper : public QwtSeriesData<QPointF>
{
private:
    QVector<double> m_data;
    static double s_band;
    static double s_freq;

public:
    DataHelper(QVector<double> &data)
    {
        for(auto i = 0; i != data.size(); i++)
            m_data.append(data[i]);

        if(data.size() == 0)
            return;

        double top = 0;
        for(size_t i = 0; i != data.size(); i++)
            if(data.at(i) > top)
                top = data.at(i);

        if(s_band == -1 || s_freq == -1)
        {
            s_band = Options::getInstance()->getBand();
            s_freq = Options::getInstance()->getFrequency();
        }

        d_boundingRect = QRectF(s_freq - s_band / 2, top, s_band, top);
    }

    virtual size_t size() const
    {
        return m_data.size();
    }

    virtual QPointF sample(size_t i) const
    {
        const double y = m_data.at(i);
        double step = s_band / size();
        double x = s_freq - s_band / 2 + step * i;
        return QPointF(x, y);
    }

    virtual QRectF boundingRect() const
    {
        return QRectF(d_boundingRect);
    }

};

double DataHelper::s_band = -1, DataHelper::s_freq = -1;



#endif // DATAHELPER_H
