#ifndef DTU_H
#define DTU_H
#include <QObject>
#include "mb_m.h"

#define REG_HOLD_BUF_NUM   300
//#define REG_HOLD_MAX_ADDR  500

class DTU : public QObject
{
    Q_OBJECT

public:

    uint16_t     m_usUnitID = 0x5601;    //群控产品ID
    uint16_t     m_usProtocolVer = 11;   //协议版本
    uint16_t     m_usSoftWareVer = 10;   //主程序版本
    uint16_t     m_usProductPro = 0x55;  //产品属性

    //9P 40 57 00 00 00 2
    uint16_t     m_usProductID_1 = 0x3950;  //产品条码1
    uint16_t     m_usProductID_2 = 0x3430;  //产品条码2
    uint16_t     m_usProductID_3 = 0x3537;  //产品条码3
    uint16_t     m_usProductID_4 = 0x3030;  //产品条码4
    uint16_t     m_usProductID_5 = 0x3030;  //产品条码5
    uint16_t     m_usProductID_6 = 0x3030;  //产品条码6
    uint16_t     m_usProductID_7 = 0x3230;  //产品条码7
    uint16_t     m_usProductID_8 = 0x3030;  //产品条码8


    uint16_t     m_usProductCode_1 = 0;  //产品密码1
    uint16_t     m_usProductCode_2 = 0;  //产品密码2
    uint16_t     m_usProductCode_3 = 0;  //产品密码3

    uint16_t     m_usStateMask_1 = 0;
    uint16_t     m_usStateMask_2 = 0;
    uint16_t     m_usStateMask_3 = 0;

    sMBSlaveDev          m_sMBDTUDev247;   //本通讯设备
    sMBSlaveDev          m_sMBDTUDev200;   //本通讯设备

private:
    sMBMasterInfo*       m_psMBMasterInfo;   //所属通讯主栈
    sMBSlaveDevCommData  m_sDevCommData;     //本设备通讯数据表

    sMasterRegHoldData  m_psRegHoldBuf[REG_HOLD_BUF_NUM];     //保持寄存器数据域
    //uint16_t  m_usRegHoldIndex[REG_HOLD_MAX_ADDR] = {0};     //保持寄存器数据域映射

private:
    void registDevCommData();

private slots:
    void systemTimeChangedSlot();

public:
    void initComm();
    explicit DTU(QObject *parent = nullptr);
};

#endif // DTU_H
