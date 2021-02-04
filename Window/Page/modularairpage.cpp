#include "modularairpage.h"
#include "ui_modularairpage.h"



#define LABEL_COLUMNS_1  1
#define LABEL_ROWS_1     5

#define LABEL_COLUMNS_2  1
#define LABEL_ROWS_2     2

#define LABEL_SIZE       140, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  110, 25

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  210
#define DATA_LABEL_INTERVAL_H   300
#define DATA_LABEL_INTERVAL_V   35

#define MODULAR_NUM   2

ModularAirPage::ModularAirPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularAirPage)
{
    ui->setupUi(this);

    initDevice();
    initLabel();
    initButton();
}

ModularAirPage::~ModularAirPage()
{
    delete ui;
}

void ModularAirPage::initDevice()
{
    ModularAir* m_psModularAir = nullptr;
    for(uint8_t n = 0; n < MODULAR_NUM; n++)
    {
        m_psModularAir = new ModularAir();
        m_ModularAirs.append(m_psModularAir);
        ui->modularAirStackedWidget->insertWidget(n, m_psModularAir);
    }
}

void ModularAirPage::initLabel()
{
    TextLabel *pLabel = nullptr;

    for(uint8_t n = 0; n < LABEL_ROWS_1; n++)
    {
        for(uint8_t m = 0; m < LABEL_COLUMNS_1; m++)
        {
            pLabel = new TextLabel(ui->frame);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels_1.append(pLabel);
        }
    }
    m_Labels_1[0]->setText(tr("启停命令"), LABEL_FONT_SIZE);
    m_Labels_1[1]->setText(tr("工作模式设定"), LABEL_FONT_SIZE);
    m_Labels_1[2]->setText(tr("运行状态"), LABEL_FONT_SIZE);
    m_Labels_1[3]->setText(tr("运行模式"), LABEL_FONT_SIZE);
    m_Labels_1[4]->setText(tr("控制模式"), LABEL_FONT_SIZE);

   // ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 组合柜");

    for(uint8_t n = 0; n < LABEL_ROWS_2; n++)
    {
        for(uint8_t m = 0; m < LABEL_COLUMNS_2; m++)
        {
            pLabel = new TextLabel(ui->frame_1);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels_1.append(pLabel);
        }
    }
    m_Labels_2[0]->setText(tr("启停命令"), LABEL_FONT_SIZE);
    m_Labels_2[1]->setText(tr("工作模式设定"), LABEL_FONT_SIZE);

    // ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 组合柜");
}

void ModularAirPage::initButton()
{
    //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(eButtonState::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(eButtonState::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(eButtonState::State0);
    m_Widgets_1.append(m_pSwitchCmdBtn);

    //机组运行工作模式设定
    m_pRunningModeCmdBtn = new ModeButton(ui->frame);
    m_pRunningModeCmdBtn->setItem(0,tr("供冷 "));
    m_pRunningModeCmdBtn->setItem(1,tr("通风"));
    m_pRunningModeCmdBtn->setItem(2,tr("供热"));
    m_pRunningModeCmdBtn->setItem(3,tr("负压通风"));
    m_pRunningModeCmdBtn->setDefaultValue(0);
    m_Widgets_1.append(m_pRunningModeCmdBtn);

    //目标温度设定
    m_pTempSetBtn = new AnalogValButton(ui->frame);
    m_pTempSetBtn->setDataParameter("℃", 1, 500, 0, 0, Monitor::Uint16t);
    m_Widgets_1.append(m_pTempSetBtn);

    //目标湿度设定
    m_pHumiSetBtn = new AnalogValButton(ui->frame);
    m_pHumiSetBtn->setDataParameter("%", 1, 0, 0, 0, Monitor::Uint16t);
    m_Widgets_1.append(m_pHumiSetBtn);

    //目标CO2设定
    m_pCO2SetBtn = new AnalogValButton(ui->frame);
    m_pCO2SetBtn->setDataParameter("ppm", 0, 0, 0, 0, Monitor::Uint16t);
    m_Widgets_1.append(m_pCO2SetBtn);


    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets_1.count(); i++)
    {
        m = i / LABEL_COLUMNS_1;
        n = i % LABEL_COLUMNS_1;
        m_Widgets_1[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    //实时功率
    m_pPowerLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pPowerLabel->setAlignment(Qt::AlignLeft);
    m_pPowerLabel->setDataParameter("kW", 1, Monitor::Uint16t);
    m_pPowerLabel->setBackGroundColor("#165588");
    m_pPowerLabel->setText("***", LABEL_FONT_SIZE);
    m_Widgets_2.append(m_pPowerLabel);

    //累计耗电量
    m_pTotalEnergyLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pTotalEnergyLabel->setAlignment(Qt::AlignLeft);
    m_pTotalEnergyLabel->setDataParameter("kWh", 1, Monitor::Uint16t);
    m_pTotalEnergyLabel->setBackGroundColor("#165588");
    m_pTotalEnergyLabel->setText("***", LABEL_FONT_SIZE);
    m_Widgets_2.append(m_pTotalEnergyLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets_2.count(); i++)
    {
        m = i / LABEL_COLUMNS_2;
        n = i % LABEL_COLUMNS_2;
        m_Widgets_2[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }

}
