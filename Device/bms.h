#ifndef BMS_H
#define BMS_H

#include <QObject>
#include "mb.h"

#define REG_HOLD_BUF_NUM  400
#define BIT_COIL_BUF_NUM  300

#define REG_HOLD_MAX_ADDR  420
#define BIT_COIL_MAX_ADDR  300

class BMS : public QObject
{
    Q_OBJECT
public:
    explicit BMS(QObject *parent = nullptr);
    void initComm();

    sMBSlaveCommData  m_sBMSCommData;  //通讯数据表
    bool m_xCommDataInited = false;

private:
    void registCommData();

    //sMBSlaveInfo*     psBMSInfo;     //所属通讯协议栈

    sMBSlaveRegData  m_psRegHoldBuf[REG_HOLD_BUF_NUM];     //保持寄存器数据域
    sMBSlaveBitData  m_psBitCoilBuf[BIT_COIL_BUF_NUM];     //线圈数据域

    uint16_t  m_usRegHoldIndex[REG_HOLD_MAX_ADDR] = {0};     //保持寄存器数据域映射
    uint16_t  m_usBitCoilIndex[BIT_COIL_MAX_ADDR] = {0};     //线圈数据域映射
};

#endif // BMS_H
