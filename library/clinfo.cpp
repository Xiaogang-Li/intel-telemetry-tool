#include "clinfo.h"

ClInfo::ClInfo(cl_device_type devType)
{
    std::vector<cl_platform_id> platforms;
    std::vector<cl_device_id>   devices;
    cl_device_id                device;

    GetPlatforms(platforms);
    
    for (auto platform : platforms)
    {
	GetDevices(platform, devices, devType);
	if (devices.size() > 0 )
	{
	    m_device = devices[0];
	    m_inited = true;
	    break;
	}

	devices.clear();
    }

    platforms.clear();

}

ClInfo::~ClInfo()
{
    m_inited = false;
}

std::string ClInfo::GetClDevStringAttr(const cl_device_info &param) 
{                                                                                       
    char        *value;                                                                 
    size_t      valueSize;                                                              
    char        name[256];                                                              

    if (!m_inited)
    {
	return "";
    }

    clGetDeviceInfo(m_device, param, 256, name, &valueSize);                              
    std::string result(name);                                                           
                                                                                        
    return result;                                                                      
}

bool ClInfo::GetPlatforms(std::vector<cl_platform_id> &platforms)   
{                                                                                               
    cl_uint numPlatforms = 0;                                                                   
    cl_platform_id pfs[32];                                                                     
                                                                                                
    cl_int status = clGetPlatformIDs(32, pfs, &numPlatforms);                                   
    if (status != CL_SUCCESS)                                                                   
    {                                                                                           
        return false;                                                                           
    }                                                                                           

    for (int i = 0; i < numPlatforms; i++)                                                      
    {                                                                                           
        platforms.push_back(pfs[i]);                                                            
    }                                                                                           
    return true;                                                                                
}                                                                                               
                                                                                                
bool ClInfo::GetDevices(const cl_platform_id &platform,
        std::vector<cl_device_id>            &devices,
	cl_device_type                       devType)                                                     
{                                                                                               
    cl_uint numDevices = 0;                                                                     
    cl_device_id tempDevices[32];                                                               
                                                                                                
    cl_int status = clGetDeviceIDs(platform, devType, 32, tempDevices, &numDevices); 
    if (status != CL_SUCCESS)                                                                   
    {                                                                                           
        return false;                                                                           
    }                                                                                           
                                                                                                
    for (int i = 0; i < numDevices; i++)                                                        
    {                                                                                           
        devices.push_back(tempDevices[i]);                                                      
    }                                                                                           
    return true;                                                                                
}

#define SUPPORT_FEATURE(fp, feature) ((fp & feature) == feature)
const cl_device_fp_config ClInfo::CL_FP_MAX = CL_FP_DENORM | CL_FP_INF_NAN |
    CL_FP_ROUND_TO_NEAREST | CL_FP_ROUND_TO_ZERO | CL_FP_ROUND_TO_INF |
    CL_FP_FMA | CL_FP_SOFT_FLOAT | CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT;
void ClInfo::ParseFloatPoint(const cl_device_fp_config &fp,
	std::vector<std::string> &enableVector)
{
    static std::map<int, std::string> map;
    map.emplace(std::make_pair(CL_FP_DENORM, "DENORM"));
    map.emplace(std::make_pair(CL_FP_INF_NAN, "INF_NAN"));
    map.emplace(std::make_pair(CL_FP_ROUND_TO_NEAREST, "ROUND_TO_NEAREST"));
    map.emplace(std::make_pair(CL_FP_ROUND_TO_ZERO, "ROUND_TO_ZERO"));
    map.emplace(std::make_pair(CL_FP_ROUND_TO_INF, "ROUND_TO_INF"));
    map.emplace(std::make_pair(CL_FP_FMA, "FMA IEEE754-2008"));
    map.emplace(std::make_pair(CL_FP_SOFT_FLOAT, "SOFT_FLOAT"));
    map.emplace(std::make_pair(CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT, "CORRECTLY_ROUNDED_DIVIDE_SQRT"));

    if (fp > CL_FP_MAX)
    {
	return;
    }

    for (auto pair: map)
    {
	if (SUPPORT_FEATURE(fp, pair.first))
	{
	    enableVector.push_back(pair.second);
	}
    }
}

