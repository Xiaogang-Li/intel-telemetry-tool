#include <sstream>
#include <iomanip>
#include "bar.h"

using namespace std;


Bar::Bar(WINDOW* win, int x, int y, int width, int height, std::string title): 
    Displayable(win, x, y, width, height, title)
{}

void Bar::Draw(float percent)
{
    mvwprintw(m_win, m_y, m_x, m_title.c_str());
    mvwprintw(m_win, m_y + 1, m_x, "[");
    mvwprintw(m_win, m_y + 1, m_x + m_width, "]");

    int contentWidth = m_width - 2;

    int barValue = contentWidth * (percent / 100);
    
    int color = 1;
    if (percent > 80.0)
    {
	color = 2;
    }

    std::ostringstream out;
    if (m_customDispContent.length() == 0)
    {
        out << std::setiosflags(std::ios_base::fixed);
        out.precision(2);
        out << percent;
    }

    wattron(m_win,COLOR_PAIR(color)); 
    for (int i = 0; i < barValue; i++)
    {
	wmove(m_win, m_y + 1, m_x + 1 +i);
	waddch(m_win, '|');
	//mvwprintw(m_win, m_y, 1 + i, "|");
    }
    wattroff(m_win,COLOR_PAIR(color));

    if (m_customDispContent.length() == 0)
    {
        std::string str = out.str();
        mvwprintw(m_win, m_y + 1, m_x + m_width - 1 - str.length(), "%s%%", str.c_str());
    }
    else
    {
	mvwprintw(m_win, m_y + 1, m_x + m_width - m_customDispContent.length(), "%s", m_customDispContent.c_str());
    }

}
