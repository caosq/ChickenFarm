#ifndef METER_H
#define METER_H

#include <QObject>
#include "datamonitor.h"

class Meter : public QObject
{
    Q_OBJECT
public:
    uint16_t m_usTotalEnergy_H = 0; //耗电量(高位)
    uint16_t m_usTotalEnergy_L = 0; //耗电量(低位)
    uint32_t m_ulTotalEnergy = 0; //耗电量

    uint16_t m_usPower = 0; //耗电功率(有功功率)
    bool m_xCommErr = 0; //通讯故障

private:
    Monitor *m_pTotalEnergy_H; //耗电量(高位)；
    Monitor *m_pTotalEnergy_L; //耗电量(低位)；

private slots:
    void totalEnergyChangedSlot(Monitor*);

public:
    //电表
    explicit Meter(QObject *parent = nullptr);
};

#endif // METER_H
