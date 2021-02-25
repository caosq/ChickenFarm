#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "modbus.h"

#define REG_HOLD_BUF_NUM  10
#define BIT_COIL_BUF_NUM  10

class Controller
{
public:
    explicit Controller();
    void initComm(sMBMasterInfo* psMBMasterInfo, uint8_t ucDevAddr);

private:

    sMBMasterInfo*       m_psMBMasterInfo;   //所属通讯主栈
    sMBSlaveDevCommData  m_sDevCommData;     //本设备通讯数据表
    sMBSlaveDev          m_sMBSlaveDev;      //本通讯设备

    sMasterRegHoldData  m_psRegHoldBuf[REG_HOLD_BUF_NUM];     //保持寄存器数据域
    sMasterBitCoilData  m_psBitCoilBuf[BIT_COIL_BUF_NUM];     //线圈数据域

    void registDevCommData();
    static uint8_t devDataMapIndex(eDataType eDataType, uint8_t ucProtocolID, uint16_t usAddr, uint16_t* psIndex);
};

#endif // CONTROLLER_H
