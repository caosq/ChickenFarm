#include "controller.h"
#include "system.h"
#include "mbmap_m.h"

#define PROTOCOL_TYPE_ID   0

Controller::Controller()
{

}

void Controller::initComm(sMBMasterInfo* psMBMasterInfo, uint8_t ucDevAddr)
{
    if(psMBMasterInfo != nullptr)
    {
        m_psMBMasterInfo = psMBMasterInfo;
        registDevCommData();
    }
    m_sMBSlaveDev.ucDevAddr = ucDevAddr;
}

void Controller::registDevCommData()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr)
    {
        return;
    }
MASTER_PBUF_INDEX_ALLOC

MASTER_TEST_CMD_INIT(m_sDevCommData.sMBDevCmdTable, 0, READ_REG_HOLD, 0x302A, TRUE)

MASTER_BEGIN_DATA_BUF(m_psRegHoldBuf, m_sDevCommData.sMBRegHoldTable)

    MASTER_REG_HOLD_DATA(0, uint16, 0, 65535,  0x302A, RO, 1, pSystem->m_usUnitID);
    MASTER_REG_HOLD_DATA(1, uint16, 1, 65535,  10,     RO, 1, pSystem->m_usProtocolVer);
    MASTER_REG_HOLD_DATA(2, uint16, 0, 3,      0,      RW, 1, pSystem->m_eSystemModeCmd);
    MASTER_REG_HOLD_DATA(3, uint16, 0, 7,      0,      RO, 1, pSystem->m_eSystemState);
    MASTER_REG_HOLD_DATA(4, int16,  -400, 700,      0,      RO, 1, pSystem->m_sAmbientOut_T);

MASTER_END_DATA_BUF(0, 4)

MASTER_BEGIN_DATA_BUF(m_psBitCoilBuf, m_sDevCommData.sMBCoilTable)

    MASTER_COIL_BIT_DATA(0, 0, RW, pSystem->xAlarmEnable);
    MASTER_COIL_BIT_DATA(1, 0, RO, pSystem->xAlarmClean);

MASTER_END_DATA_BUF(0, 1)

    m_sDevCommData.ucProtocolID = PROTOCOL_TYPE_ID;
    m_sDevCommData.pxDevDataMapIndex = devDataMapIndex;  //绑定映射函数
    m_sMBSlaveDev.psDevDataInfo = &m_sDevCommData;

    (void)xMBMasterRegistDev(m_psMBMasterInfo, &m_sMBSlaveDev);
}

uint8_t Controller::devDataMapIndex(eDataType eDataType, uint8_t ucProtocolID, uint16_t usAddr, uint16_t* psIndex)
{
    uint16_t i = 0;
    switch(ucProtocolID)
    {
    case PROTOCOL_TYPE_ID:
        if(eDataType == RegHoldData)
        {
            switch(usAddr)
            {
                case 0 :  i = 0 ;  break;
                case 1 :  i = 1 ;  break;
                case 2 :  i = 2 ;  break;
                case 3 :  i = 3 ;  break;
                case 4 :  i = 4 ;  break;

                default:
                    return false;
                break;
            }
        }
        else if(eDataType == CoilData)
        {
            switch(usAddr)
            {
                case 0  :  i = 0 ;  break;
                case 1  :  i = 1 ;  break;

                default:
                    return FALSE;
                break;
            }
        }
    break;
    default: break;
    }

    *psIndex = i;
    return true;
}

