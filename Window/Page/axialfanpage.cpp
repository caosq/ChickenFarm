#include "axialfanpage.h"
#include "ui_axialfanpage.h"


#define LABEL_COLUMNS  1
#define LABEL_ROWS     6

#define LABEL_SIZE       110, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    150
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  80, 25

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  150
#define DATA_LABEL_INTERVAL_H   300
#define DATA_LABEL_INTERVAL_V   35

#define AXIAL_FAN_NUM  4
#define WINDOW_FAN_NUM  2

AxialFanPage::AxialFanPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AxialFanPage)
{
    ui->setupUi(this);

    initDevice();
    initLabel();
    initButton();
}

AxialFanPage::~AxialFanPage()
{
    delete ui;
}

void AxialFanPage::initDevice()
{
    AxialFan* pAxialFan = nullptr;
    for(uint8_t n = 0, x = 0, y = 0; n < AXIAL_FAN_NUM; n++)
    {
        x = n / 2;
        y = n % 2;

        pAxialFan = new AxialFan();
        m_AxialFans.append(pAxialFan);
        ui->gridLayout->addWidget(pAxialFan, x, y);
    }

    WindowFan *m_psWindowFan = nullptr;
    for(uint8_t n = 0; n < WINDOW_FAN_NUM; n++)
    {
        m_psWindowFan = new WindowFan();
        m_WindowFans.append(m_psWindowFan);
        ui->verticalLayout->insertWidget(n, m_psWindowFan);
    }
}

void AxialFanPage::initLabel()
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
    m_Labels[2]->setText(tr("故障清除"), LABEL_FONT_SIZE);

    //ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 冷冻水泵");
}

void AxialFanPage::initButton()
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
}
