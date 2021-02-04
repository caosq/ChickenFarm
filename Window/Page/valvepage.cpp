#include "valvepage.h"
#include "ui_valvepage.h"

ValvePage::ValvePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ValvePage)
{
    ui->setupUi(this);
}

ValvePage::~ValvePage()
{
    delete ui;
}
