#ifndef __PERF_TOOL_MEDIA_CAP_H__
#define __PERF_TOOL_MEDIA_CAP_H__

#include <va/va.h>
#include <vector>

class MediaCapability
{
public:
    MediaCapability();
    virtual ~MediaCapability();
    
    const std::vector<std::string>& GetDecodeCapability() const { return m_decodeCapability; }
    const std::vector<std::string>& GetEncodeCapability() const { return m_encodeCapability; }
    const std::vector<std::string>& GetVppCapability() const { return m_vpCapability; }
    const std::string& GetDriverVersion() const { return m_driverVersion; }
    
    void Dump();
private:
    VADisplay OpenDisplay();
    void      CloseDisplay();
    
    bool      InitMediaDriver();
    void      UninitMediaDriver();

    const std::string& GetCodecFromProfile(const std::string& profile);

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
    void QueryDecodeCaps(const std::vector<Capability *> &capability);
    void QueryEncodeCaps(const std::vector<Capability *> &capability);
    void QueryVpCaps();
private:
    static const std::string m_devicePath;
    
    int32_t   m_drmFd   = -1;
    VADisplay m_display = nullptr;
    
    int32_t   m_majorVersion = 0;
    int32_t   m_minorVersion = 0;
    
    std::string m_driverVersion = "";
    
    std::vector<std::string> m_decodeCapability;
    std::vector<std::string> m_encodeCapability;
    std::vector<std::string> m_vpCapability;
};
#endif
