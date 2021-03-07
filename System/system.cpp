#include "system.h"
#include "messagebox.h"
#include <QDebug>

//#define PORT_NAME "/dev/ttyS0"
#define PORT_NAME "/dev/ttyUSB0"

#define SYS_OFF_LOG_TIME_COUNT  300
#define CONTROLLER_DEV_ADDR     1

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
        g_pSystem->m_Modbus.initMasterPort(eMBMode::MB_RTU, PORT_NAME, 1, 1, false);

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
    m_Controller.initComm(CONTROLLER_DEV_ADDR);
    g_pSystem->m_Modbus.masterRegistSlaveDev(&m_Controller.m_sMBSlaveDev);
    g_pSystem->m_Modbus.registMasterMode();

    connect(&m_Controller, SIGNAL(offlineChanged(bool)), this, SLOT(devOfflineChangedSlot(bool)));
    connect(&m_Controller, SIGNAL(syncChanged(bool)), this, SLOT(syncChangedSlot(bool)));
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
    if((m_xIsLogIn || m_xIsFactoryLogIn) && m_xIsInDebug == false)
    {
        m_uiOffLogCount++;
    }
    if(m_uiOffLogCount == SYS_OFF_LOG_TIME_COUNT)
    {
        m_xIsLogIn = false;
        m_xIsFactoryLogIn = false;
        m_uiOffLogCount = 0;
    }
    emit systemTimeChanged();
}

void System::devOfflineChangedSlot(bool xVal)
{
    if(!m_xIsInDebug && xVal)
    {
       /* messageBox *pConfirmationBox = new messageBox(messageBox::Question);
        pConfirmationBox->setButtonText(messageBox::Yes,"确认");
        pConfirmationBox->setButtonText(messageBox::No,"取消");
        pConfirmationBox->setInformativeText("控制器通讯故障，请检查设备联接，并重启设备，是否重启设备？");
        if(pConfirmationBox->exec() == messageBox::Yes)
        {
            qDebug("init 6 \n");
        }*/
    }
}

void System::syncChangedSlot(bool xVal)
{
    if(!xVal)
    {
        if(pConfirmationBox == nullptr)
        {
            pConfirmationBox = new messageBox(messageBox::Message);
        }
        pConfirmationBox->setInformativeText("正在同步控制器数据，请稍等...");
        pConfirmationBox->show();
    }
    else
    {
        pConfirmationBox->hide();
    }
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
