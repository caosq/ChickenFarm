#include "modularchiller.h"
#include "ui_modularchiller.h"
#include <QMessageBox>
#include "messagebox.h"
#include "system.h"

#define AB_PRESS_PIX ":UI/baseFile/abPress.png"
#define AB_RELEASE_PIX ":UI/baseFile/abRelease.png"

#define SBUTTON_STATE0 ":UI/baseFile/sbLeft.png"
#define SBUTTON_STATE1 ":UI/baseFile/sbRight.png"

#define MB_PIX2 ":UI/baseFile/mbPix2.png"

#define MODULAR_UP_MARGIN     30
#define MODULAR_LEFT_MARGIN   5

#define LABEL_COLUMNS  2
#define LABEL_ROWS     4

#define LABEL_SIZE       115, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     20
#define LABEL_LEFT_MARGIN   60
#define LABEL_INTERVAL_H    240
#define LABEL_INTERVAL_V    40

#define DATA_LABEL_SIZE  110, 28

#define DATA_LABEL_UP_MARGIN    20
#define DATA_LABEL_LEFT_MARGIN  160
#define DATA_LABEL_INTERVAL_H   280
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
    QFrame  *pFrame =nullptr;

    if(this->m_usDeviceIndex <= 1) //1#机组3个模块，2#机组有4个模块
    {
        m_usModularNum = 3;
    }
    else
    {
        m_usModularNum =  MODULAR_NUM_IN_CHILLER;
    }
    for( uint8_t i = 0, m = 0; i < m_usModularNum; i++)
    {
        m = i % 2;
        if(m == 0)
        {
            pFrame = new QFrame(ui->stackedWidget);
            ui->stackedWidget->addWidget(pFrame);
        }
        psModular = new Modular(pFrame, i+1);
        psModular->setGeometry(MODULAR_LEFT_MARGIN +  m * psModular->width(), 0,
                               psModular->width(), psModular->height());
        m_Modulars.append(psModular);
    }
    initLabel();
    initButton();
}

ModularChiller::~ModularChiller()
{
    delete ui;
}

void ModularChiller::initLabel()
{
    TextLabel *pLabel = nullptr;
    for(uint8_t m = 0; m < LABEL_COLUMNS; m++)
    {
        for(uint8_t n = 0; n < LABEL_ROWS; n++)
        {
            pLabel = new TextLabel(ui->frame_2);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels.append(pLabel);
        }
    }
    m_Labels[0]->setText(tr("启停命令"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("工作模式"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("回水温度"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("通讯故障"), LABEL_FONT_SIZE);
    m_Labels[4]->setText(tr("制冷进水温度"), LABEL_FONT_SIZE);
    m_Labels[5]->setText(tr("制冷出水温度"), LABEL_FONT_SIZE);
    m_Labels[6]->setText(tr("制热进水温度"), LABEL_FONT_SIZE);
    m_Labels[7]->setText(tr("制热出水温度"), LABEL_FONT_SIZE);

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
    m_pSwitchCmdBtn->setMonitorData(&m_eSwitchCmd, Monitor::Uint16t);
    m_Widgets.append(m_pSwitchCmdBtn);

    //机组运行工作模式设定
    m_pRunningModeCmdBtn = new ModeButton(ui->frame_2);
    m_pRunningModeCmdBtn->setItem(1,tr("制冷"));
    m_pRunningModeCmdBtn->setItem(2,tr("制热"));
    m_pRunningModeCmdBtn->setItem(3,tr("手动化霜"));
    m_pRunningModeCmdBtn->setDefaultValue(1);
    m_pRunningModeCmdBtn->setMonitorData(&m_eRunningModeCmd, Monitor::Uint16t);
    m_Widgets.append(m_pRunningModeCmdBtn);

    //平均回水温度
    m_pRetWaterTempLabel = new DataLabel(ui->frame_2, DataLabel::Data);
    m_pRetWaterTempLabel->setAlignment(Qt::AlignLeft);
    m_pRetWaterTempLabel->setDataParameter("℃", 1, Monitor::Uint16t);
    m_pRetWaterTempLabel->setMonitorData(&m_sRetWaterTemp, Monitor::Int16t);
    m_Widgets.append(m_pRetWaterTempLabel);

    //通讯故障
    m_pCommErrLabel = new DataLabel(ui->frame_2, DataLabel::Text);
    m_pCommErrLabel->setAlignment(Qt::AlignLeft);
    m_pCommErrLabel->setValueMap(0,tr("正常"));
    m_pCommErrLabel->setValueMap(1,tr("故障"));
    m_pCommErrLabel->setMonitorData(&m_xCommErr, Monitor::Boolean);
    m_Widgets.append(m_pCommErrLabel);

    //机组制冷进水温度设定值
    m_pChillerCoolInTempBtn = new AnalogValButton(ui->frame_2);
    m_pChillerCoolInTempBtn->setDataParameter("℃", 1, 120, 250, 100, Monitor::Uint16t);
    m_pChillerCoolInTempBtn->setMonitorData(&m_usChillerCoolInTemp, Monitor::Uint16t);
    m_Widgets.append(m_pChillerCoolInTempBtn);

    //机组制冷出水温度设定值
    m_pChillerCoolOutTempBtn = new AnalogValButton(ui->frame_2);
    m_pChillerCoolOutTempBtn->setDataParameter("℃", 1, 70, 200, 50, Monitor::Uint16t);
    m_pChillerCoolOutTempBtn->setMonitorData(&m_usChillerCoolOutTemp, Monitor::Uint16t);
    m_Widgets.append(m_pChillerCoolOutTempBtn);

    //机组制热进水温度设定值
    m_pChillerHeatInTempBtn = new AnalogValButton(ui->frame_2);
    m_pChillerHeatInTempBtn->setDataParameter("℃", 1, 450, 450, 300, Monitor::Uint16t);
    m_pChillerHeatInTempBtn->setMonitorData(&m_usChillerHeatInTemp, Monitor::Uint16t);
    m_Widgets.append(m_pChillerHeatInTempBtn);

    //机组制热出水温度设定值
    m_pChillerHeatOutTempBtn = new AnalogValButton(ui->frame_2);
    m_pChillerHeatOutTempBtn->setDataParameter("℃", 1, 500, 500, 350, Monitor::Uint16t);
    m_pChillerHeatOutTempBtn->setMonitorData(&m_usChillerHeatOutTemp, Monitor::Uint16t);
    m_Widgets.append(m_pChillerHeatOutTempBtn);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_ROWS;
        n = i % LABEL_ROWS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + m * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + n * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    connect(System::getInstance(), SIGNAL(sysModeCmdChanged()), this, SLOT(sysModeCmdChangedSlot()));
    sysModeCmdChangedSlot();
}

void ModularChiller::sysModeCmdChangedSlot()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}
    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_MANUAL)
    {
        m_pSwitchCmdBtn->setEnabled(true);
        m_pRunningModeCmdBtn->setEnabled(true);
    }
    else
    {
        m_pSwitchCmdBtn->setEnabled(false);
        m_pRunningModeCmdBtn->setEnabled(false);
    }
}

void ModularChiller::on_pushButton_clicked()
{
    if(ui->stackedWidget->currentIndex() == 0)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->pushButton->setText("上一页");

    }
    else if(ui->stackedWidget->currentIndex() == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->pushButton->setText("下一页");
    }
}
