#include "options.h"
#include <cstddef>
#include <QSettings>
#include <QFile>

Options* Options::getInstance()
{
    if(s_instance == nullptr)
    {
        s_instance = new Options();
        if(!s_instance->load())
        {
            s_instance->m_band = 0;
            s_instance->m_frequency = 0;
            s_instance->m_timeLeft = 0;
            s_instance->m_address = Address(192, 168, 10, 2);
        }
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

bool Options::load(const char * filename)
{
    if(!QFile::exists(filename))
        return false;

    QSettings settings(filename, QSettings::IniFormat);
    setAddress(settings.value("address").toString());
    setFrequency(settings.value("frequency").toDouble());
    setBand(settings.value("band").toDouble());

    return true;
}

Options* Options::s_instance = nullptr;
