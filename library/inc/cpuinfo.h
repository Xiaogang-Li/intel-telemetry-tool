/*************************************************************************
    > File Name: cpu.h
 ************************************************************************/
#ifndef __PERF_TOOL_CPU_INFO__
#define __PERF_TOOL_CPU_INFO__

#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include "cpu.h"

class CpuInfo
{
public:
    CpuInfo();
    virtual ~CpuInfo();

    bool Init();
    
    std::string GetDeviceName() const { return m_model; }
    std::string GetDeviceFamily() const { return m_family; }
    uint32_t GetStepping() const { return m_stepping; }
    uint32_t GetCacheSize() const { return m_cacheSize; }
    uint32_t GetCoreNumber() const { return m_cpuCores; }
    uint32_t GetProcesserNumber() const { return m_processer; }
    
    float    GetFrequency(uint32_t processerIdx);
    float    GetTemperature();

    bool GetCPUUtilization(std::vector<std::pair<std::string, float>> &utils);

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

    void Dump();
private:
    int CountProcessers(const std::ifstream& ifs);
    
    void GetTempValues(const std::string& cmd,
                       std::vector<std::string>& container);

    void ReadDeviceName(const std::string& rawString);
    void ReadDeviceFamily(const std::string& rawString);
    void ReadStepping(const std::string& rawString);
    void ReadCacheSize(const std::string& rawString);
    void ReadCpuCores(const std::string& rawString);

    bool PrepareCpuStatInfo();
    bool GetStatRawData(std::vector<std::string> &rawData);
private:
    std::string m_model     = "";
    std::string m_family    = "";
    uint32_t    m_stepping  = 0;
    uint32_t    m_cacheSize = 0;
    uint32_t    m_cpuCores  = 0;
    uint32_t    m_processer = 0;
    
    std::map<string, std::shared_ptr<Cpu>> m_cpus;
    std::ifstream m_ifs;

    std::vector<std::string> m_halfFPSupportList;
    std::vector<std::string> m_singleFPSupportList;
    std::vector<std::string> m_doubleFPSupportList;
};

#endif
