#include "modularairpage.h"
#include "ui_modularairpage.h"
#include "system.h"

#define LABEL_COLUMNS_1  1
#define LABEL_ROWS_1     6

#define LABEL_COLUMNS_2  1
#define LABEL_ROWS_2     3

#define LABEL_SIZE       120, 30
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN_1     30
#define LABEL_LEFT_MARGIN_1   35
#define LABEL_INTERVAL_H_1    300
#define LABEL_INTERVAL_V_1    40

#define LABEL_UP_MARGIN_2     50
#define LABEL_LEFT_MARGIN_2   30
#define LABEL_INTERVAL_H_2    300
#define LABEL_INTERVAL_V_2    40

#define DATA_LABEL_SIZE  110, 30

#define DATA_LABEL_UP_MARGIN_1    30
#define DATA_LABEL_LEFT_MARGIN_1  135
#define DATA_LABEL_INTERVAL_H_1   300
#define DATA_LABEL_INTERVAL_V_1   40

#define DATA_LABEL_UP_MARGIN_2    50
#define DATA_LABEL_LEFT_MARGIN_2  150
#define DATA_LABEL_INTERVAL_H_2   300
#define DATA_LABEL_INTERVAL_V_2   40

ModularAirPage::ModularAirPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularAirPage)
{
    ui->setupUi(this);

    initDevice();
    initLabel();
    initButton();
}

ModularAirPage::~ModularAirPage()
{
    delete ui;
}

void ModularAirPage::initDevice()
{
    ModularAir* pModularAir = nullptr;
    System   *pSystem = System::getInstance();

    if(pSystem == nullptr){return;}
    for(uint8_t n = 0; n < MODULAR_AIR_NUM; n++)
    {
        pModularAir = new ModularAir();
        pSystem->m_pModularAirs.append(pModularAir);

        m_ModularAirs.append(pModularAir);
        ui->modularAirStackedWidget->addWidget(pModularAir);
    }
}

void ModularAirPage::initLabel()
{
    TextLabel *pLabel = nullptr;

    for(uint8_t n = 0; n < LABEL_ROWS_1; n++)
    {
        for(uint8_t m = 0; m < LABEL_COLUMNS_1; m++)
        {
            pLabel = new TextLabel(ui->frame);
            pLabel->setGeometry( LABEL_LEFT_MARGIN_1 + m * LABEL_INTERVAL_H_1,
                                 LABEL_UP_MARGIN_1 + n * LABEL_INTERVAL_V_1,
                                 LABEL_SIZE);
            m_Labels_1.append(pLabel);
        }
    }
    m_Labels_1[0]->setText(tr("启停命令"), LABEL_FONT_SIZE);
    m_Labels_1[1]->setText(tr("工作模式"), LABEL_FONT_SIZE);
    m_Labels_1[2]->setText(tr("目标温度"), LABEL_FONT_SIZE);
    m_Labels_1[3]->setText(tr("目标湿度"), LABEL_FONT_SIZE);
    m_Labels_1[4]->setText(tr("目标CO2"), LABEL_FONT_SIZE);

    for(uint8_t n = 0; n < LABEL_ROWS_2; n++)
    {
        for(uint8_t m = 0; m < LABEL_COLUMNS_2; m++)
        {
            pLabel = new TextLabel(ui->frame_1);
            pLabel->setGeometry( LABEL_LEFT_MARGIN_2 + m * LABEL_INTERVAL_H_2,
                                 LABEL_UP_MARGIN_2 + n * LABEL_INTERVAL_V_2,
                                 LABEL_SIZE);
            m_Labels_2.append(pLabel);
        }
    }
    m_Labels_2[0]->setText(tr("耗电功率"), LABEL_FONT_SIZE);
    m_Labels_2[1]->setText(tr("累计耗电量"), LABEL_FONT_SIZE);
    m_Labels_2[2]->setText(tr("通讯故障"), LABEL_FONT_SIZE);
}

void ModularAirPage::initButton()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}
    //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets_1.append(m_pSwitchCmdBtn);

    //机组运行工作模式设定
    m_pRunningModeCmdBtn = new ModeButton(ui->frame);
    m_pRunningModeCmdBtn->setItem(0,tr("供冷 "));
    m_pRunningModeCmdBtn->setItem(1,tr("通风"));
    m_pRunningModeCmdBtn->setItem(2,tr("供热"));
    m_pRunningModeCmdBtn->setItem(3,tr("负压通风"));
    m_pRunningModeCmdBtn->setDefaultValue(0);
    m_Widgets_1.append(m_pRunningModeCmdBtn);

    //目标温度设定
    m_pTempSetBtn = new AnalogValButton(ui->frame);
    m_pTempSetBtn->setDataParameter("℃", 1, 240, 350, 160, Monitor::Uint16t);
    m_pTempSetBtn->setMonitorData(&pSystem->m_usTempSet, Monitor::Uint16t);
    m_Widgets_1.append(m_pTempSetBtn);

    //目标湿度设定
    m_pHumiSetBtn = new AnalogValButton(ui->frame);
    m_pHumiSetBtn->setDataParameter("%", 1, 60, 100, 0, Monitor::Uint16t);
    m_pHumiSetBtn->setMonitorData(&pSystem->m_usHumiSet, Monitor::Uint16t);
    m_Widgets_1.append(m_pHumiSetBtn);

    //目标CO2设定
    m_pCO2SetBtn = new AnalogValButton(ui->frame);
    m_pCO2SetBtn->setDataParameter("ppm", 0, 2000, 3000, 1000, Monitor::Uint16t);
    m_pCO2SetBtn->setMonitorData(&pSystem->m_usCO2PPMSet, Monitor::Uint16t);
    m_Widgets_1.append(m_pCO2SetBtn);


    //故障清除
    /*m_pErrorCleanCmdBtn = new StateButton(ui->frame);
    m_pErrorCleanCmdBtn->setStateText(StateButton::State0,tr("否"));
    m_pErrorCleanCmdBtn->setStateText(StateButton::State1,tr("是"));
    m_pErrorCleanCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets_1.append(m_pErrorCleanCmdBtn);*/

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets_1.count(); i++)
    {
        m = i / LABEL_COLUMNS_1;
        n = i % LABEL_COLUMNS_1;
        m_Widgets_1[i]->setGeometry(DATA_LABEL_LEFT_MARGIN_1 + n * DATA_LABEL_INTERVAL_H_1,
                                  DATA_LABEL_UP_MARGIN_1 + m * DATA_LABEL_INTERVAL_V_1,
                                  DATA_LABEL_SIZE);
    }
    m_pCurModularAir = System::getInstance()->m_pModularAirs[0];

    //实时功率
    m_pPowerLabel = new DataLabel(ui->frame_1, DataLabel::Data);
    m_pPowerLabel->setAlignment(Qt::AlignLeft);
    m_pPowerLabel->setDataParameter("kW", 1, Monitor::Uint16t);
    m_pPowerLabel->setMonitorData(&m_pCurModularAir->m_sMeter.m_usPower, Monitor::Uint16t);
    m_Widgets_2.append(m_pPowerLabel);

    //累计耗电量
    m_pTotalEnergyLabel = new DataLabel(ui->frame_1, DataLabel::Data);
    m_pTotalEnergyLabel->setAlignment(Qt::AlignLeft);
    m_pTotalEnergyLabel->setDataParameter("kWh", 0, Monitor::Uint32t);
    m_pTotalEnergyLabel->setMonitorData(&m_pCurModularAir->m_sMeter.m_ulTotalEnergy,
                                        Monitor::Uint32t);
    m_Widgets_2.append(m_pTotalEnergyLabel);

    m_pCommErrLabel = new DataLabel(ui->frame_1, DataLabel::Text);
    m_pCommErrLabel->setAlignment(Qt::AlignLeft);
    m_pCommErrLabel->setValueMap(0,tr("正常"));
    m_pCommErrLabel->setValueMap(1,tr("故障"));
    m_pCommErrLabel->setMonitorData(&m_pCurModularAir->m_sMeter.m_xCommErr, Monitor::Boolean);
    m_Widgets_2.append(m_pCommErrLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets_2.count(); i++)
    {
        m = i / LABEL_COLUMNS_2;
        n = i % LABEL_COLUMNS_2;
        m_Widgets_2[i]->setGeometry(DATA_LABEL_LEFT_MARGIN_2 + n * DATA_LABEL_INTERVAL_H_2,
                                  DATA_LABEL_UP_MARGIN_2 + m * DATA_LABEL_INTERVAL_V_2,
                                  DATA_LABEL_SIZE);
    }
    connect(m_pSwitchCmdBtn, SIGNAL(valChanged(int32_t)), this, SLOT(paramSetBtnValChanged(int32_t)));
    connect(m_pRunningModeCmdBtn, SIGNAL(valChanged(int32_t)), this, SLOT(paramSetBtnValChanged(int32_t)));
    connect(m_pTempSetBtn, SIGNAL(valChanged(int32_t)), this, SLOT(paramSetBtnValChanged(int32_t)));
    connect(m_pHumiSetBtn, SIGNAL(valChanged(int32_t)), this, SLOT(paramSetBtnValChanged(int32_t)));
    connect(m_pCO2SetBtn, SIGNAL(valChanged(int32_t)), this, SLOT(paramSetBtnValChanged(int32_t)));

    connect(System::getInstance(), SIGNAL(sysModeCmdChanged()), this, SLOT(sysModeCmdChangedSlot()));
    sysModeCmdChangedSlot();
}

void ModularAirPage::on_pushButton_clicked()
{
    if(ui->modularAirStackedWidget->currentWidget() == m_ModularAirs[0])
    {
        m_pCurModularAir = m_ModularAirs[1];
        ui->modularAirStackedWidget->setCurrentWidget(m_ModularAirs[1]);
        ui->pushButton->setText("上一页");
    }
    else if(ui->modularAirStackedWidget->currentWidget() == m_ModularAirs[1])
    {
        m_pCurModularAir = m_ModularAirs[0];
        ui->modularAirStackedWidget->setCurrentWidget(m_ModularAirs[0]);
        ui->pushButton->setText("下一页");
    }
    m_pPowerLabel->setMonitorData(&m_pCurModularAir->m_sMeter.m_usPower, Monitor::Uint16t);
    m_pTotalEnergyLabel->setMonitorData(&m_pCurModularAir->m_sMeter.m_ulTotalEnergy, Monitor::Uint32t);
    m_pCommErrLabel->setMonitorData(&m_pCurModularAir->m_sMeter.m_xCommErr, Monitor::Boolean);

    ui->label_2->setText(QString::number(ui->modularAirStackedWidget->currentIndex()+1) + "# 组合柜能耗");
}

void ModularAirPage::paramSetBtnValChanged(int32_t val)
{
    System *pSystem = System::getInstance();
    ModularAir* pCurModularAir =nullptr;
    if(pSystem == nullptr){return;}

    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL)
    {
        pSystem->m_usTempSet = uint16_t(m_pTempSetBtn->getCurrentValue());
        pSystem->m_usHumiSet = uint16_t(m_pHumiSetBtn->getCurrentValue());
        pSystem->m_usCO2PPMSet = uint16_t(m_pCO2SetBtn->getCurrentValue());

        for(uint8_t i = 0; i < m_ModularAirs.count(); i++)
        {
             pCurModularAir = m_ModularAirs[i];
             pCurModularAir->m_eSwitchCmd = ModularAir::SwitchCmd(m_pSwitchCmdBtn->getCurrentValue());
             pCurModularAir->m_eRunningModeCmd = ModularAir::RunningMode(m_pRunningModeCmdBtn->getCurrentValue());
             pCurModularAir->m_usTempSet = pSystem->m_usTempSet;
             pCurModularAir->m_usHumiSet = pSystem->m_usHumiSet;
             pCurModularAir->m_usCO2Set = pSystem->m_usCO2PPMSet;
        }
    }
}

void ModularAirPage::sysModeCmdChangedSlot()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}
    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL)
    {
        m_pSwitchCmdBtn->setEnabled(true);
        m_pRunningModeCmdBtn->setEnabled(true);
        m_pTempSetBtn->setEnabled(true);
        m_pHumiSetBtn->setEnabled(true);
        m_pCO2SetBtn->setEnabled(true);
        for(uint8_t n = 0; n < MODULAR_AIR_NUM; n++)
        {
            m_ModularAirs[n]->m_pSwitchCmdBtn->setEnabled(true);
            m_ModularAirs[n]->m_pRunningModeCmdBtn->setEnabled(true);
            m_ModularAirs[n]->m_pTempSetBtn->setEnabled(true);
            m_ModularAirs[n]->m_pHumiSetBtn->setEnabled(true);
            m_ModularAirs[n]->m_pCO2SetBtn->setEnabled(true);
        }
    }
    else
    {
        m_pSwitchCmdBtn->setEnabled(false);
        m_pRunningModeCmdBtn->setEnabled(false);
        m_pTempSetBtn->setEnabled(false);
        m_pHumiSetBtn->setEnabled(false);
        m_pCO2SetBtn->setEnabled(false);
        for(uint8_t n = 0; n < MODULAR_AIR_NUM; n++)
        {
            m_ModularAirs[n]->m_pSwitchCmdBtn->setEnabled(false);
            m_ModularAirs[n]->m_pRunningModeCmdBtn->setEnabled(false);
            m_ModularAirs[n]->m_pTempSetBtn->setEnabled(false);
            m_ModularAirs[n]->m_pHumiSetBtn->setEnabled(false);
            m_ModularAirs[n]->m_pCO2SetBtn->setEnabled(false);
        }
    }
}
