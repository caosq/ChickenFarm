#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>
#include "mbdriver.h"
#include "mb.h"
#include "mb_m.h"

typedef enum {
    None 	= 0,					/*!< No parity */
    Even, 							/*!< Even parity */
    Odd,	 						/*!< Odd parity */
    Sp_1, 							/*!< Forced "1" stick parity */
    Sp_0 							/*!< Forced "0" stick parity */
}eParityType;

class MBMaster : public QObject
{
public:
    void masterUartConfig(uint32_t usBaudRate, uint8_t ucDataBit, uint8_t ucStopBit, uint8_t pcParity);

    void initMasterPort(eMBMode eMode, const char* pcPortName, uint8_t ucMinAddr, uint8_t ucMaxAddr, bool bDTUEnable);
    void initMasterTCPPort(const char* pcPortName, uint8_t ucMinAddr, uint8_t ucMaxAddr);
    void setMasterTCPDev(sMBSlaveDev* psMBSlaveDev, const CHAR* pcMBServerIP, uint16_t uiMBServerPort);

    bool masterRegistSlaveDev(sMBSlaveDev* psMBNewDev);
    bool masterRegistDTUDev(sMBSlaveDev* psMBDTUDev247, sMBSlaveDev* psMBDTUDev200);

    bool registMasterMode();

    sMBMasterInfo* getMBMasterInfo();
    //static Modbus* getInstance();
    explicit MBMaster(QObject *parent = nullptr);

private:
    sMBMasterInfo     m_MasterInfo;  //主栈接口
    sMBMasterNodeInfo m_MBMasterNode;

    eMBType    m_MBType;
#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED
    sUART_Def  m_MasterUart;
#endif
};

class MBSlave : public QObject
{
public:
    void slaveUartConfig(uint32_t usBaudRate, uint8_t ucDataBit, uint8_t ucStopBit, uint8_t pcParity);
    void initSlavePort(eMBMode eMode, const char* pcPortName, uint8_t ucSlaveAddr);

    void registSlaveCommData(sMBSlaveCommData *psBMSCommData);
    bool registSlaveMode();

    bool registSlaveTcp(uint8_t ucSlaveAddr);
    void registTcpCommData(sMBSlaveCommData *psBMSCommData);

    void setSlaveAddr(uint8_t ucSlaveAddr);

private:
    eMBType    m_MBType;
    sMBSlaveNodeInfo  m_MBSlaveNode;

#if MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED
    sMBSlaveInfo m_SlaveInfo;  //从栈接口
    sUART_Def    m_SlaveUart;
#endif
#if MB_SLAVE_TCP_ENABLED
    sMBSlaveTcpInfo   m_SlaveTcpInfo;
#endif
};




#endif // MODBUS_H
