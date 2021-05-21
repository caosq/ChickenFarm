#ifndef USERPARAMPAGE_H
#define USERPARAMPAGE_H

#include <QWidget>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"
#include "analogvalbutton.h"
#include "statebutton.h"
#include "ubutton.h"

namespace Ui {
class UserParamPage;
}

class UserParamPage : public QWidget
{
    Q_OBJECT

private:
    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;


    AnalogValButton  *m_pCO2PPMSet;       //目标CO2浓度设定
    AnalogValButton  *m_pHumiSet_1;       //-2天到28天目标湿度
    AnalogValButton  *m_pHumiSet_2;       //29天到462天目标湿度
    AnalogValButton  *m_pEnergyTemp;      //节能温度
    AnalogValButton  *m_pSupAirMaxTemp;   //送风最大温度
    AnalogValButton  *m_pTempDeviat;      //温度偏差
    AnalogValButton  *m_pCO2PPMAlarm;     //CO2浓度报警值


    StateButton      *m_pAlarmEnable;     //声光报警使能
    StateButton      *m_pDenugEnable;     //调试模式
    ubutton          *m_pSetPassword;     //密码修改

    AnalogValButton  *m_pSysYear;         //系统时间年
    AnalogValButton  *m_pSysMon;          //系统时间月
    AnalogValButton  *m_pSysDay;          //系统时间日
    AnalogValButton  *m_pSysHour;         //系统时间时
    AnalogValButton  *m_pSysMin;          //系统时间分
    AnalogValButton  *m_pSysSec;          //系统时间秒

    AnalogValButton *m_pChillerCoolInTemp;      //机组制冷进水温度设定值
    AnalogValButton *m_pChillerCoolOutTemp;     //机组制冷出水温度设定值
    AnalogValButton *m_pChillerHeatInTemp;      //机组制热进水温度设定值
    AnalogValButton *m_pChillerHeatOutTemp;     //机组制热出水温度设定值

private:
    void initLabel();
    void initButton();

public:
    explicit UserParamPage(QWidget *parent = nullptr);
    ~UserParamPage();

private slots:
    void systemTimeChangedSlot();
    void debugBtnClicked();
    void paramSetBtnValChanged();

private:
    Ui::UserParamPage *ui;
};

#endif // USERPARAMPAGE_H
