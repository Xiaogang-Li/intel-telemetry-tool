/*************************************************************************
    > File Name: perftool.cpp
 ************************************************************************/

#include <iostream>
#include <thread>
#include "../library/inc/gtmemory.h"
#include "../library/inc/sysmemory.h"
#include "../library/inc/cpu.h"
#include "../library/inc/gtinfo.h"
#include "../library/inc/mediacapability.h"

using namespace std;

int main()
{
    bool ret = true;
    GTMemory  gtmem;
    GtInfo    gpu;
    CpuInfo   cpu;
    SysMemory sysmem;
    MediaCapability mediaCap;

    std::chrono::milliseconds dura(1000);
    
    while(true)
    {
        std::cout << "\33[2J\033[0m" << std::endl;
        cpu.Dump();
        sysmem.Dump();

        gpu.Dump();
        gtmem.Dump();
    
        mediaCap.Dump();

        std::this_thread::sleep_for(dura);
    }

    return 0;
}
