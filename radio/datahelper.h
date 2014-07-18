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

    QMutex *m_accessLock;

public:
    DataHelper();

    DataHelper(QVector<double> &data);

    ~DataHelper();
    QMutex * mutex();
    void setData(QVector<double> data);

    virtual size_t size() const;

    virtual QPointF sample(size_t i) const;

    virtual QRectF boundingRect() const;

};


#endif // DATAHELPER_H
