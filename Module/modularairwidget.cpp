#include "modularairwidget.h"
#include "ui_modularairwidget.h"


ModularAirWidget::ModularAirWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularAirWidget)
{
    ui->setupUi(this);
}

ModularAirWidget::ModularAirWidget(QWidget *parent, ModularAir* psModularAir) :
    QWidget(parent),
    ui(new Ui::ModularAirWidget)
{
    ui->setupUi(this);
    m_psModularAir = psModularAir;
}

ModularAirWidget::~ModularAirWidget()
{
    delete ui;
}
