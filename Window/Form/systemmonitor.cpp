#include "systemmonitor.h"
#include "ui_systemmonitor.h"
#include "system.h"

SystemMonitor::SystemMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemMonitor)
{
    ui->setupUi(this);

    initForm();
}

SystemMonitor::~SystemMonitor()
{
    delete ui;
}

void SystemMonitor::initForm()
{
    m_pModularAirPage     = new ModularAirPage();
    m_pModularChillerPage = new ModularChillerPage();
    m_pBumpPage           = new BumpPage();
    m_pAxialFanPage       = new AxialFanPage();
    m_pValvePage          = new ValvePage();
    m_pSensorPage         = new SensorPage();

    ui->systemStackedWidget->insertWidget(0, m_pModularAirPage);
    ui->systemStackedWidget->insertWidget(1, m_pModularChillerPage);
    ui->systemStackedWidget->insertWidget(2, m_pBumpPage);
    ui->systemStackedWidget->insertWidget(3, m_pAxialFanPage);
    ui->systemStackedWidget->insertWidget(4, m_pValvePage);
    ui->systemStackedWidget->insertWidget(5, m_pSensorPage);
    //ui->systemStackedWidget->setCurrentIndex(0);

    m_Widgets.append(ui->pushButton);
    m_Widgets.append(ui->pushButton_2);
    m_Widgets.append(ui->pushButton_3);
    m_Widgets.append(ui->pushButton_4);
    m_Widgets.append(ui->pushButton_5);
    m_Widgets.append(ui->pushButton_6);

    on_pushButton_2_clicked();

    System::getInstance()->initController();  //控制器初始化
}

void SystemMonitor::on_pushButton_clicked()
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
    ui->systemStackedWidget->setCurrentWidget(m_pModularAirPage);
}

void SystemMonitor::on_pushButton_2_clicked()
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
    ui->systemStackedWidget->setCurrentWidget(m_pModularChillerPage);
}

void SystemMonitor::on_pushButton_3_clicked()
{
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
    ui->systemStackedWidget->setCurrentWidget(m_pBumpPage);
}

void SystemMonitor::on_pushButton_4_clicked()
{
    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->pushButton_4->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    for (uint8_t i = 0; i < m_Widgets.count(); i++)
    {
        if(m_Widgets[i] != ui->pushButton_4)
        {
            m_Widgets[i]->setPalette(pe);
        }
    }
    System::getInstance()->m_uiOffLogCount = 0;
    ui->systemStackedWidget->setCurrentWidget(m_pAxialFanPage);
}

void SystemMonitor::on_pushButton_5_clicked()
{
    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->pushButton_5->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    for (uint8_t i = 0; i < m_Widgets.count(); i++)
    {
        if(m_Widgets[i] != ui->pushButton_5)
        {
            m_Widgets[i]->setPalette(pe);
        }
    }
    System::getInstance()->m_uiOffLogCount = 0;
    ui->systemStackedWidget->setCurrentWidget(m_pValvePage);
}

void SystemMonitor::on_pushButton_6_clicked()
{
    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->pushButton_6->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    for (uint8_t i = 0; i < m_Widgets.count(); i++)
    {
        if(m_Widgets[i] != ui->pushButton_6)
        {
            m_Widgets[i]->setPalette(pe);
        }
    }
    System::getInstance()->m_uiOffLogCount = 0;
    ui->systemStackedWidget->setCurrentWidget(m_pSensorPage);
}
