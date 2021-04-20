#include "modularair.h"
#include "ui_modularair.h"
#include "system.h"

#define LABEL_COLUMNS  2
#define LABEL_ROWS     14

#define LABEL_SIZE       140, 30
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   40
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  120, 30

#define DATA_LABEL_UP_MARGIN    32
#define DATA_LABEL_LEFT_MARGIN  170
#define DATA_LABEL_INTERVAL_H   320
#define DATA_LABEL_INTERVAL_V   35

#define COLOR_STATE_COOL         "#00FFFF"
#define COLOR_STATE_HEAT         "#FF5511"
#define COLOR_STATE_FAN          "#00FF00"
#define COLOR_STATE_NEGATICE_FAN "#FFBB66"
#define COLOR_STATE_EX_FAN       "#EE82EE"
#define COLOR_STATE_DEFROST      "#87CEFA"
#define COLOR_STATE_ANTI_FREEZE  "#FFFF00"

uint8_t ModularAir::m_usModularAirCount = 0;
ModularAir::ModularAir(QWidget *parent) :
    Device(parent),
    ui(new Ui::ModularAir)
{
    ui->setupUi(this);

    ModularAir::m_usModularAirCount++;
    this->m_usDeviceIndex = ModularAir::m_usModularAirCount;

    initLabel();
    initButton();
}

ModularAir::~ModularAir()
{
    delete ui;
}

void ModularAir::initLabel()
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
    m_Labels[0]->setText(tr("启停命令"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("工作模式"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("目标温度"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("目标湿度"), LABEL_FONT_SIZE);
    m_Labels[4]->setText(tr("目标CO2"), LABEL_FONT_SIZE);
    m_Labels[5]->setText(tr("运行状态"), LABEL_FONT_SIZE);
    m_Labels[6]->setText(tr("运行模式"), LABEL_FONT_SIZE);
    m_Labels[7]->setText(tr("本地/远程"), LABEL_FONT_SIZE);
    m_Labels[8]->setText(tr("排风阀开度"), LABEL_FONT_SIZE);
    m_Labels[9]->setText(tr("回风阀开度"), LABEL_FONT_SIZE);

    m_Labels[10]->setText(tr("新风阀开度"), LABEL_FONT_SIZE);
    m_Labels[11]->setText(tr("分流阀开度"), LABEL_FONT_SIZE);
    m_Labels[12]->setText(tr("盘管水阀开度"), LABEL_FONT_SIZE);
    m_Labels[13]->setText(tr("送风风机频率"), LABEL_FONT_SIZE);
    m_Labels[14]->setText(tr("排风风机频率"), LABEL_FONT_SIZE);
    m_Labels[15]->setText(tr("送风温度"), LABEL_FONT_SIZE);
    m_Labels[16]->setText(tr("送风湿度"), LABEL_FONT_SIZE);
    m_Labels[17]->setText(tr("排风量"), LABEL_FONT_SIZE);
    m_Labels[18]->setText(tr("新风量"), LABEL_FONT_SIZE);
    m_Labels[19]->setText(tr("转轮回收"), LABEL_FONT_SIZE);

    m_Labels[20]->setText(tr("湿膜加湿"), LABEL_FONT_SIZE);
    m_Labels[21]->setText(tr("送风风机"), LABEL_FONT_SIZE);
    m_Labels[22]->setText(tr("排风风机"), LABEL_FONT_SIZE);
    m_Labels[23]->setText(tr("通讯故障"), LABEL_FONT_SIZE);
    m_Labels[24]->setText(tr("综合故障标志"), LABEL_FONT_SIZE);
    m_Labels[25]->setText(tr("排风风速传感器"), LABEL_FONT_SIZE);
    m_Labels[26]->setText(tr("新风风速传感器"), LABEL_FONT_SIZE);

    ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 组合柜");
}

void ModularAir::initButton()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    Button::BtnCheckData mBtnCheckData0 = {&pSystem->m_eSystemModeCmd, System::MODE_MANUAL,
                                           Monitor::Boolean, "系统正在自动运行，请先切换成手动模式"};
    Button::BtnCheckData mBtnCheckData1 = {&pSystem->m_xIsLogIn, 1, Monitor::Boolean, "请先登录后再操作"};
    Button::BtnCheckData mBtnCheckData2 = {&m_xCommErr, 0, Monitor::Boolean, "设备通讯故障，请先检查设备通讯问题" };

     //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setValueMap(StateButton::State0, 0x0055);
    m_pSwitchCmdBtn->setValueMap(StateButton::State1, 0x00AA);
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_pSwitchCmdBtn->setMonitorData(&m_eSwitchCmd, Monitor::Uint16t);
    m_pSwitchCmdBtn->setCheckMode(3, &mBtnCheckData0, &mBtnCheckData1, &mBtnCheckData2);
    m_Widgets.append(m_pSwitchCmdBtn);

    //机组运行工作模式设定
    m_pRunningModeCmdBtn = new ModeButton(ui->frame);
    m_pRunningModeCmdBtn->setItem(0,tr("供冷"));
    m_pRunningModeCmdBtn->setItem(1,tr("通风"));
    m_pRunningModeCmdBtn->setItem(2,tr("供热"));
    m_pRunningModeCmdBtn->setItem(3,tr("负压通风"));
    m_pRunningModeCmdBtn->setItem(4,tr("自动"));
    m_pRunningModeCmdBtn->setDefaultValue(0);
    m_pRunningModeCmdBtn->setMonitorData(&m_eRunningModeCmd, Monitor::Uint16t);
    m_pRunningModeCmdBtn->setCheckMode(3, &mBtnCheckData0, &mBtnCheckData1, &mBtnCheckData2);
    m_Widgets.append(m_pRunningModeCmdBtn);

    //目标温度设定
    m_pTempSetBtn = new AnalogValButton(ui->frame);
    m_pTempSetBtn->setDataParameter("℃", 1, 240, 350, 160, Monitor::Uint16t);
    m_pTempSetBtn->setMonitorData(&m_usTempSet, Monitor::Uint16t);
    m_pTempSetBtn->setCheckMode(2, &mBtnCheckData1, &mBtnCheckData2);
    m_Widgets.append(m_pTempSetBtn);

    //目标湿度设定
    m_pHumiSetBtn = new AnalogValButton(ui->frame);
    m_pHumiSetBtn->setDataParameter("%", 1, 500, 1000, 0, Monitor::Uint16t);
    m_pHumiSetBtn->setMonitorData(&m_usHumiSet, Monitor::Uint16t);
    m_pHumiSetBtn->setCheckMode(2, &mBtnCheckData1, &mBtnCheckData2);
    m_Widgets.append(m_pHumiSetBtn);

    //目标CO2设定
    m_pCO2SetBtn = new AnalogValButton(ui->frame);
    m_pCO2SetBtn->setDataParameter("ppm", 1, 20000, 30000, 10000, Monitor::Uint16t);
    m_pCO2SetBtn->setMonitorData(&m_usCO2Set, Monitor::Uint16t);
    m_pCO2SetBtn->setCheckMode(2, &mBtnCheckData1, &mBtnCheckData2);
    m_Widgets.append(m_pCO2SetBtn);

    //机组状态
    m_pModularStateLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pModularStateLabel->setAlignment(Qt::AlignLeft);
    m_pModularStateLabel->setValueMap(0,tr("已关闭"));
    m_pModularStateLabel->setValueMap(1,tr("开机中"));
    m_pModularStateLabel->setValueMap(2,tr("关机中"));
    m_pModularStateLabel->setValueMap(3,tr("运行中"), Qt::green);
    m_pModularStateLabel->setMonitorData(&m_eModularState, Monitor::Uint16t);
    m_Widgets.append(m_pModularStateLabel);

    //机组运行模式
    m_pRunningModeLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pRunningModeLabel->setAlignment(Qt::AlignLeft);
    m_pRunningModeLabel->setValueMap(0,tr("供冷"), QColor(COLOR_STATE_COOL));
    m_pRunningModeLabel->setValueMap(1,tr("通风"), QColor(COLOR_STATE_FAN));
    m_pRunningModeLabel->setValueMap(2,tr("供热"), QColor(COLOR_STATE_HEAT));
    m_pRunningModeLabel->setValueMap(3,tr("负压通风"), QColor(COLOR_STATE_NEGATICE_FAN));
    m_pRunningModeLabel->setMonitorData(&m_eRunningMode, Monitor::Uint16t);
    m_Widgets.append(m_pRunningModeLabel);

    //机组控制模式
    m_pControlModeLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pControlModeLabel->setAlignment(Qt::AlignLeft);
    m_pControlModeLabel->setValueMap(0,tr("本地"));
    m_pControlModeLabel->setValueMap(1,tr("远程"), Qt::green);
    m_pControlModeLabel->setMonitorData(&m_eControlMode, Monitor::Uint16t);
    m_Widgets.append(m_pControlModeLabel);

    //排风阀当前开度
    m_pExitAirDamperAngLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pExitAirDamperAngLabel->setAlignment(Qt::AlignLeft);
    m_pExitAirDamperAngLabel->setDataParameter("%", 1, Monitor::Uint16t);
    m_pExitAirDamperAngLabel->setMonitorData(&m_usExitAirDamperAng, Monitor::Uint16t);
    m_Widgets.append(m_pExitAirDamperAngLabel);

    //回风阀当前开度
    m_pRetAirDamperAngLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pRetAirDamperAngLabel->setAlignment(Qt::AlignLeft);
    m_pRetAirDamperAngLabel->setDataParameter("%", 1, Monitor::Uint16t);
    m_pRetAirDamperAngLabel->setMonitorData(&m_usRetAirDamperAng, Monitor::Uint16t);
    m_Widgets.append(m_pRetAirDamperAngLabel);

    //新风阀当前开度
    m_pFreAirDamperAngLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pFreAirDamperAngLabel->setAlignment(Qt::AlignLeft);
    m_pFreAirDamperAngLabel->setDataParameter("%", 1, Monitor::Uint16t);
    m_pFreAirDamperAngLabel->setMonitorData(&m_usFreAirDamperAng, Monitor::Uint16t);
    m_Widgets.append(m_pFreAirDamperAngLabel);

    //分流阀反馈开度
    m_pDivideDamperAngLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pDivideDamperAngLabel->setAlignment(Qt::AlignLeft);
    m_pDivideDamperAngLabel->setDataParameter("%", 1, Monitor::Uint16t);
    m_pDivideDamperAngLabel->setMonitorData(&m_usDivideDamperAng, Monitor::Uint16t);
    m_Widgets.append(m_pDivideDamperAngLabel);

    //盘管水阀反馈开度
    m_pCoilerDamperAngLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pCoilerDamperAngLabel->setAlignment(Qt::AlignLeft);
    m_pCoilerDamperAngLabel->setDataParameter("%", 1, Monitor::Uint16t);
    m_pCoilerDamperAngLabel->setMonitorData(&m_usCoilerDamperAng, Monitor::Uint16t);
    m_Widgets.append(m_pCoilerDamperAngLabel);

    //送风风机频率
    m_pSupAirFanFregLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pSupAirFanFregLabel->setAlignment(Qt::AlignLeft);
    m_pSupAirFanFregLabel->setDataParameter("Hz", 1, Monitor::Uint16t);
    m_pSupAirFanFregLabel->setMonitorData(&m_usSupAirFanFreg, Monitor::Uint16t);
    m_Widgets.append(m_pSupAirFanFregLabel);

    //排风风机频率
    m_pExitAirFanFregLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pExitAirFanFregLabel->setAlignment(Qt::AlignLeft);
    m_pExitAirFanFregLabel->setDataParameter("Hz", 1, Monitor::Uint16t);
    m_pExitAirFanFregLabel->setMonitorData(&m_usExitAirFanFreg, Monitor::Uint16t);
    m_Widgets.append(m_pExitAirFanFregLabel);

    //送风温度
    m_pSupAirTempLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pSupAirTempLabel->setAlignment(Qt::AlignLeft);
    m_pSupAirTempLabel->setDataParameter("℃", 1, Monitor::Int16t);
    m_pSupAirTempLabel->setMonitorData(&m_sSupAirTemp, Monitor::Int16t);
    m_Widgets.append(m_pSupAirTempLabel);

    //送风湿度
    m_pSupAirHumiLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pSupAirHumiLabel->setAlignment(Qt::AlignLeft);
    m_pSupAirHumiLabel->setDataParameter("%", 1, Monitor::Uint16t);
    m_pSupAirHumiLabel->setMonitorData(&m_usSupAirHumi, Monitor::Uint16t);
    m_Widgets.append(m_pSupAirHumiLabel);

    //排风量
    m_pExitAirVolumeLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pExitAirVolumeLabel->setAlignment(Qt::AlignLeft);
    m_pExitAirVolumeLabel->setDataParameter("m³/h", 0, Monitor::Uint16t);
    m_pExitAirVolumeLabel->setMonitorData(&m_usExitAirVolume, Monitor::Uint16t);
    m_Widgets.append(m_pExitAirVolumeLabel);

    //新风量
    m_pFreAirVolumeLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pFreAirVolumeLabel->setAlignment(Qt::AlignLeft);
    m_pFreAirVolumeLabel->setDataParameter("m³/h", 0, Monitor::Uint16t);
    m_pFreAirVolumeLabel->setMonitorData(&m_usFreAirVolume, Monitor::Uint16t);
    m_Widgets.append(m_pFreAirVolumeLabel);

    //转轮回收
    m_pRecycleModeLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pRecycleModeLabel->setAlignment(Qt::AlignLeft);
    m_pRecycleModeLabel->setValueMap(0,tr("关闭"));
    m_pRecycleModeLabel->setValueMap(1,tr("开启"), Qt::green);
    m_pRecycleModeLabel->setMonitorData(&m_xRecycleMode, Monitor::Boolean);
    m_Widgets.append(m_pRecycleModeLabel);

    //湿膜加湿
    m_pWetModeLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pWetModeLabel->setAlignment(Qt::AlignLeft);
    m_pWetModeLabel->setValueMap(0,tr("关闭"));
    m_pWetModeLabel->setValueMap(1,tr("开启"), Qt::green);
    m_pWetModeLabel->setMonitorData(&m_xWetMode, Monitor::Boolean);
    m_Widgets.append(m_pWetModeLabel);

    //送风风机
    m_pSupAirFanLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pSupAirFanLabel->setAlignment(Qt::AlignLeft);
    m_pSupAirFanLabel->setValueMap(0,tr("关闭"));
    m_pSupAirFanLabel->setValueMap(1,tr("开启"), Qt::green);
    m_pSupAirFanLabel->setMonitorData(&m_xSupAirFan, Monitor::Boolean);
    m_Widgets.append(m_pSupAirFanLabel);

    //排风风机
    m_pExitAirFanLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pExitAirFanLabel->setAlignment(Qt::AlignLeft);
    m_pExitAirFanLabel->setValueMap(0,tr("关闭"));
    m_pExitAirFanLabel->setValueMap(1,tr("开启"), Qt::green);
    m_pExitAirFanLabel->setMonitorData(&m_xExitAirFan, Monitor::Boolean);
    m_Widgets.append(m_pExitAirFanLabel);

    //通讯故障
    m_pCommErrLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pCommErrLabel->setAlignment(Qt::AlignLeft);
    m_pCommErrLabel->setValueMap(0,tr("正常"));
    m_pCommErrLabel->setValueMap(1,tr("故障"), Qt::red);
    m_pCommErrLabel->setMonitorData(&m_xCommErr, Monitor::Boolean);
    m_Widgets.append(m_pCommErrLabel);

    //综合故障标志
    m_pAlarmFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pAlarmFlagLabel->setAlignment(Qt::AlignLeft);
    m_pAlarmFlagLabel->setValueMap(0,tr("正常"));
    m_pAlarmFlagLabel->setValueMap(1,tr("故障"), Qt::red);
    m_pAlarmFlagLabel->setMonitorData(&m_xAlarmFlag, Monitor::Boolean);
    m_Widgets.append(m_pAlarmFlagLabel);

    //排风风速传感器故障
    m_pExitAirSenErrLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pExitAirSenErrLabel->setAlignment(Qt::AlignLeft);
    m_pExitAirSenErrLabel->setValueMap(0,tr("正常"));
    m_pExitAirSenErrLabel->setValueMap(1,tr("故障"), Qt::red);
    m_pExitAirSenErrLabel->setMonitorData(&m_xExitAirSenErr, Monitor::Boolean);
    m_Widgets.append(m_pExitAirSenErrLabel);

    //新风风速传感器故障
    m_pFreAirSenErrLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pFreAirSenErrLabel->setAlignment(Qt::AlignLeft);
    m_pFreAirSenErrLabel->setValueMap(0,tr("正常"));
    m_pFreAirSenErrLabel->setValueMap(1,tr("故障"), Qt::red);
    m_pFreAirSenErrLabel->setMonitorData(&m_xFreAirSenErr, Monitor::Boolean);
    m_Widgets.append(m_pFreAirSenErrLabel);


    //m_pTempSetBtn->enableDefaultMarker(false);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_ROWS;
        n = i % LABEL_ROWS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + m * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + n * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    connect(m_pExitAirSenErrLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pFreAirSenErrLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pCommErrLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pAlarmFlagLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pModularStateLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pControlModeLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pRunningModeLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));

    connect(m_pRecycleModeLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pWetModeLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pSupAirFanLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pExitAirFanLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
}

void ModularAir::stateChangedSlot(void*)
{
    if(m_sQFrameState.IsError != nullptr)
    {
        if(m_xCommErr || m_xAlarmFlag || m_xFreAirSenErr || m_xExitAirSenErr)
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
        if(m_eModularState == STATE_CLOSED)
        {
            m_sQFrameState.IsRunning->show();
            m_eRunningState = AIR_RUN_MODE_CLOSED;
        }
        else
        {
            if(m_eRunningMode == RUN_MODE_COOL)
            {
                m_eRunningState = AIR_RUN_MODE_COOL;
            }
            else if (m_eRunningMode == RUN_MODE_FAN)
            {
                m_eRunningState = AIR_RUN_MODE_FAN;
            }
            else if (m_eRunningMode == RUN_MODE_HEAT)
            {
                m_eRunningState = AIR_RUN_MODE_HEAT;
            }
            else if (m_eRunningMode == RUN_MODE_NEGATICE_FAN)
            {
                m_eRunningState = AIR_RUN_MODE_NEGATICE_FAN;
            }
            m_sQFrameState.IsRunning->hide();
        }
    }
    m_usStateMask_1 = m_xAlarmFlag | (m_xCommErr << 1) | (m_xSupAirFan << 2) | (m_xExitAirFan << 3) |
                      (m_xWetMode << 4) | (m_xRecycleMode << 5) | (m_xExitAirSenErr << 6) | (m_xFreAirSenErr << 7);
}
