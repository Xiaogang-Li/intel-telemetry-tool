#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <fcntl.h>
#include <iomanip>
#include <va/va_drm.h>
#include <va/va_str.h>
#include "mediacapability.h"

const std::string MediaCapability::m_devicePath = "/dev/dri/card0";

MediaCapability::MediaCapability()
{
    OpenDisplay();
    InitMediaDriver();
}

MediaCapability::~MediaCapability()
{
    UninitMediaDriver();
    CloseDisplay();
    
    for(auto cap : m_capability)
    {
        delete cap;
    }
    
    m_capability.clear();
}

VADisplay MediaCapability::OpenDisplay()
{
    m_drmFd = open(m_devicePath.c_str(), O_RDWR);
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
            m_capability.push_back(new Capability(vaProfileStr(profile), 
                                                  vaEntrypointStr(entrypoints[loopEntyPoint])));
        }
    }
    
    free(entrypoints);
    free(profileList);
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
    const std::vector<Capability *>& caps = GetCapability();
    
    std::cout <<  "\33[" << ++i <<";66H" 
              << std::left << std::setw(2) << " "
              << "-------------------------- Media Capability ----------------" << "\033[0m" << std::endl;
    
    std::cout <<  "\33[" << ++i <<";66H"
              << std::left << std::setw(2) << " " << "|"
              << std::left << std::setw(58) << " " << "|" << "\033[0m" << std::endl;

    for (auto cap : caps)
    {
        std::cout << "\33[" << ++i <<";66H" << std::left << std::setw(2) << " " << "| "
                  << std::left << std::setw(32) 
                  << cap->profile << ": " 
                  << std::left << std::setw(23)
                  << cap->entrypoint     << "|" << "\033[0m" << std::endl; 
    }
    
    std::cout <<  "\33[" << ++i <<";66H" 
              << std::left << std::setw(2) << " " << "|"
              << std::left << std::setw(58) << " " << "|" << "\033[0m" << std::endl;

    std::cout << "\33[" << ++i <<";66H" << std::left << std::setw(2) << " "
              << "------------------------------------------------------------" << "\033[0m" << std::endl;
}
