#include "parawindow.h"
#include "ui_parawindow.h"

ParaWindow::ParaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ParaWindow)
{
    ui->setupUi(this);
}

ParaWindow::~ParaWindow()
{
    delete ui;
}
