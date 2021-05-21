#include "event.h"
#include "ui_event.h"
#include "system.h"

#define HY1 610
#define HY2 670
#define HY3 600
#define HY4 660

#define VX1 50
#define VX2 150
#define VX3 250
#define VX4 680

#define VX5 700
#define VX6 780

#define SIZE 100,30
#define LABELSIZE 130,22

#define REGIST_RUN_EVENT(arg1, arg2, arg3, arg4) \
currRunEvent->tableHandle()->registMonitorItem(&arg1, arg2, arg3, arg4);

#define REGIST_ERR_EVENT(arg1, arg2, arg3, arg4) \
currErrEvent->tableHandle()->registMonitorItem(&arg1, arg2, arg3, arg4, Qt::red);

Event::Event(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Event)
{
    ui->setupUi(this);
    initTable();
    initButton();
    registRunEvent();
    registErrorEvent();
}

Event::~Event()
{
    delete ui;
}

void Event::initTable()
{
    currRunEvent = new UserCurrentEvent("RunEvent");
    currErrEvent = new UserCurrentEvent("ErrEvent");

    histoRunEvent = new UserHistoryEvent("RunEvent");
    histoErrEvent = new UserHistoryEvent("ErrEvent");
    histoErrEvent->table->setStateOnColor(Qt::red);

    ui->eventStackedWidget->insertWidget(0, currRunEvent);
    ui->eventStackedWidget->insertWidget(1, currErrEvent);
    ui->eventStackedWidget->insertWidget(2, histoRunEvent);
    ui->eventStackedWidget->insertWidget(3, histoErrEvent);
}

void Event::initButton()
{
    pCurrentEventBtn = new ubutton(this);
    pCurrentEventBtn->setText(tr("当前记录"));
    pCurrentEventBtn->setGeometry(VX4,HY4,SIZE);

    pHistoryEventBtn = new ubutton(this);
    pHistoryEventBtn->setText(tr("历史记录"));
    pHistoryEventBtn->setGeometry(VX6,HY4,SIZE);

    connect(pCurrentEventBtn,SIGNAL(clicked()),this,SLOT(currentEventBtnCliked()));
    connect(pHistoryEventBtn,SIGNAL(clicked()),this,SLOT(historyEventBtnCliked()));

    on_runButton_clicked();
}

void Event::registRunEvent()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    ModularChiller *pModularChiller = nullptr;
    ModularAir     *pModularAir = nullptr;
    Modular        *pModular = nullptr;

    ChilledBump    *pChilledBump = nullptr;
    AxialFan       *pAxialFan    = nullptr;
    ButterflyValve *pValve       = nullptr;
    WindowFan      *psWindowFan  = nullptr;

    REGIST_RUN_EVENT(pSystem->m_eSystemModeCmd, Monitor::Uint16t, "系统自动运行", 2);
    REGIST_RUN_EVENT(pSystem->m_eSystemModeCmd, Monitor::Uint16t, "系统紧急送风", 3);
    REGIST_RUN_EVENT(pSystem->m_eSystemState,   Monitor::Uint16t, "系统制冷运行", 1);
    REGIST_RUN_EVENT(pSystem->m_eSystemState,   Monitor::Uint16t, "系统制热运行", 2);
    REGIST_RUN_EVENT(pSystem->m_eSystemState,   Monitor::Uint16t, "系统送风运行", 3);
    REGIST_RUN_EVENT(pSystem->m_eSystemState,   Monitor::Uint16t, "系统负压通风运行", 4);
    REGIST_RUN_EVENT(pSystem->m_eSystemState,   Monitor::Uint16t, "系统排风运行", 5);

    for(uint8_t i = 0; i < pSystem->m_pModularAirs.count(); i++)
    {
        pModularAir = pSystem->m_pModularAirs[i];
        REGIST_RUN_EVENT(pModularAir->m_eRunningState, Monitor::Uint16t, QString::number(i+1)+"#组空供冷运行", 0);
        REGIST_RUN_EVENT(pModularAir->m_eRunningState, Monitor::Uint16t, QString::number(i+1)+"#组空通风运行", 1);
        REGIST_RUN_EVENT(pModularAir->m_eRunningState, Monitor::Uint16t, QString::number(i+1)+"#组空供热运行", 2);
        REGIST_RUN_EVENT(pModularAir->m_eRunningState, Monitor::Uint16t, QString::number(i+1)+"#组空负压通风运行", 3);

        REGIST_RUN_EVENT(pModularAir->m_eControlMode, Monitor::Uint16t, QString::number(i+1)+"#组空远程控制", 1);
        REGIST_RUN_EVENT(pModularAir->m_xRecycleMode, Monitor::Boolean, QString::number(i+1)+"#组空转轮回收", 1);
        REGIST_RUN_EVENT(pModularAir->m_xWetMode,     Monitor::Boolean, QString::number(i+1)+"#组空湿膜加湿", 1);
        REGIST_RUN_EVENT(pModularAir->m_xSupAirFan,   Monitor::Boolean, QString::number(i+1)+"#组空送风风机开启", 1);
        REGIST_RUN_EVENT(pModularAir->m_xExitAirFan,  Monitor::Boolean, QString::number(i+1)+"#组空排风风机开启", 1);
    }

    for(uint8_t i = 0; i < pSystem->m_pModularChillers.count(); i++)
    {
        pModularChiller = pSystem->m_pModularChillers[i];
        REGIST_RUN_EVENT(pModularChiller->m_eRunningModeCmd, Monitor::Uint16t, QString::number(i+1)+"#机组制冷模式", 1);
        REGIST_RUN_EVENT(pModularChiller->m_eRunningModeCmd, Monitor::Uint16t, QString::number(i+1)+"#机组制热模式", 2);
        REGIST_RUN_EVENT(pModularChiller->m_eRunningModeCmd, Monitor::Uint16t, QString::number(i+1)+"#机组手动化霜", 3);
        for(uint8_t j = 0; j < pModularChiller->m_Modulars.count(); j++)
        {
            pModular = pModularChiller->m_Modulars[j];
            REGIST_RUN_EVENT(pModular->m_eModularState, Monitor::Uint16t, QString::number(i+1)+"#机组模块"+QString::number(j+1)+"制冷运行", 1);
            REGIST_RUN_EVENT(pModular->m_eModularState, Monitor::Uint16t, QString::number(i+1)+"#机组模块"+QString::number(j+1)+"制热运行", 2);
            REGIST_RUN_EVENT(pModular->m_eModularState, Monitor::Uint16t, QString::number(i+1)+"#机组模块"+QString::number(j+1)+"手动化霜", 3);
            REGIST_RUN_EVENT(pModular->m_eModularState, Monitor::Uint16t, QString::number(i+1)+"#机组模块"+QString::number(j+1)+"冬天防冻", 4);
            REGIST_RUN_EVENT(pModular->m_eModularState, Monitor::Boolean, QString::number(i+1)+"#机组模块"+QString::number(j+1)+"水流开关打开", 1);
        }
    }
    for(uint8_t i = 0; i < pSystem->m_pChilledBumps.count(); i++)
    {
        pChilledBump = pSystem->m_pChilledBumps[i];
        REGIST_RUN_EVENT(pChilledBump->m_xRunningFlag, Monitor::Boolean, QString::number(i+1)+"#冷冻水泵运行", 1);
    }
    for(uint8_t i = 0; i < pSystem->m_pAxialFans.count(); i++)
    {
        pAxialFan = pSystem->m_pAxialFans[i];
        REGIST_RUN_EVENT(pAxialFan->m_xRunningFlag, Monitor::Boolean, QString::number(i+1)+"#畜牧风机运行", 1);
    }
    for(uint8_t i = 0; i < pSystem->m_pButterflyValves.count(); i++)
    {
        pValve = pSystem->m_pButterflyValves[i];
        REGIST_RUN_EVENT(pValve->m_xOpened, Monitor::Boolean, QString::number(i+1)+"#蝶阀开到位", 1);
        REGIST_RUN_EVENT(pValve->m_xClosed, Monitor::Boolean, QString::number(i+1)+"#蝶阀关到位", 1);
    }
    for(uint8_t i = 0; i < pSystem->m_pWindowFans.count(); i++)
    {
        psWindowFan = pSystem->m_pWindowFans[i];
        REGIST_RUN_EVENT(psWindowFan->m_xSwitchCmd, Monitor::Boolean, QString::number(i+1)+"#小窗电机开启", 1);
    }
    for(uint8_t i = 0; i < pSystem->m_pWindowFans.count(); i++)
    {
        psWindowFan = pSystem->m_pWindowFans[i];
        REGIST_RUN_EVENT(psWindowFan->m_xSwitchCmd, Monitor::Boolean, QString::number(i+1)+"#小窗电机开启", 1);
    }
}

void Event::registErrorEvent()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    ModularChiller *pModularChiller = nullptr;
    ModularAir     *pModularAir = nullptr;
    Modular        *pModular = nullptr;

    ChilledBump    *pChilledBump = nullptr;
    AxialFan       *pAxialFan    = nullptr;
    ButterflyValve *pValve       = nullptr;
    CO2Sensor      *pCO2Sensor   = nullptr;
    TempHumiSensor *pTHSensor    = nullptr;
    TempSensor     *pTempSensor  = nullptr;
    PressureSensor *pPressureSensor = nullptr;

    REGIST_ERR_EVENT(pSystem->m_Controller.m_xCommErr, Monitor::Uint8t, "触摸屏与控制器通讯故障", 1);
    REGIST_ERR_EVENT(pSystem->m_pBypassValve->m_xErrorFlag, Monitor::Boolean, "旁通阀设备故障", 1);
    REGIST_ERR_EVENT(pSystem->m_pChillerMeter->m_xCommErr, Monitor::Boolean, "机组电表通讯故障", 1);
    REGIST_ERR_EVENT(pSystem->m_pBumpMeter->m_xCommErr, Monitor::Boolean, "冷冻水泵电表通讯故障", 1);

    for(uint8_t i = 0; i < pSystem->m_pModularAirs.count(); i++)
    {
        pModularAir = pSystem->m_pModularAirs[i];
        REGIST_ERR_EVENT(pModularAir->m_xCommErr,          Monitor::Boolean, QString::number(i+1)+"#组空通讯故障", 1);
        REGIST_ERR_EVENT(pModularAir->m_xAlarmFlag,        Monitor::Boolean, QString::number(i+1)+"#组空综合故障", 1);
        REGIST_ERR_EVENT(pModularAir->m_xExitAirSenErr,    Monitor::Boolean, QString::number(i+1)+"#组空排风风速传感器故障", 1);
        REGIST_ERR_EVENT(pModularAir->m_xFreAirSenErr,     Monitor::Boolean, QString::number(i+1)+"#组空新风风速传感器故障", 1);
        REGIST_ERR_EVENT(pModularAir->m_sMeter.m_xCommErr, Monitor::Boolean, QString::number(i+1)+"#组空电表通讯故障", 1);

        REGIST_ERR_EVENT(pModularAir->m_pTempHumiOutSensor->m_xTempErr, Monitor::Boolean, QString::number(i+1)+
                     "#组空室外温度传感器故障("+QString::number(pModularAir->m_pTempHumiOutSensor->m_usDeviceIndex+1)+"#室外温度传感器)", 1)
        REGIST_ERR_EVENT(pModularAir->m_pTempHumiOutSensor->m_xHumiErr, Monitor::Boolean, QString::number(i+1)+
                     "#组空室外湿度传感器故障("+QString::number(pModularAir->m_pTempHumiOutSensor->m_usDeviceIndex+1)+"#室外湿度传感器)", 1)

        for(uint8_t j = 0; j < pModularAir->m_pCO2Sensors.count(); j++)
        {
            pCO2Sensor = pModularAir->m_pCO2Sensors[j];
            REGIST_ERR_EVENT(pCO2Sensor->m_xError, Monitor::Boolean, QString::number(i+1)+"#组空"+QString::number(j+1)+
                         "#CO2传感器故障("+QString::number(pCO2Sensor->m_usDeviceIndex+1)+"#CO2传感器)", 1);
        }
        for(uint8_t j = 0; j < pModularAir->m_pCO2Sensors.count(); j++)
        {
            pTHSensor = pModularAir->m_pTempHumiInSensors[j];
            REGIST_ERR_EVENT(pTHSensor->m_xTempErr, Monitor::Boolean, QString::number(i+1)+"#组空"+QString::number(j+1)+
                         "#室内温度传感器故障("+QString::number(pTHSensor->m_usDeviceIndex+1)+"#室内温度传感器)", 1);
            REGIST_ERR_EVENT(pTHSensor->m_xHumiErr, Monitor::Boolean, QString::number(i+1)+"#组空"+QString::number(j+1)+
                         "#室内湿度传感器故障("+QString::number(pTHSensor->m_usDeviceIndex+1)+"#室内湿度传感器)", 1);
        }
    }
    for(uint8_t i = 0; i < pSystem->m_pModularChillers.count(); i++)
    {
        pModularChiller = pSystem->m_pModularChillers[i];
        REGIST_ERR_EVENT(pModularChiller->m_xCommErr, Monitor::Boolean, QString::number(i+1)+"#机组通讯故障", 1);

        for(uint8_t j = 0; j < pModularChiller->m_Modulars.count(); j++)
        {
            pModular = pModularChiller->m_Modulars[j];
            REGIST_ERR_EVENT(pModular->m_xErrorFlag, Monitor::Boolean, QString::number(i+1)+"#机组模块"+QString::number(j+1)+"总故障", 1);
            REGIST_ERR_EVENT(pModular->m_xAlarmFlag, Monitor::Boolean, QString::number(i+1)+"#机组模块"+QString::number(j+1)+"总报警", 2);
        }
    }
    for(uint8_t i = 0; i < pSystem->m_pChilledBumps.count(); i++)
    {
        pChilledBump = pSystem->m_pChilledBumps[i];
        REGIST_ERR_EVENT(pChilledBump->m_xErrorFlag, Monitor::Boolean, QString::number(i+1)+"#冷冻水泵设备故障", 1);
        REGIST_ERR_EVENT(pChilledBump->m_xControlFlag, Monitor::Boolean, QString::number(i+1)+"#冷冻水泵控制故障", 1);
    }
    for(uint8_t i = 0; i < pSystem->m_pAxialFans.count(); i++)
    {
        pAxialFan = pSystem->m_pAxialFans[i];
        REGIST_ERR_EVENT(pAxialFan->m_xErrorFlag, Monitor::Boolean, QString::number(i+1)+"#畜牧风机设备故障", 1);
        REGIST_ERR_EVENT(pAxialFan->m_xControlFlag, Monitor::Boolean, QString::number(i+1)+"#畜牧风机控制故障", 1);
    }
    for(uint8_t i = 0; i < pSystem->m_pButterflyValves.count(); i++)
    {
        pValve = pSystem->m_pButterflyValves[i];
        REGIST_ERR_EVENT(pValve->m_xErrorFlag, Monitor::Boolean, QString::number(i+1)+"#蝶阀设备故障", 1);
    }
    for(uint8_t i = 0; i < pSystem->m_pCHWTempSensors.count(); i++)
    {
        pTempSensor = pSystem->m_pCHWTempSensors[i];
        REGIST_ERR_EVENT(pTempSensor->m_xError, Monitor::Boolean, QString::number(i+1)+ "#冷冻总管温度传感器故障", 1);
    }
    for(uint8_t i = 0; i < pSystem->m_pCHWPressureSensors.count(); i++)
    {
        pPressureSensor = pSystem->m_pCHWPressureSensors[i];
        REGIST_ERR_EVENT(pPressureSensor->m_xError, Monitor::Boolean, QString::number(i+1)+"#冷冻总管压力传感器故障", 1);
    }
}


void Event::currentEventBtnCliked()
{
    ui->eventStackedWidget->setCurrentWidget(currRunEvent);

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    pCurrentEventBtn->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    pHistoryEventBtn->setPalette(pe);

    ui->previousButton->hide();
    ui->nextButton->hide();

    if(m_usIndex == 0)
    {
        ui->eventStackedWidget->setCurrentWidget(currRunEvent);
    }
    else if (m_usIndex == 1)
    {
        ui->eventStackedWidget->setCurrentWidget(currErrEvent);
    }
    System::getInstance()->m_uiOffLogCount = 0;
}

void Event::historyEventBtnCliked()
{
    ui->eventStackedWidget->setCurrentWidget(histoRunEvent);

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    pHistoryEventBtn->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    pCurrentEventBtn->setPalette(pe);

    ui->previousButton->show();
    ui->nextButton->show();

    if(m_usIndex == 0)
    {
        ui->eventStackedWidget->setCurrentWidget(histoRunEvent);
    }
    else if (m_usIndex == 1)
    {
        ui->eventStackedWidget->setCurrentWidget(histoErrEvent);
    }
    System::getInstance()->m_uiOffLogCount = 0;
}

void Event::on_runButton_clicked()
{
    m_usIndex = 0;
    currentEventBtnCliked();

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->runButton->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    ui->errButton->setPalette(pe);
    System::getInstance()->m_uiOffLogCount = 0;
}

void Event::on_errButton_clicked()
{
    m_usIndex = 1;
    currentEventBtnCliked();

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->errButton->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    ui->runButton->setPalette(pe);
    System::getInstance()->m_uiOffLogCount = 0;
}

void Event::on_previousButton_clicked()
{
    if(ui->eventStackedWidget->currentWidget() == histoRunEvent)
    {
        histoRunEvent->table->prevDay();
    }
    else if (ui->eventStackedWidget->currentWidget() == histoErrEvent)
    {
        histoErrEvent->table->prevDay();
    }
    System::getInstance()->m_uiOffLogCount = 0;
}

void Event::on_nextButton_clicked()
{
    if(ui->eventStackedWidget->currentWidget() == histoRunEvent)
    {
        histoRunEvent->table->nextDay();
    }
    else if (ui->eventStackedWidget->currentWidget() == histoErrEvent)
    {
        histoErrEvent->table->nextDay();
    }
    System::getInstance()->m_uiOffLogCount = 0;
}
