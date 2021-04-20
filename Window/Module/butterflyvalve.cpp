#include "butterflyvalve.h"
#include "ui_butterflyvalve.h"
#include "system.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     7

#define LABEL_SIZE       140, 30
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   40
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  110, 30

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  160
#define DATA_LABEL_INTERVAL_H   200
#define DATA_LABEL_INTERVAL_V   35

uint8_t ButterflyValve::m_usButterflyValveCount = 0;

ButterflyValve::ButterflyValve(QWidget *parent) :
    Device(parent),
    ui(new Ui::ButterflyValve)
{
    ui->setupUi(this);

    ButterflyValve::m_usButterflyValveCount++;
    this->m_usDeviceIndex = ButterflyValve::m_usButterflyValveCount;

    initLabel();
    initButton();
}

ButterflyValve::~ButterflyValve()
{
    delete ui;
}

void ButterflyValve::initLabel()
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
    m_Labels[1]->setText(tr("本地/远程"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("开到位"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("关到位"), LABEL_FONT_SIZE);
    m_Labels[4]->setText(tr("设备故障"), LABEL_FONT_SIZE);

    ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 冷冻蝶阀");
}

void ButterflyValve::initButton()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    Button::BtnCheckData mBtnCheckData0 = {&pSystem->m_eSystemModeCmd, System::MODE_MANUAL,
                                           Monitor::Boolean, "系统正在自动运行，请先切换成手动模式"};
    Button::BtnCheckData mBtnCheckData1 = {&pSystem->m_xIsLogIn, 1, Monitor::Boolean, "请先登录后再操作"};
    Button::BtnCheckData mBtnCheckData2 = {&m_xErrorFlag, 0, Monitor::Boolean,  "设备故障，请先检查设备后再清除故障" };
    Button::BtnCheckData mBtnCheckData3 = {&m_xRemote, 1, Monitor::Boolean, "设备处于本地模式，请先切换成远程模式"};

    //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_pSwitchCmdBtn->setMonitorData(&m_xSwitchCmd, Monitor::Boolean);
    m_pSwitchCmdBtn->setCheckMode(4, &mBtnCheckData0, &mBtnCheckData1, &mBtnCheckData2, &mBtnCheckData3);
    m_Widgets.append(m_pSwitchCmdBtn);

    //远程/本地
    m_pRemoteLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pRemoteLabel->setAlignment(Qt::AlignLeft);
    m_pRemoteLabel->setValueMap(0,tr("本地"));
    m_pRemoteLabel->setValueMap(1,tr("远程"), Qt::green);
    m_pRemoteLabel->setMonitorData(&m_xRemote, Monitor::Boolean);
    m_Widgets.append(m_pRemoteLabel);

    //开到位
    m_xOpenedLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_xOpenedLabel->setAlignment(Qt::AlignLeft);
    m_xOpenedLabel->setValueMap(0,tr("未开到位"));
    m_xOpenedLabel->setValueMap(1,tr("开到位"), Qt::green);
    m_xOpenedLabel->setMonitorData(&m_xOpened, Monitor::Boolean);
    m_Widgets.append(m_xOpenedLabel);

    //关到位
    m_xClosedLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_xClosedLabel->setAlignment(Qt::AlignLeft);
    m_xClosedLabel->setValueMap(0,tr("未关到位"));
    m_xClosedLabel->setValueMap(1,tr("关到位"), Qt::green);
    m_xClosedLabel->setMonitorData(&m_xClosed, Monitor::Boolean);
    m_Widgets.append(m_xClosedLabel);

    //设备故障
    m_pErrorFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pErrorFlagLabel->setAlignment(Qt::AlignLeft);
    m_pErrorFlagLabel->setValueMap(0,tr("正常"));
    m_pErrorFlagLabel->setValueMap(1,tr("故障"), Qt::red);
    m_pErrorFlagLabel->setMonitorData(&m_xErrorFlag, Monitor::Boolean);
    m_Widgets.append(m_pErrorFlagLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    connect(m_pErrorFlagLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_xClosedLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_xOpenedLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pSwitchCmdBtn, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pRemoteLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
}

void ButterflyValve::stateChangedSlot(void*)
{
    if(m_sQFrameState.IsError != nullptr)
    {
        if(m_xErrorFlag)
        {
            m_sQFrameState.IsError->show();
        }
        else
        {
            m_sQFrameState.IsError->hide();
        }
    }
    if(m_sQFrameState.IsRunning != nullptr)
    {
        if(m_xOpened)
        {
            m_sQFrameState.IsRunning->hide();
        }
        else if(m_xClosed)
        {
            m_sQFrameState.IsRunning->show();
        }
    }
    m_usStateMask = m_xErrClean | (m_xSwitchCmd << 1) | (m_xRemote << 2) | (m_xOpened << 3) |
                    (m_xClosed << 4) | (m_xErrorFlag << 6);
}
