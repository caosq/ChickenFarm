#include "modularchillerpage.h"
#include "ui_modularchillerpage.h"
#include "system.h"
#include "stdio.h"

#define LABEL_COLUMNS_1  1
#define LABEL_ROWS_1     6

#define LABEL_COLUMNS_2  1
#define LABEL_ROWS_2     3

#define LABEL_SIZE       120, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN_1     30
#define LABEL_LEFT_MARGIN_1   25
#define LABEL_INTERVAL_H_1    300
#define LABEL_INTERVAL_V_1    40

#define LABEL_UP_MARGIN_2     60
#define LABEL_LEFT_MARGIN_2   20
#define LABEL_INTERVAL_H_2    300
#define LABEL_INTERVAL_V_2    40

#define DATA_LABEL_SIZE  110, 28

#define DATA_LABEL_UP_MARGIN_1    30
#define DATA_LABEL_LEFT_MARGIN_1  155
#define DATA_LABEL_INTERVAL_H_1   300
#define DATA_LABEL_INTERVAL_V_1   40

#define DATA_LABEL_UP_MARGIN_2    65
#define DATA_LABEL_LEFT_MARGIN_2  130
#define DATA_LABEL_INTERVAL_H_2   300
#define DATA_LABEL_INTERVAL_V_2   38

ModularChillerPage::ModularChillerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularChillerPage)
{
    ui->setupUi(this);

    initDevice();
    initLabel();
    initButton();
}

ModularChillerPage::~ModularChillerPage()
{
    delete ui;
}

void ModularChillerPage::initDevice()
{
    System   *pSystem = System::getInstance();
    ModularChiller* pModularChiller = nullptr;

    if(pSystem == nullptr){return;}
    for(uint8_t n = 0; n < MODULAR_CHILLER_NUM; n++)
    {
        pModularChiller = new ModularChiller();

        //pSystem->m_pModularChillers.append(&m_ModularChillers[n]);

        pSystem->m_pModularChillers.append(pModularChiller);
        m_ModularChillers.append(pModularChiller);

        //ui->modularChillerStackedWidget->addWidget(&m_ModularChillers[n]);
        ui->modularChillerStackedWidget->addWidget(pModularChiller);
    }
    pSystem->m_pChillerMeter = &m_sChillerMeter;  //机组电表

    //m_ModularChillers[0].setParent(ui->modularChillerStackedWidget);
    //m_ModularChillers[1].setParent(ui->modularChillerStackedWidget);
}

void ModularChillerPage::initLabel()
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
    m_Labels_1[2]->setText(tr("制冷进水温度"), LABEL_FONT_SIZE);
    m_Labels_1[3]->setText(tr("制冷出水温度"), LABEL_FONT_SIZE);
    m_Labels_1[4]->setText(tr("制热进水温度"), LABEL_FONT_SIZE);
    m_Labels_1[5]->setText(tr("制热出水温度"), LABEL_FONT_SIZE);

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

void ModularChillerPage::initButton()
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
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_pSwitchCmdBtn->setCheckMode(2, &mBtnCheckData0, &mBtnCheckData1);
    m_Widgets_1.append(m_pSwitchCmdBtn);

    //机组运行工作模式设定
    m_pRunningModeCmdBtn = new ModeButton(ui->frame);
    m_pRunningModeCmdBtn->setItem(1,tr("制冷"));
    m_pRunningModeCmdBtn->setItem(2,tr("制热"));
    m_pRunningModeCmdBtn->setItem(3,tr("手动化霜"));
    m_pRunningModeCmdBtn->setDefaultValue(1);
    m_pRunningModeCmdBtn->setCheckMode(2, &mBtnCheckData0, &mBtnCheckData1);
    m_Widgets_1.append(m_pRunningModeCmdBtn);

    //机组制冷进水温度设定值
    m_pChillerCoolInTemp = new AnalogValButton(ui->frame);
    m_pChillerCoolInTemp->setDataParameter("℃", 1, 120, 250, 100, Monitor::Uint16t);
    m_pChillerCoolInTemp->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_pChillerCoolInTemp->setMonitorData(&pSystem->m_usChillerCoolInTemp, Monitor::Uint16t);
    m_Widgets_1.append(m_pChillerCoolInTemp);

    //机组制冷出水温度设定值
    m_pChillerCoolOutTemp = new AnalogValButton(ui->frame);
    m_pChillerCoolOutTemp->setDataParameter("℃", 1, 70, 200, 50, Monitor::Uint16t);
    m_pChillerCoolOutTemp->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_pChillerCoolOutTemp->setMonitorData(&pSystem->m_usChillerCoolOutTemp, Monitor::Uint16t);
    m_Widgets_1.append(m_pChillerCoolOutTemp);

    //机组制热进水温度设定值
    m_pChillerHeatInTemp = new AnalogValButton(ui->frame);
    m_pChillerHeatInTemp->setDataParameter("℃", 1, 400, 450, 300, Monitor::Uint16t);
    m_pChillerHeatInTemp->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_pChillerHeatInTemp->setMonitorData(&pSystem->m_usChillerHeatInTemp, Monitor::Uint16t);
    m_Widgets_1.append(m_pChillerHeatInTemp);

    //机组制热出水温度设定值
    m_pChillerHeatOutTemp = new AnalogValButton(ui->frame);
    m_pChillerHeatOutTemp->setDataParameter("℃", 1, 450, 500, 350, Monitor::Uint16t);
    m_pChillerHeatOutTemp->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_pChillerHeatOutTemp->setMonitorData(&pSystem->m_usChillerHeatOutTemp, Monitor::Uint16t);
    m_Widgets_1.append(m_pChillerHeatOutTemp);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets_1.count(); i++)
    {
        m = i / LABEL_COLUMNS_1;
        n = i % LABEL_COLUMNS_1;
        m_Widgets_1[i]->setGeometry(DATA_LABEL_LEFT_MARGIN_1 + n * DATA_LABEL_INTERVAL_H_1,
                                  DATA_LABEL_UP_MARGIN_1 + m * DATA_LABEL_INTERVAL_V_1,
                                  DATA_LABEL_SIZE);
    }
    //实时功率
    m_pPowerLabel = new DataLabel(ui->frame_1, DataLabel::Data);
    m_pPowerLabel->setAlignment(Qt::AlignLeft);
    m_pPowerLabel->setDataParameter("kW", 1, Monitor::Uint16t);
    m_pPowerLabel->setMonitorData(&m_sChillerMeter.m_usPower, Monitor::Uint16t);
    m_Widgets_2.append(m_pPowerLabel);

    //qDebug("m_pPowerLabel %d %s", m_sMeter.m_usPower, m_pPowerLabel->text().toUtf8().data());

    //累计耗电量
    m_pTotalEnergyLabel = new DataLabel(ui->frame_1, DataLabel::Data);
    m_pTotalEnergyLabel->setAlignment(Qt::AlignLeft);
    m_pTotalEnergyLabel->setDataParameter("kWh", 0, Monitor::Uint32t);
    m_pTotalEnergyLabel->setMonitorData(&m_sChillerMeter.m_ulTotalEnergy, Monitor::Uint32t);
    m_Widgets_2.append(m_pTotalEnergyLabel);

    m_pCommErrLabel = new DataLabel(ui->frame_1, DataLabel::Text);
    m_pCommErrLabel->setAlignment(Qt::AlignLeft);
    m_pCommErrLabel->setValueMap(0,tr("正常"));
    m_pCommErrLabel->setValueMap(1,tr("故障"), Qt::red);
    m_pCommErrLabel->setMonitorData(&m_sChillerMeter.m_xCommErr, Monitor::Boolean);
    m_Widgets_2.append(m_pCommErrLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets_2.count(); i++)
    {
        m = i / LABEL_COLUMNS_2;
        n = i % LABEL_COLUMNS_2;
        m_Widgets_2[i]->setGeometry(DATA_LABEL_LEFT_MARGIN_2 + n * DATA_LABEL_INTERVAL_H_2,
                                  DATA_LABEL_UP_MARGIN_2 + m * DATA_LABEL_INTERVAL_V_2,
                                  200, 28);
    }
    connect(m_pSwitchCmdBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
    connect(m_pRunningModeCmdBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
    connect(m_pChillerCoolInTemp, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
    connect(m_pChillerCoolOutTemp, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
    connect(m_pChillerHeatInTemp, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
    connect(m_pChillerHeatOutTemp, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
}

void ModularChillerPage::on_pushButton_clicked()
{
    if(ui->modularChillerStackedWidget->currentWidget() == m_ModularChillers[0])
    {
        ui->modularChillerStackedWidget->setCurrentWidget(m_ModularChillers[1]);
        ui->pushButton->setText("上一页");

    }
    else if(ui->modularChillerStackedWidget->currentWidget() == m_ModularChillers[1])
    {
        ui->modularChillerStackedWidget->setCurrentWidget(m_ModularChillers[0]);
        ui->pushButton->setText("下一页");
    }
    System::getInstance()->m_uiOffLogCount = 0;
   // qDebug("currentIndex %d", ui->modularChillerStackedWidget->currentIndex());
}

void ModularChillerPage::paramSetBtnValChanged(void* pBtn)
{
    System *pSystem = System::getInstance();
    ModularChiller* pModularChiller = nullptr;
    if(pSystem == nullptr){return;}

    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL || pSystem->m_xIsInDebug == true)
    {
        for(uint8_t i = 0; i < m_ModularChillers.count(); i++)
        {
             pModularChiller = m_ModularChillers[i];
             if(pModularChiller->m_xCommErr && pSystem->m_xIsInDebug == false){continue;}
             if(pBtn == m_pSwitchCmdBtn)
             {
                 pModularChiller->m_pSwitchCmdBtn->setValue( ModularChiller::SwitchCmd(m_pSwitchCmdBtn->getCurrentValue()) );
             }
             if(pBtn == m_pRunningModeCmdBtn)
             {
                 pModularChiller->m_pRunningModeCmdBtn->setValue( ModularChiller::RunningMode(m_pRunningModeCmdBtn->getCurrentValue()) );
             }
             if(pBtn == m_pChillerCoolInTemp)
             {
                 pModularChiller->m_pChillerCoolInTempBtn->setValue( uint16_t(m_pChillerCoolInTemp->getCurrentValue()) );
             }
             if(pBtn == m_pChillerCoolOutTemp)
             {
                 pModularChiller->m_pChillerCoolOutTempBtn->setValue( uint16_t(m_pChillerCoolOutTemp->getCurrentValue()) );
             }
             if(pBtn == m_pChillerHeatInTemp)
             {
                 pModularChiller->m_pChillerHeatInTempBtn->setValue( uint16_t(m_pChillerHeatInTemp->getCurrentValue()) );
             }
             if(pBtn == m_pChillerHeatOutTemp)
             {
                 pModularChiller->m_pChillerHeatOutTempBtn->setValue( uint16_t(m_pChillerHeatOutTemp->getCurrentValue()) );
             }
        }
    }
    System::getInstance()->m_uiOffLogCount = 0;
}
