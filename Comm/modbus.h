#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>
#include "mbdriver.h"
#include "mb.h"
#include "mb_m.h"


class Modbus : public QObject
{
public:
    void uartConfig(uint32_t usBaudRate, uint8_t ucDataBit, uint8_t ucStopBit, char* pcParity);

    sMBMasterInfo* masterInit(eMBMode eMode, char* pcPortName, uint8_t ucMinAddr, uint8_t ucMaxAddr, bool bDTUEnable);

    bool slaveInit(eMBMode eMode, char* pcPortName, uint8_t ucSlaveAddr);

    sMBMasterNodeInfo m_sMBMasterNode;
    sMBMasterInfo* getMBMasterInfo();

    static Modbus* getInstance();

    explicit Modbus(eMBType eMBType, QObject *parent = nullptr);

private:
    eMBType         m_MBType;
    sUART_Def       m_Uart;

    sMBMasterInfo*  m_MasterInfo;         //主栈接口
    sMBSlaveInfo*   m_SlaveInfo;          //从栈接口

    static Modbus *g_pModbus;

};


#endif // MODBUS_H
