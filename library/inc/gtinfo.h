#ifndef __PERF_TOOL_GT_INFO_H__
#define __PERF_TOOL_GT_INFO_H__

#include <fstream>
#include <vector>

class GtInfo
{
public:
    GtInfo();
    ~GtInfo();

    uint32_t GetFrequency();
    virtual uint32_t GetMaxFrequency();
    uint32_t GetMinFrequency();

    const std::string& GetGeneration() const { return m_generation; }
    const std::string& GetPlatform() const { return m_platform; }
    const std::string& GetVendor() const { return m_vendor; }
    const uint32_t& GetEUNumber() const { return m_euNum; }
    const uint32_t& GetMemorySize() const { return m_maxMemorySize; }
    const float& GetProfilingTimerResolution() const { return m_profilingTimerResolution; }

    const std::vector<std::string>& GetHalfFloatingPointSupport() const
    {
	return m_halfFPSupportList;
    }
    const std::vector<std::string>& GetSingleFloatingPointSupport() const
    {
	return m_singleFPSupportList;
    }
    const std::vector<std::string>& GetDoubleFloatingPointSupport() const
    {
	return m_doubleFPSupportList;
    }

    bool SetMaxFrequency(uint32_t freq);
    bool SetMinFrequency(uint32_t freq);

    void Dump();

private:
    void ReadGeneration(const std::string& rawString);
    void ReadPlatform(const std::string& rawString);

protected:
    static const std::string m_curFreqPath;
    static const std::string m_maxFreqPath;
    static const std::string m_minFreqPath;
    static const std::string m_boostFreqPath;
    static const std::string m_capsPath;

    std::ifstream m_ifsCur;
    std::ifstream m_ifsMax;
    std::ifstream m_ifsMin;
    std::ifstream m_ifsBoost;

    std::string m_generation = "";
    std::string m_platform   = "";
    std::string m_vendor     = ""; 
    uint32_t    m_euNum      = 0;
    uint32_t    m_maxMemorySize = 0;
    float    m_profilingTimerResolution = 0;

    std::vector<std::string> m_halfFPSupportList;
    std::vector<std::string> m_singleFPSupportList;
    std::vector<std::string> m_doubleFPSupportList;

};

#endif
