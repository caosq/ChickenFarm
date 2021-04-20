#include "modular.h"
#include "ui_modular.h"
#include "datamonitor.h"
#include "textlabel.h"
#include "system.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     9

#define LABEL_SIZE       120, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     20
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  100, 25

#define DATA_LABEL_UP_MARGIN    20
#define DATA_LABEL_LEFT_MARGIN  170
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   35

#define COLOR_STATE_COOL         "#00FFFF"
#define COLOR_STATE_HEAT         "#FF5511"
#define COLOR_STATE_FAN          "#00FF00"
#define COLOR_STATE_NEGATICE_FAN "#FFBB66"
#define COLOR_STATE_EX_FAN       "#EE82EE"
#define COLOR_STATE_DEFROST      "#87CEFA"
#define COLOR_STATE_ANTI_FREEZE  "#FFFF00"

uint8_t Modular::m_usModularCount = 0;
Modular::Modular(QWidget *parent, uint16_t usDeviceIndex) :
    Device(parent),
    ui(new Ui::Modular)
{
    ui->setupUi(this);

    Modular::m_usModularCount++;
    this->m_usDeviceIndex = usDeviceIndex;

    initLabel();
    initButton();
}

Modular::~Modular()
{
    delete ui;
}

void Modular::initLabel()
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
    m_Labels[0]->setText(tr("模块状态"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("进水温度"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("出水温度"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("运行标志"), LABEL_FONT_SIZE);
    m_Labels[4]->setText(tr("水流开关"), LABEL_FONT_SIZE);
    m_Labels[5]->setText(tr("总故障标志"), LABEL_FONT_SIZE);
    m_Labels[6]->setText(tr("总报警标志"), LABEL_FONT_SIZE);
    m_Labels[7]->setText(tr("压缩机1"), LABEL_FONT_SIZE);
    m_Labels[8]->setText(tr("压缩机2"), LABEL_FONT_SIZE);

    ui->label->setText("模块 " + QString::number(this->m_usDeviceIndex));
    ui->label->setGeometry(128, 15, ui->label->width(), ui->label->height());
}

void Modular::initButton()
{
    //机组状态
    m_pModularStateLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pModularStateLabel->setAlignment(Qt::AlignLeft);
    m_pModularStateLabel->setValueMap(0,tr("关机"));
    m_pModularStateLabel->setValueMap(1,tr("制冷"), QColor(COLOR_STATE_COOL));
    m_pModularStateLabel->setValueMap(2,tr("制热"), QColor(COLOR_STATE_HEAT));
    m_pModularStateLabel->setValueMap(3,tr("化霜"), QColor(COLOR_STATE_DEFROST));
    m_pModularStateLabel->setValueMap(4,tr("防冻"), QColor(COLOR_STATE_ANTI_FREEZE));
    m_pModularStateLabel->setMonitorData(&m_eModularState, Monitor::Uint16t);
    m_Widgets.append(m_pModularStateLabel);


    //进水温度
    m_pInputWaterTempLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pInputWaterTempLabel->setAlignment(Qt::AlignLeft);
    m_pInputWaterTempLabel->setDataParameter("℃", 1, Monitor::Int16t);
    m_pInputWaterTempLabel->setMonitorData(&m_sInputWaterTemp, Monitor::Int16t);
    m_Widgets.append(m_pInputWaterTempLabel);

    //出水温度
    m_pOutputWaterTempLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pOutputWaterTempLabel->setAlignment(Qt::AlignLeft);
    m_pOutputWaterTempLabel->setDataParameter("℃", 1, Monitor::Int16t);
    m_pOutputWaterTempLabel->setMonitorData(&m_sOutputWaterTemp, Monitor::Int16t);
    m_Widgets.append(m_pOutputWaterTempLabel);

    //运行标志
    m_pRunningFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pRunningFlagLabel->setAlignment(Qt::AlignLeft);
    m_pRunningFlagLabel->setValueMap(0,tr("关闭"));
    m_pRunningFlagLabel->setValueMap(1,tr("运行"), Qt::green);
    m_pRunningFlagLabel->setMonitorData(&m_xRunningFlag, Monitor::Boolean);
    m_Widgets.append(m_pRunningFlagLabel);

    //水流开关
    m_pWaterValveLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pWaterValveLabel->setAlignment(Qt::AlignLeft);
    m_pWaterValveLabel->setValueMap(0,tr("关闭"));
    m_pWaterValveLabel->setValueMap(1,tr("开启"), Qt::green);
    m_pWaterValveLabel->setMonitorData(&m_xWaterValve, Monitor::Boolean);
    m_Widgets.append(m_pWaterValveLabel);

    //总故障标志
    m_pErrorFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pErrorFlagLabel->setAlignment(Qt::AlignLeft);
    m_pErrorFlagLabel->setValueMap(0,tr("正常"));
    m_pErrorFlagLabel->setValueMap(1,tr("故障"), Qt::red);
    m_pErrorFlagLabel->setMonitorData(&m_xErrorFlag, Monitor::Boolean);
    m_Widgets.append(m_pErrorFlagLabel);

    //总报警标志
    m_pAlarmFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pAlarmFlagLabel->setAlignment(Qt::AlignLeft);
    m_pAlarmFlagLabel->setValueMap(0,tr("正常"));
    m_pAlarmFlagLabel->setValueMap(1,tr("报警"), Qt::red);
    m_pAlarmFlagLabel->setMonitorData(&m_xAlarmFlag, Monitor::Boolean);
    m_Widgets.append(m_pAlarmFlagLabel);

    //压缩机1
    m_pCompRunnnig_1 = new DataLabel(ui->frame, DataLabel::Text);
    m_pCompRunnnig_1->setAlignment(Qt::AlignLeft);
    m_pCompRunnnig_1->setValueMap(0,tr("关闭"));
    m_pCompRunnnig_1->setValueMap(1,tr("运行"), Qt::green);
    m_pCompRunnnig_1->setMonitorData(&m_xCompRunnnig_1, Monitor::Boolean);
    m_Widgets.append(m_pCompRunnnig_1);

    //压缩机2
    m_pCompRunnnig_2 = new DataLabel(ui->frame, DataLabel::Text);
    m_pCompRunnnig_2->setAlignment(Qt::AlignLeft);
    m_pCompRunnnig_2->setValueMap(0,tr("关闭"));
    m_pCompRunnnig_2->setValueMap(1,tr("运行"), Qt::green);
    m_pCompRunnnig_2->setMonitorData(&m_xCompRunnnig_2, Monitor::Boolean);
    m_Widgets.append(m_pCompRunnnig_2);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    connect(m_pErrorFlagLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pAlarmFlagLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pRunningFlagLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
}

void Modular::stateChangedSlot(void*)
{
    if(m_sQFrameState.IsError != nullptr)
    {
        if(m_xErrorFlag || m_xAlarmFlag)
        {
            m_sQFrameState.IsError->show();
        }
        else
        {
            if(m_pBrotherModular != nullptr )
            {
                if(m_pBrotherModular->m_xAlarmFlag == false && m_pBrotherModular->m_xErrorFlag == false)
                m_sQFrameState.IsError->hide();
            }
            else
            {
                m_sQFrameState.IsError->hide();
            }
        }
    }
    if(m_sQFrameState.IsRunning != nullptr)
    {
        if(m_xRunningFlag)
        {
            m_sQFrameState.IsRunning->hide();
        }
        else if(m_pBrotherModular->m_xRunningFlag == false)
        {
            m_sQFrameState.IsRunning->show();
        }
    }
}
