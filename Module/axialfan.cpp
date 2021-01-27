#include "axialfan.h"
#include "ui_axialfan.h"

AxialFan::AxialFan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AxialFan)
{
    ui->setupUi(this);
}

AxialFan::~AxialFan()
{
    delete ui;
}
