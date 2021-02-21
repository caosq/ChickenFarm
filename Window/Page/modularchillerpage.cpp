#include "modularchillerpage.h"
#include "ui_modularchillerpage.h"
#include "system.h"
#include "stdio.h"

#define LABEL_COLUMNS_1  1
#define LABEL_ROWS_1     6

#define LABEL_COLUMNS_2  1
#define LABEL_ROWS_2     3

#define LABEL_SIZE       120, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN_1     30
#define LABEL_LEFT_MARGIN_1   35
#define LABEL_INTERVAL_H_1    300
#define LABEL_INTERVAL_V_1    40

#define LABEL_UP_MARGIN_2     60
#define LABEL_LEFT_MARGIN_2   30
#define LABEL_INTERVAL_H_2    300
#define LABEL_INTERVAL_V_2    40

#define DATA_LABEL_SIZE  110, 28

#define DATA_LABEL_UP_MARGIN_1    30
#define DATA_LABEL_LEFT_MARGIN_1  130
#define DATA_LABEL_INTERVAL_H_1   300
#define DATA_LABEL_INTERVAL_V_1   40

#define DATA_LABEL_UP_MARGIN_2    65
#define DATA_LABEL_LEFT_MARGIN_2  150
#define DATA_LABEL_INTERVAL_H_2   300
#define DATA_LABEL_INTERVAL_V_2   38

#define MODULAR_CHILLER_NUM   2


ModularChillerPage::ModularChillerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularChillerPage)
{
    ui->setupUi(this);

    initDevice();
    initLabel();
    initButton();
}

ModularChillerPage::~ModularChillerPage()
{
    delete ui;
}

void ModularChillerPage::initDevice()
{
    ModularChiller* pModularChiller = nullptr;
    System   *pSystem = System::getInstance();

    if(pSystem == nullptr)
    {
        return;
    }
    for(uint8_t n = 0; n < MODULAR_CHILLER_NUM; n++)
    {
     //   pModularChiller = new ModularChiller();
        pSystem->m_pModularChillers[n] =  &m_ModularChillers[n];

       // m_ModularChillers.append(pModularChiller);
        ui->modularChillerStackedWidget->addWidget(&m_ModularChillers[n]);

        //pModularChiller->setGeometry(296, 20, 611, 545);
    }
    //ui->modularChillerStackedWidget->setCurrentWidget(&m_ModularChillers[0]);

    // ui->modularChillerStackedWidget->addWidget(&m_ModularChillers[0]);

    m_ModularChillers[0].setParent(ui->modularChillerStackedWidget);
    m_ModularChillers[1].setParent(ui->modularChillerStackedWidget);

}

void ModularChillerPage::initLabel()
{
    TextLabel *pLabel = nullptr;

    for(uint8_t n = 0; n < LABEL_ROWS_1; n++)
    {
        for(uint8_t m = 0; m < LABEL_COLUMNS_1; m++)
        {
            pLabel = new TextLabel(ui->frame);
            pLabel->setGeometry( LABEL_LEFT_MARGIN_1 + m * LABEL_INTERVAL_H_1,
                                 LABEL_UP_MARGIN_1 + n * LABEL_INTERVAL_V_1,
                                 LABEL_SIZE);
            m_Labels_1.append(pLabel);
        }
    }
    m_Labels_1[0]->setText(tr("启停命令"), LABEL_FONT_SIZE);
    m_Labels_1[1]->setText(tr("工作模式"), LABEL_FONT_SIZE);
   // m_Labels_1[2]->setText(tr("目标温度"), LABEL_FONT_SIZE);
   // m_Labels_1[3]->setText(tr("目标湿度"), LABEL_FONT_SIZE);
   // m_Labels_1[4]->setText(tr("目标CO2"), LABEL_FONT_SIZE);

    for(uint8_t n = 0; n < LABEL_ROWS_2; n++)
    {
        for(uint8_t m = 0; m < LABEL_COLUMNS_2; m++)
        {
            pLabel = new TextLabel(ui->frame_1);
            pLabel->setGeometry( LABEL_LEFT_MARGIN_2 + m * LABEL_INTERVAL_H_2,
                                 LABEL_UP_MARGIN_2 + n * LABEL_INTERVAL_V_2,
                                 LABEL_SIZE);
            m_Labels_2.append(pLabel);
        }
    }
    m_Labels_2[0]->setText(tr("耗电功率"), LABEL_FONT_SIZE);
    m_Labels_2[1]->setText(tr("累计耗电量"), LABEL_FONT_SIZE);
    m_Labels_2[2]->setText(tr("通讯故障"), LABEL_FONT_SIZE);
}

void ModularChillerPage::initButton()
{
    //启停命令
    m_pSwitchCmdBtn = new StateButton(ui->frame);
    m_pSwitchCmdBtn->setStateText(StateButton::State0,tr("关闭"));
    m_pSwitchCmdBtn->setStateText(StateButton::State1,tr("开启"));
    m_pSwitchCmdBtn->setDeafultState(StateButton::State0);
    m_Widgets_1.append(m_pSwitchCmdBtn);

    //机组运行工作模式设定
    m_pRunningModeCmdBtn = new ModeButton(ui->frame);
    m_pRunningModeCmdBtn->setItem(1,tr("制冷"));
    m_pRunningModeCmdBtn->setItem(2,tr("制热"));
    m_pRunningModeCmdBtn->setItem(3,tr("手动化霜"));
    m_pRunningModeCmdBtn->setDefaultValue(0);
    m_Widgets_1.append(m_pRunningModeCmdBtn);

/*    //目标温度设定
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
*/

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets_1.count(); i++)
    {
        m = i / LABEL_COLUMNS_1;
        n = i % LABEL_COLUMNS_1;
        m_Widgets_1[i]->setGeometry(DATA_LABEL_LEFT_MARGIN_1 + n * DATA_LABEL_INTERVAL_H_1,
                                  DATA_LABEL_UP_MARGIN_1 + m * DATA_LABEL_INTERVAL_V_1,
                                  DATA_LABEL_SIZE);
    }
    //实时功率
    m_pPowerLabel = new DataLabel(ui->frame_1, DataLabel::Data);
    m_pPowerLabel->setAlignment(Qt::AlignLeft);
    m_pPowerLabel->setDataParameter("kW", 1, Monitor::Uint16t);
    m_pPowerLabel->setMonitorData(&m_sMeter.m_usPower, Monitor::Uint16t);
    m_Widgets_2.append(m_pPowerLabel);

    qDebug("m_pPowerLabel %d %s", m_sMeter.m_usPower, m_pPowerLabel->text().toUtf8().data());

    //累计耗电量
    m_pTotalEnergyLabel = new DataLabel(ui->frame_1, DataLabel::Data);
    m_pTotalEnergyLabel->setAlignment(Qt::AlignLeft);
    m_pTotalEnergyLabel->setDataParameter("kWh", 0, Monitor::Uint32t);
    m_pTotalEnergyLabel->setMonitorData(&m_sMeter.m_ulTotalEnergy, Monitor::Uint32t);
    m_Widgets_2.append(m_pTotalEnergyLabel);

    m_pCommErrLabel = new DataLabel(ui->frame_1, DataLabel::Text);
    m_pCommErrLabel->setAlignment(Qt::AlignLeft);
    m_pCommErrLabel->setValueMap(0,tr("正常"));
    m_pCommErrLabel->setValueMap(1,tr("故障"));
    m_pCommErrLabel->setMonitorData(&m_sMeter.m_xCommErr, Monitor::Boolean);
    m_Widgets_2.append(m_pCommErrLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets_2.count(); i++)
    {
        m = i / LABEL_COLUMNS_2;
        n = i % LABEL_COLUMNS_2;
        m_Widgets_2[i]->setGeometry(DATA_LABEL_LEFT_MARGIN_2 + n * DATA_LABEL_INTERVAL_H_2,
                                  DATA_LABEL_UP_MARGIN_2 + m * DATA_LABEL_INTERVAL_V_2,
                                  DATA_LABEL_SIZE);
    }
}

void ModularChillerPage::on_pushButton_clicked()
{
    if(ui->modularChillerStackedWidget->currentWidget() == &m_ModularChillers[0])
    {
        ui->modularChillerStackedWidget->setCurrentWidget(&m_ModularChillers[1]);
        ui->pushButton->setText("上一页");

    }
    else if(ui->modularChillerStackedWidget->currentWidget() == &m_ModularChillers[1])
    {
        ui->modularChillerStackedWidget->setCurrentWidget(&m_ModularChillers[0]);
        ui->pushButton->setText("下一页");
    }
   // qDebug("currentIndex %d", ui->modularChillerStackedWidget->currentIndex());

 /*   if(m_usCurrentIndex == 0)
    {
        m_usCurrentIndex = 1;
//        m_ModularChillers[0]->hide();
//        m_ModularChillers[1]->show();
        ui->pushButton->setText("上一页");

        ui->modularChillerStackedWidget->setCurrentIndex(1);
    }
    else if(m_usCurrentIndex == 1)
    {
        m_usCurrentIndex = 0;
//        m_ModularChillers[0]->show();
//        m_ModularChillers[1]->hide();
        ui->pushButton->setText("下一页");

        ui->modularChillerStackedWidget->setCurrentIndex(0);
    }
*/

}
