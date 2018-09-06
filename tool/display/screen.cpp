#include <ncurses.h>
#include "screen.h"

Screen::Screen()
{
}

Screen::~Screen()
{
    curs_set(1);
    endwin();
}

bool Screen::Init()
{
    initscr();
    cbreak();
    noecho();
    clear();
    start_color();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);

    curs_set(0);

    return true;
}
