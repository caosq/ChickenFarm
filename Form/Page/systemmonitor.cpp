#include "systemmonitor.h"
#include "ui_systemmonitor.h"

SystemMonitor::SystemMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemMonitor)
{
    ui->setupUi(this);
}

SystemMonitor::~SystemMonitor()
{
    delete ui;
}

void SystemMonitor::on_pushButton_clicked()
{

}
