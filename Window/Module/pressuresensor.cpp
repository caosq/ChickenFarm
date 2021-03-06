#include "pressuresensor.h"
#include "ui_pressuresensor.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     1

#define LABEL_SIZE       140, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     25
#define LABEL_LEFT_MARGIN   35
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  90, 25

#define DATA_LABEL_UP_MARGIN    10
#define DATA_LABEL_LEFT_MARGIN  100
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   35

uint8_t PressureSensor::m_usSensorCount = 0;

PressureSensor::PressureSensor(QWidget *parent) :
    Device(parent),
    ui(new Ui::PressureSensor)
{
    ui->setupUi(this);

    PressureSensor::m_usSensorCount++;
    this->m_usDeviceIndex = PressureSensor::m_usSensorCount;

    initLabel();
    initButton();
}

PressureSensor::~PressureSensor()
{
    delete ui;
}

void PressureSensor::initLabel()
{
    ui->label->setText("CHW-SP-"+ QString::number(this->m_usDeviceIndex));
}

void PressureSensor::initButton()
{
    //温度
    m_pPressureLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pPressureLabel->setAlignment(Qt::AlignLeft);
    m_pPressureLabel->setDataParameter("kPa", 0, Monitor::Uint16t);
    m_pPressureLabel->setMonitorData(&m_usPressure, Monitor::Uint16t);
    m_Widgets.append(m_pPressureLabel);

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

void PressureSensor::valChangedSlot(Monitor* pMonitor)
{
    int32_t val = pMonitor->getCurVal();
    if(val==0)
    {
        m_pPressureLabel->show();
        ui->frame_2->hide();
    }
    else
    {
        m_pPressureLabel->hide();
        ui->frame_2->show();
    }
}
