#include "butterflyvalve.h"
#include "ui_butterflyvalve.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     7

#define LABEL_SIZE       140, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  100, 25

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  180
#define DATA_LABEL_INTERVAL_H   220
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
    m_Labels[0]->setText(tr("启停命令"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("远程/本地"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("开到位"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("关到位"), LABEL_FONT_SIZE);
    m_Labels[4]->setText(tr("设备故障"), LABEL_FONT_SIZE);

    ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 冷冻蝶阀");
}

void ButterflyValve::initButton()
{
    //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(eButtonState::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(eButtonState::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(eButtonState::State0);
    m_Widgets.append(m_pSwitchCmdBtn);

    //远程/本地
    m_pRemoteLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pRemoteLabel->setAlignment(Qt::AlignLeft);
    m_pRemoteLabel->setValueMap(0,tr("本地"));
    m_pRemoteLabel->setValueMap(1,tr("远程"));
    m_pRemoteLabel->setBackGroundColor("#165588");
    m_pRemoteLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pRemoteLabel);

    //开到位
    m_xOpenedLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_xOpenedLabel->setAlignment(Qt::AlignLeft);
    m_xOpenedLabel->setValueMap(0,tr("未开到位"));
    m_xOpenedLabel->setValueMap(1,tr("开到位"));
    m_xOpenedLabel->setBackGroundColor("#165588");
    m_xOpenedLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_xOpenedLabel);

    //关到位
    m_xOpenedLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_xOpenedLabel->setAlignment(Qt::AlignLeft);
    m_xOpenedLabel->setValueMap(0,tr("未关到位"));
    m_xOpenedLabel->setValueMap(1,tr("关到位"));
    m_xOpenedLabel->setBackGroundColor("#165588");
    m_xOpenedLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_xOpenedLabel);

    //设备故障
    m_pErrorFlagLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pErrorFlagLabel->setAlignment(Qt::AlignLeft);
    m_pErrorFlagLabel->setValueMap(0,tr("正常"));
    m_pErrorFlagLabel->setValueMap(1,tr("故障"));
    m_pErrorFlagLabel->setBackGroundColor("#165588");
    m_pErrorFlagLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pErrorFlagLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / 1;
        n = i % 1;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
}