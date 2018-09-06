#include "cpu.h"


float Cpu::GetUtilization(const std::string &statLine)
{
    uint64_t usertime, nicetime, systemtime, idletime;
    uint64_t  ioWait, irq, softIrq, steal, guest, guestnice;
    
    ioWait = irq = softIrq = steal = guest = guestnice = 0;

    stringstream stream(statLine);
    string name;
    stream >> name;

    if (m_name != name)
    {
        return false;
    }

    stream >> usertime >> nicetime >> systemtime >>  idletime;
    stream >> ioWait >> irq >> softIrq >> steal >> guest >> guestnice;

    // Guest time is already accounted in usertime
    usertime   -= guest;
    nicetime   -= guestnice;

    idletime   += ioWait;
    systemtime += irq + softIrq;
    
    uint64_t total = usertime + nicetime + systemtime + idletime + steal + guest + guestnice;
    float utilization = (total - m_totalTime - (idletime - m_idleTime)) * 100.0 / (total - m_totalTime);

    m_totalTime = total;
    m_idleTime  = idletime;

    return utilization;
}
