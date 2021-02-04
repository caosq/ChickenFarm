#include "tempsensor.h"
#include "ui_tempsensor.h"

TempSensor::TempSensor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TempSensor)
{
    ui->setupUi(this);
}

TempSensor::~TempSensor()
{
    delete ui;
}
