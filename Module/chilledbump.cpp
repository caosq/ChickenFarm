#include "chilledbump.h"
#include "ui_chilledbump.h"

ChilledBump::ChilledBump(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChilledBump)
{
    ui->setupUi(this);
}

ChilledBump::~ChilledBump()
{
    delete ui;
}
