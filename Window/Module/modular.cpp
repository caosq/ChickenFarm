#include "modular.h"
#include "ui_modular.h"
#include "datamonitor.h"
#include "textlabel.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     8

#define LABEL_SIZE       120, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   20
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  100, 25

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  160
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   35

Compressor::Compressor(QWidget *parent) :
    QWidget(parent)
{
    Monitor *m_pMonitor = DataMonitor::monitorRegist(&m_xRunningFlag, Monitor::Boolean);

    if(m_pMonitor != nullptr)
    {
        connect(m_pMonitor, SIGNAL(valChanged(Monitor*)), this, SLOT(valChangedSlot(Monitor*)));
    }
}

void Compressor::valChangedSlot(Monitor* pMonitor)
{
    emit stateChanged(pMonitor->getCurVal());
}

uint8_t Modular::m_usModularCount = 0;

Modular::Modular(QWidget *parent, uint8_t usCompNum) :
    Device(parent),
    ui(new Ui::Modular)
{
    ui->setupUi(this);
    m_usCompNum = usCompNum;

    Modular::m_usModularCount++;
    this->m_usDeviceIndex = m_usModularCount;

    Compressor *pComp = nullptr;
    for( uint8_t i = 0; i < usCompNum; i++)
    {
        pComp = new Compressor(this);
        if(pComp != nullptr)
        {
            connect(pComp, SIGNAL(stateChanged(bool)), this, SLOT(compStateChangedSlot(bool)));
            m_Comps.append(pComp);
        }
    }
    initLabel();
    initButton();
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
    m_Labels[5]->setText(tr("总报警标志"), LABEL_FONT_SIZE);
    m_Labels[6]->setText(tr("总故障标志"), LABEL_FONT_SIZE);
    m_Labels[7]->setText(tr("压缩机运行数"), LABEL_FONT_SIZE);

    ui->label->setText("模块 " + QString::number(this->m_usDeviceIndex));
    ui->label->setGeometry(128, 15, ui->label->width(), ui->label->height());
}

void Modular::initButton()
{
    //机组状态
    m_pModularStateLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pModularStateLabel->setAlignment(Qt::AlignLeft);
    m_pModularStateLabel->setValueMap(0,tr("已关闭"));
    m_pModularStateLabel->setValueMap(1,tr("开机中"));
    m_pModularStateLabel->setValueMap(0,tr("关机中"));
    m_pModularStateLabel->setValueMap(1,tr("运行中"));
    m_pModularStateLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pModularStateLabel);


    //进水温度
    m_pInputWaterTempLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pInputWaterTempLabel->setAlignment(Qt::AlignLeft);
    m_pInputWaterTempLabel->setDataParameter("℃", 1, Monitor::Uint16t);
    m_pInputWaterTempLabel->setText("***", LABEL_FONT_SIZE);
    m_Widgets.append(m_pInputWaterTempLabel);

    //出水温度
    m_pOutputWaterTempLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pOutputWaterTempLabel->setAlignment(Qt::AlignLeft);
    m_pOutputWaterTempLabel->setDataParameter("℃", 1, Monitor::Uint16t);
    m_pOutputWaterTempLabel->setText("***", LABEL_FONT_SIZE);
    m_Widgets.append(m_pOutputWaterTempLabel);

    //运行标志
    m_pRunningFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pRunningFlagLabel->setAlignment(Qt::AlignLeft);
    m_pRunningFlagLabel->setValueMap(0,tr("关闭"));
    m_pRunningFlagLabel->setValueMap(1,tr("运行"));
    m_pRunningFlagLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pRunningFlagLabel);

    //水流开关
    m_pWaterValveLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pWaterValveLabel->setAlignment(Qt::AlignLeft);
    m_pWaterValveLabel->setValueMap(0,tr("关闭"));
    m_pWaterValveLabel->setValueMap(1,tr("运行"));
    m_pWaterValveLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pWaterValveLabel);

    //总故障标志
    m_pErrorFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pErrorFlagLabel->setAlignment(Qt::AlignLeft);
    m_pErrorFlagLabel->setValueMap(0,tr("关闭"));
    m_pErrorFlagLabel->setValueMap(1,tr("运行"));
    m_pErrorFlagLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pErrorFlagLabel);

    //总报警标志
    m_pAlarmFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pAlarmFlagLabel->setAlignment(Qt::AlignLeft);
    m_pAlarmFlagLabel->setValueMap(0,tr("关闭"));
    m_pAlarmFlagLabel->setValueMap(1,tr("运行"));
    m_pAlarmFlagLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pAlarmFlagLabel);

    //运行压缩机数量
    m_pRunnningCompCountLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pRunnningCompCountLabel->setAlignment(Qt::AlignLeft);
    m_pRunnningCompCountLabel->setDataParameter("个", 1, Monitor::Uint16t);
    m_pRunnningCompCountLabel->setText("***", LABEL_FONT_SIZE);
    m_Widgets.append(m_pRunnningCompCountLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
}

void Modular::compStateChangedSlot(bool xRunningFlag)
{
    if(xRunningFlag)
    {
        m_RunnningCompCount++;
    }
    else if(m_RunnningCompCount > 0)
    {
        m_RunnningCompCount--;
    }
}

Modular::~Modular()
{
    delete ui;
}
