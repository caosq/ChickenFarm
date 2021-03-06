#include "temphumisensor.h"
#include "ui_temphumisensor.h"

#define LABEL_COLUMNS  2
#define LABEL_ROWS     1

#define LABEL_SIZE       140, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     25
#define LABEL_LEFT_MARGIN   35
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  90, 25

#define DATA_LABEL_UP_MARGIN    20
#define DATA_LABEL_LEFT_MARGIN  80
#define DATA_LABEL_INTERVAL_H   80
#define DATA_LABEL_INTERVAL_V   35

TempHumiSensor::TempHumiSensor(const QString strSensorType, uint8_t ucIndex, QWidget *parent) :
    Device(parent),
    ui(new Ui::TempHumiSensor)
{
    ui->setupUi(this);

    m_strSensorType = strSensorType;
    this->m_usDeviceIndex = ucIndex+1;

    initLabel();
    initButton();
}

TempHumiSensor::~TempHumiSensor()
{
    delete ui;
}

void TempHumiSensor::initLabel()
{
    ui->label->setText(m_strSensorType + "-"+ QString::number(this->m_usDeviceIndex));
}

void TempHumiSensor::initButton()
{
    //温度
    m_pTempLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pTempLabel->setAlignment(Qt::AlignLeft);
    m_pTempLabel->setDataParameter("℃", 1, Monitor::Int16t);
    m_pTempLabel->setMonitorData(&m_sTemp, Monitor::Int16t);
    m_Widgets.append(m_pTempLabel);

    //湿度
    m_pHumiLabel = new DataLabel(ui->frame, DataLabel::Data);
    m_pHumiLabel->setAlignment(Qt::AlignLeft);
    m_pHumiLabel->setDataParameter("%", 1, Monitor::Uint16t);
    m_pHumiLabel->setMonitorData(&m_usHumi, Monitor::Uint16t);
    m_Widgets.append(m_pHumiLabel);


    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    ui->frame_2->hide();
    ui->frame_3->hide();

    m_pTempErrMonitor = DataMonitor::monitorRegist(&m_xTempErr, Monitor::DataType::Boolean);
    connect(m_pTempErrMonitor, SIGNAL(valChanged(Monitor*)),this,SLOT(tempValChangedSlot(Monitor*)));

    m_pHumiErrMonitor = DataMonitor::monitorRegist(&m_xHumiErr, Monitor::DataType::Boolean);
    connect(m_pHumiErrMonitor, SIGNAL(valChanged(Monitor*)),this,SLOT(humiValChangedSlot(Monitor*)));
}

void TempHumiSensor::tempValChangedSlot(Monitor* pMonitor)
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

void TempHumiSensor::humiValChangedSlot(Monitor* pMonitor)
{
    int32_t val = pMonitor->getCurVal();
    if(val==0)
    {
        m_pHumiLabel->show();
        ui->frame_3->hide();
    }
    else
    {
        m_pHumiLabel->hide();
        ui->frame_3->show();
    }
}
