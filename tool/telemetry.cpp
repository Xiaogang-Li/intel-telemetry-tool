/*************************************************************************
    > File Name: perftool.cpp
 ************************************************************************/

#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include <iomanip>
#include "screen.h"
#include "sequence.h"
#include "bar.h"
#include "gtmemory.h"
#include "sysmemory.h"
#include "cpuinfo.h"
#include "gtinfo.h"
#include "gtinfo_by_cl.h"
#include "mediacapability.h"
#include "gtutilization.h"

using namespace std;

void DumpFloatPoint(WINDOW* win, int &line, const std::vector<std::string>& fpList,
	const std::string &name)
{
    if (fpList.size() == 0)
    {
	mvwprintw(win, line++, 1, "%s Floating-point: Not Support", name.c_str());
    }
    else
    {
	int i = 0;
	mvwprintw(win, line++, 1, "%s Floating-point:", name.c_str());
	for (auto fp : fpList)
	{
	    if (i % 2 == 1)
	    {
		mvwprintw(win, line++, 28, "(X) %s", fp.c_str());
	    }
	    else
	    {
		mvwprintw(win, line, 1, "  (X) %s", fp.c_str());
	    }

	    i++;
	}
	if (i % 2 == 1)
	{
	    line++;
	}

    }
}

void DrawLine(WINDOW* win, int line)
{
    int xmax , ymax;
    getmaxyx(win, ymax, xmax);

    for (int i = 1; i < xmax - 1; i++)
    {
	mvwprintw(win, line, i, "-");
    }
}

void DrawTitle(WINDOW* win, int line, std::string title)
{
    int xmax , ymax;
    getmaxyx(win, ymax, xmax);

    mvwprintw(win, line, (xmax - title.size()) / 2, title.c_str());
}

void CpuStaticInfoDump(WINDOW* win, int &line, CpuInfo &cpu)
{
    DrawLine(win, line);
    DrawTitle(win, line++, " CPU Infomation ");
    mvwprintw(win, line++, 1,  "Model Name : %s", cpu.GetDeviceName().c_str());
    mvwprintw(win, line, 1,    "Family     : %s", cpu.GetDeviceFamily().c_str());
    mvwprintw(win, line, 20,   "Stepping  : %d", cpu.GetStepping());
    mvwprintw(win, line++, 38,  "Processer Counts: %d", cpu.GetProcesserNumber());
    mvwprintw(win, line, 1,    "CPU Cores  : %d", cpu.GetCoreNumber());
    mvwprintw(win, line++, 20, "Cache Size: %d", cpu.GetCacheSize());

    DumpFloatPoint(win, line, cpu.GetHalfFloatingPointSupport(), "Half-precision");
    DumpFloatPoint(win, line, cpu.GetSingleFloatingPointSupport(), "Single-precision");
    DumpFloatPoint(win, line, cpu.GetDoubleFloatingPointSupport(), "Double-precision");
    DrawLine(win, line++);
    mvwprintw(win, line++, 1, " ");

}

void GpuStaticInfoDump(WINDOW* win, int &line, GtInfo &gpu)
{
    DrawLine(win, line);
    DrawTitle(win, line++, " GPU Infomation ");
    mvwprintw(win, line++, 1, "Platform   : %s", gpu.GetPlatform().c_str());
    mvwprintw(win, line++, 1, "Vendor     : %s", gpu.GetVendor().c_str());
    mvwprintw(win, line,   1,  "Generation : %s", gpu.GetGeneration().c_str());
    mvwprintw(win, line++, 28, "EU Number                 : %d", gpu.GetEUNumber());
    mvwprintw(win, line, 1,  "Memory Size: %d", gpu.GetMemorySize());
    mvwprintw(win, line++, 28, "Profiling Timer Resolution: %d", gpu.GetProfilingTimerResolution());
    DumpFloatPoint(win, line, gpu.GetHalfFloatingPointSupport(), "Half-precision");
    DumpFloatPoint(win, line, gpu.GetSingleFloatingPointSupport(), "Single-precision");
    DumpFloatPoint(win, line, gpu.GetDoubleFloatingPointSupport(), "Double-precision");
    DrawLine(win, line++);
    mvwprintw(win, line++, 1, " ");
}


void MediaCapsDump(WINDOW* win, int &line, MediaCapability &mediaCap)
{
    const std::vector<std::string>& decCaps = mediaCap.GetDecodeCapability();
    const std::vector<std::string>& encCaps = mediaCap.GetEncodeCapability();
    const std::vector<std::string>& vppCaps = mediaCap.GetVppCapability();

    int size = decCaps.size();
    size = size > encCaps.size() ? size : encCaps.size();
    size = size > vppCaps.size() ? size : vppCaps.size();

    DrawLine(win, line);
    DrawTitle(win, line++, " Media Capability ");
    int startLine = line, backupLine = line;
    line += size + 1;

    mvwprintw(win, startLine++, 1, "Encode:");
    for (auto cap : encCaps)
    {
	mvwprintw(win, startLine++, 1, "    %s", cap.c_str());
    }

    startLine = backupLine;
    mvwprintw(win, startLine++, 35, "| Decode:");
    for (auto cap : decCaps)
    {
	mvwprintw(win, startLine++, 35, "|    %s", cap.c_str());
    }

    startLine = backupLine;
    mvwprintw(win, startLine++, 45, "| VPP:");
    for (auto cap : vppCaps)
    {
	mvwprintw(win, startLine++, 45, "|    %s", cap.c_str());
    }
    DrawLine(win, line++);
    mvwprintw(win, line++, 1, " ");
}

void PrepareCpuDyncSeq(WINDOW* win, int &x, int &y, CpuInfo &cpu, 
	std::vector<std::shared_ptr<Sequence>> &seqs)
{
    std::vector<std::pair<std::string, float>> utils;
    cpu.GetCPUUtilization(utils);

    int xmax , ymax;
    getmaxyx(win, ymax, xmax);
    int size     = utils.size();
    int width = xmax / 4;
    int height = 13;

    for (int i = 1; i < utils.size(); i++)
    {
	if (i % 4 == 1 && i != 1)
	{
	    x = 0;
	    y += height;
	}

	std::pair<std::string, float> &pair = utils[i];
	std::shared_ptr<Sequence> seq = std::make_shared<Sequence>(win, x, y, width - 1, height - 2, pair.first);

	seqs.push_back(seq);
	x += width;
    }

    y += height;
}


void PrepareGpuDyncDisps(WINDOW* win, int &x, int &y, 
	std::vector<std::shared_ptr<Sequence>> &seqs,
	std::vector<std::shared_ptr<Bar>> &bars)
{
    std::vector<std::pair<std::string, float>> utils;

    int xmax , ymax;
    getmaxyx(win, ymax, xmax);
    int width = xmax / 4;
    int seqHeight = 13;
    int barHeight = 2;
    int numSeq    = 4;
    int numBar    = 4;

    for (int i = 1; i <= numSeq; i++)
    {
	if (i % 4 == 1 && i != 1)
	{
	    x = 0;
	    y += seqHeight;
	}

	std::shared_ptr<Sequence> seq = std::make_shared<Sequence>(win, x, y, width - 1, seqHeight - 2, "");

	seqs.push_back(seq);
	x += width;
    }

    x = 0;
    y += seqHeight;

    for (int i = 1; i <= numBar; i++)
    {
	std::shared_ptr<Bar> bar = std::make_shared<Bar>(win, x, y, width - 1, barHeight, "");

	bars.push_back(bar);

	x += width;
    }

    y += barHeight;
}

void PrepareCpuDyncBar(WINDOW* win, int &x, int &y, CpuInfo &cpu, 
	std::vector<std::shared_ptr<Bar>> &bars)
{
    std::vector<std::pair<std::string, float>> utils;
    cpu.GetCPUUtilization(utils);

    int xmax , ymax;
    getmaxyx(win, ymax, xmax);
    int size     = utils.size();
    int width = xmax / 4;
    int height = 1;

    for (int i = 1; i < utils.size(); i++)
    {
	if (i % 4 == 1 && i != 1)
	{
	    x = 0;
	    y += height;
	}

	std::pair<std::string, float> &pair = utils[i];
	std::shared_ptr<Bar> bar = std::make_shared<Bar>(win, x, y, width - 1, height, pair.first);

	bars.push_back(bar);
	x += width;
    }
}

void PrepareMemBar(WINDOW* win, int &x, int &y, std::shared_ptr<Bar> &bar)
{
    int xmax , ymax;
    getmaxyx(win, ymax, xmax);
    int width = xmax / 2;

    bar = std::make_shared<Bar>(win, x, y, width - 1, 2, "System Memory(GB)");

    y += 2;
}

void CpuDyncDump(WINDOW* win, CpuInfo &cpu,
	const std::vector<std::shared_ptr<Sequence>> &disps)
{
    std::vector<std::pair<std::string, float>> utils;
    cpu.GetCPUUtilization(utils);

    int i = 1;
    for (i = 0; i < disps.size(); i++)
    {
        if (utils[i+1].second >= 0.0)
	{
            std::stringstream ss, ss1;
	    ss << std::setiosflags(std::ios_base::fixed);
	    ss.precision(2);
	    ss << utils[i+1].first << " (" << utils[i+1].second << "%)";
	    disps[i]->SetTitle(ss.str());
	    
	    ss1 << cpu.GetFrequency(i) << "MHz";
	    disps[i]->SetCustomDispContent(ss1.str());
            disps[i]->Draw(utils[i+1].second);
	}
    }
}

void GetGPUsageName(const GtUtil::UsageTag tag, std::string &name)
{
    switch(tag)
    {
    case GtUtil::RENDER:
        name = "Rerder Engine";
        break;
    case GtUtil::CODEC1:
        name = "Codec 1";
        break;
    case GtUtil::CODEC2:
        name = "Codec 2";
        break;
    case GtUtil::VE1:
        name = "VEBox";
        break;
    case GtUtil::TOTALMB:
        name = "Total Bandwidth(MB/s)";
        break;
    case GtUtil::GTMB:
        name = "GT Bandwidth(MB/s)";
        break;
    case GtUtil::IAMB:
        name = "IA Bandwidth(MB/s)";
        break;
    case GtUtil::IOMB:
        name = "IO Bandwidth(MB/s)";
        break;
    default:
        break;
    }

}

void GpuDyncDump(WINDOW* win, GtUtil &gpu,
	const std::vector<std::shared_ptr<Sequence>> &disps,
	const std::vector<std::shared_ptr<Bar>> &bars)
{
    std::map<GtUtil::UsageTag, float> utils;
    gpu.GetGpuUtilization(utils);

    int seqCount = 0;
    int barCount = 0;
    std::string name;
    int maxMBValue = 10000; // MB/s
    for (auto util : utils)
    {
        if (util.second >= 0.0 && seqCount < disps.size() && util.first < GtUtil::TOTALMB)
        {
            std::stringstream ss;
            GetGPUsageName(util.first, name);
	    ss << std::setiosflags(std::ios_base::fixed);
	    ss.precision(2);
	    ss << name << " (" << util.second << "%)";
	    name = ss.str();
            disps[seqCount]->SetTitle(name);
            disps[seqCount]->Draw(util.second);
            seqCount++;
        }

	if (barCount < bars.size() && util.first >= GtUtil::TOTALMB)
	{
	    float percent = util.second / maxMBValue * 100;
	    auto bar = bars[barCount++];
	    std::stringstream ss;
	    GetGPUsageName(util.first, name);
	    bar->SetTitle(name);
	    ss << (int)util.second << "/" << maxMBValue;
	    bar->SetCustomDispContent(ss.str());
	    bar->Draw(percent);
	}
    }
}

void SystemStaticDump(WINDOW* win, int &line, 
	const SysMemory &sysmem, 
	const MediaCapability &mediaCap)
{
    mvwprintw(win, line++, 1, " ");
    mvwprintw(win, line++, 1,  "Linux Version : %s", sysmem.GetSystemVersion().c_str());
    mvwprintw(win, line++, 1,  "Driver Version: %s", mediaCap.GetDriverVersion().c_str());
    DrawLine(win, line);
    mvwprintw(win, line++, 1, " ");
    mvwprintw(win, line++, 1, " ");

}

void MemoryUsageDump(WINDOW* win, SysMemory &sysmem, std::shared_ptr<Bar> &bar)
{
    uint32_t totalSize    = sysmem.GetTotalSize();
    uint32_t availbleSize = sysmem.GetAvailableSize();

    float percent =  (totalSize - availbleSize)/ totalSize * 100.0;

    std::stringstream ss;
    ss << std::setiosflags(std::ios_base::fixed);
    ss.precision(2);
    ss << (float)((totalSize - availbleSize) / 1024.0 / 1024.0) << "/" << (float)(totalSize / 1024.0 / 1024.0);
    bar->SetCustomDispContent(ss.str());
    bar->Draw(percent);
}

int main()
{
    bool ret = true;
    GTMemory  gtmem;
    GtInfoByCl    gpu;
    CpuInfo   cpu;
    SysMemory sysmem;
    MediaCapability mediaCap;
    GtUtil          gtutilInfo;


    std::chrono::milliseconds dura(1000);
   
    Screen screen;
    screen.Init();

    refresh();

    int leftWinWidth = COLS * 2 / 6 + 5;

    WINDOW* staticWin = newwin(LINES, leftWinWidth, 0, 0 );
    WINDOW* dyncWin   = newwin(LINES, COLS - leftWinWidth, 0,  leftWinWidth);

    box(staticWin, 0, 0);
    box(dyncWin, 0, 0);

    int i = 1;
    SystemStaticDump(staticWin, i , sysmem, mediaCap);
    CpuStaticInfoDump(staticWin, i, cpu);
    GpuStaticInfoDump(staticWin, i, gpu);
    MediaCapsDump(staticWin, i, mediaCap);
    wrefresh(staticWin);
    //
    std::vector<std::shared_ptr<Sequence>> cpuSeqs;

    int x = 0, y = 0;
    PrepareCpuDyncSeq(dyncWin, x, y, cpu, cpuSeqs);
    x = 0;
    y += 1;
    //PrepareCpuDyncBar(dyncWin, x, y, cpu, bars);
    std::vector<std::shared_ptr<Sequence>> gpuSeqs;
    std::vector<std::shared_ptr<Bar>> gpuBars;
    PrepareGpuDyncDisps(dyncWin, x, y, gpuSeqs, gpuBars);

    std::shared_ptr<Bar> membar = nullptr;
    x = 0;
    y += 1;
    PrepareMemBar(dyncWin, x, y, membar);

    while(true)
    {
	wclear(dyncWin);

	CpuDyncDump(dyncWin, cpu, cpuSeqs);
	GpuDyncDump(dyncWin, gtutilInfo, gpuSeqs, gpuBars);
	MemoryUsageDump(dyncWin, sysmem, membar);

	wrefresh(dyncWin);

        //std::this_thread::sleep_for(dura);
    }

    return 0;
}
