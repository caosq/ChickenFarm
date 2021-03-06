#include "tempsensor.h"
#include "ui_tempsensor.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     1

#define LABEL_SIZE       140, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     25
#define LABEL_LEFT_MARGIN   35
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  100, 25

#define DATA_LABEL_UP_MARGIN    15
#define DATA_LABEL_LEFT_MARGIN  110
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   35

uint8_t TempSensor::m_usSensorCount = 0;

TempSensor::TempSensor(QWidget *parent) :
    Device(parent),
    ui(new Ui::TempSensor)
{
    ui->setupUi(this);

    TempSensor::m_usSensorCount++;
    this->m_usDeviceIndex = TempSensor::m_usSensorCount;

    initLabel();
    initButton();
}

TempSensor::~TempSensor()
{
    delete ui;
}

void TempSensor::initLabel()
{
    ui->label->setText("CHW-ST-"+ QString::number(this->m_usDeviceIndex));
}

void TempSensor::initButton()
{
    //温度
    m_pTempLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pTempLabel->setAlignment(Qt::AlignLeft);
    m_pTempLabel->setDataParameter("℃", 1, Monitor::Int16t);
    m_pTempLabel->setMonitorData(&m_sTemp, Monitor::Int16t);
    m_Widgets.append(m_pTempLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    ui->frame_2->hide();

    m_pErrMonitor = DataMonitor::monitorRegist(&m_xError, Monitor::DataType::Boolean);
    connect(m_pErrMonitor, SIGNAL(valChanged(Monitor*)),this,SLOT(valChangedSlot(Monitor*)));
}

void TempSensor::valChangedSlot(Monitor* pMonitor)
{
    int32_t val = pMonitor->getCurVal();
    if(val==0)
    {
        m_pTempLabel->show();
        ui->frame_2->hide();
    }
    else
    {
        m_pTempLabel->hide();
        ui->frame_2->show();
    }
}
