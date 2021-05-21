#include "modbus.h"
#include "mbdtu_m.h"
#include "mbtcp.h"
#include "mbtcp_m.h"

MBMaster::MBMaster(QObject *parent) :
    QObject(parent)
{
}
#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED
void MBMaster::masterUartConfig(uint32_t usBaudRate, uint8_t ucDataBit, uint8_t ucStopBit, uint8_t pcParity)
{
    m_MasterUart.baud     = usBaudRate;
    m_MasterUart.data_bit = ucDataBit;
    m_MasterUart.stop_bit = ucStopBit;
    m_MasterUart.parity   = pcParity;
}

void MBMaster::initMasterPort(eMBMode eMode, const char* pcPortName, uint8_t ucMinAddr, uint8_t ucMaxAddr, bool bDTUEnable)
{
    m_MasterInfo.eMode = eMode;
    m_MasterInfo.eMBState = STATE_NOT_INITIALIZED;
    m_MasterInfo.eSndState = STATE_M_TX_IDLE;
    m_MasterInfo.eRcvState = STATE_M_RX_IDLE;
    m_MasterInfo.eMBRunMode = STATE_TEST_DEV;
    m_MasterInfo.usSndPDULength = 0;
    m_MasterInfo.usSndBufferCount = 0;
    m_MasterInfo.usRcvBufferPos = 0;
    m_MasterInfo.pucSndBufferCur = nullptr;
    m_MasterInfo.pucMasterPDUCur = nullptr;
    m_MasterInfo.ucMBDestAddr = ucMinAddr;
    m_MasterInfo.xFrameIsBroadcast = false;
    m_MasterInfo.pNext = nullptr;
    m_MasterInfo.pLast = nullptr;

    m_MasterInfo.sMBDevsInfo = {0, ucMinAddr, ucMaxAddr, nullptr, nullptr};
    m_MBMasterNode = {eMode, &m_MasterUart, pcPortName, ucMinAddr, ucMaxAddr, bDTUEnable};
}
#endif

#if MB_MASTER_TCP_ENABLED
void MBMaster::initMasterTCPPort(const char* pcPortName, uint8_t ucMinAddr, uint8_t ucMaxAddr)
{
    m_MasterInfo.eMode = MB_TCP;
    m_MasterInfo.eMBState = STATE_NOT_INITIALIZED;
    m_MasterInfo.eSndState = STATE_M_TX_IDLE;
    m_MasterInfo.eRcvState = STATE_M_RX_IDLE;
    m_MasterInfo.eMBRunMode = STATE_TEST_DEV;
    m_MasterInfo.usSndPDULength = 0;
    m_MasterInfo.usSndBufferCount = 0;
    m_MasterInfo.usRcvBufferPos = 0;
    m_MasterInfo.pucSndBufferCur = nullptr;
    m_MasterInfo.pucMasterPDUCur = nullptr;
    m_MasterInfo.ucMBDestAddr = ucMinAddr;
    m_MasterInfo.xFrameIsBroadcast = false;
    m_MasterInfo.pNext = nullptr;
    m_MasterInfo.pLast = nullptr;

    m_MasterInfo.sMBDevsInfo = {0, ucMinAddr, ucMaxAddr, nullptr, nullptr, nullptr, nullptr};
    m_MBMasterNode = {MB_TCP, nullptr, pcPortName, ucMinAddr, ucMaxAddr, FALSE};
}

void MBMaster::setMasterTCPDev(sMBSlaveDev* psMBSlaveDev, const CHAR* pcMBServerIP, uint16_t uiMBServerPort)
{
    vMBMasterTCPSetDev(psMBSlaveDev, pcMBServerIP, uiMBServerPort);
}
#endif

bool MBMaster::masterRegistSlaveDev(sMBSlaveDev* psMBNewDev)
{
     return xMBMasterRegistDev(&m_MasterInfo, psMBNewDev);
}

#if MB_MASTER_DTU_ENABLED && MB_MASTER_RTU_ENABLED
bool MBMaster::masterRegistDTUDev(sMBSlaveDev* psMBDTUDev247, sMBSlaveDev* psMBDTUDev200)
{
     return xMBMasterRegistDTUDev(&m_MasterInfo, psMBDTUDev247, psMBDTUDev200);
}
#endif

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED
bool MBMaster::registMasterMode()
{
    return xMBMasterRegistNode(&m_MasterInfo, &m_MBMasterNode);
}

sMBMasterInfo* MBMaster::getMBMasterInfo()
{
    return &m_MasterInfo;
}
#endif

 #if MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED
void MBSlave::slaveUartConfig(uint32_t usBaudRate, uint8_t ucDataBit, uint8_t ucStopBit, uint8_t pcParity)
{
    m_SlaveUart.baud     = usBaudRate;
    m_SlaveUart.data_bit = ucDataBit;
    m_SlaveUart.stop_bit = ucStopBit;
    m_SlaveUart.parity   = pcParity;
}

void MBSlave::initSlavePort(eMBMode eMode, const char* pcPortName, uint8_t ucSlaveAddr)
{

    m_SlaveInfo.eMode = eMode;
    m_SlaveInfo.pLast = nullptr;
    m_SlaveInfo.pNext = nullptr;
    m_SlaveInfo.eMBState = STATE_NOT_INITIALIZED;
    m_SlaveInfo.eRcvState = STATE_RX_IDLE;
    m_SlaveInfo.eSndState = STATE_TX_IDLE;
    m_SlaveInfo.usSndBufferCount = 0;
    m_SlaveInfo.pucSndBufferCur = nullptr;
    m_SlaveInfo.usRcvBufferPos = 0;

    m_MBSlaveNode.eMode = eMode;
    m_MBSlaveNode.ucSlaveAddr = ucSlaveAddr;
    m_MBSlaveNode.pcMBPortName = pcPortName;
    m_MBSlaveNode.psSlaveUart = &m_SlaveUart;
}

void MBSlave::registSlaveCommData(sMBSlaveCommData *psBMSCommData)
{
    if(psBMSCommData)
    {
        m_SlaveInfo.sMBCommInfo.psSlaveCurData = psBMSCommData;
    }
}

bool MBSlave::registSlaveMode()
{
    return xMBSlaveRegistNode(&m_SlaveInfo, &m_MBSlaveNode);
}

void MBSlave::setSlaveAddr(uint8_t ucSlaveAddr)
{
    vMBSlaveSetAddr(&m_SlaveInfo, ucSlaveAddr);
}
#endif

#if MB_SLAVE_TCP_ENABLED

bool MBSlave::registSlaveTcp(uint8_t ucSlaveAddr)
{
    m_SlaveTcpInfo.ucSlaveAddr = ucSlaveAddr;
    return xMBSlaveTCPServerInit(&m_SlaveTcpInfo);
}

void MBSlave::registTcpCommData(sMBSlaveCommData *psBMSCommData)
{
    sMBSlaveInfo *pMBSlaveInfo =nullptr;
    if(psBMSCommData)
    {
        for(uint8_t i = 0; i < MB_SLAVE_MAX_TCP_CLIENT; i++)
        {
            pMBSlaveInfo = &m_SlaveTcpInfo.sMBSlaveTcpClients[i];
            pMBSlaveInfo->sMBCommInfo.psSlaveCurData = psBMSCommData;
        }
    }
}
#endif



/*Modbus* Modbus::getInstance()
{
    return g_pModbus;
}*/
