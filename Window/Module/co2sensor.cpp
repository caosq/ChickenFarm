#include "co2sensor.h"
#include "ui_co2sensor.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     1

#define LABEL_SIZE       140, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     25
#define LABEL_LEFT_MARGIN   35
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  100, 25

#define DATA_LABEL_UP_MARGIN    15
#define DATA_LABEL_LEFT_MARGIN  75
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   35

uint8_t CO2Sensor::m_usSensorCount = 0;

CO2Sensor::CO2Sensor(QWidget *parent) :
    Device(parent),
    ui(new Ui::CO2Sensor)
{
    ui->setupUi(this);

    CO2Sensor::m_usSensorCount++;
    this->m_usDeviceIndex = CO2Sensor::m_usSensorCount;

    initLabel();
    initButton();
}

CO2Sensor::~CO2Sensor()
{
    delete ui;
}

void CO2Sensor::initLabel()
{
    ui->label->setText( "CO2-"+ QString::number(this->m_usDeviceIndex));
}

void CO2Sensor::initButton()
{
    //温度
    m_pCO2Label = new DataLabel(ui->frame, DataLabel::Text);
    m_pCO2Label->setAlignment(Qt::AlignLeft);
    m_pCO2Label->setDataParameter("ppm", 1, Monitor::Uint16t);
    m_pCO2Label->setText("20086 ppm", LABEL_FONT_SIZE);
    m_Widgets.append(m_pCO2Label);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
}
