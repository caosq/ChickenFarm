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

    m_psModularAir     = new ModularAir(ui->page);
    m_psModularChiller = new ModularChiller(ui->page_2);

    m_psChilledBump_1    = new ChilledBump();
    ui->gridLayout->addWidget(m_psChilledBump_1,0,1);

    m_psChilledBump_2    = new ChilledBump();
    ui->gridLayout->addWidget(m_psChilledBump_2,0,2);

    m_psChilledBump_3    = new ChilledBump();
    ui->gridLayout->addWidget(m_psChilledBump_3,1,2);
    //m_psModularAirWidget->setGeometry(0,0,ui->page->width(),ui->page->height());
}

MainForm::~MainForm()
{
    delete ui;
}

