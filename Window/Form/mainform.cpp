#include "system.h"
#include "mainform.h"
#include "ui_mainform.h"

#include <QTextCodec>
#include <QBrush>
#include <QLocale>


#define AB_PRESS_PIX ":UI/baseFile/abPress.png"
#define AB_RELEASE_PIX ":UI/baseFile/abRelease.png"

#define SBUTTON_STATE0 ":UI/baseFile/sbLeft.png"
#define SBUTTON_STATE1 ":UI/baseFile/sbRight.png"

#define UB_PRESS_PIX ":UI/baseFile/ubPress.png"
#define UB_RELEASE_PIX ":UI/baseFile/ubRelease.png"

#define MB_PIX2 ":UI/baseFile/mbPix2.png"

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    setMaximumSize(1024,768);
    setWindowFlags(Qt::FramelessWindowHint);
    resize(maximumSize());

    AnalogValButton::forefather()->setFatherPixmap(AnalogValButton::press,AB_PRESS_PIX);
    AnalogValButton::forefather()->setFatherPixmap(AnalogValButton::release,AB_RELEASE_PIX);


    StateButton::forefather()->setFatherPixmap(eButtonState::State0, SBUTTON_STATE0);
    StateButton::forefather()->setFatherPixmap(eButtonState::State1, SBUTTON_STATE1);
    StateButton::forefather()->setFatherFontSize(14);

    ModeButton::forefather()->setFatherPixmap(MB_PIX2);
    ModeButton::forefather()->setFatherFontSize(14);

    //设置通用按键图标
    ubutton::forefather()->setFatherPixmap(ubutton::press,UB_PRESS_PIX);
    ubutton::forefather()->setFatherPixmap(ubutton::release,UB_RELEASE_PIX);


    m_psModularAir     = new ModularAir(ui->page);
    m_psModularChiller = new ModularChiller(ui->page_2);

    m_psChilledBump_1    = new ChilledBump();
    ui->gridLayout->addWidget(m_psChilledBump_1,0,1);

    m_psChilledBump_2    = new ChilledBump();
    ui->gridLayout->addWidget(m_psChilledBump_2,0,2);

    m_psChilledBump_3    = new ChilledBump();
    ui->gridLayout->addWidget(m_psChilledBump_3,1,1);
    //m_psModularAirWidget->setGeometry(0,0,ui->page->width(),ui->page->height());


    m_psAxialFan_1 = new AxialFan();
    ui->gridLayout_2->addWidget(m_psAxialFan_1,0,1);

    m_psAxialFan_2 = new AxialFan();
    ui->gridLayout_2->addWidget(m_psAxialFan_2,0,2);

    m_psAxialFan_3 = new AxialFan();
    ui->gridLayout_2->addWidget(m_psAxialFan_3,1,1);

    m_pButterflyValve = new ButterflyValve();
    ui->gridLayout_3->addWidget(m_pButterflyValve,0,1);


    m_psCurve = new Curve(ui->curvePage);

}

MainForm::~MainForm()
{
    delete ui;
}

