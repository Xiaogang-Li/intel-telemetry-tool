#include <iostream>
#include <sstream>
#include <exception>
#include <iomanip>
#include "gtinfo.h"

const std::string GtInfo::m_curFreqPath = "/sys/devices/pci0000:00/0000:00:02.0/drm/card0/gt_cur_freq_mhz";
const std::string GtInfo::m_maxFreqPath = "/sys/devices/pci0000:00/0000:00:02.0/drm/card0/gt_max_freq_mhz";
const std::string GtInfo::m_minFreqPath = "/sys/devices/pci0000:00/0000:00:02.0/drm/card0/gt_min_freq_mhz";
const std::string GtInfo::m_boostFreqPath = "/sys/devices/pci0000:00/0000:00:02.0/drm/card0/gt_boost_freq_mhz";

GtInfo::GtInfo()
{
    try
    {
        m_ifsCur.open(m_curFreqPath,  std::ifstream::in);
        m_ifsMax.open(m_maxFreqPath,  std::ifstream::in);
        m_ifsMin.open(m_minFreqPath,  std::ifstream::in);
    }
    catch(std::exception& e)
    {
        std::cerr << "******** Error when init GT Information **************" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "******************************************************" << std::endl;
    }
}

GtInfo::~GtInfo()
{
    m_ifsCur.close();
    m_ifsMax.close();
    m_ifsMin.close();
    m_ifsBoost.close();
}

uint32_t GtInfo::GetFrequency()
{
    uint32_t ret = 0;
    m_ifsCur.seekg (0, m_ifsCur.beg);
    if (!m_ifsCur.good())
    {
        m_ifsCur.close();
        m_ifsCur.open(m_curFreqPath,  std::ifstream::in);
    }
    m_ifsCur >> ret;
    return ret;
}

uint32_t GtInfo::GetMaxFrequency()
{
    uint32_t ret = 0;
    m_ifsMax.seekg (0, m_ifsMax.beg);
    if (!m_ifsMax.good())
    {
        m_ifsMax.close();
        m_ifsMax.open(m_maxFreqPath,  std::ifstream::in);
    }

    m_ifsMax >> ret;
    return ret;
}

uint32_t GtInfo::GetMinFrequency()
{
    uint32_t ret = 0;
    m_ifsMin.seekg (0, m_ifsMin.beg);
    if (!m_ifsMin.good())
    {
        m_ifsMin.close();
        m_ifsMin.open(m_minFreqPath,  std::ifstream::in);
    }

    m_ifsMin >> ret;
    return ret;
}

bool GtInfo::SetMaxFrequency(uint32_t freq)
{
    bool ret = true;

    std::ofstream ofsBoost;
    std::ofstream ofsMax;
    try
    {
        ofsBoost.open(m_boostFreqPath);
        ofsMax.open(m_maxFreqPath);

        ofsBoost << freq;
        ofsMax << freq;
    }
    catch(...)
    {
        ret = false;
    }

    if (ofsBoost.is_open())
    {
        ofsBoost.close();
    }

    if (ofsMax.is_open())
    {
        ofsMax.close();
    }

    return ret;
}

bool GtInfo::SetMinFrequency(uint32_t freq)
{
    bool ret = true;

    std::ofstream ofsMin;
    try
    {
        ofsMin.open(m_minFreqPath);
        ofsMin << freq;
    }
    catch(...)
    {
        ret = false;
    }

    if (ofsMin.is_open())
    {
        ofsMin.close();
    }

    return ret;
}

void GtInfo::Dump()
{
    SetMaxFrequency(950);
    SetMinFrequency(350);

    std::cout << std::endl << " --------------------------  GT Info ---------------------------" << std::endl;
    std::cout << " |" << std::left << std::setw(61) << " " << "|" << std::endl;

    std::cout << " | " << "GT Max Frequency : " << GetMaxFrequency() << std::left << std::setw(38) << " MHz" << "|" << std::endl;
    std::cout << " | " << "GT Min Frequency : " << GetMinFrequency() << std::left << std::setw(38) << " MHz" << "|" << std::endl;
    std::cout << " | " << "GT Cur Frequency : " << GetFrequency()    << std::left << std::setw(38) << " MHz" << "|" << std::endl;

    std::cout << " |" << std::left << std::setw(61) << " " << "|" << std::endl;
    std::cout              << " ---------------------------------------------------------------" << std::endl;
}
