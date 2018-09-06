#ifndef __PERF_TOOL_BAR_H__
#define __PERF_TOOL_BAR_H__

#include "displayable.h"

class Bar: public Displayable
{
public:
    Bar(WINDOW* win, int x, int y, int width, int height, std::string title); 

    virtual ~Bar() {}

    virtual void Draw(float value) override;
};

#endif
