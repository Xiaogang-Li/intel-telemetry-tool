#ifndef __PERF_TOOL_SCREEN_H__
#define __PERF_TOOL_SCREEN_H__

#include "ncurses.h"

class Screen
{
public:
    Screen();
    virtual ~Screen();

    bool Init();
};

#endif
