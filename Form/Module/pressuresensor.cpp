#include "pressuresensor.h"
#include "ui_pressuresensor.h"

PressureSensor::PressureSensor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PressureSensor)
{
    ui->setupUi(this);
}

PressureSensor::~PressureSensor()
{
    delete ui;
}
