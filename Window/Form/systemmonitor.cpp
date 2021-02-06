#include "systemmonitor.h"
#include "ui_systemmonitor.h"

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

    ui->systemStackedWidget->setCurrentIndex(5);
}
