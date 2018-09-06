#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <pciaccess.h>
#include <sys/stat.h>
#include <sstream>
#include <exception>
#include <iomanip>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include "gtutilization.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "i915_pciids.h"
#include "igfx.h"

#ifdef __cplusplus
}
#endif

#define SAMPLES_PER_SEC             10000


GtUtil::GtUtil()
{
/*
    try
    {
        ;
    }
    catch(std::exception& e)
    {

        std::cerr << "******** Error when init GT Utilization **************" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << "******************************************************" << std::endl;

    }
*/
    return;

}

GtUtil::~GtUtil()
{
    return;
}

void GtUtil::GetGpuUtilization(std::map<UsageTag, float> &utils)
{
    const struct igfx_info *info;
    struct pci_device *igfx;
    void *mmio;
    uint64_t ring_idle_rcs, ring_idle_vcs, ring_idle_vcs2, ring_idle_vecs, ring_idle_bcs, ring_time;
    uint32_t mem_gt_begin, mem_gt_end, mem_ia_begin, mem_ia_end, mem_io_begin, mem_io_end;

    igfx = igfx_get();
    if (!igfx)
    {
        std::cerr << "******** Error when get IGFX Information**************" << std::endl;
        return;
    }

    info = igfx_get_info(igfx);
    mmio = igfx_get_mmio(igfx);
    if (mmio == NULL)
        exit(127);

/*
    struct mmio_ring rcs_ring = {
        .base = INTEM_MMIO_RING_RCS,
        .id = 0,
    }, vcs_ring = {
        .base = INTEM_MMIO_RING_VCS,
        .id = 1,
    }, vcs2_ring = {
        .base = INTEM_MMIO_RING_VCS2,
        .id = 2,
    }, vecs_ring = {
        .base = INTEM_MMIO_RING_VECS,
        .id = 3,
    }, bcs_ring = {
        .base = INTEM_MMIO_RING_BCS,
        .id = 4,
    };
*/

    ring_time = 0;
    ring_idle_rcs = 0;
    ring_idle_vcs = 0;
    ring_idle_vcs2 = 0;
    ring_idle_vecs = 0;
    ring_idle_bcs = 0;

    mem_gt_begin = igfx_read(mmio,INTEM_MMIO_GT_MEM);
    mem_ia_begin = igfx_read(mmio,INTEM_MMIO_IA_MEM);
    mem_io_begin = igfx_read(mmio,INTEM_MMIO_IO_MEM);

    while(ring_time <= 10000)
    {
        uint32_t ring_head, ring_tail;
       
        // RCS 
        ring_head = igfx_read(mmio,INTEM_MMIO_RING_RCS + RING_HEAD) & HEAD_ADDR;
        ring_tail = igfx_read(mmio,INTEM_MMIO_RING_RCS + RING_TAIL) & TAIL_ADDR;
        if (ring_tail == ring_head)
            ring_idle_rcs++;

        // VCS
        ring_head = igfx_read(mmio,INTEM_MMIO_RING_VCS + RING_HEAD) & HEAD_ADDR;
        ring_tail = igfx_read(mmio,INTEM_MMIO_RING_VCS + RING_TAIL) & TAIL_ADDR;
        if (ring_tail == ring_head)
            ring_idle_vcs++;

        // VCS2
        ring_head = igfx_read(mmio,INTEM_MMIO_RING_VCS2 + RING_HEAD) & HEAD_ADDR;
        ring_tail = igfx_read(mmio,INTEM_MMIO_RING_VCS2 + RING_TAIL) & TAIL_ADDR;
        if (ring_tail == ring_head)
            ring_idle_vcs2++;

        // VECS
        ring_head = igfx_read(mmio,INTEM_MMIO_RING_VECS + RING_HEAD) & HEAD_ADDR;
        ring_tail = igfx_read(mmio,INTEM_MMIO_RING_VECS + RING_TAIL) & TAIL_ADDR;
        if (ring_tail == ring_head)
            ring_idle_vecs++;

        // BCS
        ring_head = igfx_read(mmio,INTEM_MMIO_RING_BCS + RING_HEAD) & HEAD_ADDR;
        ring_tail = igfx_read(mmio,INTEM_MMIO_RING_BCS + RING_TAIL) & TAIL_ADDR;
        if (ring_tail == ring_head)
            ring_idle_bcs++;

        ring_time++;
        usleep(1000000 / SAMPLES_PER_SEC);
    }
    
    mem_gt_end = igfx_read(mmio,INTEM_MMIO_GT_MEM);
    mem_ia_end = igfx_read(mmio,INTEM_MMIO_IA_MEM);
    mem_io_end = igfx_read(mmio,INTEM_MMIO_IO_MEM);

    utils.insert(std::make_pair(RENDER,   ((100 - ring_idle_rcs * 100. / ring_time))));
    utils.insert(std::make_pair(CODEC1,  ((100 - ring_idle_vcs * 100. / ring_time))));
    utils.insert(std::make_pair(CODEC2,  ((100 - ring_idle_vcs2 * 100. / ring_time))));
    utils.insert(std::make_pair(VE1,     ((100 - ring_idle_vecs * 100. / ring_time))));
    utils.insert(std::make_pair(BLT,     ((100 - ring_idle_bcs * 100. / ring_time))));
    utils.insert(std::make_pair(TOTALMB, (((mem_gt_end-mem_gt_begin)+(mem_ia_end-mem_ia_begin)+(mem_io_end-mem_io_begin))*64/1024/1024)));
    utils.insert(std::make_pair(GTMB,    (((mem_gt_end-mem_gt_begin))*64/1024/1024)));
    utils.insert(std::make_pair(IAMB,    (((mem_ia_end-mem_ia_begin))*64/1024/1024)));
    utils.insert(std::make_pair(IOMB,    (((mem_io_end-mem_io_begin))*64/1024/1024)));
    //std::cout << " | " << "Render Engine : " << ((100 - ring_idle_rcs * 100. / ring_time)) << std::left << std::setw(38) << " %" << "|" << std::endl;
    //std::cout << " | " << "Codec Engine  : " << ((100 - ring_idle_vcs * 100. / ring_time)) << std::left << std::setw(38) << " %" << "|" << std::endl;
    //std::cout << " | " << "Codec Engine2 : " << ((100 - ring_idle_vcs2 * 100. / ring_time)) << std::left << std::setw(38) << " %" << "|" << std::endl;
    //std::cout << " | " << "Video Process : " << ((100 - ring_idle_vecs * 100. / ring_time)) << std::left << std::setw(38) << " %" << "|" << std::endl;
    //std::cout << " | " << "BLT Engine    : " << ((100 - ring_idle_bcs * 100. / ring_time)) << std::left << std::setw(38) << " %" << "|" << std::endl;
  
    //std::cout << " |" << std::left << std::setw(61) << " " << "|" << std::endl;
    //std::cout              << " ---------------------------------------------------------------" << std::endl;
 
    //std::cout << std::endl << " -----------------------  Memory Bandwidth ----------------------" << std::endl;
    //std::cout << " |" << std::left << std::setw(61) << " " << "|" << std::endl;

    //std::cout << " | " << "Total Memory Bandwidth : " << (((mem_gt_end-mem_gt_begin)+(mem_ia_end-mem_ia_begin)+(mem_io_end-mem_io_begin))*64/1024/1024) << std::left << std::setw(38) << " MB/s" << "|" << std::endl;

    //std::cout << " | " << "GT Memory Bandwidth    : " << (((mem_gt_end-mem_gt_begin))*64/1024/1024) << std::left << std::setw(38) << " MB/s" << "|" << std::endl;
    //std::cout << " | " << "IA Memory Bandwidth    : " << (((mem_ia_end-mem_ia_begin))*64/1024/1024) << std::left << std::setw(38) << " MB/s" << "|" << std::endl; 
    //std::cout << " | " << "IO Memory Bandwidth    : " << (((mem_io_end-mem_io_begin))*64/1024/1024) << std::left << std::setw(38) << " MB/s" << "|" << std::endl;

    return;
}
