#ifndef __PERF_TOOL_GT_MEM_H__
#define __PERF_TOOL_GT_MEM_H__

#include <cstdint>
#include <fstream>

class GTMemory
{
public:
    GTMemory();
    virtual ~GTMemory();

    uint32_t GetTotalSize() { UpdateData(); return m_total; }
    uint32_t GetBoundedSize() { UpdateData(); return m_bounded; }
    uint32_t GetBufferCount() { UpdateData(); return m_bufferCount; }
    uint32_t GetEdramSize() const { return m_edram; }
    
    void Dump();
    
protected:
    void UpdateData();
    void ReadTotalSize(const std::string& rawString);
    void ReadBoundedSize(const std::string& rawString);

private:    
    static const std::string TOTAL_MEM_PREFIX;
    static const std::string AVAILABLE_MEM_PREFIX;
    
    uint32_t m_total       = 0;
    uint32_t m_bounded     = 0;
    uint32_t m_edram       = 0;
    uint32_t m_bufferCount = 0;
    
    std::ifstream m_ifs;
};

#endif
