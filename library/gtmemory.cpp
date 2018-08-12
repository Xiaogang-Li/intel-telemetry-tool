#include <iostream>
#include <sstream>
#include <exception>
#include <iomanip>
#include "util.h"
#include "gtmemory.h"

using namespace std;

const std::string GTMemory::TOTAL_MEM_PREFIX     = "objects";
const std::string GTMemory::AVAILABLE_MEM_PREFIX = "bound objects";

GTMemory::GTMemory()
{
    try
    {
        m_ifs.open("/sys/kernel/debug/dri/0/i915_gem_objects", std::ifstream::in);
        
        UpdateData();
    }
    catch(exception& e)
    {   
        std::cerr << "******** Error when get GT Memory Information ********" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "******************************************************" << std::endl;
    }
}

GTMemory::~GTMemory()
{
    m_ifs.close();
}

void GTMemory::UpdateData()
{
    std::string   line = "";

    try
    {
        m_ifs.seekg (0, m_ifs.beg);
        if (!m_ifs.good())
        {
            m_ifs.close();
            m_ifs.open("/sys/kernel/debug/dri/0/i915_gem_objects", std::ifstream::in);
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "******** Error when get Sys Memory Information ********" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "*******************************************************" << std::endl;
    }

    m_total = 0;
    while( getline(m_ifs, line) )
    {
        ReadTotalSize(line);
        ReadBoundedSize(line);
    }
}

void GTMemory::ReadTotalSize(const std::string& rawString)
{  
    if (Util::IsInclude(rawString, TOTAL_MEM_PREFIX) && m_total == 0)
    {
        m_total = Util::GetDataFromRaw<uint32_t>(rawString, 2);
        
        m_total /= 1024;
    }
}

void GTMemory::ReadBoundedSize(const std::string& rawString)
{
    if (Util::IsInclude(rawString, AVAILABLE_MEM_PREFIX))
    {
        m_bounded = Util::GetDataFromRaw<uint32_t>(rawString, 3);
        m_bounded /= 1024;
        
        m_bufferCount = Util::GetDataFromRaw<uint32_t>(rawString);
    }
}


void GTMemory::Dump()
{
    std::cout << std::endl << " -------------------------- GT Memory --------------------------" << std::endl;
    std::cout << " |" << std::left << std::setw(61) << " " << "|" << std::endl;

    std::cout << " | " << "Total GT Size : " << GetTotalSize() << std::left << std::setw(38) << " kB" << "|" << std::endl;
    std::cout << " | " << "Used GT Memory: " << GetBoundedSize() << std::left << std::setw(38) << " kB" << "|" << std::endl;
    std::cout << " | " << "Buffer Count  : " << std::left << std::setw(44) << GetBufferCount() << "|" << std::endl;
    
    std::cout << " |" << std::left << std::setw(61) << " " << "|" << std::endl;
    std::cout              << " ---------------------------------------------------------------" << std::endl;
}
