#ifndef OPTIONS_H
#define OPTIONS_H


#include "stdafx.h"
#include <string>
#include <sstream>
#include <QStringList>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

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

    bool operator == (const Address& other)const
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
public:
    static void create();

private:
    static Options* s_instance;
    Options(){}
    Options(const Options& other){}
    Options operator = (const Options & other){}

    po::options_description m_settings;

public:
    static Options* getInstance();
    void save(const char * filename = "config.ini")const;
    void load(const char * filename = "config.ini");

    operator po::options_description() { return m_settings; }

#pragma region props

private:
    Address m_address;
public:
    Address getAddress()const { return m_address; }

    void setAddress(const Address& other) { m_address = other; }

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
    double m_startFrequency;
public:
    double getStartFrequency()const { return m_startFrequency; }
    void setStartFrequency(double other) { m_startFrequency = other; }

private:
    double m_endFrequency;
public:
    double getEndFrequency()const  { return m_endFrequency; }
    void setEndFrequency(double other){ m_endFrequency = other; }

private:
    double m_signalSpeed;
public:
    double getSignalSpeed()const  { return m_signalSpeed; }
    void setSignalSpeed(const double& other) { m_signalSpeed = other; }

private:
    int m_extraTicks;
public:
    int getExtraTicks()const  { return m_extraTicks; }
    void setExtraTicks(const int& other) { m_extraTicks = other; }

private:
    double m_band;
public:
    double getBand()const  { return m_band; }
    void setBand(const double& other){ m_band = other; }

private:
    double m_actualBand;
public:
    double getActualBand()const  { return m_actualBand; }
    void setActualBand(const double& other) { m_actualBand = other; }

private:
    char m_fftThreads;
public:
    char getFFTThreads()const  { return m_fftThreads; }
    void setFFTThreads(const char& other) { m_fftThreads = other; }

public:
    static double calculateActualBand(double actualBand, double speed)
    {
        auto band = getInstance()->getBand();
        size_t window = band * actualBand / speed;
        size_t i = 2;
        while ((i *= 2) < window) ;
        window = i;
        return i * speed / band;
    }

#pragma endregion

};


#endif // OPTIONS_H
