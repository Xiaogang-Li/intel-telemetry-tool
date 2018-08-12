#ifndef __PERF_TOOL_MEDIA_CAP_H__
#define __PERF_TOOL_MEDIA_CAP_H__

#include <va/va.h>
#include <vector>

class MediaCapability
{
public:
    MediaCapability();
    virtual ~MediaCapability();
    
    
    struct Capability
    {
        std::string profile;
        std::string entrypoint;
        
        Capability(std::string profile, std::string entrypoint)
        {
            this->profile    = profile;
            this->entrypoint = entrypoint;
        }
    };
    
    const std::vector<Capability *>& GetCapability() const { return m_capability; }
    
    void Dump();
private:
    VADisplay OpenDisplay();
    void      CloseDisplay();
    
    bool      InitMediaDriver();
    void      UninitMediaDriver();
private:
    static const std::string m_devicePath;
    
    int32_t   m_drmFd   = -1;
    VADisplay m_display = nullptr;
    
    int32_t   m_majorVersion = 0;
    int32_t   m_minorVersion = 0;
    
    std::string m_driverVersion = "";
    
    std::vector<Capability *> m_capability;
};
#endif
