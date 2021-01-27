#ifndef MODBUS_H
#define MODBUS_H

#include "mbdriver.h"
#include "mb.h"
#include "mb_m.h"

class Modbus
{
public:
    Modbus(eMBType eMBType);
    void uartConfig(uint32_t usBaudRate, uint8_t ucDataBit, uint8_t ucStopBit, char* pcParity);
    bool masterInit(eMBMode eMode, char* pcPortName, uint8_t ucMinAddr, uint8_t ucMaxAddr, bool bDTUEnable);
    bool slaveInit(eMBMode eMode, char* pcPortName, uint8_t ucSlaveAddr);

private:
    eMBType         m_MBType;
    sUART_Def       m_Uart;
    sMBMasterInfo*  m_MasterInfo;         //主栈接口
    sMBSlaveInfo*   m_SlaveInfo;          //从栈接口
};

#endif // MODBUS_H
