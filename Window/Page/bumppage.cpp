#include "bumppage.h"
#include "ui_bumppage.h"

BumpPage::BumpPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BumpPage)
{
    ui->setupUi(this);
}

BumpPage::~BumpPage()
{
    delete ui;
}
