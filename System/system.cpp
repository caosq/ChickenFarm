#include "system.h"
#include "messagebox.h"

#define PORT_NAME "/dev/ttyS0"
#define SYS_OFF_LOG_TIME_COUNT  300

System* System::g_pSystem = nullptr;

System::System(QObject *parent) :
    QObject(parent)
{
     connect(&m_Timer,SIGNAL(timeout()), this, SLOT(systemTimeOut()));
     m_Timer.start(1000);
}

System* System::getInstance()
{
    if(g_pSystem == nullptr)
    {
        g_pSystem = new System();
        g_pSystem->m_Modbus.uartConfig(9600, 8, 1, Modbus::eParityType::None);
        g_pSystem->m_Modbus.masterInit(eMBMode::MB_RTU, PORT_NAME, 1, 1, false);

        g_pSystem->m_pSysModeCmdMonitor = DataMonitor::monitorRegist(&g_pSystem->m_eSystemModeCmd, Monitor::DataType::Uint16t);
        g_pSystem->m_pExAirFanRatedVolMonitor_H = DataMonitor::monitorRegist(&g_pSystem->m_usExAirFanRatedVol_H,
                                                                             Monitor::DataType::Uint16t);
        g_pSystem->m_pExAirFanRatedVolMonitor_L = DataMonitor::monitorRegist(&g_pSystem->m_usExAirFanRatedVol_L,
                                                                             Monitor::DataType::Uint16t);
        g_pSystem->m_pTotalFreAirMonitor_H = DataMonitor::monitorRegist(&g_pSystem->m_usTotalFreAir_H,
                                                                        Monitor::DataType::Uint16t);
        g_pSystem->m_pTotalFreAirMonitor_L = DataMonitor::monitorRegist(&g_pSystem->m_usTotalFreAir_L,
                                                                        Monitor::DataType::Uint16t);


        connect(g_pSystem->m_pSysModeCmdMonitor, SIGNAL(valChanged(Monitor*)), g_pSystem, SLOT(systemDataChanged(Monitor*)));
        connect(g_pSystem->m_pExAirFanRatedVolMonitor_H, SIGNAL(valChanged(Monitor*)), g_pSystem, SLOT(systemDataChanged(Monitor*)));
        connect(g_pSystem->m_pExAirFanRatedVolMonitor_L, SIGNAL(valChanged(Monitor*)), g_pSystem, SLOT(systemDataChanged(Monitor*)));
        connect(g_pSystem->m_pTotalFreAirMonitor_H, SIGNAL(valChanged(Monitor*)), g_pSystem, SLOT(systemDataChanged(Monitor*)));
        connect(g_pSystem->m_pTotalFreAirMonitor_L, SIGNAL(valChanged(Monitor*)), g_pSystem, SLOT(systemDataChanged(Monitor*)));

    }
    return g_pSystem;
}

void System::initController()
{
   // pModbus = new Modbus();


    //m_pController = new  Controller();

    //if(pModbus.masterInit(eMBMode::MB_RTU, PORT_NAME, 1, 1, false))
    //{
        //m_pController = new  Controller();
        m_Controller.initComm(m_Modbus.getMBMasterInfo(), 1);
    //}
}

void System::systemTimeOut()
{
    QDateTime datatime = QDateTime::currentDateTime();
    ModularAir* pCurModularAir =nullptr;

    if(datatime.time().hour() == 0 && datatime.time().minute() == 0 && datatime.time().second() == 0)
    {
        this->m_sChickenGrowDay++;
    }
    this->m_usSysYear = uint16_t(datatime.date().year());
    this->m_usSysMon  = uint16_t(datatime.date().month());
    this->m_usSysDay  = uint16_t(datatime.date().day());
    this->m_usSysHour = uint16_t(datatime.time().hour());
    this->m_usSysMin  = uint16_t(datatime.time().minute());
    this->m_usSysSec  = uint16_t(datatime.time().second());

    if(m_eSystemModeCmd == System::SystemMode::MODE_AUTO)
    {
        for(uint8_t i = 0; i < m_pModularAirs.count(); i++)
        {
             pCurModularAir = m_pModularAirs[i];
             pCurModularAir->m_usTempSet = m_usTempSet;
             pCurModularAir->m_usHumiSet = m_usHumiSet;
             pCurModularAir->m_usCO2Set = m_usCO2PPMSet;
        }
    }
    if(m_xIsLogIn == true && m_xIsInDebug == false)
    {
        m_uiOffLogCount++;
    }
    if(m_uiOffLogCount == SYS_OFF_LOG_TIME_COUNT)
    {
        m_xIsLogIn = false;
        m_uiOffLogCount = 0;
    }
    emit systemTimeChanged();
}

bool System::checkSystemLogIn()
{
    if(m_xIsLogIn)
    {
        m_uiOffLogCount = 0;
        return true;
    }
    else
    {
        messageBox::instance()->setInformativeText("请先登录后再操作");
        messageBox::instance()->show();
        //return false;
    }
}

void System::systemDataChanged(Monitor*)
{
    this->m_ulExAirFanRatedVol = m_usExAirFanRatedVol_H * 65536 + m_usExAirFanRatedVol_L;
    this->m_ulTotalFreAir = m_usTotalFreAir_H * 65536 + m_usTotalFreAir_L;

    emit systemDataChanged();
}
