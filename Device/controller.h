#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "mb_m.h"

class Controller
{
public:
    explicit Controller();
    bool initComm(sMBMasterInfo* psMBMasterInfo, uint16_t usRegHoldBufNum, uint16_t usRegInBufNum, uint16_t usBitCoilBufNum, uint16_t usBitDiscBufNum);

private:
    sMBMasterInfo*       m_psMBMasterInfo;             //所属通讯主栈
    sMBSlaveDevCommData  m_sDevCommData;               //本设备通讯数据表
    sMBSlaveDev          m_sMBSlaveDev;                //本通讯设备

    sMasterRegHoldData*  m_psRegHoldBuf;     //保持寄存器数据域
    sMasterBitCoilData*  m_psBitCoilBuf;     //线圈数据域

    void registDevCommData();
    bool devDataMapIndex(eDataType eDataType, uint8_t ucProtocolID, uint16_t usAddr, uint16_t* psIndex);
};

#endif // CONTROLLER_H
