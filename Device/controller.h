#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mb_m.h"
#include "datamonitor.h"
#include <QTimer>

#define REG_HOLD_BUF_NUM  400
#define BIT_COIL_BUF_NUM  300

#define REG_HOLD_MAX_ADDR  420
#define BIT_COIL_MAX_ADDR  300

class Controller : public QObject
{
    Q_OBJECT
public:
    sMBSlaveDev  m_sMBSlaveDev;   //本通讯设备
    bool m_xCommErr = false;      //通讯故障
    bool m_xOffline = false;      //设备掉线
    bool m_xSynchronizing = false; //设备正在同步

public:
    explicit Controller(QObject *parent = nullptr);
    void initComm(uint8_t ucDevAddr);

private:
    QTimer   m_DelayTimer;        //通讯故障判断延时
    Monitor *m_pCommErrMonitor;   //通讯故障监控
    Monitor *m_pSyncMonitor;      //数据同步

    sMBMasterInfo*       m_psMBMasterInfo;   //所属通讯主栈
    sMBSlaveDevCommData  m_sDevCommData;     //本设备通讯数据表

    sMasterRegHoldData  m_psRegHoldBuf[REG_HOLD_BUF_NUM];     //保持寄存器数据域
    sMasterBitCoilData  m_psBitCoilBuf[BIT_COIL_BUF_NUM];     //线圈数据域

    uint16_t  m_usRegHoldIndex[REG_HOLD_MAX_ADDR] = {0};     //保持寄存器数据域映射
    uint16_t  m_usBitCoilIndex[BIT_COIL_MAX_ADDR] = {0};     //线圈数据域映射

    void registDevCommData();
    static uint8_t devDataMapIndex(eDataType eDataType, uint8_t ucProtocolID, uint16_t usAddr, uint16_t* psIndex);

signals:
    void offlineChanged(bool);
    void syncChanged(bool);

public slots:
    void delayTimeOutSlot();
    void dataChanged(Monitor*);
    void syncChangedSlot(Monitor*);
};

#endif // CONTROLLER_H
