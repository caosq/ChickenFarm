#include "co2sensor.h"
#include "ui_co2sensor.h"

CO2Sensor::CO2Sensor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CO2Sensor)
{
    ui->setupUi(this);
}

CO2Sensor::~CO2Sensor()
{
    delete ui;
}
