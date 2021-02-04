#include "sensorpage.h"
#include "ui_sensorpage.h"

SensorPage::SensorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorPage)
{
    ui->setupUi(this);
}

SensorPage::~SensorPage()
{
    delete ui;
}
