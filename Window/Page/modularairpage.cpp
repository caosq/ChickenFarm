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
#define LABEL_LEFT_MARGIN_2   20
#define LABEL_INTERVAL_H_2    300
#define LABEL_INTERVAL_V_2    40

#define DATA_LABEL_SIZE  110, 30

#define DATA_LABEL_UP_MARGIN_1    30
#define DATA_LABEL_LEFT_MARGIN_1  135
#define DATA_LABEL_INTERVAL_H_1   300
#define DATA_LABEL_INTERVAL_V_1   40

#define DATA_LABEL_UP_MARGIN_2    55
#define DATA_LABEL_LEFT_MARGIN_2  130
#define DATA_LABEL_INTERVAL_H_2   298
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

    Button::BtnCheckData mBtnCheckData0 = {&pSystem->m_eSystemModeCmd, System::MODE_MANUAL,
                                           Monitor::Boolean, "系统正在自动运行，请先切换成手动模式"};
    Button::BtnCheckData mBtnCheckData1 = {&pSystem->m_xIsLogIn, 1, Monitor::Boolean, "请先登录后再操作"};
    //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setValueMap(StateButton::State0, 0x0055);
    m_pSwitchCmdBtn->setValueMap(StateButton::State1, 0x00AA);
    m_pSwitchCmdBtn->setCheckMode(2, &mBtnCheckData0, &mBtnCheckData1);
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets_1.append(m_pSwitchCmdBtn);

    //机组运行工作模式设定
    m_pRunningModeCmdBtn = new ModeButton(ui->frame);
    m_pRunningModeCmdBtn->setItem(0,tr("供冷 "));
    m_pRunningModeCmdBtn->setItem(1,tr("通风"));
    m_pRunningModeCmdBtn->setItem(2,tr("供热"));
    m_pRunningModeCmdBtn->setItem(3,tr("负压通风"));
    m_pRunningModeCmdBtn->setItem(4,tr("自动"));
    m_pRunningModeCmdBtn->setCheckMode(2, &mBtnCheckData0, &mBtnCheckData1);
    m_pRunningModeCmdBtn->setDefaultValue(0);
    m_Widgets_1.append(m_pRunningModeCmdBtn);

    //目标温度设定
    m_pTempSetBtn = new AnalogValButton(ui->frame);
    m_pTempSetBtn->setDataParameter("℃", 1, 240, 350, 160, Monitor::Uint16t);
    m_pTempSetBtn->setMonitorData(&pSystem->m_usTempSet, Monitor::Uint16t);
    m_pTempSetBtn->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_Widgets_1.append(m_pTempSetBtn);

    //目标湿度设定
    m_pHumiSetBtn = new AnalogValButton(ui->frame);
    m_pHumiSetBtn->setDataParameter("%", 1, 500, 1000, 0, Monitor::Uint16t);
    m_pHumiSetBtn->setMonitorData(&pSystem->m_usHumiSet, Monitor::Uint16t);
    m_pHumiSetBtn->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_Widgets_1.append(m_pHumiSetBtn);

    //目标CO2设定
    m_pCO2SetBtn = new AnalogValButton(ui->frame);
    m_pCO2SetBtn->setDataParameter("ppm", 1, 20000, 30000, 10000, Monitor::Uint16t);
    m_pCO2SetBtn->setMonitorData(&pSystem->m_usCO2PPMSet, Monitor::Uint16t);
    m_pCO2SetBtn->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
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
    m_pCommErrLabel->setValueMap(1,tr("故障"), Qt::red);
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
    connect(m_pSwitchCmdBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));

    connect(m_pRunningModeCmdBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));

    connect(m_pTempSetBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));

    connect(m_pHumiSetBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));

    connect(m_pCO2SetBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));

    connect(System::getInstance(), SIGNAL(systemDataChanged()), this, SLOT(systemDataChangedSlot()));
    systemDataChangedSlot();
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
    System::getInstance()->m_uiOffLogCount = 0;
}

void ModularAirPage::paramSetBtnValChanged(void* pBtn)
{
    System *pSystem = System::getInstance();
    ModularAir* pCurModularAir = nullptr;
    if(pSystem == nullptr){return;}

    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL ||
       pSystem->m_eSystemModeCmd == System::SystemMode::MODE_EMERGENCY || pSystem->m_xIsInDebug == true)
    {
        for(uint8_t i = 0; i < m_ModularAirs.count(); i++)
        {
             pCurModularAir = m_ModularAirs[i];
             if(pCurModularAir->m_xCommErr && pSystem->m_xIsInDebug == false){continue;}
             if(pBtn == m_pSwitchCmdBtn)
             {
                 pCurModularAir->m_pSwitchCmdBtn->setValue( ModularAir::SwitchCmd(m_pSwitchCmdBtn->getCurrentValue()) );
             }
             if(pBtn == m_pRunningModeCmdBtn)
             {
                 pCurModularAir->m_pRunningModeCmdBtn->setValue( ModularAir::RunningMode(m_pRunningModeCmdBtn->getCurrentValue()) );
             }
             if(pBtn == m_pTempSetBtn)
             {
                 pCurModularAir->m_pTempSetBtn->setValue(pSystem->m_usTempSet);
             }
             if(pBtn == m_pHumiSetBtn)
             {
                 pCurModularAir->m_pHumiSetBtn->setValue(pSystem->m_usHumiSet);
             }
             if(pBtn == m_pCO2SetBtn)
             {
                 pCurModularAir->m_pCO2SetBtn->setValue(pSystem->m_usCO2PPMSet);
             }
        }
        qDebug("ModularAirPage::paramSetBtnValChanged %d %d %d", pSystem->m_usTempSet, pSystem->m_usHumiSet, pSystem->m_usCO2PPMSet);
    }
    System::getInstance()->m_uiOffLogCount = 0;
}

void ModularAirPage::systemDataChangedSlot()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_EMERGENCY)
    {
        for(uint8_t n = 0; n < MODULAR_AIR_NUM; n++)
        {
            m_ModularAirs[n]->m_eRunningModeCmd = ModularAir::RUN_MODE_FAN;
        }
    }
}
