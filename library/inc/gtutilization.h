#ifndef __PERF_TOOL_GT_UTIL_H__
#define __PERF_TOOL_GT_UTIL_H__

#include <fstream>
#include <map>

#define INTEM_MMIO_GT_MEM       0x145040
#define INTEM_MMIO_IA_MEM       0x145044
#define INTEM_MMIO_IO_MEM       0x145048

#define INTEM_MMIO_RING_RCS     0x2030
#define INTEM_MMIO_RING_VCS     0x12030
#define INTEM_MMIO_RING_VCS2    0x1C030
#define INTEM_MMIO_RING_VECS    0x1A030
#define INTEM_MMIO_RING_BCS     0x22030

#define RING_TAIL      0x00
#define RING_HEAD      0x04
#define HEAD_ADDR      0x001FFFFC
#define TAIL_ADDR      0x000FFFF8

struct mmio_ring {
    int id;
    uint32_t base;
    void *mmio;
    int idle, wait, sema;
};

class GtUtil
{
public:
    enum UsageTag
    {
	RENDER,
	CODEC1,
	CODEC2,
	CODEC3,
	CODEC4,
	CODEC5,
	CODEC6,
	CODEC7,
	CODEC8,
	VE1,
	VE2,
	BLT,
	TOTALMB,
	GTMB,
	IAMB,
	IOMB
    };

    GtUtil();
    ~GtUtil();

    void GetGpuUtilization(std::map<UsageTag, float> &utils);    

};

#endif
