#include "system.h"
#include "mainform.h"
#include "ui_mainform.h"

#include <QTextCodec>
#include <QBrush>
#include <QLocale>
#include "password.h"
#include "pwdkeyboard.h"
#include "messagebox.h"

#define AB_PRESS_PIX ":UI/baseFile/abPress.png"
//#define AB_RELEASE_PIX ":UI/baseFile/abRelease.png"
#define AB_RELEASE_PIX ":UI/Frame/mode.png"


#define SBUTTON_STATE0 ":UI/baseFile/sbLeft.png"
#define SBUTTON_STATE1 ":UI/baseFile/sbRight.png"

//#define SBUTTON_STATE0 ":UI/userFile/sb_left_100_35.png"
//#define SBUTTON_STATE1 ":UI/userFile/sb_right_100_35.png"

#define UB_PRESS_PIX ":UI/baseFile/ubPress.png"
#define UB_RELEASE_PIX ":UI/baseFile/ubRelease.png"

//#define MB_PIX2 ":UI/baseFile/mbPix2.png"
#define MB_PIX2 ":UI/Frame/combobox.png"


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

    m_pTitleLabel = new QLabel (ui->frame);
    m_pTitleLabel->setGeometry(600,25, 300, 30);
    m_pTitleLabel->setText("");

    pe.setColor(QPalette::WindowText,Qt::white);
    font.setPointSize(14);//字体大小
    m_pTitleLabel->setPalette(pe);
    m_pTitleLabel->setFont(font);

    ui->backButton->setEnabled(false);
    ui->paraButton->setEnabled(false);
    ui->curveButton->setEnabled(false);
    ui->eventButton->setEnabled(false);
    ui->systemButton->setEnabled(false);
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
/*    m_pSystemMonitor = new SystemMonitor();
    m_pHome          = new Home();
    m_pCurve         = new Curve();
    m_pSetting       = new Setting();
    m_pEvent         = new Event();
*/
    ui->mainStackedWidget->addWidget(&m_SystemMonitor);
    ui->mainStackedWidget->addWidget(&m_Home);
    ui->mainStackedWidget->addWidget(&m_Curve);
    ui->mainStackedWidget->addWidget(&m_Setting);
    ui->mainStackedWidget->addWidget(&m_Event);
    ui->mainStackedWidget->setCurrentWidget(&m_Home);

    ui->backButton->setEnabled(true);
    ui->paraButton->setEnabled(true);
    ui->curveButton->setEnabled(true);
    ui->eventButton->setEnabled(true);
    ui->systemButton->setEnabled(true);

    m_pLogLabel = new DataLabel(ui->frame, DataLabel::Text);
    m_pLogLabel->setGeometry(500,10, 400, 30);
    m_pLogLabel->setValueMap(0,tr(""));
    m_pLogLabel->setValueMap(1,tr("触摸屏与控制器通讯故障"), Qt::red);
    m_pLogLabel->setMonitorData(&System::getInstance()->m_Controller.m_xCommErr, Monitor::Boolean);
    m_pLogLabel->setTextSize(12);
    //m_pLogLabel->setText("触摸屏与控制器通讯故障");

    connect(System::getInstance(), SIGNAL(systemTimeChanged()), this, SLOT(systemTimeChangedSlot()));
}

void MainForm::systemTimeChangedSlot()
{
    QDateTime datatime = QDateTime::currentDateTime();
    QString str1 = datatime.toString("yyyy-MM-dd"); //设置显示格式
    QString str2 = datatime.toString("hh:mm:ss");
    ui->label_6->setText(str1);
    ui->label_7->setText(str2);

    if(System::getInstance()->m_xIsLogIn)
    {
        ui->pushButton->setText("用户注销");
    }
    else
    {
        //ui->mainStackedWidget->setCurrentWidget(&m_Home);
        ui->pushButton->setText("用户登录");
    }
}

void MainForm::on_systemButton_clicked()
{
    System::getInstance()->m_uiOffLogCount = 0;
    ui->mainStackedWidget->setCurrentWidget(&m_SystemMonitor);
    m_pTitleLabel->setText("系统监控");
}

void MainForm::on_curveButton_clicked()
{
    System::getInstance()->m_uiOffLogCount = 0;
    ui->mainStackedWidget->setCurrentWidget(&m_Curve);
    m_pTitleLabel->setText("趋势曲线");
}

void MainForm::on_paraButton_clicked()
{
    if(System::getInstance()->checkSystemLogIn())
    {
        System::getInstance()->m_uiOffLogCount = 0;
        ui->mainStackedWidget->setCurrentWidget(&m_Setting);
        m_pTitleLabel->setText("参数设置");
        emit System::getInstance()->systemStackChanged(ui->mainStackedWidget->currentIndex());
    }
}

void MainForm::on_backButton_clicked()
{
    System::getInstance()->m_uiOffLogCount = 0;
    ui->mainStackedWidget->setCurrentWidget(&m_Home);
    m_pTitleLabel->setText("");
}

void MainForm::on_eventButton_clicked()
{
    System::getInstance()->m_uiOffLogCount = 0;
    ui->mainStackedWidget->setCurrentWidget(&m_Event);
    m_pTitleLabel->setText("事件记录");
}

void MainForm::on_pushButton_clicked()
{
    int passwd = pwdKeyBoard::getWrongValue();
    bool exitState = false;
    bool passwdState = false;

    if(System::getInstance()->m_xIsLogIn == false)
    {
        while( !passwdState && !exitState )//获取成功且密码正确就退出
        {
            passwd = pwdKeyBoard::getValue(passwd,&exitState);
            if(exitState){return;}
            if(password::instance()->judgementPwd(password::GR_FACTORY,passwd))
            {
                passwdState = true;
                System::getInstance()->m_xIsLogIn = true;
                System::getInstance()->m_xIsFactoryLogIn = true;
                ui->pushButton->setText("用户注销");
            }
            else if(password::instance()->judgementPwd(password::GR_USER,passwd))
            {
                passwdState = true;
                System::getInstance()->m_xIsLogIn = true;
                ui->pushButton->setText("用户注销");
            }
            else
            {
                passwdState = false;
                System::getInstance()->m_xIsLogIn = false;

                pwdKeyBoard::instance()->show();
                messageBox::instance()->setInformativeText("请输入正确的用户密码");
                messageBox::instance()->show();
                ui->pushButton->setText("用户登录");
            }
        }
    }
    else
    {
        System::getInstance()->m_xIsLogIn = false;
        System::getInstance()->m_xIsFactoryLogIn = false;
        ui->pushButton->setText("用户登录");
    }

}
