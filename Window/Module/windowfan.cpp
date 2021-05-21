#include "windowfan.h"
#include "ui_windowfan.h"
#include "system.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     3

#define LABEL_SIZE       120, 30
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     25
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    40

#define DATA_LABEL_SIZE  110, 30

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  130
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   40

uint8_t WindowFan::m_usWindowFanCount = 0;

WindowFan::WindowFan(QWidget *parent) :
    Device(parent),
    ui(new Ui::WindowFan)
{
    ui->setupUi(this);

    WindowFan::m_usWindowFanCount++;
    this->m_usDeviceIndex = WindowFan::m_usWindowFanCount;

    initLabel();
    initButton();
}

WindowFan::~WindowFan()
{
    delete ui;
}

void WindowFan::initLabel()
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
    m_Labels[1]->setText(tr("开度设置"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("开度反馈"), LABEL_FONT_SIZE);

    ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 小窗电机");
}

void WindowFan::initButton()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    Button::BtnCheckData mBtnCheckData0 = {&pSystem->m_eSystemModeCmd, System::MODE_MANUAL,
                                           Monitor::Boolean, "系统正在自动运行，请先切换成手动模式"};
    Button::BtnCheckData mBtnCheckData1 = {&pSystem->m_xIsLogIn, 1, Monitor::Boolean, "请先登录后再操作"};
    //开关控制
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_pSwitchCmdBtn->setMonitorData(&m_xSwitchCmd, Monitor::Boolean);
    m_pSwitchCmdBtn->setCheckMode(2, &mBtnCheckData0, &mBtnCheckData1);
    m_Widgets.append(m_pSwitchCmdBtn);

    //开度设置
    m_pAngSetBtn = new AnalogValButton(ui->frame);
    m_pAngSetBtn->setDataParameter("%", 0, 0, 100, 0, Monitor::Uint16t);
    m_pAngSetBtn->setCheckMode(2, &mBtnCheckData0, &mBtnCheckData1);
    m_pAngSetBtn->setMonitorData(&m_usAngSet, Monitor::Uint16t);
    m_Widgets.append(m_pAngSetBtn);

    //开度反馈
    m_pAngLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pAngLabel->setAlignment(Qt::AlignLeft);
    m_pAngLabel->setDataParameter("%", 0, Monitor::Uint16t);
    m_pAngLabel->setMonitorData(&m_usAng, Monitor::Uint16t);
    m_Widgets.append(m_pAngLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
}
