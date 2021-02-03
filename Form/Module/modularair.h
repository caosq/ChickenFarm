#ifndef MODULARAIR_H
#define MODULARAIR_H

#include <QWidget>
#include "textlabel.h"
#include "device.h"
#include "statebutton.h"
#include "modebutton.h"
#include "datalabel.h"
#include "analogvalbutton.h"

namespace Ui {
class ModularAir;
}

class ModularAir : public Device
{
    Q_OBJECT
public:

    typedef enum
    {
        CMD_OPEN  = 0x00AA,
        CMD_CLOSE = 0x0055,
    }SwitchCmd;

    typedef enum   /*系统模式*/
    {
        MODE_CLOSE     = 0,     //关闭模式
        MODE_MANUAL    = 1,     //手动模式
        MODE_AUTO      = 2,     //自动模式
        MODE_EMERGENCY = 3,     //紧急送风模式
    }SystemMode;

    typedef enum   /*运行模式*/
    {
        RUN_MODE_COOL         = 0,      //供冷
        RUN_MODE_FAN          = 1,      //通风
        RUN_MODE_HEAT         = 2,      //供热
        RUN_MODE_NEGATICE_FAN = 3,      //负压通风
    }RunningMode;

    typedef enum   /*机组状态*/
    {
        STATE_CLOSED    = 0,    //已关闭
        STATE_OPENNING  = 1,    //开机中
        STATE_CLOSING   = 2,    //关机中
        STATE_RUNNING   = 3,    //运行中
    }ModularState;

    typedef enum   /*控制模式*/
    {
        MODE_LOCATE   = 0,    //本地
        MMODE_REMOTE  = 1,    //远程
    }ControlMode;

    uint16_t      m_usUnitID;                //机型ID
    uint16_t      m_usProtocolVer;           //协议版本

    SwitchCmd     m_eSwitchCmd;              //启停命令
    RunningMode   m_eRunningModeCmd;         //机组运行工作模式设定
    ModularState  m_eModularState;           //机组状态
    RunningMode   m_eRunningMode;            //机组运行模式
    ControlMode   m_eControlMode;            //机组控制模式

    uint16_t      m_usTempSet;               //目标温度设定
    uint16_t      m_usHumiSet;               //目标湿度设定
    uint16_t      m_usCO2Set;                //目标CO2设定

    uint16_t      m_usExitAirDamperAng;      //排风阀当前开度
    uint16_t      m_usRetAirDamperAng;       //回风阀当前开度
    uint16_t      m_usFreAirDamperAng;       //新风阀当前开度

    uint16_t      m_usDivideDamperAng;       //分流阀反馈开度
    uint16_t      m_usCoilerDamperAng;       //盘管水阀反馈开度

    uint16_t      m_usSupAirFanFreg;         //送风风机频率
    uint16_t      m_usExitAirFanFreg;        //排风风机频率

    int16_t       m_sSupAirTemp;             //送风温度
    uint16_t      m_usSupAirHumi;            //送风湿度

    uint16_t      m_usExitAirVolume;         //排风量
    uint16_t      m_usFreAirVolume;          //新风量

    bool          m_xRecycleMode;             //转轮回收
    bool          m_xWetMode;                 //湿膜加湿
    bool          m_xSupAirFan;               //送风风机
    bool          m_xExitAirFan;              //排风风机

    bool          m_xCommErr;                 //通讯故障
    bool          m_xAlarmFlag;               //综合故障标志
    bool          m_xErrClean;                //故障清除

    bool          m_xExitAirSenErr;           //排风风速传感器故障
    bool          m_xFreAirSenErr;            //新风风速传感器故障

    static uint8_t  m_usModularAirCount;      //组合柜数量
private:

    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;


    StateButton   *m_pSwitchCmdBtn;         //启停命令
    ModeButton    *m_pRunningModeCmdBtn;     //机组运行工作模式设定
    DataLabel     *m_pModularStateLabel;     //机组状态
    DataLabel     *m_pRunningModeLabel;      //机组运行模式
    DataLabel     *m_pControlModeLabel;      //机组控制模式

    AnalogValButton      *m_pTempSetBtn;     //目标温度设定
    AnalogValButton      *m_pHumiSetBtn;     //目标湿度设定
    AnalogValButton      *m_pCO2SetBtn;      //目标CO2设定

    DataLabel      *m_pExitAirDamperAngLabel;      //排风阀当前开度
    DataLabel      *m_pRetAirDamperAngLabel;       //回风阀当前开度
    DataLabel      *m_pFreAirDamperAngLabel;       //新风阀当前开度
    DataLabel      *m_pDivideDamperAngLabel;       //分流阀反馈开度
    DataLabel      *m_pCoilerDamperAngLabel;       //盘管水阀反馈开度

    DataLabel      *m_pSupAirFanFregLabel;         //送风风机频率
    DataLabel      *m_pExitAirFanFregLabel;        //排风风机频率
    DataLabel      *m_pSupAirTempLabel;            //送风温度
    DataLabel      *m_pSupAirHumiLabel;            //送风湿度
    DataLabel      *m_pExitAirVolumeLabel;         //排风量
    DataLabel      *m_pFreAirVolumeLabel;          //新风量

    DataLabel      *m_pRecycleModeLabel;           //转轮回收
    DataLabel      *m_pWetModeLabel;               //湿膜加湿
    DataLabel      *m_pSupAirFanLabel;             //送风风机
    DataLabel      *m_pExitAirFanLabel;            //排风风机
    DataLabel      *m_pCommErrLabel;               //通讯故障
    DataLabel      *m_pAlarmFlagLabel;             //综合故障标志
    DataLabel      *m_pExitAirSenErrLabel;         //排风风速传感器故障
    DataLabel      *m_pFreAirSenErrLabel;          //新风风速传感器故障

private:
    void initLabel();
    void initButton();

public:
    explicit ModularAir(QWidget *parent = nullptr);
    ~ModularAir();

private:
    Ui::ModularAir *ui;
};

#endif // MODULARAIR_H
