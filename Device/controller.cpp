#include "controller.h"
#include "system.h"
#include "mbmap_m.h"

#define PROTOCOL_TYPE_ID   0

Controller::Controller()
{

}

bool Controller::initComm(sMBMasterInfo* psMBMasterInfo, uint16_t usRegHoldBufNum, uint16_t usRegInBufNum, uint16_t usBitCoilBufNum, uint16_t usBitDiscBufNum)
{
    if(psMBMasterInfo == nullptr)
    {
        return false;
    }
    else
    {
        m_psMBMasterInfo = psMBMasterInfo;
    }
    if(usRegHoldBufNum != 0)
    {
        m_psRegHoldBuf = new sMasterRegHoldData[usRegHoldBufNum];
    }
    if(usRegHoldBufNum != 0)
    {
        m_psBitCoilBuf = new sMasterBitCoilData[usBitCoilBufNum];
    }
    if(m_psRegHoldBuf != nullptr && m_psBitCoilBuf != nullptr)
    {

        registDevCommData();
        return true;
    }
    else
    {
        return false;
    }
}

void Controller::registDevCommData()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr)
    {
        return;
    }
MASTER_PBUF_INDEX_ALLOC

MASTER_TEST_CMD_INIT(this->m_sDevCommData.sMBDevCmdTable, 0, READ_REG_HOLD, 0x302A, TRUE)

MASTER_BEGIN_DATA_BUF(this->m_psRegHoldBuf, this->m_sDevCommData.sMBRegHoldTable)

    MASTER_REG_HOLD_DATA(0, uint16, 0, 65535,  0x302A, RO, 1, pSystem->m_usUnitID);
    MASTER_REG_HOLD_DATA(1, uint16, 1, 65535,  10,     RO, 1, pSystem->m_usProtocolVer);
    MASTER_REG_HOLD_DATA(2, uint16, 0, 3,      0,      RW, 1, pSystem->m_eSystemModeCmd);
    MASTER_REG_HOLD_DATA(3, uint16, 0, 7,      0,      RO, 1, pSystem->m_eSystemState);

MASTER_END_DATA_BUF(0, 3)

MASTER_BEGIN_DATA_BUF(this->m_psBitCoilBuf, this->m_sDevCommData.sMBCoilTable)

    MASTER_COIL_BIT_DATA(0, 0, RW, pSystem->xAlarmEnable);
    MASTER_COIL_BIT_DATA(1, 0, RO, pSystem->xAlarmClean);

MASTER_END_DATA_BUF(0, 1)

    this->m_sDevCommData.ucProtocolID = PROTOCOL_TYPE_ID;
    this->m_sDevCommData.pxDevDataMapIndex = devDataMapIndex;  //绑定映射函数
    this->m_sMBSlaveDev.psDevDataInfo = &(this->m_sDevCommData);
   // (void)xMBMasterRegistDev(this->m_psMBMasterInfo, &this->m_sMBSlaveDev);
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

