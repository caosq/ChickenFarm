#include "bumppage.h"
#include "ui_bumppage.h"
#include "system.h"

#define LABEL_COLUMNS_1  1
#define LABEL_ROWS_1     6

#define LABEL_COLUMNS_2  1
#define LABEL_ROWS_2     3

#define LABEL_SIZE       120, 30
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN_1     30
#define LABEL_LEFT_MARGIN_1   30
#define LABEL_INTERVAL_H_1    300
#define LABEL_INTERVAL_V_1    40

#define LABEL_UP_MARGIN_2     60
#define LABEL_LEFT_MARGIN_2   20
#define LABEL_INTERVAL_H_2    300
#define LABEL_INTERVAL_V_2    40

#define DATA_LABEL_SIZE  110, 30

#define DATA_LABEL_UP_MARGIN_1    30
#define DATA_LABEL_LEFT_MARGIN_1  135
#define DATA_LABEL_INTERVAL_H_1   300
#define DATA_LABEL_INTERVAL_V_1   40

#define DATA_LABEL_UP_MARGIN_2    62
#define DATA_LABEL_LEFT_MARGIN_2  125
#define DATA_LABEL_INTERVAL_H_2   298
#define DATA_LABEL_INTERVAL_V_2   40

BumpPage::BumpPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BumpPage)
{
    ui->setupUi(this);

    initDevice();
    initLabel();
    initButton();
}

BumpPage::~BumpPage()
{
    delete ui;
}

void BumpPage::initDevice()
{
    ChilledBump* pChilledBump = nullptr;
    System   *pSystem = System::getInstance();

    if(pSystem == nullptr){return;}
    for(uint8_t n = 0, x = 0, y = 0; n < CHILLED_BUMP_NUM; n++)
    {
        x = n / 2;
        y = n % 2;

        pChilledBump = new ChilledBump();
        pSystem->m_pChilledBumps.append(pChilledBump);
        m_ChilledBumps.append(pChilledBump);
        ui->gridLayout->addWidget(pChilledBump, x, y );
    }
    pSystem->m_pBumpMeter = &m_sBumpMeter;  //水泵电表
}

void BumpPage::initLabel()
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
    m_Labels_1[1]->setText(tr("频率设置"), LABEL_FONT_SIZE);
    m_Labels_1[2]->setText(tr("故障清除"), LABEL_FONT_SIZE);


    //ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 冷冻水泵");

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

    // ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 组合柜");
}

void BumpPage::initButton()
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
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_pSwitchCmdBtn->setCheckMode(2, &mBtnCheckData0, &mBtnCheckData1);
    m_Widgets_1.append(m_pSwitchCmdBtn);

    //频率设置
    m_pFreqSetBtn = new AnalogValButton(ui->frame);
    m_pFreqSetBtn->setDataParameter("Hz", 1, 0, 500, 0, Monitor::Uint16t);
    m_pFreqSetBtn->setMaxValMonitor(&pSystem->m_usCHWBumpMaxFreq, Monitor::Uint16t);
    m_pFreqSetBtn->setMinValMonitor(&pSystem->m_usCHWBumpMinFreq, Monitor::Uint16t);
    m_pFreqSetBtn->setCheckMode(2, &mBtnCheckData0, &mBtnCheckData1);
    m_Widgets_1.append(m_pFreqSetBtn);

    //故障清除
    m_pErrorCleanCmdBtn = new StateButton(ui->frame);
    m_pErrorCleanCmdBtn->setStateText(StateButton::State0,tr("否"));
    m_pErrorCleanCmdBtn->setStateText(StateButton::State1,tr("是"));
    m_pErrorCleanCmdBtn->setDelayMode(20000, 0);
    m_pErrorCleanCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets_1.append(m_pErrorCleanCmdBtn);

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
    m_pPowerLabel->setMonitorData(&m_sBumpMeter.m_usPower, Monitor::Uint16t);
    m_Widgets_2.append(m_pPowerLabel);

    //累计耗电量
    m_pTotalEnergyLabel = new DataLabel(ui->frame_1, DataLabel::Data);
    m_pTotalEnergyLabel->setAlignment(Qt::AlignLeft);
    m_pTotalEnergyLabel->setDataParameter("kWh", 0, Monitor::Uint32t);
    m_pTotalEnergyLabel->setMonitorData(&m_sBumpMeter.m_ulTotalEnergy, Monitor::Uint32t);
    m_Widgets_2.append(m_pTotalEnergyLabel);

    m_pCommErrLabel = new DataLabel(ui->frame_1, DataLabel::Text);
    m_pCommErrLabel->setAlignment(Qt::AlignLeft);
    m_pCommErrLabel->setValueMap(0,tr("正常"));
    m_pCommErrLabel->setValueMap(1,tr("故障"), Qt::red);
    m_pCommErrLabel->setMonitorData(&m_sBumpMeter.m_xCommErr, Monitor::Boolean);
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
    connect(m_pFreqSetBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
    connect(m_pErrorCleanCmdBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
}

void BumpPage::paramSetBtnValChanged(void*)
{
    System *pSystem = System::getInstance();
    ChilledBump* pChilledBump = nullptr;

    if(pSystem == nullptr){return;}
    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL || pSystem->m_xIsInDebug == true)
    {
        for(uint8_t i = 0; i < m_ChilledBumps.count(); i++)
        {
             pChilledBump = m_ChilledBumps[i];

             if((pChilledBump->m_xErrorFlag == false  && pChilledBump->m_xControlFlag == false && pChilledBump->m_xRemote == true) ||
                pSystem->m_xIsInDebug == true)
             {
                 pChilledBump->m_pSwitchCmdBtn->setValue( m_pSwitchCmdBtn->getCurrentValue() );
                 pChilledBump->m_pFreqSetBtn->setValue( uint16_t(m_pFreqSetBtn->getCurrentValue()) );
             }
             if(m_pErrorCleanCmdBtn->getCurrentValue() == 1)
             {
                 pChilledBump->m_xErrClean = true;
                 //pChilledBump->m_xErrorFlag = false;
                 //pChilledBump->m_xControlFlag = false;
             }
             else
             {
                 pChilledBump->m_xErrClean = false;
             }
        }
    }
    System::getInstance()->m_uiOffLogCount = 0;
}
