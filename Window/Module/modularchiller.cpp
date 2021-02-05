#include "modularchiller.h"
#include "ui_modularchiller.h"
#include <QMessageBox>
#include "messagebox.h"

#define AB_PRESS_PIX ":UI/baseFile/abPress.png"
#define AB_RELEASE_PIX ":UI/baseFile/abRelease.png"

#define SBUTTON_STATE0 ":UI/baseFile/sbLeft.png"
#define SBUTTON_STATE1 ":UI/baseFile/sbRight.png"

#define MB_PIX2 ":UI/baseFile/mbPix2.png"

#define MODULAR_NUM_IN_CHILLER  2

#define MODULAR_UP_MARGIN     30
#define MODULAR_LEFT_MARGIN   15


#define LABEL_COLUMNS  2
#define LABEL_ROWS     2

#define LABEL_SIZE       140, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   80
#define LABEL_INTERVAL_H    260
#define LABEL_INTERVAL_V    40

#define DATA_LABEL_SIZE  110, 25

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  210
#define DATA_LABEL_INTERVAL_H   260
#define DATA_LABEL_INTERVAL_V   40

uint8_t ModularChiller::m_usModularChillerCount = 0;

ModularChiller::ModularChiller(QWidget *parent) :
    Device(parent),
    ui(new Ui::ModularChiller)
{
    ui->setupUi(this);

    ModularChiller::m_usModularChillerCount++;
    this->m_usDeviceIndex = m_usModularChillerCount;

    Modular *psModular = nullptr;
    for( uint8_t i = 0; i < MODULAR_NUM_IN_CHILLER; i++)
    {
        psModular = new Modular(ui->frame);
        psModular->setGeometry(MODULAR_LEFT_MARGIN + i * psModular->width(), ui->frame_2->height(),
                               psModular->width(), psModular->height());
        m_Modulars.append(psModular);
    }
    initLabel();
    initButton();
}

void ModularChiller::initLabel()
{
    TextLabel *pLabel = nullptr;

    for(uint8_t n = 0; n < LABEL_ROWS; n++)
    {
        for(uint8_t m = 0; m < LABEL_COLUMNS; m++)
        {
            pLabel = new TextLabel(ui->frame_2);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels.append(pLabel);
        }
    }
    m_Labels[0]->setText(tr("启停命令"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("工作模式设定"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("平均回水温度"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("通讯故障"), LABEL_FONT_SIZE);

    ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 机组");
    //ui->label->setGeometry(128, 15, ui->label->width(), ui->label->height());
}

void ModularChiller::initButton()
{
    //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame_2);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets.append(m_pSwitchCmdBtn);

    //机组运行工作模式设定
    m_pRunningModeCmdBtn = new ModeButton(ui->frame_2);
    m_pRunningModeCmdBtn->setItem(0,tr("无  "));
    m_pRunningModeCmdBtn->setItem(1,tr("类型一"));
    m_pRunningModeCmdBtn->setItem(2,tr("类型二"));
    m_pRunningModeCmdBtn->setItem(3,tr("类型三"));
    m_pRunningModeCmdBtn->setItem(4,tr("类型四"));
    m_pRunningModeCmdBtn->setDefaultValue(0);
    m_Widgets.append(m_pRunningModeCmdBtn);

    //平均回水温度
    m_pRetWaterTempLabel = new DataLabel(ui->frame_2, DataLabel::Text);
    m_pRetWaterTempLabel->setAlignment(Qt::AlignLeft);
    m_pRetWaterTempLabel->setDataParameter("℃", 1, Monitor::Uint16t);
    m_pRetWaterTempLabel->setBackGroundColor("#165588");
    m_pRetWaterTempLabel->setText("***尔尔", LABEL_FONT_SIZE);
    m_Widgets.append(m_pRetWaterTempLabel);

    //通讯故障
    m_pCommErrLabel = new DataLabel(ui->frame_2, DataLabel::Text);
    m_pCommErrLabel->setAlignment(Qt::AlignLeft);
    m_pCommErrLabel->setValueMap(0,tr("正常"));
    m_pCommErrLabel->setValueMap(1,tr("故障"));
    m_pCommErrLabel->setBackGroundColor("#165588");
    m_pCommErrLabel->setText("#165588", LABEL_FONT_SIZE);
    m_Widgets.append(m_pCommErrLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / 2;
        n = i % 2;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
}


ModularChiller::~ModularChiller()
{
    delete ui;
}
