#include "home.h"
#include "ui_home.h"
#include "system.h"
#include "messagebox.h"

#define HY1 130
#define HY2 135
#define HY3 160
#define HY4 250

#define VX1 30
#define VX2 150
#define VX3 270
#define VX4 630

#define VX5 700
#define VX6 750

#define SIZE 100,30
#define LABELSIZE 130,22

#define LABEL_FONT_SIZE  14
#define DATA_LABEL_SIZE  110, 28

#define COLOR_STATE_COOL         "#00FFFF"
#define COLOR_STATE_HEAT         "#FF5511"
#define COLOR_STATE_FAN          "#00FF00"
#define COLOR_STATE_NEGATICE_FAN "#FFBB66"
#define COLOR_STATE_EX_FAN       "#EE82EE"
#define COLOR_STATE_DEFROST      "#87CEFA"
#define COLOR_STATE_ANTI_FREEZE  "#FFFF00"

Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
    initLabel();
    initButton();
}

Home::~Home()
{
    delete ui;
}

void Home::initLabel()
{
    //ui->frame_7->hide();
    //ui->frame_27->show();
}

void Home::initButton()
{
    System *pSystem = System::getInstance();

    ModularChiller *pModularChiller = nullptr;
    ModularAir     *pModularAir = nullptr;
    Modular        *pModular = nullptr;

    ChilledBump    *pChilledBump = nullptr;
    AxialFan       *pAxialFan    = nullptr;
    ButterflyValve *pValve       = nullptr;

    if(pSystem == nullptr){return;}

    //温度
    m_pTempInLabel = new DataLabel(ui->frame_2, DataLabel::Data);
    m_pTempInLabel->setAlignment(Qt::AlignLeft);
    m_pTempInLabel->setDataParameter("℃", 1, Monitor::Int16t);
    m_pTempInLabel->setGeometry(HY4, 5, DATA_LABEL_SIZE);
    m_pTempInLabel->setMonitorData(&pSystem->m_sAmbientIn_T, Monitor::Int16t);

    //湿度
    m_pHumiInLabel = new DataLabel(ui->frame_2, DataLabel::Data);
    m_pHumiInLabel->setAlignment(Qt::AlignLeft);
    m_pHumiInLabel->setDataParameter("%", 1, Monitor::Uint16t);
    m_pHumiInLabel->setGeometry(HY4, 35, DATA_LABEL_SIZE);
    m_pHumiInLabel->setMonitorData(&pSystem->m_usAmbientIn_H, Monitor::Uint16t);


    //CO2
    m_pCO2Label = new DataLabel(ui->frame_2, DataLabel::Data);
    m_pCO2Label->setAlignment(Qt::AlignLeft);
    m_pCO2Label->setDataParameter("ppm", 1, Monitor::Uint16t);
    m_pCO2Label->setGeometry(HY4, 65, DATA_LABEL_SIZE);
    m_pCO2Label->setMonitorData(&pSystem->m_usCO2PPM, Monitor::Uint16t);

    //新风量
    m_pFreAirVolumeLabel = new DataLabel(ui->frame_2, DataLabel::Data);
    m_pFreAirVolumeLabel->setAlignment(Qt::AlignLeft);
    m_pFreAirVolumeLabel->setDataParameter("m³/h", 0, Monitor::Uint16t);
    m_pFreAirVolumeLabel->setGeometry(HY4, 95, DATA_LABEL_SIZE);
    m_pFreAirVolumeLabel->setMonitorData(&pSystem->m_ulTotalFreAir, Monitor::Uint32t);

    //温度
    m_pTempOutLabel = new DataLabel(ui->frame_3, DataLabel::Data);
    m_pTempOutLabel->setAlignment(Qt::AlignLeft);
    m_pTempOutLabel->setDataParameter("℃", 1, Monitor::Int16t);
    m_pTempOutLabel->setGeometry(HY3, 20, DATA_LABEL_SIZE);
    m_pTempOutLabel->setMonitorData(&pSystem->m_sAmbientOut_T, Monitor::Int16t);

    //湿度
    m_pHumiOutLabel = new DataLabel(ui->frame_3, DataLabel::Data);
    m_pHumiOutLabel->setAlignment(Qt::AlignLeft);
    m_pHumiOutLabel->setDataParameter("%", 1, Monitor::Uint16t);
    m_pHumiOutLabel->setGeometry(HY3, 50, DATA_LABEL_SIZE);
    m_pHumiOutLabel->setMonitorData(&pSystem->m_usAmbientOut_H, Monitor::Uint16t);

    //鸡生长周期天数
    m_pGrowDaysBtn = new AnalogValButton(ui->frame_4);
    m_pGrowDaysBtn->setDataParameter("天", 0, 0, 462, -2, Monitor::Int16t);
    m_pGrowDaysBtn->setGeometry(HY2, 50, DATA_LABEL_SIZE);
    m_pGrowDaysBtn->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_pGrowDaysBtn->setMonitorData(&pSystem->m_sChickenGrowDay, Monitor::Int16t, true);

    //系统工作模式设定
    m_pSysModeCmdBtn = new ModeButton(ui->frame_4);
    m_pSysModeCmdBtn->setItem(0,tr("关闭系统"));
    m_pSysModeCmdBtn->setItem(1,tr("手动模式"));
    m_pSysModeCmdBtn->setItem(2,tr("自动模式"));
    m_pSysModeCmdBtn->setItem(3,tr("紧急送风"));
    m_pSysModeCmdBtn->setDefaultValue(1);
    m_pSysModeCmdBtn->setGeometry(HY2, 95, DATA_LABEL_SIZE);
    m_pSysModeCmdBtn->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_pSysModeCmdBtn->setConfirmMode("请确认是否执行此操作");
    m_pSysModeCmdBtn->setMonitorData(&pSystem->m_eSystemModeCmd, Monitor::Uint16t);

    //系统状态
    m_pSysStateLabel = new DataLabel(ui->frame_4, DataLabel::Text);
    m_pSysStateLabel->setAlignment(Qt::AlignLeft);
    m_pSysStateLabel->setValueMap(0,tr("已关闭"));
    m_pSysStateLabel->setValueMap(1,tr("制冷运行"), QColor(COLOR_STATE_COOL));
    m_pSysStateLabel->setValueMap(2,tr("制热运行"), QColor(COLOR_STATE_HEAT));
    m_pSysStateLabel->setValueMap(3,tr("送风运行"), QColor(COLOR_STATE_FAN));
    m_pSysStateLabel->setValueMap(4,tr("负压通风"), QColor(COLOR_STATE_NEGATICE_FAN));
    m_pSysStateLabel->setValueMap(5,tr("排风运行"), QColor(COLOR_STATE_EX_FAN));
    m_pSysStateLabel->setValueMap(6,tr("正在关闭"));
    m_pSysStateLabel->setValueMap(7,tr("正在开启"));
    //m_pSysStateLabel->setText("中文");
    m_pSysStateLabel->setGeometry(HY2, 135, DATA_LABEL_SIZE);
    m_pSysStateLabel->setMonitorData(&pSystem->m_eSystemState, Monitor::Uint16t);

    m_pModularStateLabel_1 = new DataLabel(ui->frame_5, DataLabel::Text);
    m_pModularStateLabel_1->setAlignment(Qt::AlignLeft);
    m_pModularStateLabel_1->setValueMap(0,tr("已关闭"));
    m_pModularStateLabel_1->setValueMap(1,tr("制冷运行"), QColor(COLOR_STATE_COOL));
    m_pModularStateLabel_1->setValueMap(2,tr("制热运行"), QColor(COLOR_STATE_HEAT));
    m_pModularStateLabel_1->setValueMap(3,tr("手动化霜"), QColor(COLOR_STATE_DEFROST));
    m_pModularStateLabel_1->setValueMap(4,tr("冬天防冻"), QColor(COLOR_STATE_ANTI_FREEZE));
    m_pModularStateLabel_1->setGeometry(HY1, 10, DATA_LABEL_SIZE);
    m_pModularStateLabel_1->setMonitorData(&pSystem->m_pModularChillers[0]->m_eModularState,
                                           Monitor::Uint16t);

    m_pModularStateLabel_2 = new DataLabel(ui->frame_5, DataLabel::Text);
    m_pModularStateLabel_2->setAlignment(Qt::AlignLeft);
    m_pModularStateLabel_2->setValueMap(0,tr("已关闭"));
    m_pModularStateLabel_2->setValueMap(1,tr("制冷运行"), QColor(COLOR_STATE_COOL));
    m_pModularStateLabel_2->setValueMap(2,tr("制热运行"), QColor(COLOR_STATE_HEAT));
    m_pModularStateLabel_2->setValueMap(3,tr("手动化霜"), QColor(COLOR_STATE_DEFROST));
    m_pModularStateLabel_2->setValueMap(4,tr("冬天防冻"), QColor(COLOR_STATE_ANTI_FREEZE));
    m_pModularStateLabel_2->setGeometry(HY1, 40, DATA_LABEL_SIZE);
    m_pModularStateLabel_2->setMonitorData(&pSystem->m_pModularChillers[1]->m_eModularState,
                                           Monitor::Uint16t);

    //组合柜运行模式
    m_pModularAirModeLabel_1 = new DataLabel(ui->frame_5, DataLabel::Text);
    m_pModularAirModeLabel_1->setAlignment(Qt::AlignLeft);
    m_pModularAirModeLabel_1->setValueMap(0,tr("供冷运行"), QColor(COLOR_STATE_COOL));
    m_pModularAirModeLabel_1->setValueMap(1,tr("通风运行"), QColor(COLOR_STATE_FAN));
    m_pModularAirModeLabel_1->setValueMap(2,tr("供热运行"), QColor(COLOR_STATE_HEAT));
    m_pModularAirModeLabel_1->setValueMap(3,tr("负压通风"), QColor(COLOR_STATE_NEGATICE_FAN));
    m_pModularAirModeLabel_1->setValueMap(4,tr("已关闭"));
    m_pModularAirModeLabel_1->setGeometry(HY1, 70, DATA_LABEL_SIZE);
    m_pModularAirModeLabel_1->setMonitorData(&pSystem->m_pModularAirs[0]->m_eRunningState,
                                             Monitor::Uint16t);

    //组合柜运行模式
    m_pModularAirModeLabel_2 = new DataLabel(ui->frame_5, DataLabel::Text);
    m_pModularAirModeLabel_2->setAlignment(Qt::AlignLeft);
    m_pModularAirModeLabel_2->setValueMap(0,tr("供冷运行"), QColor(COLOR_STATE_COOL));
    m_pModularAirModeLabel_2->setValueMap(1,tr("通风运行"), QColor(COLOR_STATE_FAN));
    m_pModularAirModeLabel_2->setValueMap(2,tr("供热运行"), QColor(COLOR_STATE_HEAT));
    m_pModularAirModeLabel_2->setValueMap(3,tr("负压通风"), QColor(COLOR_STATE_NEGATICE_FAN));
    m_pModularAirModeLabel_2->setValueMap(4,tr("已关闭"));
    m_pModularAirModeLabel_2->setGeometry(HY1, 100, DATA_LABEL_SIZE);
    m_pModularAirModeLabel_2->setMonitorData(&pSystem->m_pModularAirs[1]->m_eRunningState,
                                             Monitor::Uint16t);
    //1号机组模块1
    pModularChiller = pSystem->m_pModularChillers[0];

    pModular = pModularChiller->m_Modulars[0];
    pModular->m_sQFrameState.IsRunning = ui->frame_11;
    pModular->m_sQFrameState.IsError = ui->frame_31;
    pModular->m_sQFrameState.IsError->hide();

    //1号机组模块2
    pModular = pModularChiller->m_Modulars[1];
    pModular->m_sQFrameState.IsRunning = ui->frame_10;
    pModular->m_sQFrameState.IsError = ui->frame_30;
    pModular->m_sQFrameState.IsError->hide();

    //1号机组模块3
    pModular = pModularChiller->m_Modulars[2];
    pModular->m_sQFrameState.IsRunning = ui->frame_10;
    pModular->m_sQFrameState.IsError = ui->frame_30;
    pModular->m_sQFrameState.IsError->hide();

    //2号机组模块1
    pModularChiller = pSystem->m_pModularChillers[1];

    pModular = pModularChiller->m_Modulars[0];
    pModular->m_sQFrameState.IsRunning = ui->frame_9;
    pModular->m_sQFrameState.IsError = ui->frame_29;

    //2号机组模块2
    pModular = pModularChiller->m_Modulars[1];
    pModular->m_sQFrameState.IsRunning = ui->frame_9;
    pModular->m_sQFrameState.IsError = ui->frame_29;
    pModular->m_sQFrameState.IsError->hide();

    //2号机组模块3
    pModular = pModularChiller->m_Modulars[2];
    pModular->m_sQFrameState.IsRunning = ui->frame_8;
    pModular->m_sQFrameState.IsError = ui->frame_28;
    pModular->m_sQFrameState.IsError->hide();

    //2号机组模块4
    pModular = pModularChiller->m_Modulars[3];
    pModular->m_sQFrameState.IsRunning = ui->frame_8;
    pModular->m_sQFrameState.IsError = ui->frame_28;
    pModular->m_sQFrameState.IsError->hide();

    //1号组空
    pModularAir = pSystem->m_pModularAirs[0];
    pModularAir->m_sQFrameState.IsRunning = ui->frame_6;
    pModularAir->m_sQFrameState.IsError = ui->frame_26;
    pModularAir->m_sQFrameState.IsError->hide();

    //2号组空
    pModularAir = pSystem->m_pModularAirs[1];
    pModularAir->m_sQFrameState.IsRunning = ui->frame_7;
    pModularAir->m_sQFrameState.IsError = ui->frame_27;
    pModularAir->m_sQFrameState.IsError->hide();

    //1号冷冻水泵
    pChilledBump = pSystem->m_pChilledBumps[0];
    pChilledBump->m_sQFrameState.IsRunning = ui->frame_16;
    pChilledBump->m_sQFrameState.IsError = ui->frame_23;
    pChilledBump->m_sQFrameState.IsError->hide();

    //2号冷冻水泵
    pChilledBump = pSystem->m_pChilledBumps[1];
    pChilledBump->m_sQFrameState.IsRunning = ui->frame_17;
    pChilledBump->m_sQFrameState.IsError = ui->frame_24;
    pChilledBump->m_sQFrameState.IsError->hide();

    //3号冷冻水泵
    pChilledBump = pSystem->m_pChilledBumps[2];
    pChilledBump->m_sQFrameState.IsRunning = ui->frame_18;
    pChilledBump->m_sQFrameState.IsError = ui->frame_25;
    pChilledBump->m_sQFrameState.IsError->hide();

    //1号畜牧风机
    pAxialFan = pSystem->m_pAxialFans[0];
    pAxialFan->m_sQFrameState.IsRunning = ui->frame_12;
    pAxialFan->m_sQFrameState.IsError = ui->frame_36;
    pAxialFan->m_sQFrameState.IsError->hide();

    //2号畜牧风机
    pAxialFan = pSystem->m_pAxialFans[1];
    pAxialFan->m_sQFrameState.IsRunning = ui->frame_13;
    pAxialFan->m_sQFrameState.IsError = ui->frame_37;
    pAxialFan->m_sQFrameState.IsError->hide();

    //3号畜牧风机
    pAxialFan = pSystem->m_pAxialFans[2];
    pAxialFan->m_sQFrameState.IsRunning = ui->frame_14;
    pAxialFan->m_sQFrameState.IsError = ui->frame_38;
    pAxialFan->m_sQFrameState.IsError->hide();

    //4号畜牧风机
    pAxialFan = pSystem->m_pAxialFans[3];
    pAxialFan->m_sQFrameState.IsRunning = ui->frame_15;
    pAxialFan->m_sQFrameState.IsError = ui->frame_39;
    pAxialFan->m_sQFrameState.IsError->hide();

    //1号蝶阀
    pValve = pSystem->m_pButterflyValves[0];
    pValve->m_sQFrameState.IsRunning = ui->frame_19;
    pValve->m_sQFrameState.IsError = ui->frame_32;
    pValve->m_sQFrameState.IsError->hide();

    //2号蝶阀
    pValve = pSystem->m_pButterflyValves[1];
    pValve->m_sQFrameState.IsRunning = ui->frame_20;
    pValve->m_sQFrameState.IsError = ui->frame_33;
    pValve->m_sQFrameState.IsError->hide();

    //3号蝶阀
    pValve = pSystem->m_pButterflyValves[2];
    pValve->m_sQFrameState.IsRunning = ui->frame_21;
    pValve->m_sQFrameState.IsError = ui->frame_34;
    pValve->m_sQFrameState.IsError->hide();

    //4号蝶阀
    pValve = pSystem->m_pButterflyValves[3];
    pValve->m_sQFrameState.IsRunning = ui->frame_22;
    pValve->m_sQFrameState.IsError = ui->frame_35;
    pValve->m_sQFrameState.IsError->hide();

    pSystem->m_pBypassValve->m_sQFrameState.IsRunning = ui->frame_41;
    pSystem->m_pBypassValve->m_sQFrameState.IsError = ui->frame_40;
    pSystem->m_pBypassValve->m_sQFrameState.IsError->hide();

    connect(pSystem, SIGNAL(systemDataChanged()), this, SLOT(systemDataChangedSlot()));
}

void Home::systemDataChangedSlot()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL && m_xSysModeChangedAlarm)
    {
        messageBox *pConfirmationBox = new messageBox(messageBox::Information);
        pConfirmationBox->setButtonText(messageBox::Yes,"确认");
        pConfirmationBox->setInformativeText("请注意，系统退出了自动运行");
        pConfirmationBox->exec();

        m_xSysModeChangedAlarm = false;
        return;
    }
    else if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_AUTO)
    {
        m_xSysModeChangedAlarm = true;
    }
    else
    {
        m_xSysModeChangedAlarm = false;
    }
}
