#include "modularchillerwidget.h"
#include "ui_modularchillerwidget.h"

ModularChillerWidget::ModularChillerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularChillerWidget)
{
    ui->setupUi(this);
}

ModularChillerWidget::~ModularChillerWidget()
{
    delete ui;
}
