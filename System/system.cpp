#include "system.h"

#define PORT_NAME "/dev/ttyUSB0"

System* System::g_pSystem = nullptr;

System::System(QObject *parent) :
    QObject(parent)
{
     m_pSysModeCmdMonitor = DataMonitor::monitorRegist(&m_eSystemModeCmd, Monitor::DataType::Uint16t);

     connect(&m_Timer,SIGNAL(timeout()), this, SLOT(systemTimeOut()));
    // connect(m_pSysModeCmdMonitor, SIGNAL(valChanged(Monitor*)),this,SLOT(sysModeCmdChangedSlot(Monitor*)));
     m_Timer.start(1000);
}

System* System::getInstance()
{
    if(g_pSystem == nullptr)
    {
        g_pSystem = new System();
        g_pSystem->initController();
    }
    return g_pSystem;
}

void System::initController()
{
    pModbus = new Modbus(eMBType::TYPE_MASTER);
    pModbus->uartConfig(9600, 8, 1, Modbus::eParityType::None);

    if(pModbus->masterInit(eMBMode::MB_RTU, PORT_NAME, 1, 1, false))
    {
        m_pController = new Controller();
        m_pController->initComm(pModbus->getMBMasterInfo(), 1);
    }
}

void System::systemTimeOut()
{
    QDateTime datatime = QDateTime::currentDateTime();
    if(datatime.time().hour() == 0 && datatime.time().minute() == 0 && datatime.time().second() == 0)
    {
        this->m_sChickenGrowDay++;
    }
    this->m_usSysYear = datatime.date().year();
    this->m_usSysMon = datatime.date().month();
    this->m_usSysDay = datatime.date().day();
    this->m_usSysHour = datatime.time().hour();
    this->m_usSysMin = datatime.time().minute();
    this->m_usSysSec = datatime.time().second();
    emit systemTimeChanged();
}

void System::sysModeCmdChangedSlot(Monitor* pMonitor)
{
    emit sysModeCmdChanged();
}
