#ifndef DATAHELPER_H
#define DATAHELPER_H

#include "stdafx.h"
#include "qwt_raster_data.h"
#include <complex>
#include "options.h"

typedef std::complex<double> Complex;

struct DataColumn
{
    double* data;
    size_t length;
};

class DataHelper : public QwtRasterData
{
private:
    QMap<int, DataColumn> m_data;
    static double s_freq, s_endFreq;
    QMutex *m_accessLock;

public:
    DataHelper();

    DataHelper(double* data, int column, size_t length);

    ~DataHelper();

    QMutex * mutex();

    void setData(double* data, int column, size_t length);

    virtual double value(double x, double y) const;
};


#endif // DATAHELPER_H
