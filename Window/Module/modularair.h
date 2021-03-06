#ifndef MODULARAIR_H
#define MODULARAIR_H

#include <QWidget>
#include "textlabel.h"
#include "device.h"
#include "statebutton.h"
#include "modebutton.h"
#include "datalabel.h"
#include "analogvalbutton.h"
#include "meter.h"
#include "temphumisensor.h"
#include "co2sensor.h"

#define TEMP_HUMI_IN_SENSOR_NUM    4
#define CO2_SENSOR_NUM             2

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

    typedef enum   /*组空状态*/
    {
        AIR_RUN_MODE_COOL         = 0,      //供冷
        AIR_RUN_MODE_FAN          = 1,      //通风
        AIR_RUN_MODE_HEAT         = 2,      //供热
        AIR_RUN_MODE_NEGATICE_FAN = 3,      //负压通风
        AIR_RUN_MODE_CLOSED       = 4,      //已关闭
    }AirRunState;

    typedef enum   /*控制模式*/
    {
        MODE_LOCATE   = 0,    //本地
        MMODE_REMOTE  = 1,    //远程
    }ControlMode;

    uint16_t      m_usUnitID = 0x402C;          //机型ID
    uint16_t      m_usProtocolVer = 10;         //协议版本

    SwitchCmd     m_eSwitchCmd = CMD_CLOSE;              //启停命令
    RunningMode   m_eRunningModeCmd = RUN_MODE_COOL;     //机组运行工作模式设定
    RunningMode   m_eRunningMode = RUN_MODE_COOL;        //机组运行模式
    ModularState  m_eModularState = STATE_CLOSED;        //机组开关机状态
    AirRunState   m_eRunningState = AIR_RUN_MODE_CLOSED; //机组运行状态
    ControlMode   m_eControlMode = MODE_LOCATE;          //机组控制模式

    uint16_t      m_usTempSet = 240;         //目标温度设定
    uint16_t      m_usHumiSet = 600;          //目标湿度设定
    uint16_t      m_usCO2Set = 20000;         //目标CO2设定

    uint16_t      m_usExitAirDamperAng = 0;      //排风阀当前开度
    uint16_t      m_usRetAirDamperAng = 0;       //回风阀当前开度
    uint16_t      m_usFreAirDamperAng = 0;       //新风阀当前开度

    uint16_t      m_usDivideDamperAng = 0;       //分流阀反馈开度
    uint16_t      m_usCoilerDamperAng = 0;       //盘管水阀反馈开度

    uint16_t      m_usSupAirFanFreg = 0;         //送风风机频率
    uint16_t      m_usExitAirFanFreg = 0;        //排风风机频率

    int16_t       m_sSupAirTemp = 0;             //送风温度
    uint16_t      m_usSupAirHumi = 0;            //送风湿度

    uint16_t      m_usExitAirVolume = 0;         //排风量
    uint16_t      m_usFreAirVolume = 0;          //新风量

    uint16_t      m_usPower= 0;                 //实时功率
    uint16_t      m_usTotalEnergy_L = 0;         //累计耗电量低位
    uint16_t      m_usTotalEnergy_H = 0;         //累计耗电量高位

    bool          m_xRecycleMode = 0;             //转轮回收
    bool          m_xWetMode = 0;                 //湿膜加湿
    bool          m_xSupAirFan = 0;               //送风风机
    bool          m_xExitAirFan= 0;              //排风风机

    bool          m_xCommErr = 0;                 //通讯故障
    bool          m_xAlarmFlag = 0;               //综合故障标志
    bool          m_xErrClean = 0;                //故障清除

    bool          m_xExitAirSenErr = 0;           //排风风速传感器故障
    bool          m_xFreAirSenErr = 0;            //新风风速传感器故障

    Meter         m_sMeter;                       //电表

    QVector<CO2Sensor*>      m_pCO2Sensors;        //CO2传感器
    QVector<TempHumiSensor*> m_pTempHumiInSensors; //室内温湿度传感器
    TempHumiSensor *m_pTempHumiOutSensor;          //室外温湿度传感器

    static uint8_t  m_usModularAirCount;      //组合柜数量

public:
    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;


    StateButton   *m_pSwitchCmdBtn;          //启停命令
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

    Monitor        *m_pSysModeCmdMonitor;           //系统模式监控；

private:
    void initLabel();
    void initButton();

private slots:
     void stateChangedSlot(int32_t);

public:
    explicit ModularAir(QWidget *parent = nullptr);
    ~ModularAir();

private:
    Ui::ModularAir *ui;
};

#endif // MODULARAIR_H
