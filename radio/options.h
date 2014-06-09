#ifndef OPTIONS_H
#define OPTIONS_H
#include <string>
#include <sstream>
#include <QStringList>

#define FIRE_CHANGED(option) if(other != option) \
{\
    emit optionsChanged();\
}

#include <QObject>
#include <string>
#include <sstream>

#pragma warning(disable:4100)
#pragma once

typedef unsigned char byte;

struct Address
{
    Address()
        : A(0), B(0), C(0), D(0){}

    Address(byte a, byte b, byte c, byte d)
        : A(a), B(b), C(c), D(d){}

    byte A, B, C, D;
    //short AB, CD;
   // byte data[4];
};

typedef Address* pAddress;



class Options : public QObject
{
    Q_OBJECT

private:
    static Options* s_instance;
    Options(){}
    Options(const Options& other){}
    Options operator = (const Options & other){}

#pragma region props

private:
    pAddress m_address;
public:
    ~Options();

    QString getAddress()const
    {
        if(m_address == nullptr)
            return QString("0.0.0.0");
        QString s;
        s.append(QString::number(m_address->A)).append('.')
        .append(QString::number(m_address->B)).append('.')
        .append(QString::number(m_address->C)).append('.')
        .append(QString::number(m_address->D));
        return s;
    }

    void setAddress(const pAddress& other)
    {
        FIRE_CHANGED(m_address)
        if(m_address != nullptr)
        {
            delete m_address;
            m_address = nullptr;
        }

        m_address = other;
    }

    bool setAddress(const QString& src)
    {
        try
        {
            pAddress address = new Address;
            QStringList splitted = src.split(QChar('.'), QString::SplitBehavior::SkipEmptyParts);
            if(splitted.length() != 4)
                throw new std::runtime_error("bad value");
            bool success[] = { true, true, true, true };
            address->A = splitted[0].toInt(&success[0]);
            address->B = splitted[1].toInt(&success[1]);
            address->C = splitted[2].toInt(&success[2]);
            address->D = splitted[3].toInt(&success[3]);
            if(!(success[0] && success[1] && success[2] && success[4]))
                throw new std::runtime_error("non-int values");
            m_address = address;
        }
        catch(std::runtime_error)
        {
            return false;
        }

        return true;
    }

private:
    int m_timeLeft;
public:
    int getTimeLeft()const  { return m_timeLeft; }
    void setTimeLeft(int other)
    {
        FIRE_CHANGED(m_timeLeft)
        m_timeLeft = other;
    }

private:
    int m_frequency;
public:
    int getFrequency()const  { return m_frequency; }
    void setFrequency(int other)
    {
        FIRE_CHANGED(m_frequency)
        m_frequency = other;
    }

private:
    int m_band;
public:
    int getBand()const  { return m_band; }
    void setBand(const int& other)
    {
        FIRE_CHANGED(m_band)
        m_band = other;
    }

#pragma endregion

signals:
    void optionsChanged();

public:
    static Options* getInstance();
};

#undef FIRE_CHANGED

#endif // OPTIONS_H
