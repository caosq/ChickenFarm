#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>

class System
{
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

    uint16_t     m_usUnitID;           //机型ID
    uint16_t     m_usProtocolVer;      //协议版本
    SystemMode   m_eSystemModeCmd;     //系统模式设定
    SystemState  m_eSystemState;       //系统状态

    int16_t   m_sChickenGrowDays;         //鸡生长周期天数
    uint16_t  m_usCO2PPMSet;              //目标CO2浓度设定
    uint16_t  m_usTempSet;                //目标温度设定
    uint16_t  m_usHumiSet;                //目标湿度设定
    uint32_t  m_ulFreAirSet;              //目标新风风量设定

    uint16_t  m_usEnergyTemp;             //节能温度
    uint16_t  m_usTempDeviat;             //温度偏差
    uint16_t  m_usSupAirMax_T;            //送风最大温度

    uint16_t  m_usCoolWaterDamperMinAng_1;    //水阀制冷最小开度1
    uint16_t  m_usCoolWaterDamperMinAng_2;    //水阀制冷最小开度2
    uint16_t  m_usHeatWaterDamperMinAng_2;    //水阀制热最小开度
    uint16_t  m_usExAirDamperMinAng;          //排风阀最小开度

    uint16_t  m_usCO2AdjustDeviat_Up;     //CO2浓度调节上偏差
    uint16_t  m_usCO2AdjustDeviat_Down;   //CO2浓度调节下偏差
    uint16_t  m_usCO2ExAirDeviat_1;       //CO2浓度排风控制偏差1
    uint16_t  m_usCO2ExAirDeviat_2;       //CO2浓度排风控制偏差2
    uint16_t  m_usCO2AdjustThr;           //CO2浓度调节阈值
    uint16_t  m_usCO2PPMAlarm;            //CO2浓度报警值

    uint16_t  m_usExAirFanMinFreq;        //排风机最小频率
    uint16_t  m_usExAirFanMaxFreq;        //排风机最大频率
    uint16_t  m_usExAirFanAdjustFreq;     //排风机频率调节值
    uint32_t  m_ulExAirFanRatedVol;       //排风机额定风量
    uint16_t  m_usExAirFanFreqRunPeriod;  //排风机运行周期
    uint16_t  m_usExAirFanIntervalMin;    //排风机最小启停间隔
    uint16_t  m_usExAirFanRunTimeMin;     //排风机最小运行时间
    uint16_t  m_usExAirFanPlusTime;       //排风机加机时间
    uint16_t  m_usExAirFanSubTime;        //排风机减机时间
    uint16_t  m_usExAirFanFreqAdjustTime; //排风机频率调节时间

    uint16_t  m_usExAirFanPlusFreq_1;     //排风机加机频率1
    uint16_t  m_usExAirFanPlusFreq_2;     //排风机加机频率2
    uint16_t  m_usExAirFanSubFreq_1;      //排风机减机频率1
    uint16_t  m_usExAirFanSubFreq_2;      //排风机减机频率2
    uint16_t  m_usExAirFanTempDeviat;     //排风机调节温度偏差
    uint16_t  m_usExAirFanRatio;          //排风机排风百分比

    uint16_t  m_usCHWBumpMinFreq;         //冷冻泵最小频率
    uint16_t  m_usCHWBumpMaxFreq;         //冷冻泵最大频率
    uint16_t  m_usCHWBumpCloseDelay;      //冷冻泵延迟关闭时间
    uint16_t  m_usCHWBumpAdjustPeriod;    //冷冻泵频率调节间隔
    uint16_t  m_usCHWBumpAdjustFeaq;      //冷冻泵频率调节值

    uint16_t  m_usChilledDevsOpenDelay;   //冷冻设备延迟开启时间
    uint16_t  m_usChilledDevsCloseDelay;  //冷冻设备延迟关闭时间
    uint16_t  m_usButterflyValveCloseDelay; //蝶阀延迟关闭时间

    uint16_t  m_usChillerPlusPeriod;      //机组加机间隔
    uint16_t  m_usChillerSubPeriod;       //机组减机间隔
    uint16_t  m_usChillerPlusRemain;      //机组加机持续判断时间
    uint16_t  m_usChillerSubRemain;       //机组减机持续判断时间
    uint16_t  m_usChillerRespond;         //机组响应时间

    uint16_t  m_usModeChangePeriod_1;     //模式切换间隔时间t1(min)
    uint16_t  m_usModeChangePeriod_2;     //模式切换间隔时间t2(min)
    uint16_t  m_usModeChangePeriod_3;     //模式切换间隔时间t3(min)
    uint16_t  m_usModeChangePeriod_4;     //模式切换间隔时间t4(min)
    uint16_t  m_usModeChangePeriod_5;     //模式切换间隔时间t5(min)
    uint16_t  m_usModeChangePeriod_6;     //模式切换间隔时间t6(min)

    uint16_t  m_usModeAdjustTemp_0;       //模式调节温度T0
    uint16_t  m_usModeAdjustTemp_1;       //模式调节温度T1
    uint16_t  m_usModeAdjustTemp_2;       //模式调节温度T2
    uint16_t  m_usModeAdjustTemp_3;       //模式调节温度T3
    uint16_t  m_usModeAdjustTemp_4;       //模式调节温度T4
    uint16_t  m_usModeAdjustTemp_5;       //模式调节温度T5
    uint16_t  m_usModeAdjustTemp_6;       //模式调节温度T6

    uint16_t  m_usCHWOutletCoolTemp;      //冷冻总管制冷供水温度设定值
    uint16_t  m_usCHWOutletHeatTemp;      //冷冻总管制热供水温度设定值
    uint16_t  m_usCHWTempDiff;            //冷冻总管供回水温差设定值
    uint16_t  m_usCHWPressureDiff;        //冷冻总管压差设定
    uint16_t  m_usCHWPressureDeviat;      //冷冻压差偏差百分比设定

    uint16_t  m_usCHWBypassRange;         //冷冻旁通阀调节范围设定
    uint16_t  m_usCHWBypassMaxAng;        //冷冻旁通阀最大开度设定
    uint16_t  m_usCHWBypassMinAng;        //冷冻旁通阀最小开度设定
    uint16_t  m_usCHWBypassAdjustTime;    //冷冻旁通阀调节时间设定
    uint16_t  m_usCHWBypassAngSet;        //冷冻旁通阀开度设定
    uint16_t  m_usCHWBypassAng;           //冷冻旁通阀开度反馈

    uint16_t  m_usChillerCoolInTemp;      //机组制冷进水温度设定值
    uint16_t  m_usChillerCoolOutTemp;     //机组制冷出水温度设定值
    uint16_t  m_usChillerHeatInTemp;      //机组制热进水温度设定值
    uint16_t  m_usChillerHeatOutTemp;     //机组制热出水温度设定值

    uint16_t  m_usCHCoolPlusSupTempDeviat;  //制冷加机供水温度偏差设定值
    uint16_t  m_usCHCoolPlusTempDiff;       //制冷加机供回水温差偏差设定值
    uint16_t  m_usCHHeatPlusSupTempDeviat;  //制热加机供水温度偏差设定值
    uint16_t  m_usCHHeatPlusTempDiff;       //制热加机供回水温差偏差设定值

    uint16_t  m_usCHCoolSubSupTempDeviat;   //制冷减机供水温度偏差设定值
    uint16_t  m_usCHCoolSubTempDiff;        //制冷减机供回水温差偏差设定值
    uint16_t  m_usCHHeatSubSupTempDeviat;   //制热减机供水温度偏差设定值
    uint16_t  m_usCHHeatSubTempDiff;        //制热减机供回水温差偏差设定值

    uint16_t  m_usCHWBCoolPlusFreqTempDeviat;  //制冷水泵升频供水温度偏差设定值
    uint16_t  m_usCHWBCoolPlusFreqTempDiff;    //制冷水泵升频供回水温差偏差设定值
    uint16_t  m_usCHWBHeatPlusFreqTempDeviat;  //制热水泵升频供水温度偏差设定值
    uint16_t  m_usCHWBHeatPlusFreqTempDiff;    //制热水泵升频供回水温差偏差设定值

    uint16_t  m_usCHWBCoolSubFreqTempDeviat;  //制冷水泵降频供水温度偏差设定值
    uint16_t  m_usCHWBCoolSubFreqTempDiff;    //制冷水泵降频供回水温差偏差设定值
    uint16_t  m_usCHWBHeatSubFreqTempDeviat;  //制热水泵降频供水温度偏差设定值
    uint16_t  m_usCHWBHeatSubFreqTempDiff;    //制热水泵降频供回水温差偏差设定值

    int16_t   m_sAmbientIn_T;             //室内环境干球温度
    int16_t   m_sAmbientOut_T;            //室外环境干球温度
    uint16_t  m_usAmbientIn_H;            //室内环境湿度
    uint16_t  m_usAmbientOut_H;           //室外环境湿度
    uint16_t  m_usCO2PPM;                 //室内CO2浓度
    uint32_t  m_ulTotalFreAir;            //室内新风风量

    uint16_t  m_usCHWOutletTemp;          //冷冻总管出水温度
    uint16_t  m_usCHWInletTemp;           //冷冻总管回水温度
    uint16_t  m_usCHWOutletPressure;      //冷冻总管出水压力
    uint16_t  m_usCHWInletPressure;       //冷冻总管回水压力

    uint16_t  m_usSysYear;              //系统时间年
    uint16_t  m_usSysMon;               //系统时间月
    uint16_t  m_usSysDay;               //系统时间日
    uint16_t  m_usSysHour;              //系统时间时
    uint16_t  m_usSysMin;               //系统时间分
    uint16_t  m_usSysSec;               //系统时间秒

    uint16_t  m_usCHWBPower;            //冷冻泵实时功率
    uint16_t  m_usCHWBTotalEnergy_L;    //冷冻泵累计耗电量低位
    uint16_t  m_usCHWBTotalEnergy_H;    //冷冻泵累计耗电量高位

    uint16_t  m_usCHPower;              //机组实时功率
    uint16_t  m_usCHTotalEnergy_L;      //机组累计耗电量低位
    uint16_t  m_usCHTotalEnergy_H;      //机组累计耗电量高位

    bool      xAlarmEnable;             //声光报警使能
    bool      xAlarmClean;              //声光报警清除

public:
    System();
};

#endif // SYSTEM_H
