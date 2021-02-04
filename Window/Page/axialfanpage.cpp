#include "axialfanpage.h"
#include "ui_axialfanpage.h"

AxialFanPage::AxialFanPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AxialFanPage)
{
    ui->setupUi(this);
}

AxialFanPage::~AxialFanPage()
{
    delete ui;
}
