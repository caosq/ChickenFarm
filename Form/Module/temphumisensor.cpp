#include "temphumisensor.h"
#include "ui_temphumisensor.h"

TempHumiSensor::TempHumiSensor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TempHumiSensor)
{
    ui->setupUi(this);
}

TempHumiSensor::~TempHumiSensor()
{
    delete ui;
}
