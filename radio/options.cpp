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
    settings.setValue("start_frequency", m_startFrequency);
    settings.setValue("end_frequency", m_endFrequency);
    settings.setValue("extra_ticks", m_extraTicks);
    settings.setValue("signal_speed", m_signalSpeed);
    settings.setValue("actual_band", m_actualBand);
}

// load from cfg or assign defaults
void Options::load(const char * filename)
{
    QSettings settings(filename, QSettings::IniFormat);

    QVariant setting = settings.value("address", QString(""));
    if(setting.toString().length() != 0)
        setAddress(setting.toString());
    else
        setAddress(Address(192, 168, 10, 2));

    setting = settings.value("start_frequency", QString(""));
    if(setting.toString().length() != 0)
        setStartFrequency(setting.toDouble());
    else
        setStartFrequency(3e6);

    setting = settings.value("end_frequency", QString(""));
    if(setting.toString().length() != 0)
        setEndFrequency(setting.toDouble());
    else
        setEndFrequency(30e6);

    setting = settings.value("extra_ticks", QString(""));
    if(setting.toString().length() != 0)
        setExtraTicks(setting.toInt());
    else
        setExtraTicks(200);

    setting = settings.value("signal_speed", QString(""));
    if(setting.toString().length() != 0)
        setSignalSpeed(setting.toDouble());
    else
        setSignalSpeed(0.55e6);

    setting = settings.value("band", QString(""));
    if(setting.toString().length() != 0)
        setBand(setting.toDouble());
    else
        setBand(2e5);

    setting = settings.value("actual_band", QString(""));
    if(setting.toString().length() != 0)
        setActualBand(setting.toDouble());
    else
        setActualBand(0.4e5);

    bool ok;
    size_t rx_buffer_size = settings.value("rx_buffer_size", -1).toInt(&ok);
    if(ok && rx_buffer_size != (size_t)-1)
        setRXBufferSize(rx_buffer_size);
    else
        setRXBufferSize(m_band / 60);

    char fftThreads = settings.value("fft_threads", -1).toInt(&ok);
    if(ok && fftThreads != -1)
        setFFTThreads(fftThreads);
    else
        setFFTThreads(4);
}

void Options::create()
{
    getInstance();
}


Options* Options::s_instance = nullptr;
