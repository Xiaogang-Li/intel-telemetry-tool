#include "intem_imp.h"
#include "cpptoml.h"

namespace intem
{
// CPU Information
bool GetCpuUtilization(vector<CpuUtil> &utils)
{
    return Intem::Instance()->GetCpuUtilization(utils);
}

string GetCpuModel()
{
    return Intem::Instance()->GetCpuModel();
}

string GetCpuFamily()
{
    return Intem::Instance()->GetCpuFamily();
}

uint32_t GetCpuStepping()
{
    return Intem::Instance()->GetCpuStepping();
}

uint32_t GetCpuCacheSize()
{
    return Intem::Instance()->GetCpuCacheSize();
}

uint32_t GetCpuCoreNumber()
{
    return Intem::Instance()->GetCpuCoreNumber();
}

uint32_t GetCpuProcesserNumber()
{
    return Intem::Instance()->GetCpuProcesserNumber();
}

float GetCpuFrequency(uint32_t processerIndex)
{
    return Intem::Instance()->GetCpuFrequency(processerIndex);
}

float GetCpuTemerature()
{
    return Intem::Instance()->GetCpuTemerature();
}

bool IsCpuSupportHalfFloatingPoint()
{
    return Intem::Instance()->IsCpuSupportHalfFloatingPoint();
}

bool IsCpuSupportSingleFloatingPoint()
{
    return Intem::Instance()->IsCpuSupportSingleFloatingPoint();
}

bool IsCpuSupportDoubleFloatingPoint()
{
    return Intem::Instance()->IsCpuSupportDoubleFloatingPoint();
}

// Memory Infomation
uint32_t GetMemSpeed()
{
    return Intem::Instance()->GetMemSpeed();
}

uint32_t GetMemChannel()
{
    return Intem::Instance()->GetMemChannel();
}

uint32_t GetMemCapacity()
{
    return Intem::Instance()->GetMemCapacity();
}

uint32_t GetMemBitWidth()
{
    return Intem::Instance()->GetMemBitWidth();
}

uint32_t GetMemIdealBandwidth()
{
    return Intem::Instance()->GetMemIdealBandwidth();
}

uint32_t GetMemAvailableSize()
{
    return Intem::Instance()->GetMemAvailableSize();
}

uint32_t GetMemFreeSize()
{
    return Intem::Instance()->GetMemFreeSize();
}

uint32_t GetMemCachedSize()
{
    return Intem::Instance()->GetMemCachedSize();
}

float GetMemUtilization()
{
    return Intem::Instance()->GetMemUtilization();
}

string GetMemType()
{
    return Intem::Instance()->GetMemType();
}

// GT Information
uint32_t GetGtFrequency()
{
    return Intem::Instance()->GetGtFrequency();
}

string GetGtGeneration()
{
    return Intem::Instance()->GetGtGeneration();
}

string GetGtPlatform()
{
    return Intem::Instance()->GetGtPlatform();
}

string GetGtVendor()
{
    return Intem::Instance()->GetGtVendor();
}

uint32_t GetGtEUNumber()
{
    return Intem::Instance()->GetGtEUNumber();
}

uint32_t GetGtFFCodecEngNumber()
{
    return Intem::Instance()->GetGtFFCodecEngNumber();
}

uint32_t GetGtFFVideoProcessEngNumber()
{
    return Intem::Instance()->GetGtFFVideoProcessEngNumber();
}

bool IsGtSupportHalfFloatingPoint()
{
    return Intem::Instance()->IsGtSupportHalfFloatingPoint();
}

bool IsGtSupportSingleFloatingPoint()
{
    return Intem::Instance()->IsGtSupportSingleFloatingPoint();
}

bool IsGtSupportDoubleFloatingPoint()
{
    return Intem::Instance()->IsGtSupportDoubleFloatingPoint();
}

bool FixGtFrequency(uint32_t freq)
{
    return Intem::Instance()->FixGtFrequency(freq);
}

bool GetGtUtilizaion(vector<GtUtil> &utils)
{
    return Intem::Instance()->GetGtUtilizaion(utils);
}

bool GetGtBandwidth(vector<GtMemBandwidth> &bandwidths)
{
    return Intem::Instance()->GetGtBandwidth(bandwidths);
}

string GetDeviceInfoInToml()
{
    shared_ptr<cpptoml::table> root = cpptoml::make_table();

    auto device = cpptoml::make_table_array();
    root->insert("device", device);

    auto table = cpptoml::make_table();
    device->push_back(table);

    table->insert("name", GetCpuModel());
    vector<CpuUtil> utils;
    GetCpuUtilization(utils);

    for(auto util: utils)
    {
        table->insert(util.cpuName + ".utilization", util.utilization);
    }
    
    table->insert("cpu.capacity", GetCpuFrequency(0));
    table->insert("cpu.family", GetCpuFamily());
    table->insert("cpu.stepping", GetCpuStepping());
    table->insert("cpu.cachesize", GetCpuCacheSize());
    table->insert("cpu.corenumber", GetCpuCoreNumber());
    table->insert("cpu.processernumber", GetCpuProcesserNumber());
    table->insert("cpu.temprature", GetCpuTemerature());
    table->insert("cpu.supported_types.int8", IsCpuSupportHalfFloatingPoint());
    table->insert("cpu.supported_types.fp16", IsCpuSupportSingleFloatingPoint());
    table->insert("cpu.supported_types.fp32", IsCpuSupportDoubleFloatingPoint());

    table->insert("mem.speed", GetMemSpeed());
    table->insert("mem.channel", GetMemChannel());
    table->insert("mem.capacity", GetMemCapacity());
    table->insert("mem.bitwidth", GetMemBitWidth());
    table->insert("mem.idealbandwidth", GetMemIdealBandwidth());
    table->insert("mem.availablesize", GetMemAvailableSize());
    table->insert("mem.freesize", GetMemFreeSize());
    table->insert("mem.cachedsize", GetMemCachedSize());
    table->insert("mem.utilization", GetMemUtilization());
    table->insert("mem.type", GetMemType());

    table->insert("gpu.frequency", GetGtFrequency());
    table->insert("gpu.generation", GetGtGeneration());
    table->insert("gpu.platform", GetGtPlatform());
    table->insert("gpu.vendor", GetGtVendor());
    table->insert("gpu.eu.number", GetGtEUNumber());
    table->insert("gpu.vdbox.number", GetGtFFCodecEngNumber());
    table->insert("gpu.vebox.number", GetGtFFVideoProcessEngNumber());
    table->insert("gpu.supported_types.int8", IsGtSupportHalfFloatingPoint());
    table->insert("gpu.supported_types.fp16", IsGtSupportSingleFloatingPoint());
    table->insert("gpu.supported_types.fp32", IsGtSupportDoubleFloatingPoint());

    stringstream ss;
    ss << (*root);
    return ss.str();
}
} // namespace intem
