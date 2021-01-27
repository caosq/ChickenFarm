#include "system.h"
#include "mainform.h"
#include "ui_mainform.h"


MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    setMaximumSize(1024,768);
    setWindowFlags(Qt::FramelessWindowHint);
    resize(maximumSize());

    m_psModularAir = new ModularAir(ui->page, new ModularAir());
    //m_psModularAirWidget->setGeometry(0,0,ui->page->width(),ui->page->height());
}

MainForm::~MainForm()
{
    delete ui;
}

