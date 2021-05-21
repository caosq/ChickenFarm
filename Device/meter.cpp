#include "meter.h"

Meter::Meter(QObject *parent) :
QObject(parent)
{
    m_pTotalEnergy_H = DataMonitor::monitorRegist(&m_usTotalEnergy_H, Monitor::DataType::Uint16t);
    m_pTotalEnergy_L = DataMonitor::monitorRegist(&m_usTotalEnergy_L, Monitor::DataType::Uint16t);

    connect(m_pTotalEnergy_H, SIGNAL(valChanged(Monitor*)),this,SLOT(totalEnergyChangedSlot(Monitor*)));
    connect(m_pTotalEnergy_L, SIGNAL(valChanged(Monitor*)),this,SLOT(totalEnergyChangedSlot(Monitor*)));
}

void Meter::totalEnergyChangedSlot(Monitor*)
{
    m_ulTotalEnergy = m_usTotalEnergy_H * 65535 + m_usTotalEnergy_L;
}
