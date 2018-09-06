#ifndef __CL_INFO_H__
#define __CL_INFO_H__

#include <CL/cl.h>
#include <map>
#include <vector>

#ifndef CL_DEVICE_HALF_FP_CONFIG
#define CL_DEVICE_HALF_FP_CONFIG 0x1033
#endif


class ClInfo
{
public:
    ClInfo(cl_device_type devType);
    virtual ~ClInfo();

    bool IsValid() const { return m_inited; }

    template <class Type>                                                     
    Type GetClDevAttr(const cl_device_info &param)
    {                                                                                       
        Type value;
    
        if (!m_inited)
        {
    	return 0;
        }
    
        clGetDeviceInfo(m_device, param, sizeof(Type), &value, NULL);                         
                                                                                            
        return value;                                                                       
    }
    std::string GetClDevStringAttr(const cl_device_info &param);

    void ParseFloatPoint(const cl_device_fp_config &fp, std::vector<std::string> &enableVector);
private:
    bool GetPlatforms(std::vector<cl_platform_id> &platforms);
    bool GetDevices(const cl_platform_id &platform,
	std::vector<cl_device_id>        &devices,
	cl_device_type                   devType);

private:
    static const cl_device_fp_config CL_FP_MAX;
    cl_device_id m_device;
    bool         m_inited = false;
};

#endif
