#include "system.h"

//#define PORT_NAME "/dev/ttyUSB0"

#define PORT_NAME "/dev/ttyS0"

System* System::g_pSystem = nullptr;

System::System(QObject *parent) :
    QObject(parent)
{

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
