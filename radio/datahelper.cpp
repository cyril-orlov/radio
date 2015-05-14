#include "datahelper.h"

DataHelper::DataHelper()
    : m_accessLock(new QMutex())
{
    setInterval(Qt::Axis::YAxis, QwtInterval(0, 1));
}

DataHelper::~DataHelper()
{
    foreach (auto i, m_data)
        delete i;
    delete m_accessLock;
}

QMutex * DataHelper::mutex()
{
    return m_accessLock;
}

int DataHelper::columns()const
{
    return m_data.size();
}

int DataHelper::rows()const
{
    return m_data.size() == 0 ? 0 : m_data[0]->length();
}

void DataHelper::setData(FilterResult* data)
{
    m_accessLock->lock();
    m_data.push_back(data);

    static double zTop = 0;
    static double maxFrequency = 0, minFrequency = 0;

    if(data->frequency() > maxFrequency)
    {
        maxFrequency = data->frequency();
        setInterval(Qt::Axis::XAxis, QwtInterval(minFrequency, maxFrequency));
    }
    else
    if(data->frequency() < minFrequency)
    {
        minFrequency = data->frequency();
        setInterval(Qt::Axis::XAxis, QwtInterval(minFrequency, maxFrequency));
    }

    auto d = data->getBuffer();
    for(auto i = 0; i != data->length(); i++)
    {
        if(d[i] > zTop)
            zTop = d[i];
    }

    setInterval(Qt::Axis::ZAxis, QwtInterval(0, zTop));

    m_accessLock->unlock();
}

double DataHelper::value(double x, double y)const
{
 /*   const double c = 0.842;

    const double v1 = x * x + (y-c) * (y+c);
    const double v2 = x * (y+c) + x * (y+c);

    return 1.0 / (v1 * v1 + v2 * v2);*/    
    if(m_data.size() == 0)
        return 0;

    size_t columnIndex = 0;
    if(m_data.size() != 1)
    {
        double delta = abs(x - m_data[0]->frequency()), nextDelta;

        for (size_t i = 1; i < m_data.size(); i++)
        {
            nextDelta = abs(x - m_data[i]->frequency());
            if(nextDelta < delta)
            {
                columnIndex = i;
                delta = nextDelta;
            }
        }
    }

    auto col = m_data[columnIndex];
    size_t i = (size_t)(y * col->length());
    return col->getBuffer()[i];
}
