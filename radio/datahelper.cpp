#include "datahelper.h"

DataHelper::DataHelper()
    : m_accessLock(new QMutex())
{}

DataHelper::DataHelper(QVector<double> &data)
{
    DataHelper();
    if(data.size() == 0)
        return;
    setData(data);
}

DataHelper::~DataHelper()
{
    delete m_accessLock;
}

QMutex * DataHelper::mutex()
{
    return m_accessLock;
}

void DataHelper::setData(QVector<double> data)
{
    m_accessLock->lock();
    m_data.clear();
    static double top = 0;

    for(auto i = 0; i != data.size(); i++)
    {
        m_data.append(data[i]);
        if(data.at(i) > top)
            top = data.at(i);
    }

    s_band = Options::getInstance()->getBand();
    s_freq = Options::getInstance()->getFrequency();

    d_boundingRect = QRectF(s_freq - s_band / 2, top, s_band, top);
    m_accessLock->unlock();
}

size_t DataHelper::size()const
{
    return m_data.size();
}

QPointF DataHelper::sample(size_t i)const
{
    if(m_data.size() == 0)
        return QPointF();

    double y = m_data[i];
    double step = s_band / size();
    double x = s_freq - s_band / 2 + step * i;
    return QPointF(x, y);
}

QRectF DataHelper::boundingRect()const
{
    return QRectF(d_boundingRect);
}

double DataHelper::s_band = -1, DataHelper::s_freq = -1;
