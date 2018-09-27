#include "intem_imp.h"

namespace intem
{
Intem::Intem()
{
    try
    {
        m_gpuInfo = make_shared<GtInfoByCl>();
    }
    catch (...)
    {
        m_gpuInfo = make_shared<GtInfo>();
    }

    m_gtUtil = make_shared<GtUtilization>();
    m_cpuInfo = make_shared<CpuInfo>();
    m_sysMem = make_shared<SysMemory>();
    m_mediaCap = make_shared<MCB>();
}

Intem::~Intem()
{
}

Intem *Intem::Instance()
{
    static Intem intemLib;

    return &intemLib;
}

bool Intem::GetCpuUtilization(vector<CpuUtil> &utils)
{
    bool ret = true;

    ret = m_cpuInfo->GetCPUUtilization(utils);

    return ret;
}

string Intem::GetCpuModel()
{
    string ret = "";

    ret = m_cpuInfo->GetDeviceName();

    return ret;
}

string Intem::GetCpuFamily()
{
    string ret = "";

    ret = m_cpuInfo->GetDeviceFamily();

    return ret;
}

uint32_t Intem::GetCpuStepping()
{
    uint32_t ret = 0;
    ret = m_cpuInfo->GetStepping();
    return ret;
}

uint32_t Intem::GetCpuCacheSize()
{
    uint32_t ret = 0;
    ret = m_cpuInfo->GetCacheSize();
    return ret;
}

uint32_t Intem::GetCpuCoreNumber()
{
    uint32_t ret = 0;
    ret = m_cpuInfo->GetCoreNumber();
    return ret;
}

uint32_t Intem::GetCpuProcesserNumber()
{
    uint32_t ret = 0;
    ret = m_cpuInfo->GetProcesserNumber();
    return ret;
}

float Intem::GetCpuFrequency(uint32_t processerIndex)
{
    float ret = 0;
    ret = m_cpuInfo->GetFrequency(processerIndex);
    return ret;
}

float Intem::GetCpuTemerature()
{
    float ret = 0;
    ret = m_cpuInfo->GetTemperature();
    return ret;
}

bool Intem::IsCpuSupportHalfFloatingPoint()
{
    bool ret = true;
    const std::vector<std::string>& vec = m_cpuInfo->GetHalfFloatingPointSupport();
    ret = (vec.size() > 0);
    return ret;
}

bool Intem::IsCpuSupportSingleFloatingPoint()
{
    bool ret = true;
    const std::vector<std::string>& vec = m_cpuInfo->GetSingleFloatingPointSupport();
    ret = (vec.size() > 0);
    return ret;
}

bool Intem::IsCpuSupportDoubleFloatingPoint()
{
    bool ret = true;
    const std::vector<std::string>& vec = m_cpuInfo->GetDoubleFloatingPointSupport();
    ret = (vec.size() > 0);
    return ret;
}

// Memory Infomation
uint32_t Intem::GetMemSpeed()
{
    uint32_t ret = 0;
    ret = m_sysMem->GetSpeed();
    return ret;
}

uint32_t Intem::GetMemChannel()
{
    uint32_t ret = 0;
    ret = m_sysMem->GetChannel();
    return ret;
}

uint32_t Intem::GetMemCapacity()
{
    uint32_t ret = 0;
    ret = m_sysMem->GetCapacity();
    return ret;
}

uint32_t Intem::GetMemBitWidth()
{
    uint32_t ret = 0;
    ret = m_sysMem->GetBitWidth();
    return ret;
}

uint32_t Intem::GetMemIdealBandwidth()
{
    uint32_t ret = 0;
    ret = m_sysMem->GetIdealBandWidth();
    return ret;
}

uint32_t Intem::GetMemAvailableSize()
{
    uint32_t ret = 0;
    ret = m_sysMem->GetAvailableSize();
    return ret;
}

uint32_t Intem::GetMemFreeSize()
{
    uint32_t ret = 0;
    ret = m_sysMem->GetFreeSize();
    return ret;
}

uint32_t Intem::GetMemCachedSize()
{
    uint32_t ret = 0;
    ret = m_sysMem->GetCachedSize();
    return ret;
}

float Intem::GetMemUtilization()
{
    float ret = 0;
    ret = m_sysMem->GetUsage();
    return ret;
}

string Intem::GetMemType()
{
    string ret = "";
    ret = m_sysMem->GetType();
    return ret;
}

// GT Information
uint32_t Intem::GetGtFrequency()
{
    uint32_t ret = 0;
    ret = m_gpuInfo->GetFrequency();
    return ret;
}

string Intem::GetGtGeneration()
{
    string ret = "";
    ret = m_gpuInfo->GetGeneration();
    return ret;
}

string Intem::GetGtPlatform()
{
    string ret = "";
    ret = m_gpuInfo->GetPlatform();
    return ret;
}

string Intem::GetGtVendor()
{
    string ret = "";
    ret = m_gpuInfo->GetVendor();
    return ret;
}

uint32_t Intem::GetGtEUNumber()
{
    uint32_t ret = 0;
    ret = m_gpuInfo->GetEUNumber();
    return ret;
}

uint32_t Intem::GetGtFFCodecEngNumber()
{
    uint32_t ret = 0;
    ret = 2;
    return ret;
}

uint32_t Intem::GetGtFFVideoProcessEngNumber()
{
    uint32_t ret = 0;
    ret = 1;
    return ret;
}

bool Intem::IsGtSupportHalfFloatingPoint()
{
    bool ret = true;
    const std::vector<std::string>& vec = m_gpuInfo->GetHalfFloatingPointSupport();
    ret = (vec.size() > 0);
    return ret;
}

bool Intem::IsGtSupportSingleFloatingPoint()
{
    bool ret = true;
    const std::vector<std::string>& vec = m_gpuInfo->GetSingleFloatingPointSupport();
    ret = (vec.size() > 0);
    return ret;
}

bool Intem::IsGtSupportDoubleFloatingPoint()
{
    bool ret = true;
    const std::vector<std::string>& vec = m_gpuInfo->GetDoubleFloatingPointSupport();
    ret = (vec.size() > 0);
    return ret;
}

bool Intem::FixGtFrequency(uint32_t freq)
{
    bool ret = true;
    ret = m_gpuInfo->SetMinFrequency(freq);
    ret = m_gpuInfo->SetMaxFrequency(freq);

    return ret;
}

bool Intem::GetGtUtilizaion(vector<GtUtil> &utils)
{
    bool ret = true;
    m_gtUtil->GetUtilization(utils);
    return ret;
}

bool Intem::GetGtBandwidth(vector<GtMemBandwidth> &bandwidths)
{
    bool ret = true;
    m_gtUtil->GetBandwidth(bandwidths);
    return ret;
}

} // namespace intem
