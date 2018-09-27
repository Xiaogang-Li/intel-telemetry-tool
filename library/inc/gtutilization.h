#ifndef __PERF_TOOL_GT_UTIL_H__
#define __PERF_TOOL_GT_UTIL_H__

#include <fstream>
#include <map>
#include <vector>
#include <thread>
#include <pciaccess.h>
#include "intem.h"

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

struct GfxInfo
{
    int gen;
};

using namespace std;

using PciDevice = pci_device;
using PciDeviceIterator = pci_device_iterator;

class GtUtilization
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

    GtUtilization();
    virtual ~GtUtilization();

    void GetGpuUtilization(std::map<UsageTag, float> &utils);    
    void GetUtilization(vector<intem::GtUtil> &utils);    
    void GetBandwidth(vector<intem::GtMemBandwidth> &bandwidths);

private:
    PciDevice *GetPciDevice();
    const GfxInfo *GetGfxInfo(PciDevice *device);
    void GetMmioIndexAndSize(const int &gen, int &index, int &size);
    void *GetMmioAddr(PciDevice *device);
    void ReturnMmioSpace(PciDevice *device);
    void Forcewake();
    void Sampling();

    inline uint32_t ReadMmio(void *mmio, uint32_t reg)
    {
    	return *(volatile uint32_t *)((volatile char *)mmio + reg);
    }


private:
    static constexpr uint32_t m_mmioMemGT = 0x145040;
    static constexpr uint32_t m_mmioMemIA = 0x145044;
    static constexpr uint32_t m_mmioMemIO = 0x145048;

    static constexpr uint32_t m_mmioRingRcs  = 0x2030;
    static constexpr uint32_t m_mmioRingVcs  = 0x12030;
    static constexpr uint32_t m_mmioRingVcs2 = 0x1C030;
    static constexpr uint32_t m_mmioRingVecs = 0x1A030;
    static constexpr uint32_t m_mmioRingBcs  = 0x22030;

    static constexpr uint32_t m_ringTailOffset = 0x00;
    static constexpr uint32_t m_ringHeadOffset = 0x04;
    static constexpr uint32_t m_ringTailMask = 0x000FFFF8;
    static constexpr uint32_t m_ringHeadMask = 0x001FFFFC;

    PciDevice *m_device = nullptr;
    void      *m_mmio   = nullptr;

    thread    *m_sampleThread = nullptr;
    bool      m_quitThread    = false;

    float m_utilCompute  = 0.0;
    float m_utilFFCodec1 = 0.0;
    float m_utilFFCodec2 = 0.0;
    float m_utilFFVp1    = 0.0;

    float m_mbGT         = 0.0;
    float m_mbIA         = 0.0;
    float m_mbIO         = 0.0;
};

#endif
