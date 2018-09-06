#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <fcntl.h>
#include <iomanip>
#include <algorithm>
#include <map>
#include <va/va_drm.h>
#include <va/va_str.h>
#include <va/va_vpp.h>
#include "mediacapability.h"

const std::string MediaCapability::m_devicePath = "/dev/dri/card0";

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

MediaCapability::MediaCapability()
{
    OpenDisplay();
    InitMediaDriver();
}

MediaCapability::~MediaCapability()
{
    m_decodeCapability.clear();
    m_encodeCapability.clear();
    m_vpCapability.clear();
}

VADisplay MediaCapability::OpenDisplay()
{
    m_drmFd = open(m_devicePath.c_str(), O_RDONLY);
    if (m_drmFd < 0) 
    {
        std::cout << "Failed to open the given device!" << std::endl;
        return nullptr;
    }
    
    m_display = vaGetDisplayDRM(m_drmFd);
    if (m_display != nullptr)
    {
        return m_display;
    }
    
    return nullptr;
}

void MediaCapability::CloseDisplay()
{
    if (m_drmFd < 0)
    {
        return;
    }
    
    close(m_drmFd);
    m_drmFd = -1;
}

bool MediaCapability::InitMediaDriver()
{
    VAStatus     vaStatus;
    int32_t      numEntrypoint = 0;
    int32_t      loopEntyPoint = 0;
    VAEntrypoint *entrypoints  = NULL;
    
    int32_t      numProfiles    = 0;
    int32_t      maxNumProfiles = 0;
    int32_t      loopProfile    = 0;
    VAProfile    *profileList   = NULL;
    VAProfile    profile;
  
    if (m_display == nullptr)
    {
        return false;
    }
    
    vaSetInfoCallback(m_display, NULL, NULL);
    vaSetErrorCallback(m_display, NULL, NULL);
    vaStatus = vaInitialize(m_display, &m_majorVersion, &m_minorVersion);
    if (vaStatus != VA_STATUS_SUCCESS)
    {
        return false;
    }

    
    m_driverVersion = vaQueryVendorString(m_display);
    
    numEntrypoint = vaMaxNumEntrypoints(m_display);
    entrypoints   = (VAEntrypoint *)malloc(numEntrypoint * sizeof (VAEntrypoint));
    if (!entrypoints) 
    {
        std::cout <<"Failed to allocate memory for entrypoint list" << std::endl;
        return false;
    }
    
    maxNumProfiles = vaMaxNumProfiles(m_display);
    profileList    = (VAProfile *)malloc(maxNumProfiles * sizeof(VAProfile));
    
    if (!profileList)
    {
        std::cout << "Failed to allocate memory for profile list" << std::endl;
        free(entrypoints);
        return false;
    }
    
    vaStatus = vaQueryConfigProfiles(m_display, profileList, &numProfiles);
    if (vaStatus != VA_STATUS_SUCCESS)
    {
        free(entrypoints);
        free(profileList);
        return false;
    }

    std::vector<Capability *> capability;
    
    for (loopProfile = 0; loopProfile < numProfiles; loopProfile++)
    {
        profile = profileList[loopProfile];
        vaStatus = vaQueryConfigEntrypoints(m_display, profile, entrypoints, 
                                            &numEntrypoint);
        if (vaStatus == VA_STATUS_ERROR_UNSUPPORTED_PROFILE)
	        continue;

        if (vaStatus != VA_STATUS_SUCCESS)
        {
            free(entrypoints);
            free(profileList);
            return false;
        }

        for (loopEntyPoint = 0; loopEntyPoint < numEntrypoint; loopEntyPoint++)
        {
            capability.push_back(new Capability(vaProfileStr(profile), 
                                                  vaEntrypointStr(entrypoints[loopEntyPoint])));
        }
    }

    QueryDecodeCaps(capability);
    QueryEncodeCaps(capability);
    QueryVpCaps();


    for(auto cap : capability)
    {
        delete cap;
    }
    
    capability.clear();

    free(entrypoints);
    free(profileList);

    UninitMediaDriver();
    CloseDisplay();
}

void MediaCapability::QueryDecodeCaps(const std::vector<Capability *> &capability)
{
    std::vector<std::string>::iterator iter;
    for (auto cap : capability)
    {

        if (cap->entrypoint.find("VAEntrypointVLD") != std::string::npos)
        {
            const std::string& codec = GetCodecFromProfile(cap->profile);
            iter = std::find(m_decodeCapability.begin(), m_decodeCapability.end(), codec);
            if (iter == m_decodeCapability.end())
            {
                m_decodeCapability.push_back(codec);
            }
        }
    }
}


void MediaCapability::QueryEncodeCaps(const std::vector<Capability *> &capability)
{
    std::vector<std::string>::iterator iter;
    for (auto cap : capability)
    {
        bool supportEncSliceLP = cap->entrypoint.find("VAEntrypointEncSliceLP") != std::string::npos;
        bool supportEncSlice = cap->entrypoint.find("VAEntrypointEncSlice") != std::string::npos;
        bool supportEncPicture = cap->entrypoint.find("VAEntrypointEncPicture") != std::string::npos;
        std::string postStr = supportEncSliceLP ? "  (Low Power Slice Level)" : 
                              (supportEncSlice ? "  (Slice Level)" : "  (Picture Level)");
        if (supportEncSlice || supportEncSliceLP || supportEncPicture)
        {
            const std::string& codec = GetCodecFromProfile(cap->profile);
            iter = std::find(m_encodeCapability.begin(), m_encodeCapability.end(), codec + postStr);
            if (iter == m_encodeCapability.end())
            {
                m_encodeCapability.push_back(codec + postStr);
            }
        }
    }
}

void MediaCapability::QueryVpCaps()
{
    VAStatus         vaStatus;
    VAProcFilterType filters[VAProcFilterCount];

    uint32_t  num_filters = VAProcFilterCount;
    uint32_t  i           = 0;

    std::map<VAProcFilterType, std::string> filterNameMap;
    filterNameMap.emplace(std::make_pair(VAProcFilterNoiseReduction,       "Noise Reduction"));
    filterNameMap.emplace(std::make_pair(VAProcFilterDeinterlacing,        "Deinterlaceing"));
    filterNameMap.emplace(std::make_pair(VAProcFilterSharpening,           "Sharpning"));
    filterNameMap.emplace(std::make_pair(VAProcFilterColorBalance,         "Color Balance"));
    filterNameMap.emplace(std::make_pair(VAProcFilterSkinToneEnhancement,  "Skintone Enhancement"));
    filterNameMap.emplace(std::make_pair(VAProcFilterTotalColorCorrection, "Total Color Correction"));

    if (m_display == nullptr)
    {   
        return;
    }   

    vaStatus = vaQueryVideoProcFilters(m_display, 0, filters, &num_filters);
    if (vaStatus != VA_STATUS_SUCCESS)
    {   
	printf("failed to get the vpp filter\n");
        return;
    }   


    for (i = 0; i < num_filters; i++)
    {   
        auto it = filterNameMap.find(filters[i]);
	if (it != filterNameMap.end())
	{
	    m_vpCapability.push_back(it->second);
	}
    }   

    return;
}

const std::string& MediaCapability::GetCodecFromProfile(const std::string& profile)
{
    static std::string codecs[] = {" ","MPEG2", "H264", "VC1", "HEVC", "VP8", "VP9", "JPEG"};

    for (uint32_t loop = 0; loop < sizeof(codecs); loop++)
    {
        if (profile.find(codecs[loop]) != std::string::npos)
        {
            return codecs[loop];
        }
    }


    return codecs[0];
}

void MediaCapability::UninitMediaDriver()
{
    if (m_display != nullptr)
    {
        vaTerminate(m_display);
    }
}

void MediaCapability::Dump()
{
    int i = 1;

    const std::vector<std::string>& decCaps = GetDecodeCapability();
    const std::vector<std::string>& encCaps = GetEncodeCapability();
    const std::vector<std::string>& vppCaps = GetVppCapability();
    
    std::cout <<  "\33[" << ++i <<";66H" 
              << std::left << std::setw(2) << " "
              << "+----------------------- Media Infomation -----------------+" << "\033[0m" << std::endl;
    
    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << std::left << std::setw(58) << " " << "|" << "\033[0m" << std::endl;

    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << " Driver Version: " << std::left << std::setw(41) << GetDriverVersion() << "|" << "\033[0m" << std::endl;

    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << std::left << std::setw(58) << " " << "|" << "\033[0m" << std::endl;


    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << "-------------------- Capability --------------------------"
              << "|" << "\033[0m" << std::endl;

    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << std::left << std::setw(58) << " " << "|" << "\033[0m" << std::endl;
    
    bool titlePrinted = false;
    for (auto cap : decCaps)
    {
        if (titlePrinted)
        {
            std::cout << "\33[" << ++i <<";66H" << std::left << std::setw(2) << " " << "| "
                  << std::left << std::setw(23) << " " << ": " 
                  << std::left << std::setw(32)
                  << cap     << "|" << "\033[0m" << std::endl; 
        }
        else
        {
            std::cout << "\33[" << ++i <<";66H" << std::left << std::setw(2) << " " << "| "
                  << std::left << std::setw(23) << "Decode" << ": " 
                  << std::left << std::setw(32)
                  << cap     << "|" << "\033[0m" << std::endl; 
            titlePrinted = true;
        }
        
    }

    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << "----------------------------------------------------------"
              << "|" << "\033[0m" << std::endl;
    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << std::left << std::setw(58) << " " << "|" << "\033[0m" << std::endl;

    titlePrinted = false;
    for (auto cap : encCaps)
    {
        if (titlePrinted)
        {
            std::cout << "\33[" << ++i <<";66H" << std::left << std::setw(2) << " " << "| "
                  << std::left << std::setw(23) << " " << ": " 
                  << std::left << std::setw(32)
                  << cap     << "|" << "\033[0m" << std::endl; 
        }
        else
        {
            std::cout << "\33[" << ++i <<";66H" << std::left << std::setw(2) << " " << "| "
                  << std::left << std::setw(23) << "Encode" << ": " 
                  << std::left << std::setw(32)
                  << cap     << "|" << "\033[0m" << std::endl; 
            titlePrinted = true;
        }
        
    }
    
    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << "----------------------------------------------------------"
              << "|" << "\033[0m" << std::endl;
    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << std::left << std::setw(58) << " " << "|" << "\033[0m" << std::endl;
    titlePrinted = false;
    for (auto cap : vppCaps)
    {
        if (titlePrinted)
        {
            std::cout << "\33[" << ++i <<";66H" << std::left << std::setw(2) << " " << "| "
                  << std::left << std::setw(23) << " " << ": " 
                  << std::left << std::setw(32)
                  << cap     << "|" << "\033[0m" << std::endl; 
        }
        else
        {
            std::cout << "\33[" << ++i <<";66H" << std::left << std::setw(2) << " " << "| "
                  << std::left << std::setw(23) << "VPP" << ": " 
                  << std::left << std::setw(32)
                  << cap     << "|" << "\033[0m" << std::endl; 
            titlePrinted = true;
        }
    }

    std::cout <<  "\33[" << ++i <<";66H" 
              << std::left << std::setw(2) << " " << "|"
              << std::left << std::setw(58) << " " << "|" << "\033[0m" << std::endl;

    std::cout << "\33[" << ++i <<";66H" << std::left << std::setw(2) << " "
              << "+----------------------------------------------------------+" << "\033[0m" << std::endl;
}
