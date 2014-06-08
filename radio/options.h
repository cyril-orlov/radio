#ifndef OPTIONS_H
#define OPTIONS_H

#define FIRE_CHANGED(option) if(other != option) \
{\
    emit optionsChanged();\
}

#include <QObject>

#pragma warning(disable:4100)
#pragma once

typedef unsigned char byte;
typedef struct
{
    union
    {
        byte A, B, C, D;
        short AB, CD;
    };
} Address, *pAddress;

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
    pAddress getAddress()const  { return m_address; }
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
