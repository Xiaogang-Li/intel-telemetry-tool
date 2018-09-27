#ifndef __TELEMETRY_INTEM_IMP_H__
#define __TELEMETRY_INTEM_IMP_H__
#include <memory>
#include "intem.h"
#include "gtmemory.h"
#include "sysmemory.h"
#include "cpuinfo.h"
#include "gtinfo.h"
#include "gtinfo_by_cl.h"
#include "mediacapability.h"
#include "gtutilization.h"

using namespace std;

namespace intem
{

using MCB = MediaCapability;
class Intem
{
public:
    static Intem* Instance();

    // CPU Information
    bool GetCpuUtilization(vector<CpuUtil> &utils);
    string GetCpuModel();
    string GetCpuFamily();
    uint32_t GetCpuStepping();
    uint32_t GetCpuCacheSize();
    uint32_t GetCpuCoreNumber();
    uint32_t GetCpuProcesserNumber();
    float GetCpuFrequency(uint32_t processerIndex);
    float GetCpuTemerature();
    bool IsCpuSupportHalfFloatingPoint();
    bool IsCpuSupportSingleFloatingPoint();
    bool IsCpuSupportDoubleFloatingPoint();

    // Memory Infomation
    uint32_t GetMemSpeed();
    uint32_t GetMemChannel();
    uint32_t GetMemCapacity();
    uint32_t GetMemBitWidth();
    uint32_t GetMemIdealBandwidth();
    uint32_t GetMemAvailableSize();
    uint32_t GetMemFreeSize();
    uint32_t GetMemCachedSize();
    float    GetMemUtilization();
    string   GetMemType();

    // GT Information
    uint32_t GetGtFrequency();
    string   GetGtGeneration();
    string   GetGtPlatform();
    string   GetGtVendor();
    uint32_t GetGtEUNumber();
    uint32_t GetGtFFCodecEngNumber();
    uint32_t GetGtFFVideoProcessEngNumber();
    bool IsGtSupportHalfFloatingPoint();
    bool IsGtSupportSingleFloatingPoint();
    bool IsGtSupportDoubleFloatingPoint();
    bool FixGtFrequency(uint32_t freq);
    bool GetGtUtilizaion(vector<GtUtil> &utils);
    bool GetGtBandwidth(vector<GtMemBandwidth> &bandwidths);

private:
    Intem();
    virtual ~Intem();
private:
    shared_ptr<GtInfo>        m_gpuInfo  = nullptr;
    shared_ptr<GtUtilization> m_gtUtil   = nullptr;
    shared_ptr<CpuInfo>       m_cpuInfo  = nullptr;
    shared_ptr<SysMemory>     m_sysMem   = nullptr;
    shared_ptr<MCB>           m_mediaCap = nullptr;
};

}
#endif
