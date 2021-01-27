#include "modularchiller.h"
#include "ui_modularchiller.h"

ModularChiller::ModularChiller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularChiller)
{
    ui->setupUi(this);

   // m_psModular1 = new Modular(ui->frame_4);
   // m_psModular2 = new Modular(ui->frame_5);
}

ModularChiller::~ModularChiller()
{
    delete ui;
}
