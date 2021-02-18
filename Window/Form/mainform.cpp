#include "system.h"
#include "mainform.h"
#include "ui_mainform.h"

#include <QTextCodec>
#include <QBrush>
#include <QLocale>


#define AB_PRESS_PIX ":UI/baseFile/abPress.png"
//#define AB_RELEASE_PIX ":UI/baseFile/abRelease.png"
#define AB_RELEASE_PIX ":UI/userFile/button100_35.png"


#define SBUTTON_STATE0 ":UI/baseFile/sbLeft.png"
#define SBUTTON_STATE1 ":UI/baseFile/sbRight.png"

//#define SBUTTON_STATE0 ":UI/userFile/sb_left_100_35.png"
//#define SBUTTON_STATE1 ":UI/userFile/sb_right_100_35.png"

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
    QPalette pe;
    QFont font;

    m_pLogLabel = new QLabel (ui->frame);
    m_pLogLabel->setGeometry(740,10, 400, 30);
    m_pLogLabel->setText("触摸屏与控制器通讯故障");

    pe.setColor(QPalette::WindowText,Qt::red);
    font.setPointSize(12);//字体大小
    m_pLogLabel->setPalette(pe);
    m_pLogLabel->setFont(font);


    m_pTitleLabel = new QLabel (ui->frame);
    m_pTitleLabel->setGeometry(600,25, 300, 30);
    m_pTitleLabel->setText("系统监控");

    pe.setColor(QPalette::WindowText,Qt::white);
    font.setPointSize(14);//字体大小
    m_pTitleLabel->setPalette(pe);
    m_pTitleLabel->setFont(font);


}

void MainForm::initButton()
{
    AnalogValButton::forefather()->setFatherPixmap(AnalogValButton::press,AB_PRESS_PIX);
    AnalogValButton::forefather()->setFatherPixmap(AnalogValButton::release,AB_RELEASE_PIX);

    StateButton::forefather()->setFatherPixmap(StateButton::State0, SBUTTON_STATE0);
    StateButton::forefather()->setFatherPixmap(StateButton::State1, SBUTTON_STATE1);
    StateButton::forefather()->setFatherFontSize(14);

    ModeButton::forefather()->setFatherPixmap(MB_PIX2);
    ModeButton::forefather()->setFatherFontSize(14);

    //设置通用按键图标
    ubutton::forefather()->setFatherPixmap(ubutton::press,UB_PRESS_PIX);
    ubutton::forefather()->setFatherPixmap(ubutton::release,UB_RELEASE_PIX);
}


void MainForm::initForm()
{
    m_pSystemMonitor = new SystemMonitor();
    m_pHome          = new Home();
    m_pCurve         = new Curve();
    m_pSetting       = new Setting();
    m_pEvent         = new Event();


    ui->mainStackedWidget->insertWidget(0, m_pHome);
    ui->mainStackedWidget->insertWidget(1, m_pSystemMonitor);
    ui->mainStackedWidget->insertWidget(2, m_pCurve);
    ui->mainStackedWidget->insertWidget(3, m_pSetting);
    ui->mainStackedWidget->insertWidget(4, m_pEvent);


    ui->mainStackedWidget->setCurrentWidget(m_pHome);
}

void MainForm::on_systemButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(m_pSystemMonitor);
}

void MainForm::on_curveButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(m_pCurve);
}

void MainForm::on_paraButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(m_pSetting);
}

void MainForm::on_backButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(m_pHome);
}

void MainForm::on_eventButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(m_pEvent);
}
