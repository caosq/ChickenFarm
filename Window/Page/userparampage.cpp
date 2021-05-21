#include "userparampage.h"
#include "ui_userparampage.h"
#include "system.h"
#include "password.h"
#include "pwdkeyboard.h"
#include "messagebox.h"
#include "passwdedit.h"
#include <QDebug>

#define LABEL_COLUMNS  2
#define LABEL_ROWS     16

#define LABEL_SIZE       200, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    450
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  130, 25

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  220
#define DATA_LABEL_INTERVAL_H   420
#define DATA_LABEL_INTERVAL_V   35

UserParamPage::UserParamPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserParamPage)
{
    ui->setupUi(this);

    initLabel();
    initButton();
}

UserParamPage::~UserParamPage()
{
    delete ui;
}

void UserParamPage::initLabel()
{
    TextLabel *pLabel = nullptr;

    for(uint8_t m = 0; m < LABEL_COLUMNS; m++)
    {
        for(uint8_t n = 0; n < LABEL_ROWS; n++)
        {
            pLabel = new TextLabel(ui->frame);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels.append(pLabel);
        }
    }
    m_Labels[0]->setText(tr("CO2目标浓度"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("-2天到28天湿度"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("29天到462天湿度"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("机组制冷进水温度"), LABEL_FONT_SIZE);
    m_Labels[4]->setText(tr("机组制冷出水温度"), LABEL_FONT_SIZE);
    m_Labels[5]->setText(tr("机组制热进水温度"), LABEL_FONT_SIZE);
    m_Labels[6]->setText(tr("机组制热出水温度"), LABEL_FONT_SIZE);
    m_Labels[7]->setText(tr("节能温度"), LABEL_FONT_SIZE);
    m_Labels[8]->setText(tr("送风最大温度"), LABEL_FONT_SIZE);
    m_Labels[9]->setText(tr("温度偏差"), LABEL_FONT_SIZE);

    m_Labels[10]->setText(tr("CO2报警浓度"), LABEL_FONT_SIZE);
    m_Labels[11]->setText(tr("系统时间年"), LABEL_FONT_SIZE);
    m_Labels[12]->setText(tr("系统时间月"), LABEL_FONT_SIZE);
    m_Labels[13]->setText(tr("系统时间日"), LABEL_FONT_SIZE);
    m_Labels[14]->setText(tr("系统时间时"), LABEL_FONT_SIZE);
    m_Labels[15]->setText(tr("系统时间分"), LABEL_FONT_SIZE);
    //m_Labels[16]->setText(tr("系统时间秒"), LABEL_FONT_SIZE);
    m_Labels[16]->setText(tr("声光报警使能"), LABEL_FONT_SIZE);
    m_Labels[17]->setText(tr("调试模式"), LABEL_FONT_SIZE);
    m_Labels[18]->setText(tr("密码修改"), LABEL_FONT_SIZE);
}

void UserParamPage::initButton()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    //目标CO2浓度设定
    m_pCO2PPMSet = new AnalogValButton(ui->frame);
    m_pCO2PPMSet->setDataParameter("ppm", 1, 20000, 35000, 0, Monitor::Uint16t);
    m_pCO2PPMSet->setMonitorData(&pSystem->m_usCO2PPMSet, Monitor::Uint16t);
    m_Widgets.append(m_pCO2PPMSet);

    //-2天到28天目标湿度
    m_pHumiSet_1 = new AnalogValButton(ui->frame);
    m_pHumiSet_1->setDataParameter("%", 1, 650, 1000, 0, Monitor::Uint16t);
    m_pHumiSet_1->setMonitorData(&pSystem->m_usHumiSet_1, Monitor::Uint16t, true);
    m_Widgets.append(m_pHumiSet_1);
    //29天到462天目标湿度
    m_pHumiSet_2 = new AnalogValButton(ui->frame);
    m_pHumiSet_2->setDataParameter("%", 1, 550, 1000, 0, Monitor::Uint16t);
    m_pHumiSet_2->setMonitorData(&pSystem->m_usHumiSet_2, Monitor::Uint16t, true);
    m_Widgets.append(m_pHumiSet_2);

    //机组制冷进水温度设定值
    m_pChillerCoolInTemp = new AnalogValButton(ui->frame);
    m_pChillerCoolInTemp->setDataParameter("℃", 1, 120, 250, 100, Monitor::Uint16t);
    m_pChillerCoolInTemp->setMonitorData(&pSystem->m_usChillerCoolInTemp, Monitor::Uint16t);
    m_Widgets.append(m_pChillerCoolInTemp);

    //机组制冷出水温度设定值
    m_pChillerCoolOutTemp = new AnalogValButton(ui->frame);
    m_pChillerCoolOutTemp->setDataParameter("℃", 1, 70, 200, 50, Monitor::Uint16t);
    m_pChillerCoolOutTemp->setMonitorData(&pSystem->m_usChillerCoolOutTemp, Monitor::Uint16t);
    m_Widgets.append(m_pChillerCoolOutTemp);

    //机组制热进水温度设定值
    m_pChillerHeatInTemp = new AnalogValButton(ui->frame);
    m_pChillerHeatInTemp->setDataParameter("℃", 1, 400, 450, 300, Monitor::Uint16t);
    m_pChillerHeatInTemp->setMonitorData(&pSystem->m_usChillerHeatInTemp, Monitor::Uint16t);
    m_Widgets.append(m_pChillerHeatInTemp);

    //机组制热出水温度设定值
    m_pChillerHeatOutTemp = new AnalogValButton(ui->frame);
    m_pChillerHeatOutTemp->setDataParameter("℃", 1, 450, 500, 350, Monitor::Uint16t);
    m_pChillerHeatOutTemp->setMonitorData(&pSystem->m_usChillerHeatOutTemp, Monitor::Uint16t);
    m_Widgets.append(m_pChillerHeatOutTemp);

    //节能温度
    m_pEnergyTemp = new AnalogValButton(ui->frame);
    m_pEnergyTemp->setDataParameter("℃", 1, 250, 450, 0, Monitor::Uint16t);
    m_pEnergyTemp->setMonitorData(&pSystem->m_usEnergyTemp, Monitor::Uint16t);
    m_Widgets.append(m_pEnergyTemp);

    //送风最大温度
    m_pSupAirMaxTemp = new AnalogValButton(ui->frame);
    m_pSupAirMaxTemp->setDataParameter("℃", 1, 450, 550, 0, Monitor::Uint16t);
    m_pSupAirMaxTemp->setMonitorData(&pSystem->m_usSupAirMaxTemp, Monitor::Uint16t);
    m_Widgets.append(m_pSupAirMaxTemp);

    //温度偏差
    m_pTempDeviat = new AnalogValButton(ui->frame);
    m_pTempDeviat->setDataParameter("℃", 1, 5, 100, 0, Monitor::Uint16t);
    m_pTempDeviat->setMonitorData(&pSystem->m_usTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pTempDeviat);

    //CO2浓度报警值
    m_pCO2PPMAlarm = new AnalogValButton(ui->frame);
    m_pCO2PPMAlarm->setDataParameter("ppm", 1, 30000, 35000, 0, Monitor::Uint16t);
    m_pCO2PPMAlarm->setMonitorData(&pSystem->m_usCO2PPMAlarm, Monitor::Uint16t);
    m_Widgets.append(m_pCO2PPMAlarm);

    //系统时间年
    m_pSysYear = new AnalogValButton(ui->frame);
    m_pSysYear->setDataParameter("年", 0, 2021, 9999, 2020, Monitor::Uint16t);
    m_pSysYear->setMonitorData(&pSystem->m_usSysYear, Monitor::Uint16t);
    m_Widgets.append(m_pSysYear);
    //系统时间月
    m_pSysMon = new AnalogValButton(ui->frame);
    m_pSysMon->setDataParameter("月", 0, 1, 12, 1, Monitor::Uint16t);
    m_pSysMon->setMonitorData(&pSystem->m_usSysMon, Monitor::Uint16t);
    m_Widgets.append(m_pSysMon);
    //系统时间日
    m_pSysDay = new AnalogValButton(ui->frame);
    m_pSysDay->setDataParameter("日", 0, 1, 31, 1, Monitor::Uint16t);
    m_pSysDay->setMonitorData(&pSystem->m_usSysDay, Monitor::Uint16t);
    m_Widgets.append(m_pSysDay);
    //系统时间时
    m_pSysHour = new AnalogValButton(ui->frame);
    m_pSysHour->setDataParameter("时", 0, 0, 23, 0, Monitor::Uint16t);
    m_pSysHour->setMonitorData(&pSystem->m_usSysHour, Monitor::Uint16t);
    m_Widgets.append(m_pSysHour);
    //系统时间分
    m_pSysMin = new AnalogValButton(ui->frame);
    m_pSysMin->setDataParameter("分", 0, 0, 59, 0, Monitor::Uint16t);
    m_pSysMin->setMonitorData(&pSystem->m_usSysMin, Monitor::Uint16t);
    m_Widgets.append(m_pSysMin);

    //系统时间秒
    /*m_pSysSec = new AnalogValButton(ui->frame);
    m_pSysSec->setDataParameter("秒", 0, 0, 59, 0, Monitor::Uint16t);
    //m_pSysSec->setMonitorData(&pSystem->m_usSysSec, Monitor::Uint16t);
    m_Widgets.append(m_pSysSec);*/

    //声光报警使能
    m_pAlarmEnable = new StateButton(ui->frame);
    m_pAlarmEnable->setStateText(StateButton::State0,tr("关闭"));
    m_pAlarmEnable->setStateText(StateButton::State1,tr("开启"));
    m_pAlarmEnable->setDeafultState(StateButton::State0);
    m_pAlarmEnable->setMonitorData(&pSystem->m_xAlarmEnable, Monitor::Boolean);
    m_Widgets.append(m_pAlarmEnable);

    //调试模式
    m_pDenugEnable = new StateButton(ui->frame);
    m_pDenugEnable->setStateText(StateButton::State0,tr("关闭"));
    m_pDenugEnable->setStateText(StateButton::State1,tr("开启"));
    m_pDenugEnable->setDeafultState(StateButton::State0);
    //m_pDenugEnable->setCheckMode(&pSystem->m_xIsInDebug, 1, "请输入厂家密码", Monitor::Boolean);
    m_pDenugEnable->setMonitorData(&pSystem->m_xIsInDebug, Monitor::Boolean);
    m_Widgets.append(m_pDenugEnable);

    //密码设置
    m_pSetPassword = new ubutton(ui->frame);
    m_pSetPassword->setText("进入");
    m_Widgets.append(m_pSetPassword);

    for (uint8_t i = 0, m = 0, n = 0, p=0; i < m_Widgets.count(); i++)
    {
        p = i % (LABEL_COLUMNS * LABEL_ROWS);
        m = p / LABEL_ROWS;
        n = p % LABEL_ROWS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + m * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + n * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    connect(m_pSysYear, SIGNAL(clicked()), this, SLOT(paramSetBtnValChanged()));
    connect(m_pSysMon, SIGNAL(clicked()), this, SLOT(paramSetBtnValChanged()));
    connect(m_pSysDay, SIGNAL(clicked()), this, SLOT(paramSetBtnValChanged()));
    connect(m_pSysHour, SIGNAL(clicked()), this, SLOT(paramSetBtnValChanged()));
    connect(m_pSysMin, SIGNAL(clicked()), this, SLOT(paramSetBtnValChanged()));
    //connect(m_pSysSec, SIGNAL(clicked()), this, SLOT(paramSetBtnValChanged()));
    connect(m_pSetPassword, SIGNAL(clicked()), passwdEdit::instance(), SLOT(show()));
    connect(m_pDenugEnable, SIGNAL(pressed()), this, SLOT(debugBtnClicked()));

    connect(System::getInstance(), SIGNAL(systemTimeChanged()), this, SLOT(systemTimeChangedSlot()));
}

void UserParamPage::systemTimeChangedSlot()
{
    System *pSystem = System::getInstance();

    if(pSystem == nullptr){return;}

    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_AUTO)
    {
        pSystem->m_usCO2PPMSet = uint16_t(m_pCO2PPMSet->getCurrentValue());
        if(pSystem->m_sChickenGrowDay < 29)
        {
            pSystem->m_usHumiSet = uint16_t(m_pHumiSet_1->getCurrentValue());
        }
        else
        {
            pSystem->m_usHumiSet = uint16_t(m_pHumiSet_2->getCurrentValue());
        }
        pSystem->m_usChillerCoolInTemp = uint16_t(m_pChillerCoolInTemp->getCurrentValue());
        pSystem->m_usChillerCoolOutTemp = uint16_t(m_pChillerCoolOutTemp->getCurrentValue());
        pSystem->m_usChillerHeatInTemp = uint16_t(m_pChillerHeatInTemp->getCurrentValue());
        pSystem->m_usChillerHeatOutTemp = uint16_t(m_pChillerHeatOutTemp->getCurrentValue());
        //qDebug("UserParamPage::systemTimeChangedSlot \n");
    }
}

void UserParamPage::paramSetBtnValChanged()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    QString cmd;

    QString year = QString("%1").arg(QString::number(pSystem->m_usSysYear).toInt(), 4, 10, QLatin1Char('0'));
    QString month = QString("%1").arg(QString::number(pSystem->m_usSysMon).toInt(), 2, 10, QLatin1Char('0'));
    QString day = QString("%1").arg(QString::number(pSystem->m_usSysDay).toInt(), 2, 10, QLatin1Char('0'));

    cmd = "date -s " + year + "/" + month + "/" + day;
    system(cmd.toLatin1().data());

    QString hour = QString("%1").arg(QString::number(pSystem->m_usSysHour).toInt(), 2, 10, QLatin1Char('0'));
    QString min = QString("%1").arg(QString::number(pSystem->m_usSysMin).toInt(), 2, 10, QLatin1Char('0'));
    QString sec = QString("%1").arg(QString::number(pSystem->m_usSysSec).toInt(), 2, 10, QLatin1Char('0'));

    cmd = "date -s " + hour + ":" + min + ":" + sec;
    system(cmd.toLatin1().data());
    system("hwclock -w");

    qDebug("timeSetBtnRelased %s\n", cmd.toLocal8Bit().data());
}

void UserParamPage::debugBtnClicked()
{
    int passwd = pwdKeyBoard::getWrongValue();
    bool exitState = false;
    bool passwdState = false;

    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    if(pSystem->m_xIsInDebug)
    {
        Button::setDebugMode(false);
        //pSystem->m_xIsInDebug = false;
        return;
    }
    while( !passwdState && !exitState )//获取成功且密码正确就退出
    {
        passwd = pwdKeyBoard::getValue(passwd,&exitState);
        if(exitState){return;}
        if(password::instance()->judgementPwd(password::GR_FACTORY,passwd))
        {
            pSystem->m_xIsInDebug = true;
            passwdState = true;
            Button::setDebugMode(true);
        }
        else
        {
            passwdState = false;
            pwdKeyBoard::instance()->show();
            messageBox::instance()->setInformativeText("请输入正确的厂家密码");
            messageBox::instance()->show();
        }
    }
}
