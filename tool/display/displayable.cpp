#include "displayable.h"

Displayable::Displayable(WINDOW* win, int x, int y, int width, int height, std::string title):
m_win(win),
m_x(x),
m_y(y),
m_width(width),
m_height(height),
m_title(title)
{
}
