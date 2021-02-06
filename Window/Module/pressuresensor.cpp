#include "pressuresensor.h"
#include "ui_pressuresensor.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     1

#define LABEL_SIZE       140, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     25
#define LABEL_LEFT_MARGIN   35
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  90, 25

#define DATA_LABEL_UP_MARGIN    25
#define DATA_LABEL_LEFT_MARGIN  170
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   35


PressureSensor::PressureSensor(QWidget *parent) :
    Device(parent),
    ui(new Ui::PressureSensor)
{
    ui->setupUi(this);
}

PressureSensor::~PressureSensor()
{
    delete ui;
}

void PressureSensor::initLabel()
{
    ui->label->setText("CHW-SP "+ QString::number(this->m_usDeviceIndex));
}

void PressureSensor::initButton()
{
    //温度
    m_pPressureLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pPressureLabel->setAlignment(Qt::AlignLeft);
    m_pPressureLabel->setDataParameter("kPa", 1, Monitor::Uint16t);
    m_pPressureLabel->setBackGroundColor("#165588");
    m_pPressureLabel->setText("***", LABEL_FONT_SIZE);
    m_Widgets.append(m_pPressureLabel);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
}
