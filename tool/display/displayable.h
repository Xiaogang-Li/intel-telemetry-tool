#ifndef __PERF_TOOL_DISPLAYABLE_H__
#define __PERF_TOOL_DISPLAYABLE_H__

#include <iostream>
#include <ncurses.h>

class Displayable
{
public:
    Displayable(WINDOW* win, int x, int y, int width, int height, std::string title);
    virtual ~Displayable() {}

    void SetTitle(std::string title) { m_title = title; }
    void SetCustomDispContent(std::string content) { m_customDispContent = content; } 
    virtual void Draw(float value) = 0;

protected:
    WINDOW *m_win   = nullptr;
    int    m_x      = 0;
    int    m_y      = 0;
    int    m_width  = 0;
    int    m_height = 0;
    std::string m_title = "";
    std::string m_customDispContent = "";
};

#endif
