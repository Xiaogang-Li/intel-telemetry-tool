#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>

#include "gtutilization.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "i915_pciids.h"

#ifdef __cplusplus
}
#endif

#define SAMPLES_PER_SEC 10000

static const struct GfxInfo generic_info =
{
    .gen = -1,
};

static const struct GfxInfo i830_info =
{
    .gen = 020,
};
static const struct GfxInfo i845_info =
{
    .gen = 020,
};
static const struct GfxInfo i855_info =
{
    .gen = 021,
};
static const struct GfxInfo i865_info =
{
    .gen = 022,
};

static const struct GfxInfo i915_info =
{
    .gen = 030,
};
static const struct GfxInfo i945_info =
{
    .gen = 031,
};

static const struct GfxInfo g33_info =
{
    .gen = 033,
};

static const struct GfxInfo i965_info =
{
    .gen = 040,
};

static const struct GfxInfo g4x_info =
{
    .gen = 045,
};

static const struct GfxInfo ironlake_info =
{
    .gen = 050,
};

static const struct GfxInfo sandybridge_info =
{
    .gen = 060,
};

static const struct GfxInfo ivybridge_info =
{
    .gen = 070,
};

static const struct GfxInfo valleyview_info =
{
    .gen = 071,
};

static const struct GfxInfo haswell_info =
{
    .gen = 075,
};

static const struct GfxInfo broadwell_info =
{
    .gen = 0100,
};

static const struct GfxInfo cherryview_info =
{
    .gen = 0101,
};

static const struct GfxInfo skylake_info =
{
    .gen = 0110,
};

static const struct GfxInfo broxton_info =
{
    .gen = 0111,
};

static const struct GfxInfo kabylake_info =
{
    .gen = 1000,
};

static const struct GfxInfo coffeelake_info =
{
    .gen = 1001,
};

static const struct GfxInfo icelake_info =
{
    .gen = 1010,
};

static const struct pci_id_match PCIIDXS[] =
{
    INTEL_I830_IDS(&i830_info),
    INTEL_I845G_IDS(&i845_info),
    INTEL_I85X_IDS(&i855_info),
    INTEL_I865G_IDS(&i865_info),

    INTEL_I915G_IDS(&i915_info),
    INTEL_I915GM_IDS(&i915_info),
    INTEL_I945G_IDS(&i945_info),
    INTEL_I945GM_IDS(&i945_info),

    INTEL_G33_IDS(&g33_info),
    INTEL_PINEVIEW_IDS(&g33_info),

    INTEL_I965G_IDS(&i965_info),
    INTEL_I965GM_IDS(&i965_info),

    INTEL_G45_IDS(&g4x_info),
    INTEL_GM45_IDS(&g4x_info),

    INTEL_IRONLAKE_D_IDS(&ironlake_info),
    INTEL_IRONLAKE_M_IDS(&ironlake_info),

    INTEL_SNB_D_IDS(&sandybridge_info),
    INTEL_SNB_M_IDS(&sandybridge_info),

    INTEL_IVB_D_IDS(&ivybridge_info),
    INTEL_IVB_M_IDS(&ivybridge_info),

    INTEL_HSW_IDS(&haswell_info),

    INTEL_VLV_IDS(&valleyview_info),

    INTEL_BDW_IDS(&broadwell_info),

    INTEL_CHV_IDS(&cherryview_info),

    INTEL_SKL_IDS(&skylake_info),
    INTEL_BXT_IDS(&broxton_info),
    INTEL_KBL_IDS(&kabylake_info),
    INTEL_CFL_IDS(&coffeelake_info),
    INTEL_ICL_11_IDS(&icelake_info),

    INTEL_VGA_DEVICE(PCI_MATCH_ANY, &generic_info),

    {0, 0, 0}

};

GtUtilization::GtUtilization()
{

    bool ret = true;
    try
    {
        do
        {
            if (pci_system_init())
            {
                ret = false;
                break;
            }

            m_device = GetPciDevice();
            if (m_device == nullptr)
            {
                ret = false;
                break;
            }

            m_mmio = GetMmioAddr(m_device);
            if (m_mmio == nullptr)
            {
                ret = false;
                break;
            }

            m_sampleThread = new thread(&GtUtilization::Sampling, this);

        } while(0);

    }
    catch (std::exception &e)
    {
        ret = false;
    }

    if (!ret)
    {
        if (m_mmio)
        {
            ReturnMmioSpace(m_device);
            m_mmio = nullptr;
        }

        pci_system_cleanup();
        std::cerr << "******** Error when init GT Utilization **************" << std::endl;
    }
}

GtUtilization::~GtUtilization()
{
    if (m_sampleThread != nullptr)
    {
        m_quitThread = true;

        m_sampleThread->join();

        delete m_sampleThread;
        m_sampleThread = nullptr;
    }
    if (m_mmio)
    {
        ReturnMmioSpace(m_device);
        m_mmio = nullptr;
    }

    pci_system_cleanup();
    return;
}

PciDevice *GtUtilization::GetPciDevice()
{
    PciDevice *dev = nullptr;

    dev = pci_device_find_by_slot(0, 0, 2, 0);
    if (dev == NULL || dev->vendor_id != 0x8086)
    {
        PciDeviceIterator *iter;

        iter = pci_id_match_iterator_create(PCIIDXS);
        if (!iter)
            return 0;

        dev = pci_device_next(iter);
        pci_iterator_destroy(iter);
    }

    return dev;
}

const GfxInfo *GtUtilization::GetGfxInfo(PciDevice *dev)
{
    int i;

    if (dev == nullptr)
    {
        return 0;
    }

    for (i = 0; PCIIDXS[i].device_id != PCI_MATCH_ANY; i++)
        if (dev->device_id == PCIIDXS[i].device_id)
            return (const GfxInfo *)PCIIDXS[i].match_data;

    return &generic_info;
}

void GtUtilization::GetMmioIndexAndSize(const int &gen, int &index, int &size)
{
    int mmio_bar, mmio_size;

    if (gen >> 3 == 2)
    {
        index = 1;
    }
    else
    {
        index = 0;
    }

    if (gen < 050)
    {
        size = 512 * 1024;
    }
    else
    {
        size = 2 * 1024 * 1024;
    }
}

void GtUtilization::Forcewake()
{
	char buf[1024];
    int forcewake = -1;
	const char *path[] = {
		"/sys/kernel/debug/dri/",
		"/debug/dri/",
		0,
	};
	int i, j;

	for (j = 0; path[j]; j++) {
		struct stat st;

		if (stat(path[j], &st))
			continue;

		for (i = 0; i < 16; i++) {
			snprintf(buf, sizeof(buf),
				 "%s/%i/i915_forcewake_user",
				 path[j], i);
			forcewake = open(buf, 0);
			if (forcewake != -1)
				return;
		}
	}
}

void *GtUtilization::GetMmioAddr(PciDevice *dev)
{
    const struct GfxInfo *info;
    int mmio_bar, mmio_size;
    void *mmio;

    if (dev == nullptr)
    {
        return nullptr;
    }

    info = GetGfxInfo(dev);

    GetMmioIndexAndSize(info->gen, mmio_bar, mmio_size);

    //if (info->gen >= 060)
    //{ 
    //    Forcewake();
    //}

    if (pci_device_probe(dev))
    {
        return nullptr;
    }

    if (pci_device_map_range(dev,
                             dev->regions[mmio_bar].base_addr,
                             mmio_size,
                             PCI_DEV_MAP_FLAG_WRITABLE,
                             &mmio))
    {
        return nullptr;
    }

    return mmio;
}

void GtUtilization::ReturnMmioSpace(PciDevice *dev)
{
    const struct GfxInfo *info;
    int mmio_bar, mmio_size;

    if (dev == nullptr)
    {
        return;
    }

    info = GetGfxInfo(dev);
    GetMmioIndexAndSize(info->gen, mmio_bar, mmio_size);

    pci_device_unmap_range(dev,
                           dev->regions[mmio_bar].memory,
                           mmio_size);
}

void GtUtilization::Sampling()
{
    PciDevice *device = nullptr;

    if (m_mmio == nullptr)
    {
        m_mmio = GetMmioAddr(device);

        if (m_mmio == nullptr)
        {
            return;
        }
    }

    
    uint64_t ring_idle_rcs, ring_idle_vcs, ring_idle_vcs2, ring_idle_vecs, ring_idle_bcs, ring_time;
    uint32_t mem_gt_begin, mem_gt_end, mem_ia_begin, mem_ia_end, mem_io_begin, mem_io_end;

    while (!m_quitThread)
    {
        ring_time = 0;
        ring_idle_rcs = 0;
        ring_idle_vcs = 0;
        ring_idle_vcs2 = 0;
        ring_idle_vecs = 0;
        ring_idle_bcs = 0;
        mem_gt_begin = 0;
        mem_ia_begin = 0;
        mem_io_begin = 0;
        mem_gt_end = 0;
        mem_ia_end = 0;
        mem_io_end = 0;

        try
        {
            mem_gt_begin = ReadMmio(m_mmio, m_mmioMemGT);
            mem_ia_begin = ReadMmio(m_mmio, m_mmioMemIA);
            mem_io_begin = ReadMmio(m_mmio, m_mmioMemIO);
        }
        catch (...)
        {
            mem_gt_begin = 0;
            mem_ia_begin = 0;
            mem_io_begin = 0;
        }

        while (ring_time <= 10000 && !m_quitThread)
        {
            uint32_t ring_head, ring_tail;

            // RCS
            ring_head = ReadMmio(m_mmio, m_mmioRingRcs + m_ringHeadOffset) & m_ringHeadMask;
            ring_tail = ReadMmio(m_mmio, m_mmioRingRcs + m_ringTailOffset) & m_ringTailMask;
            if (ring_tail == ring_head)
                ring_idle_rcs++;

            // VCS
            ring_head = ReadMmio(m_mmio, m_mmioRingVcs + m_ringHeadOffset) & m_ringHeadMask;
            ring_tail = ReadMmio(m_mmio, m_mmioRingVcs + m_ringTailOffset) & m_ringTailMask;
            if (ring_tail == ring_head)
                ring_idle_vcs++;

            // VCS2
            ring_head = ReadMmio(m_mmio, m_mmioRingVcs2 + m_ringHeadOffset) & m_ringHeadMask;
            ring_tail = ReadMmio(m_mmio, m_mmioRingVcs2 + m_ringTailOffset) & m_ringTailMask;
            if (ring_tail == ring_head)
                ring_idle_vcs2++;

            // VECS
            ring_head = ReadMmio(m_mmio, m_mmioRingVecs + m_ringHeadOffset) & m_ringHeadMask;
            ring_tail = ReadMmio(m_mmio, m_mmioRingVecs + m_ringTailOffset) & m_ringTailMask;
            if (ring_tail == ring_head)
                ring_idle_vecs++;

            // BCS
            ring_head = ReadMmio(m_mmio, m_mmioRingBcs + m_ringHeadOffset) & m_ringHeadMask;
            ring_tail = ReadMmio(m_mmio, m_mmioRingBcs + m_ringTailOffset) & m_ringTailMask;
            if (ring_tail == ring_head)
                ring_idle_bcs++;

            ring_time++;
            this_thread::sleep_for(chrono::microseconds(1000000 / SAMPLES_PER_SEC));
        }

        try
        {
            mem_gt_end = ReadMmio(m_mmio, m_mmioMemGT);
            mem_ia_end = ReadMmio(m_mmio, m_mmioMemIA);
            mem_io_end = ReadMmio(m_mmio, m_mmioMemIO);
        }
        catch (...)
        {
            mem_gt_end = 0;
            mem_ia_end = 0;
            mem_io_end = 0;
        }

        m_utilCompute  = 100 - ring_idle_rcs  * 100. / ring_time;
        m_utilFFCodec1 = 100 - ring_idle_vcs  * 100. / ring_time;
        m_utilFFCodec2 = 100 - ring_idle_vcs2 * 100. / ring_time;
        m_utilFFVp1    = 100 - ring_idle_vecs * 100. / ring_time;

        m_mbGT = (mem_gt_end - mem_gt_begin) * 64 / 1024 / 1024;
        m_mbIA = (mem_ia_end - mem_ia_begin) * 64 / 1024 / 1024;
        m_mbIO = (mem_io_end - mem_io_begin) * 64 / 1024 / 1024;
    }

}

void GtUtilization::GetGpuUtilization(std::map<UsageTag, float> &utils)
{
    
    utils.insert(std::make_pair(RENDER, m_utilCompute));
    utils.insert(std::make_pair(CODEC1, m_utilFFCodec1));
    utils.insert(std::make_pair(CODEC2, m_utilFFCodec2));
    utils.insert(std::make_pair(VE1, m_utilFFVp1));
    utils.insert(std::make_pair(TOTALMB, m_mbGT + m_mbIA + m_mbIO));
    utils.insert(std::make_pair(GTMB, m_mbGT));
    utils.insert(std::make_pair(IAMB, m_mbIA));
    utils.insert(std::make_pair(IOMB, m_mbIO));
    
    return;
}

void GtUtilization::GetUtilization(vector<intem::GtUtil> &utils)
{
    utils.clear();

    utils.push_back(intem::GtUtil(intem::COMPUTE,   m_utilCompute));
    utils.push_back(intem::GtUtil(intem::FF_CODEC1, m_utilFFCodec1));
    utils.push_back(intem::GtUtil(intem::FF_CODEC2, m_utilFFCodec2));
    utils.push_back(intem::GtUtil(intem::FF_VP1,    m_utilFFVp1));

}

void GtUtilization::GetBandwidth(vector<intem::GtMemBandwidth> &bandwidths)
{
    bandwidths.clear();

    bandwidths.push_back(intem::GtMemBandwidth(intem::GtMBType::MB_GT, m_mbGT));
    bandwidths.push_back(intem::GtMemBandwidth(intem::GtMBType::MB_IA, m_mbIA));
    bandwidths.push_back(intem::GtMemBandwidth(intem::GtMBType::MB_IO, m_mbIO));
}

