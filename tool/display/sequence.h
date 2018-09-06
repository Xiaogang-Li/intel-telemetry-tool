#ifndef __PERF_TOOL_SEQUENCE_H__
#define __PERF_TOOL_SEQUENCE_H__

#include <vector>
#include "displayable.h"

class Sequence: public Displayable
{
public:
    Sequence(WINDOW* win, int x, int y, int width, int height, std::string title); 
    virtual ~Sequence();

    virtual void Draw(float value) override;

private:
    std::vector<int> m_values;
};

#endif
