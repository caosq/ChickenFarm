#include "valvepage.h"
#include "butterflyvalve.h"
#include "ui_valvepage.h"
#include "system.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     2

#define LABEL_SIZE       110, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    150
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  90, 28

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  135
#define DATA_LABEL_INTERVAL_H   300
#define DATA_LABEL_INTERVAL_V   40

ValvePage::ValvePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ValvePage)
{
    ui->setupUi(this);
    initDevice();
    initLabel();
    initButton();
}

ValvePage::~ValvePage()
{
    delete ui;
}

void ValvePage::initDevice()
{
    ButterflyValve* pButterflyValve = nullptr;
    System   *pSystem = System::getInstance();

    if(pSystem == nullptr){return;}
    for(uint8_t n = 0, x = 0, y = 0; n < BUTTRERFLY_VALVE_NUM; n++)
    {
        x = n / 2;
        y = n % 2;

        pButterflyValve = new ButterflyValve();
        pSystem->m_pButterflyValves.append(pButterflyValve);

        m_ButterflyValves.append(pButterflyValve);
        ui->gridLayout->addWidget(pButterflyValve, x, y);
    }
    m_BypassValve = new BypassValve(ui->frame_1);
    pSystem->m_pBypassValve = m_BypassValve;
}

void ValvePage::initLabel()
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
    m_Labels[0]->setText(tr("开关控制"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("故障清除"), LABEL_FONT_SIZE);

    //ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 冷冻水泵");
}

void ValvePage::initButton()
{
    //开关控制
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets.append(m_pSwitchCmdBtn);

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
    connect(System::getInstance(), SIGNAL(sysModeCmdChanged()), this, SLOT(sysModeCmdChangedSlot()));
    sysModeCmdChangedSlot();
}

void ValvePage::paramSetBtnValChanged(int32_t val)
{
    System *pSystem = System::getInstance();
    ButterflyValve* pButterflyValve = nullptr;

    if(pSystem == nullptr){return;}
    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL)
    {
        for(uint8_t i = 0; i < m_ButterflyValves.count(); i++)
        {
             pButterflyValve = m_ButterflyValves[i];
             pButterflyValve->m_eSwitchCmd = ButterflyValve::SwitchCmd(m_pSwitchCmdBtn->getCurrentValue());
        }
    }
}

void ValvePage::sysModeCmdChangedSlot()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}
    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL)
    {
        m_pSwitchCmdBtn->setEnabled(true);
        m_BypassValve->m_pAngSetBtn->setEnabled(true);
        for(uint8_t n = 0; n < BUTTRERFLY_VALVE_NUM; n++)
        {
            m_ButterflyValves[n]->m_pSwitchCmdBtn->setEnabled(true);
        }
    }
    else
    {
        m_pSwitchCmdBtn->setEnabled(false);
        m_BypassValve->m_pAngSetBtn->setEnabled(false);
        for(uint8_t n = 0; n < BUTTRERFLY_VALVE_NUM; n++)
        {
            m_ButterflyValves[n]->m_pSwitchCmdBtn->setEnabled(false);
        }
    }
}
