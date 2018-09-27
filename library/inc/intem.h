#ifndef __TELEMETRY_INTEM_H__
#define __TELEMETRY_INTEM_H__
#include <string>
#include <vector>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

namespace intem
{
// System Information



// CPU Information
struct CpuUtil
{
    string cpuName;
    float  utilization;

    CpuUtil(string name, float util)
    {
        cpuName     = name;
        utilization = util;
    }
};

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


// Memory Information
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


// GT Infomation
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

enum GtEngine
{
    COMPUTE,
    FF_CODEC1,
    FF_CODEC2,
    FF_CODEC3,
    FF_CODEC4,
    FF_CODEC5,
    FF_CODEC6,
    FF_CODEC7,
    FF_CODEC8,
    FF_VP1,
    FF_VP2,
    MAX_ENGINES
};

enum GtMBType
{
    MB_GT,
    MB_IA,
    MB_IO
};

struct GtUtil
{
    GtEngine engine;
    float    utilization;

    GtUtil(GtEngine eng, float util)
    {
        engine      = eng;
        utilization = util;
    }
};

bool GetGtUtilizaion(vector<GtUtil> &utils);

struct GtMemBandwidth
{
    GtMBType type;
    float    bandwidth;

    GtMemBandwidth(GtMBType tp, float bw)
    {
        type      = tp;
        bandwidth = bw;
    }
};

bool GetGtBandwidth(vector<GtMemBandwidth> &bandwidths);

// Media capability
//

string  GetDeviceInfoInToml();
}

#ifdef __cplusplus
}
#endif

#endif
