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

void DumpFloatPoint(WINDOW *win, int &line, const std::vector<std::string> &fpList,
                    const std::string &name)
{
    if (fpList.size() == 0)
    {
        mvwprintw(win, line++, 1, "%s Floating-point: Not Support", name.c_str());
    }
    else
    {
        int i = 0;
        mvwprintw(win, line++, 1, "%s Floating-point: Support", name.c_str());
    }
}

void DrawLine(WINDOW *win, int line)
{
    int xmax, ymax;
    getmaxyx(win, ymax, xmax);

    for (int i = 1; i < xmax - 1; i++)
    {
        mvwprintw(win, line, i, "-");
    }
}

void DrawTitle(WINDOW *win, int line, std::string title)
{
    int xmax, ymax;
    getmaxyx(win, ymax, xmax);

    mvwprintw(win, line, (xmax - title.size()) / 2, title.c_str());
}

void CpuStaticInfoDump(WINDOW *win, int &line, CpuInfo &cpu)
{
    DrawLine(win, line);
    DrawTitle(win, line++, " CPU Infomation ");
    mvwprintw(win, line++, 1, "Model Name : %s", cpu.GetDeviceName().c_str());
    mvwprintw(win, line, 1, "Family     : %s", cpu.GetDeviceFamily().c_str());
    mvwprintw(win, line, 20, "Stepping  : %d", cpu.GetStepping());
    mvwprintw(win, line++, 38, "Processer Counts: %d", cpu.GetProcesserNumber());
    mvwprintw(win, line, 1, "CPU Cores  : %d", cpu.GetCoreNumber());
    mvwprintw(win, line++, 20, "Cache Size: %d KB", cpu.GetCacheSize());

    DumpFloatPoint(win, line, cpu.GetHalfFloatingPointSupport(), "Half-precision");
    DumpFloatPoint(win, line, cpu.GetSingleFloatingPointSupport(), "Single-precision");
    DumpFloatPoint(win, line, cpu.GetDoubleFloatingPointSupport(), "Double-precision");
    DrawLine(win, line++);
    mvwprintw(win, line++, 1, " ");
}

void GpuStaticInfoDump(WINDOW *win, int &line, GtInfo &gpu)
{
    DrawLine(win, line);
    DrawTitle(win, line++, " GPU Infomation ");
    mvwprintw(win, line++, 1, "Platform   : %s", gpu.GetPlatform().c_str());
    mvwprintw(win, line++, 1, "Vendor     : %s", gpu.GetVendor().c_str());
    mvwprintw(win, line, 1, "Generation : %s", gpu.GetGeneration().c_str());
    mvwprintw(win, line++, 28, "EU Number                 : %d", gpu.GetEUNumber());
    //mvwprintw(win, line++, 1,  "Memory Size: %d GB", gpu.GetMemorySize() / 1024);
    //mvwprintw(win, line++, 28, "Profiling Timer Resolution: %d", gpu.GetProfilingTimerResolution());
    DumpFloatPoint(win, line, gpu.GetHalfFloatingPointSupport(), "Half-precision");
    DumpFloatPoint(win, line, gpu.GetSingleFloatingPointSupport(), "Single-precision");
    DumpFloatPoint(win, line, gpu.GetDoubleFloatingPointSupport(), "Double-precision");
    DrawLine(win, line++);
    mvwprintw(win, line++, 1, " ");
}

void MemoryInfoDump(WINDOW *win, int &line, SysMemory &mem)
{
    DrawLine(win, line);
    DrawTitle(win, line++, " Memory Infomation ");
    mvwprintw(win, line++, 1, "Capacity  : %d GB", mem.GetCapacity() / 1024);
    mvwprintw(win, line++, 1, "GetType   : %s", mem.GetType().c_str());
    mvwprintw(win, line++, 1, "Channels  : %d", mem.GetChannel());
    mvwprintw(win, line++, 1, "Speed     : %d MHz", mem.GetSpeed());
    mvwprintw(win, line++, 1, "Bit Width : %d", mem.GetBitWidth());
    DrawLine(win, line++);
    mvwprintw(win, line++, 1, " ");
}

void MediaCapsDump(WINDOW *win, int &line, MediaCapability &mediaCap)
{
    const std::vector<std::string> &decCaps = mediaCap.GetDecodeCapability();
    const std::vector<std::string> &encCaps = mediaCap.GetEncodeCapability();
    const std::vector<std::string> &vppCaps = mediaCap.GetVppCapability();

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

void PrepareCpuDyncSeq(WINDOW *win, int &x, int &y, CpuInfo &cpu,
                       std::vector<std::shared_ptr<Sequence>> &seqs)
{
    std::vector<std::pair<std::string, float>> utils;
    cpu.GetCPUUtilization(utils);

    int xmax, ymax;
    getmaxyx(win, ymax, xmax);
    int size = utils.size();
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

void PrepareGpuDyncDisps(WINDOW *win, int &x, int &y,
                         std::vector<std::shared_ptr<Sequence>> &seqs,
                         std::vector<std::shared_ptr<Bar>> &bars)
{
    std::vector<std::pair<std::string, float>> utils;

    int xmax, ymax;
    getmaxyx(win, ymax, xmax);
    int width = xmax / 4;
    int seqHeight = 13;
    int barHeight = 2;
    int numSeq = 4;
    int numBar = 4;

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

        //x += width;
        y += 1;
        y += barHeight;
    }

    y -= barHeight;
    //y += barHeight;
}

void PrepareCpuDyncBar(WINDOW *win, int &x, int &y, CpuInfo &cpu,
                       std::vector<std::shared_ptr<Bar>> &bars)
{
    std::vector<std::pair<std::string, float>> utils;
    cpu.GetCPUUtilization(utils);

    int xmax, ymax;
    getmaxyx(win, ymax, xmax);
    int size = utils.size();
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

void PrepareMemBar(WINDOW *win, int &x, int &y, std::shared_ptr<Bar> &bar)
{
    int xmax, ymax;
    getmaxyx(win, ymax, xmax);
    int width = xmax / 3;

    bar = std::make_shared<Bar>(win, xmax - width - sizeof("System Memory(GB):"), y, width - 1, 1, "System Memory(GB):");

    y += 2;
}

void CpuDyncDump(WINDOW *win, CpuInfo &cpu,
                 const std::vector<std::shared_ptr<Sequence>> &disps)
{
    std::vector<std::pair<std::string, float>> utils;
    cpu.GetCPUUtilization(utils);

    std::string tempTitle = "Temperature:";
    int color = 1;
    float temperature = cpu.GetTemperature();
    if (temperature >= 70.0)
    {
        color = 2;
    }
    wattron(win, A_REVERSE | A_BOLD);
    mvwprintw(win, disps[0]->GetY() - 3, 1, "%s", tempTitle.c_str());
    wattron(win, COLOR_PAIR(color));
    mvwprintw(win, disps[0]->GetY() - 3, 1 + tempTitle.size(), " %.2f", cpu.GetTemperature());
    wattroff(win, COLOR_PAIR(color));
    wattroff(win, A_REVERSE | A_BOLD);

    int i = 1;
    for (i = 0; i < disps.size(); i++)
    {
        if (utils.size() > 1 && utils[i + 1].second >= 0.0)
        {
            std::stringstream ss, ss1;
            ss << std::setiosflags(std::ios_base::fixed);
            ss.precision(2);
            ss << utils[i + 1].first << " (" << utils[i + 1].second << "%)";
            disps[i]->SetTitle(ss.str());

            ss1 << cpu.GetFrequency(i) << "MHz";
            disps[i]->SetCustomDispContent(ss1.str());
            disps[i]->Draw(utils[i + 1].second);
        }
    }
}

void GetGPUsageName(const GtUtilization::UsageTag tag, std::string &name)
{
    switch (tag)
    {
    case GtUtilization::RENDER:
        name = "Compute Engine";
        break;
    case GtUtilization::CODEC1:
        name = "FF Codec 1";
        break;
    case GtUtilization::CODEC2:
        name = "FF Codec 2";
        break;
    case GtUtilization::VE1:
        name = "FF Video Process";
        break;
    case GtUtilization::TOTALMB:
        name = "Total Bandwidth(MB/s)";
        break;
    case GtUtilization::GTMB:
        name = "GT Bandwidth(MB/s)";
        break;
    case GtUtilization::IAMB:
        name = "IA Bandwidth(MB/s)";
        break;
    case GtUtilization::IOMB:
        name = "IO Bandwidth(MB/s)";
        break;
    default:
        break;
    }
}

void GpuDyncDump(WINDOW *win, GtUtilization &gpu,
                 const std::vector<std::shared_ptr<Sequence>> &disps,
                 const std::vector<std::shared_ptr<Bar>> &bars,
                 SysMemory &mem)
{
    std::map<GtUtilization::UsageTag, float> utils;
    gpu.GetGpuUtilization(utils);

    int seqCount = 0;
    int barCount = 0;
    std::string name;
    int maxMBValue = mem.GetIdealBandWidth(); // MB/s

    DrawLine(win, disps[0]->GetY() - 2);

    for (auto util : utils)
    {
        if (util.second >= 0.0 && seqCount < disps.size() && util.first < GtUtilization::TOTALMB)
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

        if (barCount < bars.size() && util.first >= GtUtilization::TOTALMB)
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

void SystemStaticDump(WINDOW *win, int &line,
                      const SysMemory &sysmem,
                      const MediaCapability &mediaCap)
{
    mvwprintw(win, line++, 1, " ");
    mvwprintw(win, line++, 1, "Linux Version : %s", sysmem.GetSystemVersion().c_str());
    mvwprintw(win, line++, 1, "Driver Version: %s", mediaCap.GetDriverVersion().c_str());
    DrawLine(win, line);
    mvwprintw(win, line++, 1, " ");
    mvwprintw(win, line++, 1, " ");
}

void MemoryUsageDump(WINDOW *win, SysMemory &sysmem, std::shared_ptr<Bar> &bar)
{
    uint32_t totalSize = sysmem.GetTotalSize();
    uint32_t availbleSize = sysmem.GetAvailableSize();

    float percent = (totalSize - availbleSize) / totalSize * 100.0;

    DrawLine(win, bar->GetY() - 2);

    std::stringstream ss;
    ss << std::setiosflags(std::ios_base::fixed);
    ss.precision(2);
    ss << (float)((totalSize - availbleSize) / 1024.0 / 1024.0) << "/" << (float)(totalSize / 1024.0 / 1024.0);
    bar->SetCustomDispContent(ss.str());
    bar->Draw(percent);
}

static bool g_exit = false;
static bool g_showStatic = false;
static bool g_redraw = false;

void ReleaseWin(WINDOW *&win)
{
    if (win != nullptr)
    {
        delwin(win);
        win = nullptr;
    }
}

void DrawEngine()
{
    bool ret = true;
    GTMemory gtmem;
    GtInfoByCl gpu;
    CpuInfo cpu;
    SysMemory sysmem;
    MediaCapability mediaCap;
    GtUtilization gtutilInfo;

    std::chrono::milliseconds dura(1000);

    int maxX = COLS;
    int maxY = LINES;
    getmaxyx(stdscr, maxY, maxX);

    int leftWinWidth = 80;

    WINDOW *staticWin = nullptr; //newwin(LINES, leftWinWidth, 0, 0 );
    WINDOW *dyncWin = nullptr;   //newwin(LINES, COLS - leftWinWidth, 0,  leftWinWidth);

    std::vector<std::shared_ptr<Sequence>> cpuSeqs;

    std::vector<std::shared_ptr<Sequence>> gpuSeqs;
    std::vector<std::shared_ptr<Bar>> gpuBars;

    std::shared_ptr<Bar> membar = nullptr;

    g_redraw = true;

    while (true)
    {
        int maxTempX, maxTempY;
        getmaxyx(stdscr, maxTempY, maxTempX);
        if (maxX != maxTempX || maxY != maxTempY)
        {
            g_redraw = true;
            maxX = maxTempX;
            maxY = maxTempY;
        }

        if (maxX <= 100)
        {
            g_showStatic = false;
        }

        if (g_redraw)
        {
            clear();
            if (g_showStatic)
            {
                ReleaseWin(staticWin);
                ReleaseWin(dyncWin);

                staticWin = newwin(maxY, leftWinWidth, 0, 0);
                dyncWin = newwin(maxY, maxX - leftWinWidth, 0, leftWinWidth);

                box(staticWin, 0, 0);
                box(dyncWin, 0, 0);

                int i = 1;
                SystemStaticDump(staticWin, i, sysmem, mediaCap);
                CpuStaticInfoDump(staticWin, i, cpu);
                GpuStaticInfoDump(staticWin, i, gpu);
                MemoryInfoDump(staticWin, i, sysmem);
                MediaCapsDump(staticWin, i, mediaCap);
                wrefresh(staticWin);
            }
            else
            {
                ReleaseWin(staticWin);
                ReleaseWin(dyncWin);

                dyncWin = newwin(maxY, maxX, 0, 0);
            }

            int x = 0, y = 3;
            cpuSeqs.clear();
            PrepareCpuDyncSeq(dyncWin, x, y, cpu, cpuSeqs);

            x = 0;
            y += 2;
            gpuSeqs.clear();
            gpuBars.clear();
            PrepareGpuDyncDisps(dyncWin, x, y, gpuSeqs, gpuBars);

            x = 40;
            y = 0;
            membar = nullptr;
            PrepareMemBar(dyncWin, x, y, membar);

            g_redraw = false;
        }
        wclear(dyncWin);

        CpuDyncDump(dyncWin, cpu, cpuSeqs);
        GpuDyncDump(dyncWin, gtutilInfo, gpuSeqs, gpuBars, sysmem);
        MemoryUsageDump(dyncWin, sysmem, membar);

        wrefresh(dyncWin);

        if (g_exit)
        {
            break;
        }
        std::this_thread::sleep_for(dura);
    }
}

int main()
{
    Screen screen;
    screen.Init();

    refresh();
    std::thread *drawThread = new std::thread(DrawEngine);

    char ch;
    while (ch = getch())
    {
        switch (ch)
        {
        case 'q':
            g_exit = true;
            break;
        case 's':
            g_showStatic = !g_showStatic;
            g_redraw = true;
            break;
        default:
            break;
        }

        if (g_exit)
        {
            drawThread->join();
            break;
        }
    }

    delete drawThread;
    return 0;
}
