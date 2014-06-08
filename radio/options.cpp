#include "options.h"

Options* Options::getInstance()
{
    if(s_instance == nullptr)
    {
        s_instance = new Options();
        s_instance->m_band = 0;
        s_instance->m_frequency = 0;
        s_instance->m_timeLeft = 0;
    }

    return s_instance;
}

Options* Options::s_instance = nullptr;
