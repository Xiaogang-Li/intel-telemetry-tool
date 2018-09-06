#include <iostream>
#include <iomanip>
#include <exception>
#include "util.h"
#include "sysmemory.h"

const std::string SysMemory::TOTAL_MEM_PREFIX     = "MemTotal:";
const std::string SysMemory::FREE_MEM_PREFIX      = "MemFree:";
const std::string SysMemory::AVAILABLE_MEM_PREFIX = "MemAvailable:";
const std::string SysMemory::CACHED_MEM_PREFIX    = "Cached:";

const std::string SysMemory::SYSTEM_VERSION_FILE  = "/proc/sys/kernel/osrelease";

SysMemory::SysMemory()
{   
    try
    {
        m_ifs.open("/proc/meminfo", std::ifstream::in);
        UpdateData();

        ReadSystemVersion();
    }
    catch(std::exception& e)
    {
        std::cerr << "******** Error when Open Sys Memory Information ********" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "*******************************************************" << std::endl;
    }
}

SysMemory::~SysMemory()
{
    m_ifs.close();
}

void SysMemory::ReadSystemVersion()
{
    std::ifstream ifs;
    try
    {
        ifs.open(SYSTEM_VERSION_FILE);

        ifs >> m_systemVersion;
    }
    catch(...)
    {
        m_systemVersion = "";
    }
}

void SysMemory::UpdateData()
{
    std::string   line = "";

    try
    {
        m_ifs.seekg (0, m_ifs.beg);
        if (!m_ifs.good())
        {
            m_ifs.close();
            m_ifs.open("/proc/meminfo", std::ifstream::in);
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "******** Error when get Sys Memory Information ********" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "*******************************************************" << std::endl;
    }

    m_cached = 0;
    while( getline(m_ifs, line) )
    {
        ReadTotalSize(line);
        ReadAvailableSize(line);
        ReadFreeSize(line);
        ReadCachedSize(line);
    }
}

void SysMemory::ReadTotalSize(const std::string& rawString)
{
    if (Util::IsInclude(rawString, TOTAL_MEM_PREFIX))
    {
        m_total = Util::GetDataFromRaw<uint32_t>(rawString, 1);
    }
}

void SysMemory::ReadAvailableSize(const std::string& rawString)
{
    if (Util::IsInclude(rawString, AVAILABLE_MEM_PREFIX))
    {
        m_available = Util::GetDataFromRaw<uint32_t>(rawString, 1);
    }
}

void SysMemory::ReadFreeSize(const std::string& rawString)
{
    if (Util::IsInclude(rawString, FREE_MEM_PREFIX))
    {
        m_free = Util::GetDataFromRaw<uint32_t>(rawString, 1);
    }
}

void SysMemory::ReadCachedSize(const std::string& rawString)
{
    if (Util::IsInclude(rawString, CACHED_MEM_PREFIX) && m_cached == 0)
    {
        m_cached = Util::GetDataFromRaw<uint32_t>(rawString, 1);
    }
}

void SysMemory::Dump()
{
    std::cout << std::endl << " +----------------------- System Infomation -------------------+" << std::endl;
    std::cout << " |"<< std::left << std::setw(61) << " " << "|" << std::endl;
    std::cout << " | "<< "Linux Version   : " << std::left << std::setw(42) << GetSystemVersion() << "|" << std::endl;
    std::cout << " |"<< std::left << std::setw(61) << " " << "|" << std::endl;
    std::cout << " | "<< "Memory Total    : " << GetTotalSize()     << std::left << std::setw(34) << " kB" << "|" << std::endl;
    std::cout << " | "<< "Memory Available: " << GetAvailableSize() << std::left << std::setw(34) << " kB" << "|" << std::endl;
    std::cout << " | "<< "Memory Free     : " << GetFreeSize()      << std::left << std::setw(34) << " kB" << "|" << std::endl;
    std::cout << " | "<< "Memory Cached   : " << std::left << std::setw(8) << GetCachedSize() << std::left << std::setw(34) << " kB" << "|" << std::endl;
    std::cout << " | ";
    std::cout.setf(std::ios::fixed);
    std::cout << std::setprecision(2) << "Memory Usage    : " << std::left << std::setw(9) << GetUsage() * 100 << std::left << std::setw(33) << "%" << "|" << std::endl;
    std::cout << " |"<< std::left << std::setw(61) << " " << "|" << std::endl;
    std::cout << " +-------------------------------------------------------------+" << std::endl;
}
