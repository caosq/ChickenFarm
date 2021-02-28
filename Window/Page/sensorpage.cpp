#include "sensorpage.h"
#include "ui_sensorpage.h"
#include "system.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     2

#define LABEL_SIZE       110, 28
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

SensorPage::SensorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorPage)
{
    ui->setupUi(this);

    initDevice();
}

SensorPage::~SensorPage()
{
    delete ui;
}

void SensorPage::initDevice()
{
    TempHumiSensor *pTempHumi = nullptr;
    CO2Sensor      *pCO2      = nullptr;
    TempSensor     *pTemp     = nullptr;
    PressureSensor *pPressure = nullptr;

    System   *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    for(uint8_t n = 0, x = 0, y = 0; n < TEMP_HUMI_IN_NUM; n++)
    {
        x = n / 2;
        y = n % 2;
        pTempHumi = new TempHumiSensor("ITH", n);
        pSystem->m_pModularAirs[n/TEMP_HUMI_IN_NUM_IN_MODULAR_AIR]->m_pTempHumiInSensors.append(pTempHumi);
        ui->gridLayout->addWidget(pTempHumi, x, y);
    }
    for(uint8_t n = 0, x = 0, y = 0; n < TEMP_HUMI_OUT_NUM; n++)
    {
        x = n / 2;
        y = n % 2;
        pTempHumi = new TempHumiSensor("OTH", n);
        pSystem->m_pModularAirs[n/TEMP_HUMI_OUT_NUM_IN_MODULAR_AIR]->m_pTempHumiOutSensor = pTempHumi;
        ui->gridLayout_1->addWidget(pTempHumi, x, y);
    }
    for(uint8_t n = 0, x = 0, y = 0; n < CO2_NUM; n++)
    {
        x = n / 2;
        y = n % 2;
        pCO2 = new CO2Sensor();
        pSystem->m_pModularAirs[n/CO2_NUM_IN_MODULAR_AIR]->m_pCO2Sensors.append(pCO2);
        ui->gridLayout_2->addWidget(pCO2, x, y);
    }
    for(uint8_t n = 0, x = 0, y = 0; n < CHW_TEMP_SENSOR_NUM; n++)
    {
        x = n / 2;
        y = n % 2;
        pTemp = new TempSensor();
        pSystem->m_pCHWTempSensors.append(pTemp);
        ui->gridLayout_4->addWidget(pTemp, x, y);
    }
    for(uint8_t n = 0, x = 0, y = 0; n < CHW_PRESSURE_SENSOR_NUM; n++)
    {
        x = n / 2;
        y = n % 2;
        pPressure = new PressureSensor();
        pSystem->m_pCHWPressureSensors.append(pPressure);
        ui->gridLayout_3->addWidget(pPressure, x, y);
    }
}

void SensorPage::initLabel()
{

}

void SensorPage::initButton()
{

}
