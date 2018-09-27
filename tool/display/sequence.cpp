#include "sequence.h"

static const uint32_t GRAPHIC_DOT_PER_LINE = 2;

Sequence::Sequence(WINDOW* win, int x, int y, int width, int height, std::string title): 
    Displayable(win, x, y, width, height, title)
{
    m_values.clear();
}
Sequence::~Sequence()
{
    m_values.clear();
}

void DrawY(WINDOW* win, int x, int y, int height, int width)
{
    for(int i = 0; i < height; i++)
    {
        wmove(win, y + i, x+2);
        waddch(win, ACS_VLINE);
    }

    mvwprintw(win, y, x, "%d", 100);
    mvwprintw(win, y + (height / 4), x, "%d", 75);
    mvwprintw(win, y + (height / 2), x, "%d", 50);
    mvwprintw(win, y + (height * 3 / 4), x, "%d", 25);
    mvwprintw(win, y + height, x + 1, "%d", 0);


    for(int i = 3; i < width; i++)
    {
        wmove(win, y + height, x + i);
        waddch(win, ACS_HLINE);
    }

    wmove(win, y + height, x + 2);
    waddch(win, ACS_LLCORNER);
}

void Sequence::Draw(float percent)
{
    int xMax, yMax;
    getmaxyx(m_win, yMax, xMax);

    int startX = m_x + m_width;
    startX = startX > xMax ? xMax : startX;
    int mdWidth = startX - m_x + 1; 

    int startY = m_y + m_height - 1;
    startY = startY > yMax ? yMax : startY;
    int mdHeight = startY - m_y + 1;

    int displayVal = (int)(percent / 100 * mdHeight * GRAPHIC_DOT_PER_LINE);

    if(percent > 0.0 && displayVal == 0)
    {
        displayVal = 1;
    }

    m_values.push_back(displayVal);
    if (m_values.size() > mdWidth - 3)
    {
        m_values.erase(m_values.begin());
    }

    int currX = startX;


    int size = m_values.size();
    wattron(m_win, COLOR_PAIR(1));

    for (int j = size - 1; j >= 0; j --)
    {
        int currY = startY;

        displayVal = m_values[j];
        int v1 = displayVal / GRAPHIC_DOT_PER_LINE;
        int v2 = displayVal % GRAPHIC_DOT_PER_LINE;

        for (int i = 0; i < v1; i++)
        {
           mvwprintw(m_win, currY, currX, ":");
           currY -= 1;
        }
    
        if (v2 > 0)
        {
           mvwprintw(m_win, currY, currX, ".");
        }

        currX -= 1;
    }
    wattroff(m_win, COLOR_PAIR(1));
    
    DrawY(m_win, m_x, m_y, mdHeight, mdWidth);
    mvwprintw(m_win, m_y, m_x + (mdWidth - m_title.size()) / 2, m_title.c_str());    
    if (m_customDispContent.length() > 0)
    {
        mvwprintw(m_win, m_y + 1, m_x + (mdWidth - m_customDispContent.size()) / 2, m_customDispContent.c_str());    
    }
}
