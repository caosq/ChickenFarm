#include "axialfanpage.h"
#include "ui_axialfanpage.h"
#include "system.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     6

#define LABEL_SIZE       110, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     25
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    150
#define LABEL_INTERVAL_V    38

#define DATA_LABEL_SIZE  110, 28

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  135
#define DATA_LABEL_INTERVAL_H   300
#define DATA_LABEL_INTERVAL_V   38

AxialFanPage::AxialFanPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AxialFanPage)
{
    ui->setupUi(this);

    initDevice();
    initLabel();
    initButton();
}

AxialFanPage::~AxialFanPage()
{
    delete ui;
}

void AxialFanPage::initDevice()
{
    AxialFan *pAxialFan = nullptr;
    System   *pSystem = System::getInstance();

    if(pSystem == nullptr){return;}
    for(uint8_t n = 0, x = 0, y = 0; n < AXIAL_FAN_NUM; n++)
    {
        x = n / 2;
        y = n % 2;

        pAxialFan = new AxialFan(); 
        m_AxialFans.append(pAxialFan);
        pSystem->m_pAxialFans.append(pAxialFan);
        ui->gridLayout->addWidget(pAxialFan, x, y);
    }

    WindowFan *m_psWindowFan = nullptr;
    for(uint8_t n = 0; n < WINDOW_FAN_NUM; n++)
    {
        m_psWindowFan = new WindowFan();
        m_WindowFans.append(m_psWindowFan);
        pSystem->m_pWindowFans.append(m_psWindowFan);
        ui->verticalLayout->insertWidget(n, m_psWindowFan);
    }
}

void AxialFanPage::initLabel()
{
    TextLabel *pLabel = nullptr;

    for(uint8_t n = 0; n < LABEL_ROWS; n++)
    {
        for(uint8_t m = 0; m < LABEL_COLUMNS; m++)
        {
            pLabel = new TextLabel(ui->frame);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels.append(pLabel);
        }
    }
    m_Labels[0]->setText(tr("启停命令"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("频率设置"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("故障清除"), LABEL_FONT_SIZE);

    //ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 冷冻水泵");
}

void AxialFanPage::initButton()
{
    //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets.append(m_pSwitchCmdBtn);

    //频率设置
    m_pFreqSetBtn = new AnalogValButton(ui->frame);
    m_pFreqSetBtn->setDataParameter("Hz", 1, 350, 500, 350, Monitor::Uint16t);
    m_Widgets.append(m_pFreqSetBtn);

    //故障清除
    m_pErrorCleanCmdBtn = new StateButton(ui->frame);
    m_pErrorCleanCmdBtn->setStateText(StateButton::State0,tr("否"));
    m_pErrorCleanCmdBtn->setStateText(StateButton::State1,tr("是"));
    m_pErrorCleanCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets.append(m_pErrorCleanCmdBtn);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    connect(m_pSwitchCmdBtn, SIGNAL(valChanged(int32_t)), this, SLOT(paramSetBtnValChanged(int32_t)));
    connect(m_pFreqSetBtn, SIGNAL(valChanged(int32_t)), this, SLOT(paramSetBtnValChanged(int32_t)));
    connect(System::getInstance(), SIGNAL(sysModeCmdChanged()), this, SLOT(sysModeCmdChangedSlot()));
    sysModeCmdChangedSlot();
}

void AxialFanPage::paramSetBtnValChanged(int32_t val)
{
    System *pSystem = System::getInstance();
    AxialFan *pAxialFan = nullptr;

    if(pSystem == nullptr){return;}
    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL)
    {
        for(uint8_t i = 0; i < m_AxialFans.count(); i++)
        {
             pAxialFan = m_AxialFans[i];
             pAxialFan->m_eSwitchCmd = AxialFan::SwitchCmd(m_pSwitchCmdBtn->getCurrentValue());
             pAxialFan->m_usFreqSet = uint16_t(m_pFreqSetBtn->getCurrentValue());
        }
    }
}

void AxialFanPage::sysModeCmdChangedSlot()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}
    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL)
    {
        m_pSwitchCmdBtn->setEnabled(true);
        m_pFreqSetBtn->setEnabled(true);
        for(uint8_t n = 0; n < AXIAL_FAN_NUM; n++)
        {
            m_AxialFans[n]->m_pSwitchCmdBtn->setEnabled(true);
            m_AxialFans[n]->m_pFreqSetBtn->setEnabled(true);
        }
        for(uint8_t n = 0; n < WINDOW_FAN_NUM; n++)
        {
            m_WindowFans[n]->m_pSwitchCmdBtn->setEnabled(true);
            m_WindowFans[n]->m_pAngSetBtn->setEnabled(true);
        }
    }
    else
    {
        m_pSwitchCmdBtn->setEnabled(false);
        m_pFreqSetBtn->setEnabled(false);
        for(uint8_t n = 0; n < AXIAL_FAN_NUM; n++)
        {
            m_AxialFans[n]->m_pSwitchCmdBtn->setEnabled(false);
            m_AxialFans[n]->m_pFreqSetBtn->setEnabled(false);
        }
        for(uint8_t n = 0; n < WINDOW_FAN_NUM; n++)
        {
            m_WindowFans[n]->m_pSwitchCmdBtn->setEnabled(false);
            m_WindowFans[n]->m_pAngSetBtn->setEnabled(false);
        }
    }
}
