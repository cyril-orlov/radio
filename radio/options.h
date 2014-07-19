#ifndef OPTIONS_H
#define OPTIONS_H
#include <string>
#include <sstream>
#include <QStringList>

#define FIRE_CHANGED(option) if(option != other) \
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

    bool operator != (const Address& other)const
    {
        return A == other.A && B == other.B && C == other.C && D == other.D;
    }

    QString toString()const
    {
        QString s;
        s.append(QString::number(A)).append('.')
        .append(QString::number(B)).append('.')
        .append(QString::number(C)).append('.')
        .append(QString::number(D));
        return s;
    }
};

typedef Address* pAddress;



class Options : public QObject
{
    Q_OBJECT

public:
    static void create();

private:
    static Options* s_instance;
    Options(){}
    Options(const Options& other){}
    Options operator = (const Options & other){}

signals:
    void optionsChanged();

public:
    static Options* getInstance();
    void save(const char * filename = "config.ini")const;
    void load(const char * filename = "config.ini");

#pragma region props

private:
    Address m_address;
public:
    Address getAddress()const { return m_address; }

    void setAddress(const Address& other)
    {
        FIRE_CHANGED(m_address)
        m_address = other;
    }

    bool setAddress(const QString& src)
    {
        QStringList splitted = src.split(QChar('.'), QString::SplitBehavior::SkipEmptyParts);
        if(splitted.length() != 4)
            return false;

        bool success[] = { true, true, true, true };
        byte A = splitted[0].toInt(&success[0]);
        byte B = splitted[1].toInt(&success[1]);
        byte C = splitted[2].toInt(&success[2]);
        byte D = splitted[3].toInt(&success[3]);
        if(!(success[0] && success[1] && success[2] && success[3]))
            return false;

        setAddress(Address(A, B, C, D));

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
    double m_frequency;
public:
    double getFrequency()const  { return m_frequency; }
    void setFrequency(double other)
    {
        FIRE_CHANGED(m_frequency)
        m_frequency = other;
    }

private:
    double m_band;
public:
    double getBand()const  { return m_band; }
    void setBand(const double& other)
    {
        FIRE_CHANGED(m_band)
        m_band = other;
    }


private:
    size_t m_FFTWindow;
public:
    size_t getFFTWindow()const  { return m_FFTWindow; }
    void setFFTWindow(const size_t& other) { m_FFTWindow = other; }

private:
    int m_FFTOverlap;
public:
    int getFFTOverlap()const  { return m_FFTOverlap; }
    void setFFTOverlap(const int& other) { m_FFTOverlap = other; }


#pragma endregion

};

#undef FIRE_CHANGED

#endif // OPTIONS_H
