#include "options.h"
#include <cstddef>
#include <QSettings>
#include <QFile>

Options* Options::getInstance()
{
    if(s_instance == nullptr)
    {
        s_instance = new Options();
        s_instance->load();
    }

    return s_instance;
}

void Options::save(const char * filename)const
{
    QSettings settings(filename, QSettings::IniFormat);
    settings.setValue("address", m_address.toString());
    settings.setValue("frequency", m_frequency);
    settings.setValue("band", m_band);    
}

// load from cfg or assign defaults
void Options::load(const char * filename)
{
    QSettings settings(filename, QSettings::IniFormat);

    setTimeLeft(0);

    QVariant setting = settings.value("address", QString(""));
    if(setting.toString().length() != 0)
        setAddress(setting.toString());
    else
        setAddress(Address(192, 168, 10, 2));

    setting = settings.value("frequency", QString(""));
    if(setting.toString().length() != 0)
        setFrequency(setting.toDouble());
    else
        setFrequency(5e6);

    setting = settings.value("band", QString(""));
    if(setting.toString().length() != 0)
        setBand(setting.toDouble());
    else
        setBand(2e5);

    bool ok;
    size_t fftWindow = settings.value("fft_window", -1).toInt(&ok);
    if(ok && fftWindow != (size_t)-1)
        setFFTWindow(fftWindow);
    else
        setFFTWindow(1 << 13);

    size_t fftOverlap = settings.value("fft_overlap", -1).toInt(&ok);
    if(ok && fftOverlap != (size_t)-1)
        setFFTOverlap(fftOverlap);
    else
        setFFTOverlap(0);
}

void Options::create()
{
    getInstance();
}


Options* Options::s_instance = nullptr;
