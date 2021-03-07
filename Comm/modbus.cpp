#include "modbus.h"

//Modbus* Modbus::g_pModbus = nullptr;
//sMBMasterInfo Modbus::m_MasterInfo;

#define MODBUS_SLAVE_MAX_DEV_ADDR    1
#define MODBUS_SLAVE_MIN_DEV_ADDR    1

Modbus::Modbus(QObject *parent) :
    QObject(parent)
{
}

void Modbus::uartConfig(uint32_t usBaudRate, uint8_t ucDataBit, uint8_t ucStopBit, uint8_t pcParity)
{
    m_Uart.baud     = usBaudRate;
    m_Uart.data_bit = ucDataBit;
    m_Uart.stop_bit = ucStopBit;
    m_Uart.parity   = pcParity;
}

void Modbus::initMasterPort(eMBMode eMode, const char* pcPortName, uint8_t ucMinAddr, uint8_t ucMaxAddr, bool bDTUEnable)
{
    m_MasterInfo.eMode = MB_RTU;
    m_MasterInfo.eMBState = STATE_NOT_INITIALIZED;
    m_MasterInfo.eSndState = STATE_M_TX_IDLE;
    m_MasterInfo.eRcvState = STATE_M_RX_IDLE;
    m_MasterInfo.eMBRunMode = STATE_TEST_DEV;
    m_MasterInfo.usSndPDULength = 0;
    m_MasterInfo.usSndBufferCount = 0;
    m_MasterInfo.usRcvBufferPos = 0;
    m_MasterInfo.pucSndBufferCur = nullptr;
    m_MasterInfo.pucMasterPDUCur = nullptr;
    m_MasterInfo.ucMBDestAddr = MODBUS_SLAVE_MIN_DEV_ADDR;
    m_MasterInfo.xFrameIsBroadcast = false;
    m_MasterInfo.pNext = nullptr;
    m_MasterInfo.pLast = nullptr;

    m_MasterInfo.sMBDevsInfo = {0, MODBUS_SLAVE_MIN_DEV_ADDR, MODBUS_SLAVE_MAX_DEV_ADDR, nullptr, nullptr};
    mMBMasterNode = {eMode, &m_Uart, pcPortName, ucMinAddr, ucMaxAddr, bDTUEnable};
}

/*bool Modbus::slaveInit(eMBMode eMode, char* pcPortName, uint8_t ucSlaveAddr)
{
   if(this->m_MBType == TYPE_MASTER)
    {
        return false;
    }
    sMBSlaveNodeInfo sMBSlaveNode = {eMode, &this->m_Uart, pcPortName, ucSlaveAddr};

    this->m_SlaveInfo = new sMBSlaveInfo;
    return xMBSlaveRegistNode(this->m_SlaveInfo, &sMBSlaveNode);

} */

bool Modbus::masterRegistSlaveDev(sMBSlaveDev* psMBNewDev)
{
     return xMBMasterRegistDev(&m_MasterInfo, psMBNewDev);
}

bool Modbus::registMasterMode()
{
    return xMBMasterRegistNode(&m_MasterInfo, &mMBMasterNode);
}

sMBMasterInfo* Modbus::getMBMasterInfo()
{
    return &m_MasterInfo;
}

/*Modbus* Modbus::getInstance()
{
    return g_pModbus;
}*/
