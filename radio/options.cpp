#include "options.h"
#include <cstddef>

Options* Options::getInstance()
{
    if(s_instance == nullptr)
    {
        s_instance = new Options();
        s_instance->m_band = 0;
        s_instance->m_frequency = 0;
        s_instance->m_timeLeft = 0;
        s_instance->m_address = new Address(192, 168, 10, 2);
    }

    return s_instance;
}

Options::~Options()
{
    if(m_address != nullptr)
        delete m_address;
}

Options* Options::s_instance = nullptr;
