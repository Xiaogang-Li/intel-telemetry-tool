#ifndef __PERF_TOOL_GT_INFO_H__
#define __PERF_TOOL_GT_INFO_H__

#include <fstream>

class GtInfo
{
public:
    GtInfo();
    ~GtInfo();

    uint32_t GetFrequency();    
    uint32_t GetMaxFrequency();
    uint32_t GetMinFrequency();

    bool SetMaxFrequency(uint32_t freq);
    bool SetMinFrequency(uint32_t freq);

    void Dump();

private:
    static const std::string m_curFreqPath;
    static const std::string m_maxFreqPath;
    static const std::string m_minFreqPath;
    static const std::string m_boostFreqPath;

    std::ifstream m_ifsCur;
    std::ifstream m_ifsMax;
    std::ifstream m_ifsMin;
    std::ifstream m_ifsBoost;
};

#endif