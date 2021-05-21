#include "bypassvalve.h"
#include "ui_bypassvalve.h"
#include "system.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     4

#define LABEL_SIZE       120, 30
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   20
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    40

#define DATA_LABEL_SIZE  110, 30

#define DATA_LABEL_UP_MARGIN    32
#define DATA_LABEL_LEFT_MARGIN  120
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   40

BypassValve::BypassValve(QWidget *parent) :
    Device(parent),
    ui(new Ui::BypassValve)
{
    ui->setupUi(this);

    initLabel();
    initButton();
}

BypassValve::~BypassValve()
{
    delete ui;
}

void BypassValve::initLabel()
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
    m_Labels[0]->setText(tr("控制模式"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("开度设置"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("开度反馈"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("设备故障"), LABEL_FONT_SIZE);

    //ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 小窗电机");
}

void BypassValve::initButton()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    //模式设定
    m_pControlModeCmdBtn = new ModeButton(ui->frame);
    m_pControlModeCmdBtn->setItem(0,tr("手动"));
    m_pControlModeCmdBtn->setItem(1,tr("自动"));
    m_pControlModeCmdBtn->setDefaultValue(0);
    m_pControlModeCmdBtn->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_pControlModeCmdBtn->setMonitorData(&m_xControlMode, Monitor::Boolean);
    m_Widgets.append(m_pControlModeCmdBtn);

    //开度设置
    m_pAngSetBtn = new AnalogValButton(ui->frame);
    m_pAngSetBtn->setDataParameter("%", 0, 0, 100, 0, Monitor::Uint16t);
    m_pAngSetBtn->setMonitorData(&m_usAngSet, Monitor::Uint16t);
    m_pAngSetBtn->setCheckMode(&pSystem->m_xIsLogIn, 1, "请先登录后再操作", Monitor::Boolean);
    m_pAngSetBtn->setMaxValMonitor(&pSystem->m_usCHWBypassMaxAng, Monitor::Uint16t);
    m_pAngSetBtn->setMinValMonitor(&pSystem->m_usCHWBypassMinAng, Monitor::Uint16t);
    m_Widgets.append(m_pAngSetBtn);

    //开度反馈
    m_pAngLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pAngLabel->setAlignment(Qt::AlignLeft);
    m_pAngLabel->setDataParameter("%", 0, Monitor::Uint16t);
    m_pAngLabel->setMonitorData(&m_usAng, Monitor::Uint16t);
    m_Widgets.append(m_pAngLabel);

    //设备故障
    m_pErrLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pErrLabel->setAlignment(Qt::AlignLeft);
    m_pErrLabel->setValueMap(0,tr("正常"));
    m_pErrLabel->setValueMap(1,tr("故障"), Qt::red);
    m_pErrLabel->setMonitorData(&m_xErrorFlag, Monitor::Boolean);
    m_Widgets.append(m_pErrLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    connect(m_pControlModeCmdBtn, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
    connect(m_pAngLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
    connect(m_pErrLabel, SIGNAL(valChanged(void*)), this, SLOT(stateChangedSlot(void*)));
}

void BypassValve::paramSetBtnValChanged(void*)
{
    if(m_xControlMode)
    {
        m_pAngSetBtn->setEnabled(false);
    }
    else
    {
        m_pAngSetBtn->setEnabled(true);
    }
}

void BypassValve::stateChangedSlot(void*)
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
        if(m_usAng == 0)
        {
            m_sQFrameState.IsRunning->show();
        }
        else
        {
             m_sQFrameState.IsRunning->hide();
        }
    }
}
