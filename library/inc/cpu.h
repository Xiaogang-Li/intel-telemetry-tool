#ifndef __PERF_TOOL_CPU_H__
#define __PERF_TOOL_CPU_H__

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Cpu
{
public:
    float GetUtilization(const std::string &statLine);

    static inline string ParseName(const std::string &statLine)
    {
	stringstream stream(statLine);

	string name;
	stream >> name;

	return name;
    }

    void SetName(const string &name) { m_name = name; }
private:
    uint64_t m_userTime   = 1;
    uint64_t m_niceTime   = 1;
    uint64_t m_systemTime = 1;
    uint64_t m_idleTime   = 1;
    uint64_t m_totalTime  = 1;

    string m_name;
};

#endif
