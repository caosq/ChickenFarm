#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include "modularair.h"
#include "modularchiller.h"
#include "chilledbump.h"
#include "temphumisensor.h"
#include "tempsensor.h"
#include "pressuresensor.h"
#include "axialfan.h"
#include "butterflyvalve.h"
#include "bypassvalve.h"
#include "co2sensor.h"
#include "windowfan.h"
#include "meter.h"
#include "controller.h"
#include "messagebox.h"
#include "dtu.h"
#include "modbus.h"
#include "bms.h"

#define MB_MASTER_RTU_ENABLE   1
#define MB_MASTER_TCP_ENABLE   0
#define MB_MASTER_DTU_ENABLE   1

#define MB_SLAVE_RTU_ENABLE    0
#define MB_SLAVE_TCP_ENABLE    0

#define MODULAR_AIR_NUM          2
#define MODULAR_CHILLER_NUM      2
#define CHILLED_BUMP_NUM         3
#define AXIAL_FAN_NUM            4
#define WINDOW_FAN_NUM           2
#define BUTTRERFLY_VALVE_NUM     4
#define CHW_TEMP_SENSOR_NUM      2
#define CHW_PRESSURE_SENSOR_NUM  2

#define TEMP_HUMI_IN_NUM    8
#define TEMP_HUMI_OUT_NUM   2
#define CO2_NUM             4

#define TEMP_HUMI_IN_NUM_IN_MODULAR_AIR   4
#define TEMP_HUMI_OUT_NUM_IN_MODULAR_AIR  1
#define CO2_NUM_IN_MODULAR_AIR            2

#define MODULAR_NUM_IN_CHILLER  4
#define COMP_NUM_IN_CHILLER     2

class System : public QObject
{
    Q_OBJECT
public:

    typedef enum   /*系统模式*/
    {
        MODE_CLOSE     = 0,     //关闭模式
        MODE_MANUAL    = 1,     //手动模式
        MODE_AUTO      = 2,     //自动模式
        MODE_EMERGENCY = 3,     //紧急送风模式
    }SystemMode;

    typedef enum   /*系统模式*/
    {
        STATE_CLOSED       = 0,    //已关闭
        STATE_COOL         = 1,    //制冷运行
        STATE_HEAT         = 2,    //制热运行
        STATE_FAN          = 3,    //送风运行
        STATE_NEGATICE_FAN = 4,    //负压通风运行
        STATE_EX_FAN       = 5,    //排风运行
        STATE_CLOSING      = 6,    //关机中
        STATE_OPENNING     = 7,    //开机中
    }SystemState;

    uint16_t     m_usUnitID = 0x302A;             //机型ID
    uint16_t     m_usProtocolVer = 10;            //协议版本
    SystemMode   m_eSystemModeCmd = MODE_MANUAL;  //系统模式设定
    SystemState  m_eSystemState = STATE_CLOSED;   //系统状态

    int16_t   m_sChickenGrowDay = 0;          //鸡生长周期天数
    uint16_t  m_usCO2PPMSet = 20000;          //目标CO2浓度设定
    uint16_t  m_usTempSet = 240;              //目标温度设定
    uint16_t  m_usHumiSet = 500;              //目标湿度设定
    uint16_t  m_usHumiSet_1 = 650;            //-2天到28天目标湿度
    uint16_t  m_usHumiSet_2 = 550;            //29天到462天目标湿度

    uint32_t  m_ulFreAirSet = 0;               //目标新风风量设定

    uint16_t  m_usEnergyTemp = 250;             //节能温度
    uint16_t  m_usTempDeviat = 5;               //温度偏差
    uint16_t  m_usSupAirMaxTemp = 450;          //送风最大温度

    uint16_t  m_usCoolWaterDamperMinAng_1 = 350;   //水阀制冷最小开度1
    uint16_t  m_usCoolWaterDamperMinAng_2 = 50;    //水阀制冷最小开度2
    uint16_t  m_usHeatWaterDamperMinAng = 0;      //水阀制热最小开度
    uint16_t  m_usExAirDamperMinAng = 100;         //排风阀最小开度

    uint16_t  m_usCO2AdjustDeviat_Up =5000;      //CO2浓度调节上偏差
    uint16_t  m_usCO2AdjustDeviat_Down = 5000;   //CO2浓度调节下偏差
    uint16_t  m_usCO2ExAirDeviat_1 = 2000;       //CO2浓度排风控制偏差1
    uint16_t  m_usCO2ExAirDeviat_2 = 2000;       //CO2浓度排风控制偏差2
    uint16_t  m_usCO2AdjustThr = 27000;          //CO2浓度调节阈值
    uint16_t  m_usCO2PPMAlarm = 30000;           //CO2浓度报警值

    uint16_t  m_usExAirFanMinFreq = 350;        //排风机最小频率
    uint16_t  m_usExAirFanMaxFreq = 500;        //排风机最大频率
    uint16_t  m_usExAirFanAdjustFreq = 10;      //排风机频率调节值
    uint16_t  m_usExAirFanRatedVol_H = 0;       //排风机额定风量
    uint16_t  m_usExAirFanRatedVol_L = 30000;   //排风机额定风量


    uint16_t  m_usExAirFanFreqRunPeriod = 1200; //排风机运行周期
    uint16_t  m_usExAirFanIntervalMin = 60;     //排风机最小启停间隔
    uint16_t  m_usExAirFanRunTimeMin = 300;     //排风机最小运行时间
    uint16_t  m_usExAirFanPlusTime = 180;       //排风机加机时间
    uint16_t  m_usExAirFanSubTime = 180;        //排风机减机时间
    uint16_t  m_usExAirFanFreqAdjustTime = 30;  //排风机频率调节时间

    uint16_t  m_usExAirFanPlusFreq_1 = 350;    //排风机加机频率1
    uint16_t  m_usExAirFanPlusFreq_2 = 380;    //排风机加机频率2
    uint16_t  m_usExAirFanSubFreq_1 = 350;     //排风机减机频率1
    uint16_t  m_usExAirFanSubFreq_2 = 380;     //排风机减机频率2
    uint16_t  m_usExAirFanTempDeviat = 10;     //排风机调节温度偏差
    uint16_t  m_usExAirFanRatio = 90;          //排风机排风百分比

    uint16_t  m_usCHWBumpMinFreq = 350;        //冷冻泵最小频率
    uint16_t  m_usCHWBumpMaxFreq = 500;        //冷冻泵最大频率
    uint16_t  m_usCHWBumpCloseDelay = 10;      //冷冻泵延迟关闭时间
    uint16_t  m_usCHWBumpAdjustPeriod = 300;   //冷冻泵频率调节间隔
    uint16_t  m_usCHWBumpAdjustFreq_1 = 10;    //冷冻泵频率调节值_1
    uint16_t  m_usCHWBumpAdjustFreq_2 = 5;     //冷冻泵频率调节值_2
    uint16_t  m_usCHWBumpAdjustFreq_3 = 10;    //冷冻泵频率调节值_3
    uint16_t  m_usCHWBumpAdjustFreq_4 = 5;     //冷冻泵频率调节值_4

    uint16_t  m_usChilledDevsOpenDelay = 10;   //冷冻设备延迟开启时间
    uint16_t  m_usChilledDevsCloseDelay = 10;  //冷冻设备延迟关闭时间
    uint16_t  m_usButterflyValveCloseDelay = 10; //蝶阀延迟关闭时间

    uint16_t  m_usChillerPlusPeriod = 10;      //机组加机间隔
    uint16_t  m_usChillerSubPeriod = 10;       //机组减机间隔
    uint16_t  m_usChillerPlusRemain = 10;      //机组加机持续判断时间
    uint16_t  m_usChillerSubRemain = 10;       //机组减机持续判断时间
    uint16_t  m_usChillerRespond = 10;         //机组响应时间

    uint16_t  m_usModeChangePeriod_1 = 10;     //模式切换间隔时间t1(min)
    uint16_t  m_usModeChangePeriod_2 = 10;     //模式切换间隔时间t2(min)
    uint16_t  m_usModeChangePeriod_3 = 10;     //模式切换间隔时间t3(min)
    uint16_t  m_usModeChangePeriod_4 = 10;     //模式切换间隔时间t4(min)
    uint16_t  m_usModeChangePeriod_5 = 10;     //模式切换间隔时间t5(min)
    uint16_t  m_usModeChangePeriod_6 = 10;     //模式切换间隔时间t6(min)

    uint16_t  m_usModeAdjustTemp_0 = 10;       //模式调节温度T0
    uint16_t  m_usModeAdjustTemp_1 = 10;       //模式调节温度T1
    uint16_t  m_usModeAdjustTemp_2 = 10;       //模式调节温度T2
    uint16_t  m_usModeAdjustTemp_3 = 10;       //模式调节温度T3
    uint16_t  m_usModeAdjustTemp_4 = 10;       //模式调节温度T4
    uint16_t  m_usModeAdjustTemp_5 = 10;       //模式调节温度T5
    uint16_t  m_usModeAdjustTemp_6 = 10;       //模式调节温度T6

    uint16_t  m_usCHWOutletCoolTemp = 70;      //冷冻总管制冷供水温度设定值
    uint16_t  m_usCHWOutletHeatTemp = 500;     //冷冻总管制热供水温度设定值
    uint16_t  m_usCHWTempDiff = 50;            //冷冻总管供回水温差设定值
    uint16_t  m_usCHWPressureDiff = 240;       //冷冻总管压差设定
    uint16_t  m_usCHWPressureDeviat = 5;       //冷冻压差偏差百分比设定

    uint16_t  m_usCHWBypassRange = 5;         //冷冻旁通阀调节范围设定
    uint16_t  m_usCHWBypassMaxAng = 100;      //冷冻旁通阀最大开度设定
    uint16_t  m_usCHWBypassMinAng = 0;        //冷冻旁通阀最小开度设定
    uint16_t  m_usCHWBypassAdjustTime = 10;   //冷冻旁通阀调节时间设定

    uint16_t  m_usChillerCoolInTemp = 120;     //机组制冷进水温度设定值
    uint16_t  m_usChillerCoolOutTemp = 70;     //机组制冷出水温度设定值
    uint16_t  m_usChillerHeatInTemp = 400;     //机组制热进水温度设定值
    uint16_t  m_usChillerHeatOutTemp = 450;    //机组制热出水温度设定值

    uint16_t  m_usCHCoolPlusSupTempDeviat = 25;  //制冷加机供水温度偏差设定值
    uint16_t  m_usCHCoolPlusTempDiff = 21;       //制冷加机供回水温差偏差设定值
    uint16_t  m_usCHHeatPlusSupTempDeviat = 25;  //制热加机供水温度偏差设定值
    uint16_t  m_usCHHeatPlusTempDiff = 21;       //制热加机供回水温差偏差设定值

    uint16_t  m_usCHCoolSubSupTempDeviat = 20;   //制冷减机供水温度偏差设定值
    uint16_t  m_usCHCoolSubTempDiff = 21;        //制冷减机供回水温差偏差设定值
    uint16_t  m_usCHHeatSubSupTempDeviat = 20;   //制热减机供水温度偏差设定值
    uint16_t  m_usCHHeatSubTempDiff = 21;        //制热减机供回水温差偏差设定值

    uint16_t  m_usCHWBCoolPlusFreqTempDeviat = 50; //制冷水泵升频供水温度偏差设定值
    uint16_t  m_usCHWBCoolPlusFreqTempDiff_1 = 10;    //制冷水泵升频供回水温差偏差设定值_1
    uint16_t  m_usCHWBCoolPlusFreqTempDiff_2 = 5;    //制冷水泵升频供回水温差偏差设定值_2
    uint16_t  m_usCHWBHeatPlusFreqTempDeviat = 50; //制热水泵升频供水温度偏差设定值
    uint16_t  m_usCHWBHeatPlusFreqTempDiff_1 = 10;    //制热水泵升频供回水温差偏差设定值_1
    uint16_t  m_usCHWBHeatPlusFreqTempDiff_2 = 5;    //制热水泵升频供回水温差偏差设定值_2

    uint16_t  m_usCHWBCoolSubFreqTempDeviat = 10;   //制冷水泵降频供水温度偏差设定值
    uint16_t  m_usCHWBCoolSubFreqTempDiff_1 = 10;    //制冷水泵降频供回水温差偏差设定值_1
    uint16_t  m_usCHWBCoolSubFreqTempDiff_2 = 5;    //制冷水泵降频供回水温差偏差设定值_2
    uint16_t  m_usCHWBHeatSubFreqTempDeviat = 10;   //制热水泵降频供水温度偏差设定值
    uint16_t  m_usCHWBHeatSubFreqTempDiff_1 = 10;    //制热水泵降频供回水温差偏差设定值_1
    uint16_t  m_usCHWBHeatSubFreqTempDiff_2 = 5;    //制热水泵降频供回水温差偏差设定值_2

    int16_t   m_sAmbientIn_T = 0;             //室内环境干球温度
    int16_t   m_sAmbientOut_T = 0;            //室外环境干球温度
    uint16_t  m_usAmbientIn_H = 0;            //室内环境湿度
    uint16_t  m_usAmbientOut_H = 0;           //室外环境湿度
    uint16_t  m_usCO2PPM = 0;                 //室内CO2浓度
    uint16_t  m_usTotalFreAir_H = 0;          //室内新风风量
    uint16_t  m_usTotalFreAir_L = 0;          //室内新风风量

    uint16_t  m_usSysYear = 1970;             //系统时间年
    uint16_t  m_usSysMon = 1;                 //系统时间月
    uint16_t  m_usSysDay = 1;                 //系统时间日
    uint16_t  m_usSysHour = 0;                //系统时间时
    uint16_t  m_usSysMin = 0;                 //系统时间分
    uint16_t  m_usSysSec = 0;                 //系统时间秒

    uint32_t  m_ulExAirFanRatedVol = 30000;  //排风机额定风量
    uint32_t  m_ulTotalFreAir = 0;           //室内新风风量
    uint32_t  m_uiOffLogCount = 0;           //系统没进行任何操作计时

    bool      m_xAlarmEnable = 0;             //声光报警使能
    bool      m_xAlarmClean = 0;              //声光报警清除
    bool      m_xIsLogIn = false;             //系统是否登录
    bool      m_xIsFactoryLogIn = false;      //是否厂家登录
    bool      m_xIsInDebug = false;           //系统是否处于调试模式

    QVector<ModularAir*>     m_pModularAirs;        //组空
    QVector<ModularChiller*> m_pModularChillers;    //机组
    QVector<ChilledBump*>    m_pChilledBumps;       //冷冻泵
    QVector<AxialFan*>       m_pAxialFans;          //畜牧风机
    QVector<WindowFan*>      m_pWindowFans;         //小窗电机
    QVector<ButterflyValve*> m_pButterflyValves;    //蝶阀
    QVector<TempSensor*>     m_pCHWTempSensors;     //冷冻总管温度传感器
    QVector<PressureSensor*> m_pCHWPressureSensors; //冷冻总管压力传感器

    BypassValve *m_pBypassValve;                 //旁通阀
    Meter       *m_pChillerMeter;                //机组电表
    Meter       *m_pBumpMeter;                   //水泵电表

#if MB_MASTER_TCP_ENABLE
    MBMaster    m_ModbusTCP;   //TCP协议栈
#endif

#if MB_MASTER_RTU_ENABLE
    MBMaster    m_Modbus;       //RTU协议栈
#endif

#if MB_MASTER_DTU_ENABLE
    MBMaster    m_ModbusDTU;   //DTU协议栈
#endif

#if MB_SLAVE_RTU_ENABLE || MB_SLAVE_TCP_ENABLE
    MBSlave     m_ModbusBMS;   //BMS协议栈
    BMS         m_BMS;         //BMS接口
#endif

    Controller  m_Controller;   //控制器
    DTU         m_DTU;         //GPRS模块

    Monitor     *m_pSysModeCmdMonitor;          //系统模式监控
    Monitor     *m_pExAirFanRatedVolMonitor_H;  //系统排风机额定风量监控
    Monitor     *m_pExAirFanRatedVolMonitor_L;  //系统排风机额定风量监控
    Monitor     *m_pTotalFreAirMonitor_H;       //系统室内新风风量监控
    Monitor     *m_pTotalFreAirMonitor_L;       //系统室内新风风量监控

    messageBox  *pConfirmationBox = nullptr;

public:
    void initController();
    bool checkSystemLogIn();
    void systemStackChanged(int32_t iVal);
    static System* getInstance();

private:
    QTimer m_Timer;
    explicit System(QObject *parent = nullptr);
    static System *g_pSystem;

signals:
    void systemTimeChanged();
    void systemDataChanged();
    void stackChanged(int32_t);
    void systemLogChanged(bool);

private slots:
    void devOfflineChangedSlot(bool xVal);
    void syncChangedSlot(bool xVal);
    void systemTimeOut();
    void systemDataChanged(Monitor* pMonitor);
};

#endif // SYSTEM_H
