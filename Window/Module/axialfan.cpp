#include "axialfan.h"
#include "ui_axialfan.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     7

#define LABEL_SIZE       140, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     20
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  100, 25

#define DATA_LABEL_UP_MARGIN    20
#define DATA_LABEL_LEFT_MARGIN  180
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   35

uint8_t AxialFan::m_usAxialFanCount = 0;

AxialFan::AxialFan(QWidget *parent) :
    Device(parent),
    ui(new Ui::AxialFan)
{
    ui->setupUi(this);

    AxialFan::m_usAxialFanCount++;
    this->m_usDeviceIndex = AxialFan::m_usAxialFanCount;

    initLabel();
    initButton();
}

AxialFan::~AxialFan()
{
    delete ui;
}

void AxialFan::initLabel()
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
    m_Labels[2]->setText(tr("频率反馈"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("远程/本地"), LABEL_FONT_SIZE);
    m_Labels[4]->setText(tr("运行标志"), LABEL_FONT_SIZE);
    m_Labels[5]->setText(tr("设备故障"), LABEL_FONT_SIZE);
    m_Labels[6]->setText(tr("控制故障"), LABEL_FONT_SIZE);

    ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 畜牧风机");
}

void AxialFan::initButton()
{
    //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets.append(m_pSwitchCmdBtn);

    //频率设置
    m_pFreqSetBtn = new AnalogValButton(ui->frame);
    m_pFreqSetBtn->setDataParameter("Hz", 1, 500, 0, 0, Monitor::Uint16t);
    m_Widgets.append(m_pFreqSetBtn);

    //频率反馈
    m_pFreqLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pFreqLabel->setAlignment(Qt::AlignLeft);
    m_pFreqLabel->setDataParameter("Hz", 1, Monitor::Uint16t);
    m_pFreqLabel->setBackGroundColor("#165588");
    m_pFreqLabel->setText("***", LABEL_FONT_SIZE);
    m_Widgets.append(m_pFreqLabel);

    //远程/本地
    m_pRemoteLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pRemoteLabel->setAlignment(Qt::AlignLeft);
    m_pRemoteLabel->setValueMap(0,tr("本地"));
    m_pRemoteLabel->setValueMap(1,tr("远程"));
    m_pRemoteLabel->setBackGroundColor("#165588");
    m_pRemoteLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pRemoteLabel);

    //运行标志
    m_pRemoteLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pRemoteLabel->setAlignment(Qt::AlignLeft);
    m_pRemoteLabel->setValueMap(0,tr("停止"));
    m_pRemoteLabel->setValueMap(1,tr("运行"));
    m_pRemoteLabel->setBackGroundColor("#165588");
    m_pRemoteLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pRemoteLabel);

    //设备故障
    m_pErrorFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pErrorFlagLabel->setAlignment(Qt::AlignLeft);
    m_pErrorFlagLabel->setValueMap(0,tr("正常"));
    m_pErrorFlagLabel->setValueMap(1,tr("故障"));
    m_pErrorFlagLabel->setBackGroundColor("#165588");
    m_pErrorFlagLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pErrorFlagLabel);

    //控制故障
    m_pControlFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pControlFlagLabel->setAlignment(Qt::AlignLeft);
    m_pControlFlagLabel->setValueMap(0,tr("正常"));
    m_pControlFlagLabel->setValueMap(1,tr("故障"));
    m_pControlFlagLabel->setBackGroundColor("#165588");
    m_pControlFlagLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pControlFlagLabel);



    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / 1;
        n = i % 1;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
}
