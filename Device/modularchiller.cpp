#include "modularchiller.h"
#include "datamonitor.h"

ModularChiller::ModularChiller()
{
    //DataMonitor::getInstance()->monitorRegist(m_CO2PPM,uint16, this);

}

void ModularChiller::initData()
{
    MONITOR(m_CO2PPM, uint16);
}


