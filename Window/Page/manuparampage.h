#ifndef MANUPARAMPAGE_H
#define MANUPARAMPAGE_H

#include <QWidget>
#include "datalabel.h"
#include "textlabel.h"
#include "analogvalbutton.h"

namespace Ui {
class ManuParamPage;
}

class ManuParamPage : public QWidget
{
    Q_OBJECT

private:
    AnalogValButton *m_pCoolWaterDamperMinAng_1;    //水阀制冷最小开度1
    AnalogValButton *m_pCoolWaterDamperMinAng_2;    //水阀制冷最小开度2
    AnalogValButton *m_pHeatWaterDamperMinAng ;      //水阀制热最小开度
    AnalogValButton *m_pExAirDamperMinAng;          //排风阀最小开度

    AnalogValButton *m_pCO2AdjustDeviat_Up;     //CO2浓度调节上偏差
    AnalogValButton *m_pCO2AdjustDeviat_Down;   //CO2浓度调节下偏差
    AnalogValButton *m_pCO2ExAirDeviat_1;       //CO2浓度排风控制偏差1
    AnalogValButton *m_pCO2ExAirDeviat_2;       //CO2浓度排风控制偏差2
    AnalogValButton *m_pCO2AdjustThr;           //CO2浓度调节阈值

    AnalogValButton *m_pExAirFanMinFreq;        //排风机最小频率
    AnalogValButton *m_pExAirFanMaxFreq;        //排风机最大频率
    AnalogValButton *m_pExAirFanAdjustFreq;     //排风机频率调节值
    AnalogValButton *m_pExAirFanRatedVol;       //排风机额定风量
    AnalogValButton *m_pExAirFanFreqRunPeriod;  //排风机运行周期
    AnalogValButton *m_pExAirFanIntervalMin;    //排风机最小启停间隔
    AnalogValButton *m_pExAirFanRunTimeMin;     //排风机最小运行时间
    AnalogValButton *m_pExAirFanPlusTime;       //排风机加机时间
    AnalogValButton *m_pExAirFanSubTime;        //排风机减机时间
    AnalogValButton *m_pExAirFanFreqAdjustTime; //排风机频率调节时间

    AnalogValButton *m_pExAirFanPlusFreq_1;     //排风机加机频率1
    AnalogValButton *m_pExAirFanPlusFreq_2;     //排风机加机频率2
    AnalogValButton *m_pExAirFanSubFreq_1;      //排风机减机频率1
    AnalogValButton *m_pExAirFanSubFreq_2;      //排风机减机频率2
    AnalogValButton *m_pExAirFanTempDeviat;     //排风机调节温度偏差
    AnalogValButton *m_pExAirFanRatio;          //排风机排风百分比

    AnalogValButton *m_pCHWBumpMinFreq;         //冷冻泵最小频率
    AnalogValButton *m_pCHWBumpMaxFreq;         //冷冻泵最大频率
    AnalogValButton *m_pCHWBumpCloseDelay;      //冷冻泵延迟关闭时间
    AnalogValButton *m_pCHWBumpAdjustPeriod;    //冷冻泵频率调节间隔
    AnalogValButton *m_pCHWBumpAdjustFreq_1;    //冷冻泵频率调节值-1
    AnalogValButton *m_pCHWBumpAdjustFreq_2;    //冷冻泵频率调节值-2
    AnalogValButton *m_pCHWBumpAdjustFreq_3;    //冷冻泵频率调节值-3
    AnalogValButton *m_pCHWBumpAdjustFreq_4;    //冷冻泵频率调节值-4

    AnalogValButton *m_pChilledDevsOpenDelay;   //冷冻设备延迟开启时间
    AnalogValButton *m_pChilledDevsCloseDelay;  //冷冻设备延迟关闭时间
    AnalogValButton *m_pButterflyValveCloseDelay; //蝶阀延迟关闭时间

    AnalogValButton *m_pChillerPlusPeriod;      //机组加机间隔
    AnalogValButton *m_pChillerSubPeriod;       //机组减机间隔
    AnalogValButton *m_pChillerPlusRemain;      //机组加机持续判断时间
    AnalogValButton *m_pChillerSubRemain;       //机组减机持续判断时间
    AnalogValButton *m_pChillerRespond;         //机组响应时间

    AnalogValButton *m_pModeChangePeriod_1;     //模式切换间隔时间t1(min)
    AnalogValButton *m_pModeChangePeriod_2;     //模式切换间隔时间t2(min)
    AnalogValButton *m_pModeChangePeriod_3;     //模式切换间隔时间t3(min)
    AnalogValButton *m_pModeChangePeriod_4;     //模式切换间隔时间t4(min)
    AnalogValButton *m_pModeChangePeriod_5;     //模式切换间隔时间t5(min)
    AnalogValButton *m_pModeChangePeriod_6;     //模式切换间隔时间t6(min)

    AnalogValButton *m_pModeAdjustTemp_0;       //模式调节温度T0
    AnalogValButton *m_pModeAdjustTemp_1;       //模式调节温度T1
    AnalogValButton *m_pModeAdjustTemp_2;       //模式调节温度T2
    AnalogValButton *m_pModeAdjustTemp_3;       //模式调节温度T3
    AnalogValButton *m_pModeAdjustTemp_4;       //模式调节温度T4
    AnalogValButton *m_pModeAdjustTemp_5;       //模式调节温度T5
    AnalogValButton *m_pModeAdjustTemp_6;       //模式调节温度T6

    AnalogValButton *m_pCHWOutletCoolTemp;      //冷冻总管制冷供水温度设定值
    AnalogValButton *m_pCHWOutletHeatTemp;      //冷冻总管制热供水温度设定值
    AnalogValButton *m_pCHWTempDiff;            //冷冻总管供回水温差设定值
    AnalogValButton *m_pCHWPressureDiff;        //冷冻总管压差设定
    AnalogValButton *m_pCHWPressureDeviat;      //冷冻压差偏差百分比设定

    AnalogValButton *m_pCHWBypassRange;         //冷冻旁通阀调节范围设定
    AnalogValButton *m_pCHWBypassMaxAng;        //冷冻旁通阀最大开度设定
    AnalogValButton *m_pCHWBypassMinAng;        //冷冻旁通阀最小开度设定
    AnalogValButton *m_pCHWBypassAdjustTime;    //冷冻旁通阀调节时间设定
    AnalogValButton *m_pCHWBypassAngSet;        //冷冻旁通阀开度设定
    AnalogValButton *m_pCHWBypassAng;           //冷冻旁通阀开度反馈

    AnalogValButton *m_pCHCoolPlusSupTempDeviat;  //制冷加机供水温度偏差设定值
    AnalogValButton *m_pCHCoolPlusTempDiff;       //制冷加机供回水温差偏差设定值
    AnalogValButton *m_pCHHeatPlusSupTempDeviat;  //制热加机供水温度偏差设定值
    AnalogValButton *m_pCHHeatPlusTempDiff;       //制热加机供回水温差偏差设定值

    AnalogValButton *m_pCHCoolSubSupTempDeviat;   //制冷减机供水温度偏差设定值
    AnalogValButton *m_pCHCoolSubTempDiff;        //制冷减机供回水温差偏差设定值
    AnalogValButton *m_pCHHeatSubSupTempDeviat;   //制热减机供水温度偏差设定值
    AnalogValButton *m_pCHHeatSubTempDiff;        //制热减机供回水温差偏差设定值

    AnalogValButton *m_pCHWBCoolPlusFreqTempDeviat;   //制冷水泵升频供水温度偏差设定值
    AnalogValButton *m_pCHWBCoolPlusFreqTempDiff_1;   //制冷水泵升频供回水温差偏差设定值_1
    AnalogValButton *m_pCHWBCoolPlusFreqTempDiff_2;   //制冷水泵升频供回水温差偏差设定值_2
    AnalogValButton *m_pCHWBHeatPlusFreqTempDeviat;   //制热水泵升频供水温度偏差设定值
    AnalogValButton *m_pCHWBHeatPlusFreqTempDiff_1;   //制热水泵升频供回水温差偏差设定值_1
    AnalogValButton *m_pCHWBHeatPlusFreqTempDiff_2;   //制热水泵升频供回水温差偏差设定值_2

    AnalogValButton *m_pCHWBCoolSubFreqTempDeviat;    //制冷水泵降频供水温度偏差设定值
    AnalogValButton *m_pCHWBCoolSubFreqTempDiff_1;    //制冷水泵降频供回水温差偏差设定值_1
    AnalogValButton *m_pCHWBCoolSubFreqTempDiff_2;    //制冷水泵降频供回水温差偏差设定值_2
    AnalogValButton *m_pCHWBHeatSubFreqTempDeviat;    //制热水泵降频供水温度偏差设定值
    AnalogValButton *m_pCHWBHeatSubFreqTempDiff_1;    //制热水泵降频供回水温差偏差设定值_1
    AnalogValButton *m_pCHWBHeatSubFreqTempDiff_2;    //制热水泵降频供回水温差偏差设定值_2

    AnalogValButton *m_pProductID_1;  //产品条码1
    AnalogValButton *m_pProductID_2;  //产品条码2
    AnalogValButton *m_pProductID_3;  //产品条码3
    AnalogValButton *m_pProductID_4;  //产品条码4
    AnalogValButton *m_pProductID_5;  //产品条码5
    AnalogValButton *m_pProductID_6;  //产品条码6
    AnalogValButton *m_pProductID_7;  //产品条码7
    AnalogValButton *m_pProductID_8;  //产品条码8

    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

private:
    void initLabel();
    void initButton();

public:
    //void initButton();


    explicit ManuParamPage(QWidget *parent = nullptr);
    ~ManuParamPage();

private slots:
    void paramSetBtnValChanged(void*);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::ManuParamPage *ui;
};

#endif // MANUPARAMPAGE_H
