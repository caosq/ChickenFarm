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

    initLabel();
    initButton();
    initForm();
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::initLabel()
{

}

void MainForm::initButton()
{
    AnalogValButton::forefather()->setFatherPixmap(AnalogValButton::press,AB_PRESS_PIX);
    AnalogValButton::forefather()->setFatherPixmap(AnalogValButton::release,AB_RELEASE_PIX);


    StateButton::forefather()->setFatherPixmap(StateButton::State0, SBUTTON_STATE0);
    StateButton::forefather()->setFatherPixmap(StateButton::State1, SBUTTON_STATE1);
    StateButton::forefather()->setFatherFontSize(12);

    ModeButton::forefather()->setFatherPixmap(MB_PIX2);
    ModeButton::forefather()->setFatherFontSize(14);

    //设置通用按键图标
    ubutton::forefather()->setFatherPixmap(ubutton::press,UB_PRESS_PIX);
    ubutton::forefather()->setFatherPixmap(ubutton::release,UB_RELEASE_PIX);
}


void MainForm::initForm()
{
    m_pSystemMonitor = new SystemMonitor();
    m_pCurve =         new Curve();

    ui->mainStackedWidget->insertWidget(0, m_pSystemMonitor);
    ui->mainStackedWidget->insertWidget(1, m_pCurve);
    ui->mainStackedWidget->setCurrentWidget(m_pSystemMonitor);
}
