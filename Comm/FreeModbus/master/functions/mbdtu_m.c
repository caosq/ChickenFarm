#include "mbdtu_m.h"
#include "mbscan_m.h"
#include "mbfunc_m.h"
#include "mbtest_m.h"

#if MB_MASTER_DTU_ENABLED && MB_MASTER_RTU_ENABLED

#define DTU_TIMEOUT_S      60
#define DTU_PROTOCOL_VERSIPON   0x7f1b

#define DTU200_SLAVE_ADDR  200
#define DTU247_SLAVE_ADDR  247

#define DTU247_INIT_START_REG_HOLD_ADDR   0x00
#define INIT_DTU247_REG_HOLD_VALUE        0xF0
#define INITED_DTU247_REG_HOLD_VALUE      0x0F

#define TEST_DTU200_PROTOCOL_REG_HOLD_ADDR      0
#define TEST_DTU200_PROTOCOL_REG_HOLD_VALUE     0x5601

#define TEST_DTU247_PROTOCOL_REG_HOLD_ADDR      1
#define TEST_DTU247_PROTOCOL_REG_HOLD_VALUE     0x0F

#define TEST_DTU247_PROTOCOL_REG_IN_ADDR        0
#define TEST_DTU247_PROTOCOL_REG_IN_VALUE       0xFF

#define MB_MASTER_DTU_TIMEOUT_MS    1000

#define MB_SCAN_MAX_REG_NUM         100    //寄存器轮询最大数量



USHORT usDTUInitCmd[]  = {DTU_PROTOCOL_VERSIPON, INIT_DTU247_REG_HOLD_VALUE};
USHORT usDTUInitedCmd[]= {DTU_PROTOCOL_VERSIPON, INITED_DTU247_REG_HOLD_VALUE};

USHORT usDTUDataCmd[]= {DTU_PROTOCOL_VERSIPON, INITED_DTU247_REG_HOLD_VALUE, 0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 1};

void vDTUDevInit(sMBMasterInfo* psMBMasterInfo)
{
    UCHAR n, ucSlaveDevMaxAddr, ucSlaveDevMinAddr;
    eMBMasterReqErrCode   errorCode     = MB_MRE_EILLSTATE;
    sMBMasterDevsInfo*    psMBDevsInfo  = &psMBMasterInfo->sMBDevsInfo;   //从设备状态表

    sMBSlaveDev*  psDevDTU247 = psMBDevsInfo->psMBDTUDev247;
    sMBSlaveDev*  psDevDTU200 = psMBDevsInfo->psMBDTUDev200;

    if(psMBMasterInfo->bDTUEnable == FALSE || psDevDTU247 == NULL || psDevDTU200 == NULL || psDevDTU247->xDevOnTimeout == TRUE)
    {
        return;
    }
    psMBMasterInfo->eMBRunMode = STATE_TEST_DEV;  //接口处于测试从设备状态

    /***********************开始测试DTU的状态*************************************/
    ucSlaveDevMaxAddr = psMBDevsInfo->ucSlaveDevMaxAddr;  //切换最大，最小地址
    ucSlaveDevMinAddr = psMBDevsInfo->ucSlaveDevMinAddr;
    psMBDevsInfo->ucSlaveDevMaxAddr = DTU247_SLAVE_ADDR;
    psMBDevsInfo->ucSlaveDevMinAddr = DTU200_SLAVE_ADDR;

    if(psDevDTU247->xOnLine == FALSE || psDevDTU200->xOnLine == FALSE)
    {
        for (n = 0; n < 100; n++)
        {
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED
            errorCode = eMBMasterReqWriteMultipleHoldingRegister(psMBMasterInfo, DTU247_SLAVE_ADDR, DTU247_INIT_START_REG_HOLD_ADDR,
                                                                 2, usDTUInitCmd, MB_MASTER_DTU_TIMEOUT_MS);	//进行初始化
#endif
            if(errorCode == MB_MRE_NO_ERR){break;}
        }
    }
    for (n = 0; n < 50; n++)
    {
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED
        errorCode = eMBMasterReqWriteMultipleHoldingRegister(psMBMasterInfo, DTU247_SLAVE_ADDR, DTU247_INIT_START_REG_HOLD_ADDR,
                                                             2, usDTUInitedCmd, MB_MASTER_DTU_TIMEOUT_MS); //查看GPRS模块是否已完成初始化
#endif
        if(errorCode == MB_MRE_NO_ERR){break;}
    }
    if (errorCode != MB_MRE_NO_ERR)  //未完成初始化
    {
        psDevDTU247->xOnLine = FALSE;
        psDevDTU200->xOnLine = FALSE;

        psDevDTU247->xDataReady = FALSE;
        psDevDTU200->xDataReady = FALSE;
        (void)xMBMasterDevOfflineTmrEnable(psDevDTU247, DTU_TIMEOUT_S);
    }
    else   //GPRS模块已经初始化
    {
        psDevDTU247->xOnLine = TRUE;
        psDevDTU200->xOnLine = TRUE;
        psDevDTU247->xDataReady = TRUE;
    }
    psMBDevsInfo->ucSlaveDevMaxAddr = ucSlaveDevMaxAddr;
    psMBDevsInfo->ucSlaveDevMinAddr = ucSlaveDevMinAddr;
}

/**********************************************************************
 * @brief   DTU测试
 * @return	none
 *********************************************************************/
void vDTUDevTest(sMBMasterInfo* psMBMasterInfo)
{
    USHORT usDataVal = 0;
    UCHAR ucSlaveDevMaxAddr, ucSlaveDevMinAddr;
    UCHAR* pcPDUDataCur = NULL;
    eMBMasterReqErrCode   errorCode     = MB_MRE_NO_ERR;
    sMBMasterDevsInfo*    psMBDevsInfo  = &psMBMasterInfo->sMBDevsInfo;   //从设备状态表

    sMBSlaveDev*  psDevDTU247 = psMBDevsInfo->psMBDTUDev247;
    sMBSlaveDev*  psDevDTU200 = psMBDevsInfo->psMBDTUDev200;

    if(psDevDTU247->xOnLine == TRUE && psDevDTU200->xOnLine == TRUE)
    {
        psMBMasterInfo->eMBRunMode = STATE_TEST_DEV;

        /***********************开始测试DTU的状态*************************************/
        ucSlaveDevMaxAddr = psMBDevsInfo->ucSlaveDevMaxAddr;  //切换最大，最小地址
        ucSlaveDevMinAddr = psMBDevsInfo->ucSlaveDevMinAddr;
        psMBDevsInfo->ucSlaveDevMaxAddr = DTU247_SLAVE_ADDR;
        psMBDevsInfo->ucSlaveDevMinAddr = DTU200_SLAVE_ADDR;

#if MB_FUNC_READ_INPUT_ENABLED > 0
        errorCode = eMBMasterReqReadInputRegister(psMBMasterInfo, DTU247_SLAVE_ADDR, 9, 1, MB_MASTER_DTU_TIMEOUT_MS); //查看GPRS模块状态
#endif
        if (errorCode != MB_MRE_NO_ERR)  //GPRS模块参数
        {
            if(psDevDTU247->ucOfflineTimes > 2)
            {
                psDevDTU247->xOnLine = FALSE;
                psDevDTU200->xOnLine = FALSE;
            }
            else
            {
                psDevDTU247->ucOfflineTimes++;
            }
        }
        else
        {
            pcPDUDataCur = psMBMasterInfo->pucMasterPDUCur + MB_PDU_VALUE_OFF;  //当前帧的数据域
            usDataVal  = ( (USHORT)(*pcPDUDataCur++) ) << 8;   //数据
            usDataVal |= ( (USHORT)(*pcPDUDataCur++) ) & 0xFF;
            if(usDataVal == 2)  //传输链路故障
            {
                if(psDevDTU247->ucOfflineTimes > 2)
                {
                    psDevDTU247->xOnLine = FALSE;
                    psDevDTU200->xOnLine = FALSE;
                }
                else
                {
                    psDevDTU247->ucOfflineTimes++;
                }
            }
            else
            {
                 psDevDTU247->ucOfflineTimes = 0;
                 psDevDTU247->xOnLine = TRUE;
                 psDevDTU200->xOnLine = TRUE;
            }
        }
    }
    else //不在线则进行初始化
    {
        vDTUDevInit(psMBMasterInfo);
    }
    psMBDevsInfo->ucSlaveDevMaxAddr = ucSlaveDevMaxAddr;
    psMBDevsInfo->ucSlaveDevMinAddr = ucSlaveDevMinAddr;
}

eMBMasterReqErrCode
eMBMasterScanWriteDTUHoldReg(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, ULONG ulTimeOut)
{

    USHORT iIndex, iAddr, iWriteStartRegAddr, iWriteCount;

    USHORT usRegHoldValue = 0;
    SHORT  sRegHoldValue  = 0;
    int8_t cRegHoldValue  =0;

    eMBMasterReqErrCode eStatus = MB_MRE_NO_ERR;
    sMasterRegHoldData* psRegHoldValue = NULL;
    sMBDevDataTable* psMBRegHoldTable  = &psMBSlaveDev->psDevCurData->sMBRegHoldTable; //从设备通讯协议表
    //sMBMasterInfo* psMBMasterInfo = psMBSlaveDev->psMBMasterInfo;

    iWriteStartRegAddr =0;
    iWriteCount = 0;

//    debug("ucSlaveAddr %d  eMBMasterScanWriteHoldingRegister\n", ucSndAddr);

    if( (psMBRegHoldTable == NULL) || (psMBRegHoldTable->pvDataBuf  == NULL) || (psMBRegHoldTable->usDataCount == 0)) //非空且数据点不为0
    {
        return MB_MRE_EILLSTATE;
    }
    for(iAddr = 0, iIndex = 0; iAddr < psMBRegHoldTable->usEndAddr && iIndex < psMBRegHoldTable->usDataCount; iAddr++)  //轮询
    {
        psRegHoldValue = (sMasterRegHoldData*)psMBRegHoldTable->pvDataBuf + iIndex;
        if( (psRegHoldValue != NULL) && (psRegHoldValue->pvValue != NULL) && (psRegHoldValue->ucAccessMode != RO) )   //寄存器非只读
        {
            if(psRegHoldValue->usAddr != iAddr)
            {
                psMBMasterInfo->RegHoldValList[iWriteCount] = 0;
                iWriteCount++;
            }
            else
            {
                switch (psRegHoldValue->ucDataType)
                {
                case uint16:
                    usRegHoldValue = *(USHORT*)psRegHoldValue->pvValue;
                    if( psRegHoldValue->ucTmitMult != 1)
                    {
                        usRegHoldValue = (USHORT)( (float)usRegHoldValue * (float)psRegHoldValue->ucTmitMult ); //传输因子
                    }
                    if( (usRegHoldValue >= (USHORT)psRegHoldValue->usMinVal) && (usRegHoldValue <= (USHORT)psRegHoldValue->usMaxVal) )
                    {
                        psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)usRegHoldValue;
                        iWriteCount++;
                    }
                break;
                case uint8:
                    usRegHoldValue = *(UCHAR*)psRegHoldValue->pvValue;
                    if( psRegHoldValue->ucTmitMult != 1.0f)
                    {
                        usRegHoldValue =  (UCHAR)( (float)usRegHoldValue * (float)psRegHoldValue->ucTmitMult ); //传输因子
                    }

                    if( (usRegHoldValue >= (USHORT)psRegHoldValue->usMinVal) && (usRegHoldValue <= (USHORT)psRegHoldValue->usMaxVal) )
                    {
                        psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)usRegHoldValue;
                        iWriteCount++;
                    }
                break;
                case int16:
                    sRegHoldValue = *(SHORT*)psRegHoldValue->pvValue;
                    if( psRegHoldValue->ucTmitMult != 1.0f)
                    {
                        sRegHoldValue = (SHORT)( (float)sRegHoldValue * (float)psRegHoldValue->ucTmitMult ); //传输因子
                    }

                    if( (sRegHoldValue >= (SHORT)psRegHoldValue->usMinVal) && (sRegHoldValue <= (SHORT)psRegHoldValue->usMaxVal) )
                    {
                        psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)sRegHoldValue;
                        iWriteCount++;
                    }
                break;
                case int8:
                    cRegHoldValue = *(int8_t*)psRegHoldValue->pvValue;
                    if( psRegHoldValue->ucTmitMult != 1.0f)
                    {
                        cRegHoldValue = (int8_t)( (float)cRegHoldValue * (float)psRegHoldValue->ucTmitMult ); //传输因子
                    }

                    if( (cRegHoldValue >= (int8_t)psRegHoldValue->usMinVal) && (cRegHoldValue <= (int8_t)psRegHoldValue->usMaxVal) )
                    {
                        psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)cRegHoldValue;
                        iWriteCount++;
                    }
                break;
                }
                iIndex++;
            }
        }
        if(iWriteCount == 1)    //记录首地址
        {
            iWriteStartRegAddr = iAddr;
        }
        if(iWriteCount >= MB_SCAN_MAX_REG_NUM || iAddr == psMBRegHoldTable->usEndAddr-1)
        {
            eStatus = eMBMasterWriteRegHold(psMBMasterInfo, psMBSlaveDev, iWriteStartRegAddr, iWriteCount,
                                            (USHORT*)psMBMasterInfo->RegHoldValList, ulTimeOut);	//写寄存器
            iWriteCount = 0;
        }
    }
    return eStatus;
}

/**********************************************************************
 * @brief   DTU轮询
 * @return	none
 *********************************************************************/
void vDTUScanDev(sMBMasterInfo* psMBMasterInfo)
{
    UCHAR ucSlaveDevMaxAddr, ucSlaveDevMinAddr;

    eMBMasterReqErrCode  errorCode    = MB_MRE_EILLSTATE;
    sMBMasterDevsInfo*   psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;   //从设备状态表

    sMBSlaveDev*  psDevDTU247 = psMBDevsInfo->psMBDTUDev247;
    sMBSlaveDev*  psDevDTU200 = psMBDevsInfo->psMBDTUDev200;

    if(psDevDTU247 == NULL || psDevDTU200 == NULL || psDevDTU247->xDevOnTimeout == TRUE)
    {
        return;
    }
    /***********************开始测试DTU的状态*************************************/
    ucSlaveDevMaxAddr = psMBDevsInfo->ucSlaveDevMaxAddr;  //切换最大，最小地址
    ucSlaveDevMinAddr = psMBDevsInfo->ucSlaveDevMinAddr;
    psMBDevsInfo->ucSlaveDevMaxAddr = DTU247_SLAVE_ADDR;
    psMBDevsInfo->ucSlaveDevMinAddr = DTU200_SLAVE_ADDR;

    vDTUDevTest(psMBMasterInfo);

    if( psDevDTU247->xOnLine == TRUE)
    {
#if MB_FUNC_READ_INPUT_ENABLED
        errorCode = eMBMasterReqReadInputRegister(psMBMasterInfo, DTU247_SLAVE_ADDR, TEST_DTU247_PROTOCOL_REG_IN_ADDR,
                                                  17, MB_MASTER_DTU_TIMEOUT_MS);     //查看GPRS模块
#endif

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED
        errorCode = eMBMasterReqWriteMultipleHoldingRegister(psMBMasterInfo, DTU247_SLAVE_ADDR, DTU247_INIT_START_REG_HOLD_ADDR,
                                                             14, usDTUDataCmd, MB_MASTER_DTU_TIMEOUT_MS);
#endif
    }
    if( psDevDTU200->xOnLine == TRUE)   //轮询DTU模块数据表
    {
        if(psDevDTU200->psDevCurData == NULL)  //数据表为空则不进行轮询
        {
            psMBDevsInfo->ucSlaveDevMaxAddr = ucSlaveDevMaxAddr;
            psMBDevsInfo->ucSlaveDevMinAddr = ucSlaveDevMinAddr;
            return;
        }
        psMBMasterInfo->eMBRunMode = STATE_SCAN_DTU;
        (void)vMBTimeDly(2, 0);

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED
        errorCode = eMBMasterScanWriteDTUHoldReg(psMBMasterInfo, psDevDTU200, MB_MASTER_DTU_TIMEOUT_MS);
#endif
    }
    psMBDevsInfo->ucSlaveDevMaxAddr = ucSlaveDevMaxAddr;
    psMBDevsInfo->ucSlaveDevMinAddr = ucSlaveDevMinAddr;
}

BOOL xMBMasterRegistDTUDev(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBDTUDev247, sMBSlaveDev* psMBDTUDev200)
{
    sMBMasterDevsInfo* psMBDevsInfo = NULL;
    if(psMBMasterInfo == NULL || psMBDTUDev247 == NULL || psMBDTUDev200 == NULL)
    {
        return FALSE;
    }
    psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;
    psMBDevsInfo->psMBDTUDev247 = psMBDTUDev247;
    psMBDevsInfo->psMBDTUDev200 = psMBDTUDev200;

    return TRUE;
}

#endif
