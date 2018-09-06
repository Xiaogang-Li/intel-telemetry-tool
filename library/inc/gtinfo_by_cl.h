#ifndef __GT_INFO_BY_CL_H__
#define __GT_INFO_BY_CL_H__

#include <memory>
#include "gtinfo.h"

using namespace std;

class GtInfoByCl: public GtInfo
{
public:
    GtInfoByCl();
    virtual ~GtInfoByCl() {}

    virtual uint32_t GetMaxFrequency() { return m_maxFrequency; }

private:
    uint32_t m_maxFrequency = 0;
};

#endif
