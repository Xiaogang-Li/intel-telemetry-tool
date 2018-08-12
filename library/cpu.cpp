/*************************************************************************
    > File Name: cpu.cpp
 ************************************************************************/

#include <iostream>
#include <streambuf>
#include <sstream>
#include <iomanip>
#include <exception>
#include <string.h>
#include "util.h"
#include "cpu.h"

using namespace std;

CpuInfo::CpuInfo()
{
    std::string   line = "";
    
    try
    {
        m_ifs.open("/proc/cpuinfo", std::ifstream::in);
        
        m_processer = CountProcessers(m_ifs);
        
        m_ifs.seekg (0, m_ifs.beg);
        while( getline(m_ifs, line) )
        {
            ReadDeviceName(line);
            ReadDeviceFamily(line);
            
            ReadCacheSize(line);
            ReadStepping(line);
            ReadCpuCores(line);
        }
    }
    catch(exception& e)
    {
        std::cerr << "******** Error when get CPU Information **************" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "******************************************************" << std::endl;
    }
}

CpuInfo::~CpuInfo()
{
    m_ifs.close();
}

float CpuInfo::GetFrequency(uint32_t processerIdx)
{
    float freq = 0.0;
    
    if (processerIdx >= m_processer)
    {
        return freq;
    }
    
    try
    {
        std::string line              = "";
        uint32_t    localProcesserIdx = 0;
        
        m_ifs.seekg (0, m_ifs.beg);
        if (!m_ifs.good())
        {
            m_ifs.close();
            m_ifs.open("/proc/cpuinfo", std::ifstream::in);
        }
        
        while( getline(m_ifs, line) )
        {   
            if (Util::IsInclude(line, "processor"))
            {
                localProcesserIdx = Util::GetDataFromRaw<uint32_t>(line, 2);
            }
            
            if (Util::IsInclude(line, "cpu MHz") && (localProcesserIdx == processerIdx))
            {
                freq = Util::GetDataFromRaw<float>(line, 3);
                break;
            }
        }
    }
    catch(exception& e)
    {
        std::cerr << "******** Error when get CPU Frequency   **************" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "******************************************************" << std::endl;
    }
    
    return freq;
}

float CpuInfo::GetTemperature()
{
    std::vector<std::string> types;
    std::vector<std::string> temps;
    std::string              strTemp = "";
    float                    temp    = 0.0;
    uint8_t                  index   = 0;
    
    std::string cmd = "cat /sys/class/thermal/thermal_zone*/type";
    GetTempValues(cmd, types);
    
    cmd = "cat /sys/class/thermal/thermal_zone*/temp";
    GetTempValues(cmd, temps);
    
    for (auto type : types)
    {
        if (type == "x86_pkg_temp")
        {
            break;
        }
        
        index++;
    }
    
    strTemp = temps[index];
    
    std::stringstream stream(strTemp);
    
    stream >> temp;
    
    return temp / 1000.00;
}

void CpuInfo::GetTempValues(const std::string& cmd, std::vector<std::string>& container)
{
    char tmp[1024];
    
    FILE *pp = popen(cmd.c_str(), "r");
    if (!pp) {
        return;
    }
    
    while (fgets(tmp, sizeof(tmp), pp) != NULL)
    {
        if (tmp[strlen(tmp) - 1] == '\n')
        {
            tmp[strlen(tmp) - 1] = '\0';
        }
        container.push_back(tmp);
    }
    
    pclose(pp);
}

int CpuInfo::CountProcessers(const std::ifstream& ifs)
{
    string::size_type pos  = 0;
    uint32_t          num  = 0;
    
    std::stringstream buffer;
    
    buffer << ifs.rdbuf();
    std::string cpuinfoInStr(buffer.str());

    pos = cpuinfoInStr.find("processor", 0);
    while (pos != string::npos)
    {
       ++num;
       ++pos;
       pos = cpuinfoInStr.find("processor", pos);      
    }

    return num;
}

void CpuInfo::ReadDeviceName(const std::string& rawString)
{
    if (Util::IsInclude(rawString, "model name") && m_model.empty())
    {
        m_model = Util::GetSringDataFromRaw(rawString);
    }
}

void CpuInfo::ReadDeviceFamily(const std::string& rawString)
{
    if (Util::IsInclude(rawString, "cpu family") && m_family.empty())
    {
        m_family = Util::GetSringDataFromRaw(rawString);
    }
}

void CpuInfo::ReadStepping(const std::string& rawString)
{
    if (Util::IsInclude(rawString, "stepping") && m_stepping == 0)
    {
        m_stepping = Util::GetDataFromRaw<uint32_t>(rawString, 2);
    }
}

void CpuInfo::ReadCacheSize(const std::string& rawString)
{
    if (Util::IsInclude(rawString, "cache size") && m_cacheSize ==0)
    {
        m_cacheSize = Util::GetDataFromRaw<uint32_t>(rawString, 3);
    }
}

void CpuInfo::ReadCpuCores(const std::string& rawString)
{
    if (Util::IsInclude(rawString, "cpu cores") && m_cpuCores == 0)
    {
        m_cpuCores = Util::GetDataFromRaw<uint32_t>(rawString, 3);
    }
}

void CpuInfo::Dump()
{
    uint32_t loop = 0;

    std::cout <<  "\33[2;0H" << " -------------------------- GPU Info ---------------------------" << std::endl;
    std::cout << " | " << std::left << std::setw(60) << " " << "|" << std::endl;
    
    std::cout << " | Processer Counts: " << std::left << std::setw(42) << m_processer << "|" << endl;
    std::cout << " | Model Name      : " << std::left << std::setw(42) << m_model << "|" << endl;
    std::cout << " | Family          : " << std::left << std::setw(42) << m_family << "|" << endl;
    std::cout << " | Stepping        : " << std::left << std::setw(42) << m_stepping << "|" << endl;
    std::cout << " | Cache Size      : " << std::left << std::setw(42) << m_cacheSize << "|" << endl;
    std::cout << " | CPU Cores       : " << std::left << std::setw(42) << m_cpuCores << "|" << endl;
    for (loop = 0; loop < m_processer; loop++)
    {
        std::cout << " | Frequency(" << loop << ")    : " << std::left << std::setw(42) << GetFrequency(loop) << "|" << endl;
    }
    std::cout << " | Temperature     : " << std::left << std::setw(42) << GetTemperature() << "|" << endl;
    std::cout << " | " <<std::left << std::setw(60) << " " << "|" << std::endl;
    std::cout                << " ---------------------------------------------------------------" << std::endl;
}
