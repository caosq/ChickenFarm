#include "system.h"

#define REG_HOLD_BUF_NUM  300
#define BIT_COIL_BUF_NUM  200

QString strPortName ="/dev/ttyS0";

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
    sMBMasterInfo *pMasterInfo = pModbus->masterInit(eMBMode::MB_RTU, strPortName.toUtf8().data(), 1, 1, false);

    if(pMasterInfo != nullptr)
    {
        xMBMasterRegistNode(pMasterInfo, &pModbus->m_sMBMasterNode);
        m_pController = new Controller();

        m_pController->initComm(pMasterInfo, REG_HOLD_BUF_NUM, 0, BIT_COIL_BUF_NUM, 0);
    }
}
