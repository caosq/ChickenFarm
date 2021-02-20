#include "modbus.h"

Modbus* Modbus::g_pModbus = nullptr;

Modbus::Modbus(eMBType eMBType, QObject *parent) :
    QObject(parent)
{
    this->m_MBType = eMBType;
}

void Modbus::uartConfig(uint32_t usBaudRate, uint8_t ucDataBit, uint8_t ucStopBit, char* pcParity)
{
    this->m_Uart.baud     = usBaudRate;
    this->m_Uart.data_bit = ucDataBit;
    this->m_Uart.stop_bit = ucStopBit;
    this->m_Uart.parity   = pcParity;
}

sMBMasterInfo* Modbus::masterInit(eMBMode eMode, char* pcPortName, uint8_t ucMinAddr,  uint8_t ucMaxAddr, bool bDTUEnable)
{
    if(this->m_MBType == TYPE_SLAVE)
    {
        return nullptr;
    }
    m_sMBMasterNode = {eMode, &this->m_Uart, pcPortName, ucMinAddr, ucMaxAddr, bDTUEnable};
    m_MasterInfo = new sMBMasterInfo;

//    xMBMasterRegistNode(m_MasterInfo, &m_sMBMasterNode);

    return m_MasterInfo;
}

bool Modbus::slaveInit(eMBMode eMode, char* pcPortName, uint8_t ucSlaveAddr)
{
    if(this->m_MBType == TYPE_MASTER)
    {
        return false;
    }
    sMBSlaveNodeInfo sMBSlaveNode = {eMode, &this->m_Uart, pcPortName, ucSlaveAddr};

    this->m_SlaveInfo = new sMBSlaveInfo;
    return xMBSlaveRegistNode(this->m_SlaveInfo, &sMBSlaveNode);
}

sMBMasterInfo* Modbus::getMBMasterInfo()
{
    return m_MasterInfo;
}

