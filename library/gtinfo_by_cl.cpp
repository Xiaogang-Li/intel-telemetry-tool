#include "gtinfo_by_cl.h"
#include "clinfo.h"

GtInfoByCl::GtInfoByCl()
{
    ClInfo clInfo(CL_DEVICE_TYPE_GPU);
    if (clInfo.IsValid())
    {
        m_euNum = clInfo.GetClDevAttr<cl_uint>(CL_DEVICE_MAX_COMPUTE_UNITS);
        m_maxMemorySize = clInfo.GetClDevAttr<cl_ulong>(CL_DEVICE_GLOBAL_MEM_SIZE) / 1024 / 1024;
        m_profilingTimerResolution = clInfo.GetClDevAttr<cl_float>(CL_DEVICE_PROFILING_TIMER_RESOLUTION);

        m_platform = clInfo.GetClDevStringAttr(CL_DEVICE_NAME);
        m_vendor = clInfo.GetClDevStringAttr(CL_DEVICE_VENDOR);

        m_maxFrequency = clInfo.GetClDevAttr<cl_uint>(CL_DEVICE_MAX_CLOCK_FREQUENCY);

        cl_device_fp_config fp = 0;
        fp = clInfo.GetClDevAttr<cl_device_fp_config>(CL_DEVICE_HALF_FP_CONFIG);
        clInfo.ParseFloatPoint(fp, m_halfFPSupportList);

        fp = 0;
        fp = clInfo.GetClDevAttr<cl_device_fp_config>(CL_DEVICE_SINGLE_FP_CONFIG);
        clInfo.ParseFloatPoint(fp, m_singleFPSupportList);

        fp = 0;
        fp = clInfo.GetClDevAttr<cl_device_fp_config>(CL_DEVICE_DOUBLE_FP_CONFIG);
        clInfo.ParseFloatPoint(fp, m_doubleFPSupportList);
    }
}
