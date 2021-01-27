#include "butterflyvalve.h"
#include "ui_butterflyvalve.h"

ButterflyValve::ButterflyValve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButterflyValve)
{
    ui->setupUi(this);
}

ButterflyValve::~ButterflyValve()
{
    delete ui;
}
