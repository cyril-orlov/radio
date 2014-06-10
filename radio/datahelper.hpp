#ifndef DATAHELPER_H
#define DATAHELPER_H
#include "qwt_series_data.h"
#include <complex>

typedef std::complex<double> Complex;

class DataHelper : public QwtSeriesData<QPointF>
{
private:
    QVector<Complex> * m_data;
public:
    DataHelper(QVector<Complex> * data):
        m_data(data)
    {
        if(data->size() == 0)
            return;

        double top = 0;
        for(size_t i = 0; i != data->size(); i++)
            if(data->at(i).imag() > top)
                top = data->at(i).imag();

        d_boundingRect = QRectF(0, top, data->last().real(), 0);
    }

    virtual size_t size() const
    {
        return m_data->size();
    }

    virtual QPointF sample(size_t i) const
    {
        const Complex* c = &m_data->at(i);
        return QPointF(c->real(), c->imag());
    }

    virtual QRectF boundingRect() const
    {
        return QRectF(d_boundingRect);
    }

};

#endif // DATAHELPER_H
