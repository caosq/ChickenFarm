#include "setting.h"
#include "ui_setting.h"
#include "password.h"
#include "pwdkeyboard.h"
#include "messagebox.h"
#include "system.h"

Setting::Setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);

    initForm();
    connect(System::getInstance(), SIGNAL(stackChanged(int32_t)),
            this, SLOT(systemStackChanged(int32_t)));
}

Setting::~Setting()
{
    delete ui;
}

void Setting::show()
{
    ui->systemStackedWidget->setCurrentWidget(m_pTempSetPage);
}

void Setting::initForm()
{
    m_pTempSetPage  = new TempSetPage();
    m_pUserParamPage = new UserParamPage();
    m_pManuParamPage = new ManuParamPage();

    ui->systemStackedWidget->insertWidget(0, m_pTempSetPage);
    ui->systemStackedWidget->insertWidget(1, m_pUserParamPage);
    ui->systemStackedWidget->insertWidget(2, m_pManuParamPage);
    ui->systemStackedWidget->setCurrentIndex(0);

    m_Widgets.append(ui->pushButton);
    m_Widgets.append(ui->pushButton_2);
    m_Widgets.append(ui->pushButton_3);

    on_pushButton_clicked();
}

void Setting::systemStackChanged(int32_t iVal)
{
    if(iVal  == 3)
    {
        on_pushButton_clicked();
    }
}

void Setting::on_pushButton_clicked()
{
    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->pushButton->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    for (uint8_t i = 0; i < m_Widgets.count(); i++)
    {
        if(m_Widgets[i] != ui->pushButton)
        {
            m_Widgets[i]->setPalette(pe);
        }
    }
    System::getInstance()->m_uiOffLogCount = 0;
    ui->systemStackedWidget->setCurrentWidget(m_pTempSetPage);
}

void Setting::on_pushButton_2_clicked()
{
    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->pushButton_2->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    for (uint8_t i = 0; i < m_Widgets.count(); i++)
    {
        if(m_Widgets[i] != ui->pushButton_2)
        {
            m_Widgets[i]->setPalette(pe);
        }
    }
    System::getInstance()->m_uiOffLogCount = 0;
    ui->systemStackedWidget->setCurrentWidget(m_pUserParamPage);
}

void Setting::on_pushButton_3_clicked()
{
    int passwd = pwdKeyBoard::getWrongValue();
    bool exitState = false;
    bool passwdState = false;

    if(!System::getInstance()->m_xIsFactoryLogIn && !System::getInstance()->m_xIsInDebug)
    {
        while( !passwdState && !exitState )//获取成功且密码正确就退出
        {
            passwd = pwdKeyBoard::getValue(passwd,&exitState);
            if(exitState){return;}
            if(password::instance()->judgementPwd(password::GR_FACTORY,passwd))
            {
                passwdState = true;
            }
            else
            {
                passwdState = false;
                pwdKeyBoard::instance()->show();
                messageBox::instance()->setInformativeText("请输入正确的厂家密码");
                messageBox::instance()->show();
            }
        }
    }
    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->pushButton_3->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    for (uint8_t i = 0; i < m_Widgets.count(); i++)
    {
        if(m_Widgets[i] != ui->pushButton_3)
        {
            m_Widgets[i]->setPalette(pe);
        }
    }
    System::getInstance()->m_uiOffLogCount = 0;
    ui->systemStackedWidget->setCurrentWidget(m_pManuParamPage);
}
