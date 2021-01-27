#include "modularair.h"
#include "ui_modularair.h"

ModularAir::ModularAir(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularAir)
{
    ui->setupUi(this);
}

ModularAir::~ModularAir()
{
    delete ui;
}
