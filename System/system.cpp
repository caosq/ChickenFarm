#include "system.h"
#include "messagebox.h"
#include <QDebug>

#define PORT_NAME "/dev/ttyO3"
//#define PORT_NAME "/dev/ttyUSB0"
//#define PORT_NAME "/dev/ttyS0"
//#define PORT_NAME_DTU "/dev/ttyS2"
//#define PORT_NAME_DTU "/dev/ttyUSB3"
#define PORT_NAME_DTU "/dev/ttyO2"
//#define PORT_NAME_BMS "/dev/ttyUSB0"
//#define PORT_NAME_BMS "/dev/ttyS1"


#define SYS_OFF_LOG_TIME_COUNT  300

#define CONTROLLER_DEV_ADDR       1
#define DEV_SELF_COMM_ADDR        1

#define MODBUS_SLAVE_MAX_DEV_ADDR    1
#define MODBUS_SLAVE_MIN_DEV_ADDR    1

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
#if MB_MASTER_RTU_ENABLE && MB_MASTER_RTU_ENABLED
        g_pSystem->m_Modbus.masterUartConfig(9600, 8, 1, eParityType::None);
        g_pSystem->m_Modbus.initMasterPort(eMBMode::MB_RTU, PORT_NAME, MODBUS_SLAVE_MIN_DEV_ADDR, MODBUS_SLAVE_MAX_DEV_ADDR, false);
#endif
#if MB_MASTER_TCP_ENABLE && MB_MASTER_TCP_ENABLED
        g_pSystem->m_ModbusTCP.initMasterTCPPort("MB_MASTER_TCP", MODBUS_SLAVE_MIN_DEV_ADDR, MODBUS_SLAVE_MAX_DEV_ADDR);
#endif
#if MB_MASTER_DTU_ENABLE && MB_MASTER_RTU_ENABLED
        g_pSystem->m_ModbusDTU.masterUartConfig(9600, 8, 1,eParityType::None);
        g_pSystem->m_ModbusDTU.initMasterPort(eMBMode::MB_RTU, PORT_NAME_DTU, 200, 247, true);
#endif
#if MB_SLAVE_RTU_ENABLE && MB_SLAVE_RTU_ENABLED
        g_pSystem->m_ModbusBMS.slaveUartConfig(9600, 8, 1,eParityType::None);
        g_pSystem->m_ModbusBMS.initSlavePort(eMBMode::MB_RTU, PORT_NAME_BMS, DEV_SELF_COMM_ADDR);
#endif
        g_pSystem->m_pSysModeCmdMonitor = DataMonitor::monitorRegist(&g_pSystem->m_eSystemModeCmd, Monitor::DataType::Uint16t);
        g_pSystem->m_pExAirFanRatedVolMonitor_H = DataMonitor::monitorRegist(&g_pSystem->m_usExAirFanRatedVol_H, Monitor::DataType::Uint16t);
        g_pSystem->m_pExAirFanRatedVolMonitor_L = DataMonitor::monitorRegist(&g_pSystem->m_usExAirFanRatedVol_L, Monitor::DataType::Uint16t);
        g_pSystem->m_pTotalFreAirMonitor_H = DataMonitor::monitorRegist(&g_pSystem->m_usTotalFreAir_H, Monitor::DataType::Uint16t);
        g_pSystem->m_pTotalFreAirMonitor_L = DataMonitor::monitorRegist(&g_pSystem->m_usTotalFreAir_L, Monitor::DataType::Uint16t);

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
#if MB_MASTER_RTU_ENABLE && MB_MASTER_RTU_ENABLED
    m_Controller.initComm(CONTROLLER_DEV_ADDR);
    m_Modbus.masterRegistSlaveDev(&m_Controller.m_sMBSlaveDev);
    if(!m_Modbus.registMasterMode())
    {
        qDebug("modbus port error \n");
    }
#endif
#if MB_MASTER_TCP_ENABLE && MB_MASTER_TCP_ENABLED
    m_Controller.initComm(CONTROLLER_DEV_ADDR);
    m_ModbusTCP.masterRegistSlaveDev(&m_Controller.m_sMBSlaveDev);
    if(!m_ModbusTCP.registMasterMode())
    {
        qDebug("m_ModbusTCP connect error \n");
    }
#endif

#if MB_MASTER_DTU_ENABLE && MB_MASTER_RTU_ENABLED
    m_DTU.initComm();
    m_ModbusDTU.masterRegistDTUDev(&m_DTU.m_sMBDTUDev247, &m_DTU.m_sMBDTUDev200);
    if(!m_ModbusDTU.registMasterMode())
    {
        qDebug("modbusDTU port error \n");
    }
#endif

#if MB_SLAVE_RTU_ENABLE && MB_SLAVE_RTU_ENABLED
    m_BMS.initComm();
    m_ModbusBMS.registSlaveCommData(&m_BMS.m_sBMSCommData);
    if(!m_ModbusBMS.registSlaveMode())
    {
       qDebug("modbusBMS port error \n");
    }
#endif
#if MB_SLAVE_TCP_ENABLE && MB_SLAVE_TCP_ENABLED
    m_BMS.initComm();
    m_ModbusBMS.registTcpCommData(&m_BMS.m_sBMSCommData);
    if(!m_ModbusBMS.registSlaveTcp(DEV_SELF_COMM_ADDR))
    {
        qDebug("m_ModbusBMS registSlaveTCP error \n");
    }
#endif
    connect(&m_Controller, SIGNAL(offlineChanged(bool)), this, SLOT(devOfflineChangedSlot(bool)));
    connect(&m_Controller, SIGNAL(syncChanged(bool)), this, SLOT(syncChangedSlot(bool)));
}

void System::systemTimeOut()
{
    QDateTime datatime = QDateTime::currentDateTime();
    ModularAir* pCurModularAir =nullptr;
    ModularChiller* pModularChiller = nullptr;

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
        for(uint8_t i = 0; i < m_pModularChillers.count(); i++)
        {
             pModularChiller = m_pModularChillers[i];
             pModularChiller->m_usChillerCoolInTemp  = m_usChillerCoolInTemp;
             pModularChiller->m_usChillerCoolOutTemp = m_usChillerCoolOutTemp;
             pModularChiller->m_usChillerHeatInTemp  = m_usChillerHeatInTemp;
             pModularChiller->m_usChillerHeatOutTemp = m_usChillerHeatOutTemp;
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

        emit systemLogChanged(m_xIsLogIn);
    }
    emit systemTimeChanged();
}

void System::devOfflineChangedSlot(bool xVal)
{
    if(!m_xIsInDebug && xVal)
    {
       /* messageBox *pConfirmationBox = new messageBox(messageBox::Question);
        pConfirmationBox->setButtonText(messageBox::Yes,"??????");
        pConfirmationBox->setButtonText(messageBox::No,"??????");
        pConfirmationBox->setInformativeText("???????????????????????????????????????????????????????????????????????????????????????");
        if(pConfirmationBox->exec() == messageBox::Yes)
        {
            qDebug("init 6 \n");
        }*/
    }
}

void System::syncChangedSlot(bool)
{
    if(pConfirmationBox == nullptr)
    {
        pConfirmationBox = new messageBox(messageBox::Message);
    }
    if(!m_Controller.m_xOffline && m_Controller.m_xSynchronizing)
    {
        pConfirmationBox->setInformativeText("???????????????????????????????????????...");
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
        messageBox::instance()->setInformativeText("????????????????????????");
        messageBox::instance()->exec();
        return false;
    }
}

void System::systemDataChanged(Monitor*)
{
    this->m_ulExAirFanRatedVol = m_usExAirFanRatedVol_H * 65536 + m_usExAirFanRatedVol_L;
    this->m_ulTotalFreAir = m_usTotalFreAir_H * 65536 + m_usTotalFreAir_L;

    emit systemDataChanged();
}

void System::systemStackChanged(int32_t iVal)
{
    emit stackChanged(iVal);
}
