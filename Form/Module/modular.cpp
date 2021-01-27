#include "modular.h"
#include "ui_modular.h"

Modular::Modular(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Modular)
{
    ui->setupUi(this);
}

Modular::~Modular()
{
    delete ui;
}
