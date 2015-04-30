#include "datahelper.h"

DataHelper::DataHelper()
    : m_accessLock(new QMutex())
{
    s_freq = Options::getInstance()->getStartFrequency();
    s_endFreq = Options::getInstance()->getEndFrequency();
    setInterval(Qt::Axis::YAxis, QwtInterval(0, 1));
}

DataHelper::DataHelper(double* data, int column, size_t length)
{
    DataHelper();
    if(length == 0)
        return;
    setData(data, column, length);
}

DataHelper::~DataHelper()
{
    foreach (int i, m_data.keys())
        delete[] m_data[i].data;
    delete m_accessLock;
}

QMutex * DataHelper::mutex()
{
    return m_accessLock;
}

void DataHelper::setData(double* data, int column, size_t length)
{
    m_accessLock->lock();
    m_data[column].data = new double[length];
    m_data[column].length = length;
    memcpy(m_data[column].data, data, sizeof(double) * length);
    static double zTop = 0;
    static size_t rightmostColumn = 0;

    if(column > rightmostColumn)
    {
        rightmostColumn = column;
        setInterval(Qt::Axis::XAxis, QwtInterval(0, column));
    }

    for(auto i = 0; i != length; i++)
    {
        if(data[i] > zTop)
            zTop = data[i];
    }
    setInterval(Qt::Axis::ZAxis, QwtInterval(0, zTop));

    m_accessLock->unlock();
    delete[] data;
}

double DataHelper::value(double x, double y)const
{
 /*   const double c = 0.842;

    const double v1 = x * x + (y-c) * (y+c);
    const double v2 = x * (y+c) + x * (y+c);

    return 1.0 / (v1 * v1 + v2 * v2);*/
    if(!m_data.contains((size_t)x))
        return 0;
    auto col = m_data[(size_t)x];
    size_t i = (size_t)(y * col.length);
    return col.data[i];
}


double DataHelper::s_freq = -1, DataHelper::s_endFreq = -1;
