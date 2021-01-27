#include "controller.h"
#include "mbmap_m.h"

Controller::Controller()
{

}

bool Controller::initComm(sMBMasterInfo* psMBMasterInfo, uint16_t usRegHoldBufNum, uint16_t usRegInBufNum, uint16_t usBitCoilBufNum, uint16_t usBitDiscBufNum)
{
    if(psMBMasterInfo == nullptr)
    {
        return false;
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
        return true;
    }
    else
    {
        return false;
    }
}

void Controller::registDevCommData()
{

MASTER_PBUF_INDEX_ALLOC

MASTER_TEST_CMD_INIT(&this->m_sDevCommData.sMBDevCmdTable, 0, READ_REG_HOLD, 0x302A, TRUE)

MASTER_BEGIN_DATA_BUF(this->m_psRegHoldBuf, &this->m_sDevCommData.sMBRegHoldTable)

   // MASTER_REG_HOLD_DATA(0, uint16,    0, 65535,  0x302A,  RO, 1, (void*)usUnitID);

MASTER_END_DATA_BUF(0, 631)
}

bool Controller::devDataMapIndex(eDataType eDataType, uint8_t ucProtocolID, uint16_t usAddr, uint16_t* psIndex)
{

}

