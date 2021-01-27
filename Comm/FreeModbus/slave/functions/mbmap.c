#include "mbmap.h"
#include "mbdict.h"

#if MB_FUNC_READ_INPUT_ENABLED > 0
/***********************************************************************************
 * @brief  输入寄存器映射
 * @param  usRegInAddr    寄存器地址
 * @param  pvRegInValue   寄存器指针
 * @return eMBErrorCode 错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBErrorCode 
eMBSlaveRegInMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usRegInAddr, sMBSlaveRegData** pvRegInValue)
{
    USHORT usIndex;
	eMBErrorCode eStatus = MB_ENOERR;
    
    sMBSlaveCommData*            psCurData  = psMBSlaveInfo->sMBCommInfo.psSlaveCurData;
	const sMBSlaveDataTable* psMBRegInTable = &psCurData->sMBRegInTable;
	
    if(psCurData->pxSlaveDataMapIndex == NULL || psMBRegInTable == NULL || psMBRegInTable->pvDataBuf == NULL)
    {
         return MB_EILLSTATE;
    }
    if(psCurData->pxSlaveDataMapIndex(RegInputData, usRegInAddr, &usIndex))    //从栈字典映射函数
	{
        *pvRegInValue = (sMBSlaveRegData*)psMBRegInTable->pvDataBuf + usIndex;
    }
	else
    {
        *pvRegInValue = NULL;
        return MB_ENOREG;
    }
    return eStatus;	
}
#endif

#if MB_FUNC_WRITE_HOLDING_ENABLED > 0 || MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0 \
    || MB_FUNC_READ_HOLDING_ENABLED > 0 || MB_FUNC_READWRITE_HOLDING_ENABLED > 0

/***********************************************************************************
 * @brief  保持寄存器映射
 * @param  usRegHoldAddr    寄存器地址
 * @param  pvRegHoldValue   寄存器指针
 * @return eMBErrorCode 错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBErrorCode 
eMBSlaveRegHoldMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usRegHoldAddr, sMBSlaveRegData** pvRegHoldValue)
{
    USHORT usIndex;
    eMBErrorCode eStatus = MB_ENOERR;
    
    sMBSlaveCommData*              psCurData  = psMBSlaveInfo->sMBCommInfo.psSlaveCurData;
	const sMBSlaveDataTable* psMBRegHoldTable = &psCurData->sMBRegHoldTable;

    if(psCurData->pxSlaveDataMapIndex == NULL || psMBRegHoldTable == NULL || psMBRegHoldTable->pvDataBuf == NULL)
    {
         return MB_EILLSTATE;
    }
	if(psCurData->pxSlaveDataMapIndex(RegHoldData, usRegHoldAddr, &usIndex))    //从栈字典映射函数
    {
        *pvRegHoldValue = (sMBSlaveRegData*)(psMBRegHoldTable->pvDataBuf) + usIndex;
    }
    else
    {
        *pvRegHoldValue = NULL;
        return MB_ENOREG;
    }
    return eStatus;	
}
#endif

#if MB_FUNC_READ_COILS_ENABLED > 0 || MB_FUNC_WRITE_COIL_ENABLED > 0 || MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
/***********************************************************************************
 * @brief  线圈字典映射
 * @param  usCoilAddr    线圈地址
 * @param  pvCoilValue   线圈指针
 * @return eMBErrorCode 错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBErrorCode 
eMBSlaveCoilsMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usCoilAddr, sMBSlaveBitData** pvCoilValue)
{
    USHORT usIndex;
    eMBErrorCode eStatus = MB_ENOERR;
    
	sMBSlaveCommData*           psCurData  = psMBSlaveInfo->sMBCommInfo.psSlaveCurData;
	const sMBSlaveDataTable* psMBCoilTable = &psCurData->sMBCoilTable;
    
    if(psCurData->pxSlaveDataMapIndex == NULL || psMBCoilTable == NULL || psMBCoilTable->pvDataBuf == NULL)
    {
         return MB_EILLSTATE;
    }
	if(psCurData->pxSlaveDataMapIndex(CoilData, usCoilAddr, &usIndex))    //从栈字典映射函数
    {
        *pvCoilValue = (sMBSlaveBitData*)psMBCoilTable->pvDataBuf + usIndex;
    }
	else
    {
        *pvCoilValue = NULL;
        return MB_ENOREG;
    }
    return eStatus;	
} 

#endif

#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
/***********************************************************************************
 * @brief  离散量字典映射
 * @param  usCoilAddr    线圈地址
 * @param  pvCoilValue   线圈指针
 * @return eMBErrorCode 错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBErrorCode 
eMBSlaveDiscreteMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usDiscreteAddr, sMBSlaveBitData** pvDiscreteValue)
{
    USHORT usIndex;
    eMBErrorCode eStatus = MB_ENOERR;

	sMBSlaveCommData*             psCurData  = psMBSlaveInfo->sMBCommInfo.psSlaveCurData;
	const sMBSlaveDataTable* psMBDiscInTable = &psCurData->sMBDiscInTable;
    
    if(psCurData->pxSlaveDataMapIndex == NULL || psMBDiscInTable == NULL || psMBDiscInTable->pvDataBuf == NULL)
    {
         return MB_EILLSTATE;
    }
	if(psCurData->pxSlaveDataMapIndex(DiscInData, usDiscreteAddr, &usIndex))    //从栈字典映射函数
    {
        *pvDiscreteValue = (sMBSlaveBitData*)psMBDiscInTable->pvDataBuf + usIndex;
    }
    else
    {
        *pvDiscreteValue = NULL;
        return MB_ENOREG;
    }
    return eStatus;	
}
#endif

/***********************************************************************************
 * @brief  寄存器数据表初始化
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
void vMBSlaveRegDataInit(sMBSlaveRegData* pData, USHORT usAddr, UCHAR ucDataType, LONG lMinVal, 
                         LONG lMaxVal, UCHAR ucAccessMode, float fTransmitMultiple, void* pvValue)                                  
{
    pData->usAddr            = usAddr;                    
    pData->ucDataType        = ucDataType;                         
    pData->lMinVal           = lMinVal;                  
    pData->lMaxVal           = lMaxVal;                  
    pData->ucAccessMode      = ucAccessMode;        
    pData->fTransmitMultiple = fTransmitMultiple;
    pData->pvValue           = pvValue;  

//    if(usAddr == 37)
//    {
//        myprintf("pData %d  pData->pvValue %d \n",  pData, (uint16_t*)pvValue );
//    }
    
}

/***********************************************************************************
 * @brief  线圈和离散量数据表初始化
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
void vMBSlaveBitDataInit(sMBSlaveBitData* pData, USHORT usAddr, UCHAR ucAccessMode, void* pvValue)                                  
{
    pData->usAddr        = usAddr;                            
    pData->ucAccessMode  = ucAccessMode;        
    pData->pvValue       = pvValue; 

//    if(usAddr == 160)
//    {
//        myprintf("pData %d  pData->pvValue %d \n",  pData, (uint16_t*)pvValue );
//    }    
}

/***********************************************************************************
 * @brief 数据表初始化
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
void vMBSlaveDevDataTableInit(sMBSlaveDataTable* pDataTable, void* pvDataBuf, USHORT usStartAddr, 
                              USHORT usEndAddr, USHORT usDataCount)                                  
{
    pDataTable->pvDataBuf   = pvDataBuf;        //协议数据域
    pDataTable->usStartAddr = usStartAddr;      //起始地址
    pDataTable->usEndAddr   = usEndAddr;        //末尾地址
    pDataTable->usDataCount = usDataCount;      //协议点位总数
    
}
