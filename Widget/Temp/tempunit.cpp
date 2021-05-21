#include "tempunit.h"
#include "ui_tempunit.h"

#define LABEL_COLUMNS  1
#define LABEL_ROWS     1

#define LABEL_SIZE       140, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     25
#define LABEL_LEFT_MARGIN   35
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  80, 33

#define DATA_LABEL_UP_MARGIN    2
#define DATA_LABEL_LEFT_MARGIN  0
#define DATA_LABEL_INTERVAL_H   220
#define DATA_LABEL_INTERVAL_V   35

#define NOT_TODAY_PIX ":UI/Widget/System_Sensor_5.png"
//#define IS_TODAY_PIX ":UI/Widget/IsToday.png"
#define IS_TODAY_PIX ":UI/userFile/button100_35.png"

#define M_BC  #F70B0B
#define M_TC   #EBB50E

#define MAX_GROW_DAY            462
#define MIN_GROW_DAY            -2

uint8_t TempUnit::m_usDayCount = 0;

TempUnit::TempUnit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TempUnit)
{
    ui->setupUi(this);


    this->m_sDayIndex = TempUnit::m_usDayCount + MIN_GROW_DAY;
    TempUnit::m_usDayCount++;

    initLabel();
    initButton();
}

TempUnit::~TempUnit()
{
    delete ui;
}

void TempUnit::initLabel()
{
    ui->label->setText(QString::number(this->m_sDayIndex) + " 天");
}

void TempUnit::initButton()
{
    //温度
    m_pTempBtn = new AnalogValButton(ui->frame_2);
    m_pTempBtn->setDataParameter("℃", 1, 190, 350, 160, Monitor::Uint16t);
    m_pTempBtn->setMonitorData(&m_usTemp, Monitor::Uint16t);
    m_pTempBtn->setTextColor(QPalette::ColorGroup::Current, Qt::white);
    m_Widgets.append(m_pTempBtn);

    for (uint8_t i = 0, m = 0, n = 0; i < m_Widgets.count(); i++)
    {
        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
    }
    connect(m_pTempBtn, SIGNAL(valChanged(void*)), this, SLOT(valChangedSlot(void*)));
}

void TempUnit::setCurrentDay(bool xIsToday)
{
     QPalette pe;
    if(xIsToday)
    {
        ui->frame->setStyleSheet("#frame{border-image: url(" + QString(IS_TODAY_PIX)+ ");}" );
        pe.setColor(QPalette::WindowText, QColor(255, 0, 0));
        ui->label->setPalette(pe);
        m_pTempBtn->setTextColor(QPalette::ColorGroup::Current, QColor(255, 0, 0));
    }
    else
    {
        ui->frame->setStyleSheet("#frame{border-image: url(" + QString(NOT_TODAY_PIX)+ ");}" );
        pe.setColor(QPalette::WindowText, QColor(252, 175, 62));
        ui->label->setPalette(pe);
        m_pTempBtn->setTextColor(QPalette::ColorGroup::Current, Qt::white);
    }
}

void TempUnit::valChangedSlot(void*)
{
    emit tempChanged(this);
}
