#include "mbport_m.h"
#include "mb_m.h"
#include "mbframe.h"

#include "mbfunc_m.h"
#include "mbdict_m.h"
#include "mbutils.h"
#include "mbtest_m.h"
#include "mbscan_m.h"
#include "mbdtu_m.h"

#if MB_UCOSIII_ENABLED

#elif MB_LINUX_ENABLED
#include <pthread.h>
#endif

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED

#define MB_SCAN_SLAVE_DELAY_MS             200    //主栈扫描从设备
#define MB_SCAN_SLAVE_INTERVAL_MS          200

#define MB_SCAN_MAX_REG_INTERVAL           10    //寄存器轮询地址最大间隔
#define MB_SCAN_MAX_REG_NUM                50    //寄存器轮询最大数量

#define MB_SCAN_MAX_BIT_INTERVAL           80    //线圈轮询地址最大间隔
#define MB_SCAN_MAX_BIT_NUM                400   //线圈轮询最大数量

#if MB_FUNC_READ_INPUT_ENABLED
/***********************************************************************************
 * @brief  轮询输入寄存器
 * @param  ucSndAddr            从栈地址
 * @return eMBMasterReqErrCode  错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterScanReadRegIn(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, ULONG ulTimeOut)
{
    USHORT iIndex, iStartAddr, iLastAddr, iCount;
    
    eMBMasterReqErrCode       eStatus = MB_MRE_NO_ERR;
    sMasterRegInData* psRegInputValue = NULL;
    sMBDevDataTable*  psMBRegInTable = &psMBSlaveDev->psDevCurData->sMBRegInTable;  //从设备通讯协议表
    //sMBMasterInfo*    psMBMasterInfo = psMBSlaveDev->psMBMasterInfo;

    iLastAddr = 0;
	iStartAddr = 0;
	iCount = 0;
  
    if(psMBRegInTable->pvDataBuf == NULL || psMBRegInTable->usDataCount == 0) //非空且数据点不为0
	{
        return MB_MRE_NO_ERR;
	}
	for(iIndex = 0; iIndex < psMBRegInTable->usDataCount ; iIndex++)    //轮询
	{
		psRegInputValue = (sMasterRegInData*)psMBRegInTable->pvDataBuf + iIndex;

	    /***************************** 读输入寄存器 **********************************/
        if( (psRegInputValue != NULL) && (psRegInputValue->usAddr - iLastAddr + 1) > MB_SCAN_MAX_REG_INTERVAL)    //地址间隔超过最大间隔，则发送读请求
		{
			if( iCount > 0)
			{
                eStatus = eMBMasterReqReadInputRegister(psMBMasterInfo, psMBSlaveDev->ucDevAddr, iStartAddr, iCount, ulTimeOut);
                iCount = 0;	
			}
           	if( psRegInputValue->ucAccessMode != WO )
			{
				iCount = 1;
			    iStartAddr = psRegInputValue->usAddr;
			}
		}
		else
		{	
			if( (iCount == 0) && (psRegInputValue->ucAccessMode != WO) )
			{
				iStartAddr = psRegInputValue->usAddr;
			}
			if( psRegInputValue->ucAccessMode != WO )
			{
				iCount = psRegInputValue->usAddr - iStartAddr + 1;
			}
		}
        // 1. 寄存器为只写 2. 到达数据域末尾 3.数据超过Modbus数据帧最大字节数，则发送读请求
        if( (psRegInputValue->ucAccessMode == WO || iIndex == psMBRegInTable->usDataCount-1 || iCount >= MB_SCAN_MAX_REG_NUM) && (iCount > 0) ) 
        {
            eStatus = eMBMasterReqReadInputRegister(psMBMasterInfo, psMBSlaveDev->ucDevAddr, iStartAddr, iCount, ulTimeOut);
            iCount = 0; 
        }
        iLastAddr = psRegInputValue->usAddr;		
	}
	return eStatus;
}
#endif

#if MB_FUNC_READ_HOLDING_ENABLED
/***********************************************************************************
 * @brief  轮询保持寄存器
 * @param  ucSndAddr            从栈地址
 * @param  xCheckPreValue       检查先前值
 * @return eMBMasterReqErrCode  错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode
eMBMasterScanReadRegHold(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, ULONG ulTimeOut)
{
    BOOL   bStarted;
    USHORT iIndex, iReadStartRegAddr, iLastAddr, iReadCount;

    eMBMasterReqErrCode eStatus = MB_MRE_NO_ERR;
    sMasterRegHoldData* psRegHoldValue = NULL;
    sMBDevDataTable* psMBRegHoldTable  = &psMBSlaveDev->psDevCurData->sMBRegHoldTable; //从设备通讯协议表
    //sMBMasterInfo* psMBMasterInfo = psMBSlaveDev->psMBMasterInfo;

    iLastAddr = 0;
    iReadStartRegAddr = 0;
    iReadCount = 0;
    bStarted = FALSE;

//    debug("ucSlaveAddr %d  eMBMasterScanWriteHoldingRegister\n", ucSndAddr);

    if( (psMBRegHoldTable == NULL) || (psMBRegHoldTable->pvDataBuf  == NULL) || (psMBRegHoldTable->usDataCount == 0)) //非空且数据点不为0
    {
        return eStatus;
    }
    for(iIndex = 0; iIndex < psMBRegHoldTable->usDataCount; iIndex++)  //轮询
    {
        psRegHoldValue = (sMasterRegHoldData*)psMBRegHoldTable->pvDataBuf + iIndex;

        /***************************** 读保持寄存器 **********************************/
        if( (psRegHoldValue != NULL) && (psRegHoldValue->usAddr - iLastAddr + 1) > MB_SCAN_MAX_REG_INTERVAL)    //地址间隔超过最大间隔，则发送读请求
        {
            if(iReadCount > 0)
            {
                eStatus = eMBMasterReqReadHoldingRegister(psMBMasterInfo, psMBSlaveDev->ucDevAddr, iReadStartRegAddr, iReadCount, ulTimeOut);
                iReadCount = 0;
            }
            if(psRegHoldValue->ucAccessMode != WO)
            {
                iReadCount = 1;
                iReadStartRegAddr = psRegHoldValue->usAddr;
            }
        }
        else   //地址连续
        {
            if(iReadCount == 0 && psRegHoldValue->ucAccessMode != WO)
            {
                iReadStartRegAddr = psRegHoldValue->usAddr;
            }
            if(psRegHoldValue->ucAccessMode != WO)
            {
                iReadCount = psRegHoldValue->usAddr - iReadStartRegAddr + 1;
            }
        }
        //1.寄存器为只写，2. 到达数据域的末尾，3. 数据超过Modbus数据帧最大字节数 发送读请求
        if( (psRegHoldValue->ucAccessMode == WO || iIndex == psMBRegHoldTable->usDataCount-1 || iReadCount >= MB_SCAN_MAX_REG_NUM) && (iReadCount > 0) )
        {
            eStatus = eMBMasterReqReadHoldingRegister(psMBMasterInfo, psMBSlaveDev->ucDevAddr,
                                                      iReadStartRegAddr, iReadCount, ulTimeOut);
            iReadCount = 0;
        }
        iLastAddr = psRegHoldValue->usAddr;
    }
    return eStatus;
}
#endif

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED || MB_FUNC_WRITE_HOLDING_ENABLED
/***********************************************************************************
 * @brief  写保持寄存器
 *************************************************************************************/
eMBMasterReqErrCode 
eMBMasterWriteRegHold(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, USHORT usRegAddr,
                      USHORT usNRegs, USHORT* pusDataBuffer, ULONG ulTimeOut)
{
    //sMBMasterInfo* psMBMasterInfo = psMBSlaveDev->psMBMasterInfo;

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED && MB_FUNC_WRITE_HOLDING_ENABLED
    if(usNRegs == 1)      //写单个寄存器 根据实际情况调整有些设备不支持06功能码
    {
        return eMBMasterReqWriteHoldingRegister(psMBMasterInfo, ucSndAddr, usRegAddr, *(USHORT*)pusDataBuffer, lTimeOut);
    }
    else if(usNRegs > 1) //写多个寄存器
    {
        return eMBMasterReqWriteMultipleHoldingRegister(psMBMasterInfo, psMBSlaveDev->ucDevAddr, usRegAddr,
                                                        usNRegs, pusDataBuffer, ulTimeOut);
    }
#elif MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED
    return eMBMasterReqWriteMultipleHoldingRegister(psMBMasterInfo, psMBSlaveDev->ucDevAddr, usRegAddr,
                                                    usNRegs, pusDataBuffer, ulTimeOut);
#endif
}
#endif

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED || MB_FUNC_WRITE_HOLDING_ENABLED
/***********************************************************************************
 * @brief  轮询保持寄存器
 * @param  ucSndAddr            从栈地址
 * @param  xCheckPreValue       检查先前值
 * @return eMBMasterReqErrCode  错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode
eMBMasterScanWriteRegHold(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, BOOL xCheckPreValue, ULONG ulTimeOut)
{
    BOOL   bStarted;
    USHORT iIndex, iWriteStartRegAddr, iLastAddr, iWriteCount, iRegs, nRegs;

    USHORT usRegHoldValue = 0;
    SHORT  sRegHoldValue  = 0;
    int8_t cRegHoldValue  =0;

    eMBMasterReqErrCode eStatus = MB_MRE_NO_ERR;
    sMasterRegHoldData* psRegHoldValue = NULL;
    sMBDevDataTable* psMBRegHoldTable  = &psMBSlaveDev->psDevCurData->sMBRegHoldTable; //从设备通讯协议表

    iLastAddr = 0;
    iWriteStartRegAddr =0;
    iWriteCount = 0;
    iRegs = 0;
    bStarted = FALSE;
//    debug("ucSlaveAddr %d  eMBMasterScanWriteHoldingRegister\n", ucSndAddr);
    if( (psMBRegHoldTable == NULL) || (psMBRegHoldTable->pvDataBuf  == NULL) || (psMBRegHoldTable->usDataCount == 0)) //非空且数据点不为0
    {
        return eStatus;
    }
    for(iIndex = 0; iIndex < psMBRegHoldTable->usDataCount; iIndex++)  //轮询
    {
        psRegHoldValue = (sMasterRegHoldData*)psMBRegHoldTable->pvDataBuf + iIndex;

        /******************* 写保持寄存器***************************/
        if( (psRegHoldValue != NULL) && (psRegHoldValue->pvValue != NULL) && (psRegHoldValue->ucAccessMode != RO) )   //寄存器非只读
        {
            switch (psRegHoldValue->ucDataType)
            {
            case uint16:
                usRegHoldValue = *(USHORT*)psRegHoldValue->pvValue;
                if( psRegHoldValue->ucTmitMult != 1)
                {
                    usRegHoldValue = (USHORT)(usRegHoldValue * psRegHoldValue->ucTmitMult ); //传输因子
                }
                if( (USHORT)psRegHoldValue->usPreVal != usRegHoldValue || xCheckPreValue == FALSE )  //变量变化且或者不检查是否变化
                {
                    if( (usRegHoldValue >= (USHORT)psRegHoldValue->usMinVal) &&
                            (usRegHoldValue <= (USHORT)psRegHoldValue->usMaxVal) )
                    {
                        psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)usRegHoldValue;
                        iWriteCount++;
                    }
                }
            break;
            case uint8:
                usRegHoldValue = *(UCHAR*)psRegHoldValue->pvValue;
                if( psRegHoldValue->ucTmitMult != 1.0f)
                {
                    usRegHoldValue =  (UCHAR)(usRegHoldValue * psRegHoldValue->ucTmitMult); //传输因子
                }
                if( (USHORT)psRegHoldValue->usPreVal != usRegHoldValue || xCheckPreValue == FALSE )  //变量变化且或者不检查是否变化
                {
                    if( (usRegHoldValue >= (USHORT)psRegHoldValue->usMinVal) &&
                         (usRegHoldValue <= (USHORT)psRegHoldValue->usMaxVal) )
                    {
                        psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)usRegHoldValue;
                        iWriteCount++;
                    }
                }
            break;
            case int16:
                sRegHoldValue = *(SHORT*)psRegHoldValue->pvValue;
                if( psRegHoldValue->ucTmitMult != 1.0f)
                {
                    sRegHoldValue = (SHORT)( (float)sRegHoldValue * (float)psRegHoldValue->ucTmitMult ); //传输因子
                }
                if( (SHORT)psRegHoldValue->usPreVal != sRegHoldValue || xCheckPreValue == FALSE )  //变量变化且或者不检查是否变化
                {
                    if( (sRegHoldValue >= (SHORT)psRegHoldValue->usMinVal) &&
                            (sRegHoldValue <= (SHORT)psRegHoldValue->usMaxVal) )
                    {
                        psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)sRegHoldValue;
                        iWriteCount++;
                    }
                }
            break;
            case int8:
                cRegHoldValue = *(int8_t*)psRegHoldValue->pvValue;
                if( psRegHoldValue->ucTmitMult != 1.0f)
                {
                    cRegHoldValue = (int8_t)( (float)cRegHoldValue * (float)psRegHoldValue->ucTmitMult ); //传输因子
                }
                if( (int8_t)psRegHoldValue->usPreVal != cRegHoldValue || xCheckPreValue == FALSE )  //变量变化且或者不检查是否变化
                {
                    if( (cRegHoldValue >= (int8_t)psRegHoldValue->usMinVal) &&
                            (cRegHoldValue <= (int8_t)psRegHoldValue->usMaxVal) )
                    {
                        psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)cRegHoldValue;
                        iWriteCount++;
                    }
                }
            break;
            }
        }
        iRegs++;

        if(iWriteCount == 1 && (bStarted != TRUE))    //记录首地址
        {
            iWriteStartRegAddr = psRegHoldValue->usAddr;
            bStarted = TRUE;
            iRegs = 1;
        }
        if( psRegHoldValue->usAddr != iLastAddr+1 && iWriteCount > 0 && iRegs > 1)    //地址不连续，则发送写请求
        {
            if(iRegs == iWriteCount)    //地址不连续且当前寄存器也发生了变化
            {
                nRegs = iWriteCount-1;
                eStatus = eMBMasterWriteRegHold(psMBMasterInfo, psMBSlaveDev, iWriteStartRegAddr, iWriteCount-1,
                                               (USHORT*)psMBMasterInfo->RegHoldValList, ulTimeOut);	//写寄存器
                iWriteCount = 1;    //记录当前位置
                iRegs = 1;
                bStarted = TRUE;
                iWriteStartRegAddr = psRegHoldValue->usAddr;

                psMBMasterInfo->RegHoldValList[0] = psMBMasterInfo->RegHoldValList[nRegs];
            }
            else                         //地址不连续但当前寄存器也没有变化
            {
                nRegs = iWriteCount;
                eStatus = eMBMasterWriteRegHold(psMBMasterInfo, psMBSlaveDev, iWriteStartRegAddr, iWriteCount,
                                               (USHORT*)psMBMasterInfo->RegHoldValList, ulTimeOut);	//写寄存器
                iWriteCount = 0;
                iRegs = 0;
                bStarted = FALSE;
            }
        }
        else if(iWriteCount > 0)  //地址连续
        {
            //1. 未发生变化  2.地址到达字典最后 3.数据超过Modbus数据帧最大字节数，则发送写请求
            if( (iRegs != iWriteCount) || (iIndex == psMBRegHoldTable->usDataCount-1) || (iWriteCount >= MB_SCAN_MAX_REG_NUM))
            {
                nRegs = iWriteCount;
                eStatus = eMBMasterWriteRegHold(psMBMasterInfo, psMBSlaveDev, iWriteStartRegAddr, iWriteCount,
                                               (USHORT*)psMBMasterInfo->RegHoldValList, ulTimeOut);	//写寄存器
                iWriteCount = 0;
                iRegs = 0;
                bStarted = FALSE;
            }
        }
        iLastAddr = psRegHoldValue->usAddr;
    }
    return eStatus;
}
#endif

#if MB_FUNC_READ_COILS_ENABLED
/***********************************************************************************
 * @brief  轮询线圈
 * @param  ucSndAddr            从栈地址
 * @return eMBMasterReqErrCode  错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterScanReadCoils(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, ULONG ulTimeOut)
{
    USHORT iIndex, iLastAddr, iReadStartCoilAddr, iReadCount;

    eMBMasterReqErrCode eStatus = MB_MRE_NO_ERR;
    sMasterBitCoilData* psCoilValue = NULL;
    sMBDevDataTable* psMBCoilTable = &psMBSlaveDev->psDevCurData->sMBCoilTable;    //从设备通讯协议表

    iLastAddr = 0;
    iReadStartCoilAddr = 0;
    iReadCount = 0;
//    debug("ucSlaveAddr %d  eMBMasterScanWriteCoils\n", ucSndAddr);
    if( (psMBCoilTable == NULL) ||(psMBCoilTable->pvDataBuf == NULL) || (psMBCoilTable->usDataCount == 0)) //非空且数据点不为0
    {
        return eStatus;
    }
    for(iIndex = 0; iIndex < psMBCoilTable->usDataCount; iIndex++)
    {
        psCoilValue = (sMasterBitCoilData*)(psMBCoilTable->pvDataBuf) + iIndex;
//      debug("psCoilValue  ucSndAddr %d  usCoilAddr %d pvValue %d ucPreVal %d\n\n",
//                ucSndAddr, psCoilValue->usAddr, *(UCHAR*)psCoilValue->pvValue, psCoilValue->ucPreVal);

        if( (psCoilValue != NULL) && (psCoilValue->usAddr - iLastAddr + 1) > MB_SCAN_MAX_BIT_INTERVAL)     //地址间隔超过最大间隔，则发送读请求
        {
            if(iReadCount>0)
            {
                eStatus = eMBMasterReqReadCoils(psMBMasterInfo, psMBSlaveDev->ucDevAddr,
                                                iReadStartCoilAddr, iReadCount, ulTimeOut);
                iReadCount = 0;
                iReadStartCoilAddr = psCoilValue->usAddr;
            }
            if( psCoilValue->ucAccessMode != WO )
            {
                iReadCount = 1;
                iReadStartCoilAddr = psCoilValue->usAddr;
            }
        }
        else  //连续
        {
            if(iReadCount == 0)
            {
                iReadStartCoilAddr = psCoilValue->usAddr;
            }
            if(psCoilValue->ucAccessMode != WO)
            {
                iReadCount = psCoilValue->usAddr - iReadStartCoilAddr + 1;
            }
        }
        //1. 只写 2.数据超过Modbus数据帧最大字节数，3. 到达数据域末尾， 发送读请求
        if( (psCoilValue->ucAccessMode == WO || iIndex == psMBCoilTable->usDataCount-1
            || iReadCount >= MB_SCAN_MAX_BIT_NUM) && (iReadCount>0) )
        {
            eStatus = eMBMasterReqReadCoils(psMBMasterInfo, psMBSlaveDev->ucDevAddr,iReadStartCoilAddr, iReadCount, ulTimeOut);
            if(eStatus == MB_MRE_ETIMEDOUT)
            {
                return eStatus;
            }
            iReadCount = 0;
        }
        iLastAddr = psCoilValue->usAddr;
    }
    return eStatus;
}
#endif

#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED
/*************************************************************************************
 * @brief  写线圈
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterWriteCoil(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, USHORT usCoilAddr,
                                       USHORT usNCoils, UCHAR* pucDataBuffer, ULONG ulTimeOut)
{
    eMBMasterReqErrCode eStatus = MB_MRE_NO_ERR;
    //sMBMasterInfo* psMBMasterInfo = psMBSlaveDev->psMBMasterInfo;
    USHORT usMBBitData = 0;

#if MB_FUNC_WRITE_COIL_ENABLED
    if(usNCoils == 1)      //写单个线圈
    {
        usMBBitData = (*(UCHAR*)pucDataBuffer >0) ? 0xFF00 : 0x0000;

      //debug("eMBMasterWriteCoil ucSndAddr  %d usCoilAddr %d  usMBBitData %d %d %d\n\n",ucSndAddr,
      //          usCoilAddr, usMBBitData, *(UCHAR*)pucDataBuffer, pSystem->psModularRoofList[0]->xErrClean);

        return = eMBMasterReqWriteCoil(psMBMasterInfo, psMBSlaveDev->ucDevAddr, usCoilAddr, usMBBitData, ulTimeOut);
    }
    else if(usNCoils > 1)  //写多个线圈
    {
        eStatus = eMBMasterReqWriteMultipleCoils(psMBMasterInfo, psMBSlaveDev->ucDevAddr, usCoilAddr,
                                                 usNCoils, (UCHAR*)pucDataBuffer, ulTimeOut);
    }
#elif MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED
    eStatus = eMBMasterReqWriteMultipleCoils(psMBMasterInfo, psMBSlaveDev->ucDevAddr, usCoilAddr,
                                             usNCoils, (UCHAR*)pucDataBuffer, ulTimeOut);
#endif
    //(void)vMBTimeDly(1, 0);
    return eStatus;
}
#endif

#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED
/***********************************************************************************
 * @brief  轮询线圈
 * @param  ucSndAddr            从栈地址
 * @return eMBMasterReqErrCode  错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterScanWriteCoils(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, BOOL xCheckPreValue, ULONG ulTimeOut)
{
    UCHAR iBitInByte, cByteValue;
    BOOL bStarted;
    USHORT iIndex, iLastAddr, iWriteStartCoilAddr, iWriteBytes, iWriteBits, iBits;
  
	eMBMasterReqErrCode      eStatus = MB_MRE_NO_ERR;
	sMasterBitCoilData*  psCoilValue = NULL;
    //sMBMasterInfo* psMBMasterInfo = psMBSlaveDev->psMBMasterInfo;
    sMBDevDataTable* psMBCoilTable = &psMBSlaveDev->psDevCurData->sMBCoilTable;    //从设备通讯协议表
   
	iLastAddr = 0;
	iWriteStartCoilAddr = 0;
	iWriteBits = 0;
	iWriteBytes = 0;
    iBitInByte = 0;
	iBits = 0;
    cByteValue = 0;
	bStarted = FALSE;
	
//    debug("ucSlaveAddr %d  eMBMasterScanWriteCoils\n", ucSndAddr);
	if( (psMBCoilTable == NULL) ||(psMBCoilTable->pvDataBuf == NULL) || (psMBCoilTable->usDataCount == 0)) //非空且数据点不为0
	{
		return eStatus;
	}
	for(iIndex = 0; iIndex < psMBCoilTable->usDataCount; iIndex++)
	{
		psCoilValue = (sMasterBitCoilData*)(psMBCoilTable->pvDataBuf) + iIndex;
        
//      debug("psCoilValue  ucSndAddr %d  usCoilAddr %d pvValue %d ucPreVal %d\n\n",
//                ucSndAddr, psCoilValue->usAddr, *(UCHAR*)psCoilValue->pvValue, psCoilValue->ucPreVal);

        /******************* 写线圈***************************/
        if ( (psCoilValue != NULL) && (psCoilValue->pvValue != NULL) && (psCoilValue->ucAccessMode != RO) )   //非只读
        {
            if( psCoilValue->ucPreVal != *(UCHAR*)(psCoilValue->pvValue) || xCheckPreValue == FALSE ) //线圈发生变化或者不检查是否发生变化
            {
                if(iBitInByte%8 ==0)   //byte中bit的偏移
                {
                    cByteValue = 0;
                    iBitInByte = 0;
                    iWriteBytes++;   //字节+1  8bit就增加1byte
                }
                if( *(UCHAR*)psCoilValue->pvValue > 0 )  //线圈状态为1
                {
                    cByteValue |= (1<<iBitInByte);	     //组成一个字节
                }
                iBitInByte++;      //byte中bit的偏移+1
                iWriteBits++;	   //发生变化的线圈数量+1
                psMBMasterInfo->BitCoilByteValList[iWriteBytes-1] = cByteValue;    //记录要下发的byte数据
            }
        }
        iBits++;  //线圈计数+1

        if( iWriteBits == 1 && bStarted == FALSE )
        {
            iWriteStartCoilAddr = psCoilValue->usAddr;     //记录线圈起始地址
            bStarted = TRUE;
            iBits = 1;
            iWriteBytes = 1;
        }
        if( psCoilValue->usAddr != iLastAddr + 1 )   //地址不连续
        {
            if( iWriteBits>0 && iBits>1 )
            {
                if(iBits == iWriteBits)   //线圈地址不连续，且该线圈地址也发生了变化
                {
                    eStatus = eMBMasterWriteCoil(psMBMasterInfo, psMBSlaveDev, iWriteStartCoilAddr, iWriteBits-1,
                                                 psMBMasterInfo->BitCoilByteValList, ulTimeOut);	     //写线圈
                    iWriteBits = 1;    //写完一帧得复位所有状态,
                    iWriteBytes = 1;
                    iBits = 1;
                    cByteValue = 0;
                    iBitInByte = 0;
                    bStarted = TRUE;

                    iWriteStartCoilAddr = psCoilValue->usAddr;       //以该线圈地址为第一个地址
                    if( *(UCHAR*)psCoilValue->pvValue > 0 ) //重新记录，
                    {
                        cByteValue |= (1 << iBitInByte);
                    }
                    psMBMasterInfo->BitCoilByteValList[0]  = cByteValue;
                    iBitInByte++;
                }
                else if(iBits > iWriteBits)  //线圈地址不连续，但该线圈没发生变化
                {
                    eStatus = eMBMasterWriteCoil(psMBMasterInfo, psMBSlaveDev, iWriteStartCoilAddr, iWriteBits,
                                                 psMBMasterInfo->BitCoilByteValList, ulTimeOut);	//写线圈
                    iWriteBits = 0;      //写完一帧得复位所有状态
                    iWriteBytes = 0;
                    iBits = 0;
                    cByteValue = 0;
                    iBitInByte = 0;
                    bStarted = FALSE;
                }
            }
        }
        else //地址连续
        {
            if(iWriteBits > 0)
            {
                //1. 未发生变化  2.地址到达字典最后 3.数据超过Modbus数据帧最大字节数，则发送写请求
                if( (iBits > iWriteBits) || (iIndex == psMBCoilTable->usDataCount-1)
                    || (iWriteBits >= MB_SCAN_MAX_BIT_NUM) )
                {
                    eStatus = eMBMasterWriteCoil(psMBMasterInfo, psMBSlaveDev, iWriteStartCoilAddr, iWriteBits,
                                                 psMBMasterInfo->BitCoilByteValList, ulTimeOut);	//写线圈
                    iWriteBits = 0;
                    iWriteBytes = 0;
                    iBits = 0;
                    bStarted = FALSE;
                    cByteValue = 0;
                    iBitInByte = 0;
                }
            }
        }
        iLastAddr = psCoilValue->usAddr;
	}
	return eStatus;
}
#endif

#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
/***********************************************************************************
 * @brief  轮询读离散量字典
 * @param  ucSndAddr            从栈地址
 * @return eMBMasterReqErrCode  错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterScanReadDiscs(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr)
{
    USHORT iIndex, iLastAddr, iStartAddr, iCount;
   
    eMBMasterReqErrCode             eStatus = MB_MRE_NO_ERR;
    sMasterBitDiscData*      pDiscreteValue = NULL;
    
    sMBSlaveDev*     psMBSlaveDevCur = psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur;     //当前从设备
    sMBDevDataTable* psMBDiscInTable = &psMBSlaveDevCur->psDevCurData->sMBDiscInTable;  //从设备通讯协议表

	iLastAddr = 0;
	iStartAddr = 0;
	iCount = 0;
	
    if(psMBSlaveDevCur->ucDevAddr != ucSndAddr) //如果当前从设备地址与要轮询从设备地址不一致，则更新从设备
    {
        psMBSlaveDevCur = psMBMasterGetDev(psMBMasterInfo, ucSndAddr);
        psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDevCur;
        psMBDiscInTable = &psMBSlaveDevCur->psDevCurData->sMBDiscInTable;
    } 
    if( (psMBDiscInTable->pvDataBuf == NULL) || (psMBDiscInTable->usDataCount == 0)) //非空且数据点不为0
	{
		return eStatus;
	}
	
	for(iIndex = 0; iIndex < psMBDiscInTable->usDataCount ; iIndex++)
	{
		pDiscreteValue = (sMasterBitDiscData*)psMBDiscInTable->pvDataBuf + iIndex;	
	
        if( (pDiscreteValue->usAddr-iLastAddr+1) > MB_SCAN_MAX_BIT_INTERVAL)  //地址间隔超过最大间隔，则发送读请求
		{
			if(iCount > 0)
            {
                eStatus = eMBMasterReadDiscreteInputs(psMBMasterInfo, ucSndAddr, iStartAddr, iCount, MB_MASTER_WAITING_DELAY);
                iCount = 0;
				iStartAddr = pDiscreteValue->usAddr;
			}
			if(pDiscreteValue->ucAccessMode != WO)
			{
				iCount++;
				iStartAddr = pDiscreteValue->usAddr;
			}
		}
		else
		{
			if(iCount == 0)
			{
				iStartAddr = pDiscreteValue->usAddr;
			}	
			if(pDiscreteValue->ucAccessMode != WO)
			{
				iCount = pDiscreteValue->usAddr - iStartAddr +1;
			}	
		}
        //1. 只写 2.数据超过Modbus数据帧最大字节数，3. 到达数据域末尾， 发送读请求
		if( (pDiscreteValue->ucAccessMode == WO || iIndex== psMBDiscInTable->usDataCount-1 || iCount >= MB_SCAN_MAX_BIT_NUM) && (iCount > 0) )  
		{
             eStatus = eMBMasterReqReadDiscreteInputs(psMBMasterInfo, ucSndAddr, iStartAddr, iCount, MB_MASTER_WAITING_DELAY);	
             iCount = 0;
		}	
        iLastAddr = pDiscreteValue->usAddr ;		
	}
	return eStatus;
}
#endif

/**********************************************************************
 * @brief   主栈轮询某个从设备
 * @param   psMBMasterInfo  主栈信息块
 * @param   ucSlaveAddr      从设备地址
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
void vMBMasterScanReadSlaveDevData(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, BOOL xSynchronize)
{
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    //sMBMasterInfo* psMBMasterInfo = psMBSlaveDev->psMBMasterInfo;
    psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDev;
    if(xSynchronize)
    {
        psMBMasterInfo->eMBRunMode = STATE_SYSN_DEV;
    }
    else
    {
        psMBMasterInfo->eMBRunMode = STATE_SCAN_DEV;
    }

#if MB_FUNC_READ_HOLDING_ENABLED
    errorCode = eMBMasterScanReadRegHold(psMBMasterInfo, psMBSlaveDev, MB_MASTER_TIMEOUT_MS_RESPOND); //保持寄存器
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDev->xStateTestRequest = TRUE;
        psMBSlaveDev->xSynchronized = FALSE;
        return;
    }         
#endif
					
#if MB_FUNC_READ_COILS_ENABLED
    errorCode = eMBMasterScanReadCoils(psMBMasterInfo, psMBSlaveDev, MB_MASTER_TIMEOUT_MS_RESPOND);           //线圈
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDev->xStateTestRequest = TRUE;
        psMBSlaveDev->xSynchronized = FALSE;
        return;
    }     
#endif
					
#if MB_FUNC_READ_INPUT_ENABLED
    errorCode = eMBMasterScanReadRegIn(psMBMasterInfo, psMBSlaveDev, MB_MASTER_TIMEOUT_MS_RESPOND);	  //读输入寄存器
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDev->xStateTestRequest = TRUE;
        psMBSlaveDev->xSynchronized = FALSE;
        return;
    }         
#endif
				
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED
    errorCode = eMBMasterScanReadDiscs(psMBMasterInfo, ucSlaveAddr);   //读离散量
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDevCur->xStateTestRequest = TRUE;
        psMBSlaveDevCur->xSynchronized = FALSE;
        return;
    }     
#endif
}

/**********************************************************************
 * @brief   主栈轮询某个从设备
 * @param   psMBMasterInfo  主栈信息块
 * @param   ucSlaveAddr      从设备地址
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
void vMBMasterScanWriteSlaveDevData(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, BOOL xCheckPreValue)
{
    eMBMasterReqErrCode errorCode = MB_MRE_NO_ERR;
    //sMBMasterInfo* psMBMasterInfo = psMBSlaveDev->psMBMasterInfo;

    psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDev;
    psMBMasterInfo->eMBRunMode = STATE_SCAN_DEV;

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED || MB_FUNC_WRITE_HOLDING_ENABLED
    errorCode = eMBMasterScanWriteRegHold(psMBMasterInfo, psMBSlaveDev, xCheckPreValue, MB_MASTER_TIMEOUT_MS_RESPOND); //保持寄存器
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDev->xStateTestRequest = TRUE;
        psMBSlaveDev->xSynchronized = FALSE;
        return;
    }
#endif

#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED
    errorCode = eMBMasterScanWriteCoils(psMBMasterInfo, psMBSlaveDev, xCheckPreValue, MB_MASTER_TIMEOUT_MS_RESPOND);           //线圈
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDev->xStateTestRequest = TRUE;
        psMBSlaveDev->xSynchronized = FALSE;
        return;
    }
#endif
}

/**********************************************************************
 * @brief   主栈轮询某个从设备
 * @param   psMBMasterInfo  主栈信息块 
 * @param   psMBSlaveDev    从设备
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
void vMBMasterScanReadSlaveDev(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev)
{
    //psMBSlaveDev->psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDev;
    if(psMBSlaveDev->psDevCurData == NULL){ return;}   //数据表为空则不进行轮询

    if( (psMBSlaveDev != NULL) && (psMBSlaveDev->xOnLine == TRUE) )       //如果设备在线则进行轮询
    {
        if(psMBSlaveDev->xDataReady == TRUE)
        {	 	    
            if(psMBSlaveDev->xSynchronized == FALSE)
            {
                vMBMasterScanReadSlaveDevData(psMBMasterInfo, psMBSlaveDev, TRUE);  //同步从设备数据
                psMBSlaveDev->xSynchronized = TRUE;
            }
            else   //同步完成后
            {
                vMBMasterScanReadSlaveDevData(psMBMasterInfo, psMBSlaveDev, FALSE);
            }
        }
//        debug("************vMBMasterScanSlaveDev  ucSlaveAddr %d  xDataReady %d  xSynchronized %d**************\n",
//        ucSlaveAddr, psMBSlaveDev->xDataReady, psMBSlaveDev->xSynchronized);
    }		
}

/**********************************************************************
 * @brief   主栈轮询某个从设备
 * @param   psMBMasterInfo  主栈信息块
 * @param   psMBSlaveDev    从设备
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
void vMBMasterScanWriteSlaveDev(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev)
{
    //psMBSlaveDev->psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDev;
    if(psMBSlaveDev->psDevCurData == NULL){ return;}   //数据表为空则不进行轮询

    if( (psMBSlaveDev != NULL) && (psMBSlaveDev->xOnLine == TRUE) )    //如果设备在线则进行轮询
    {
        if(psMBSlaveDev->xDataReady == TRUE)
        {
            if(psMBSlaveDev->xSynchronized == TRUE)
            {
                vMBMasterScanWriteSlaveDevData(psMBMasterInfo, psMBSlaveDev, TRUE);   //写从设备数据
            }
        }
        else  //从设备数据未好，则只进行写不读
        {
            if(psMBSlaveDev->xSynchronized == FALSE)
            {
                vMBMasterScanWriteSlaveDevData(psMBMasterInfo, psMBSlaveDev, FALSE);
            }
        }
//        debug("************vMBMasterScanSlaveDev  ucSlaveAddr %d  xDataReady %d  xSynchronized %d**************\n",
//        ucSlaveAddr, psMBSlaveDev->xDataReady, psMBSlaveDev->xSynchronized);
    }
}

/**********************************************************************
 * @brief   主栈轮询从设备任务
 * @param   *p_arg    
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
#if MB_UCOSIII_ENABLED
void vMBMasterScanReadSlaveDevTask(void *p_arg)
#elif MB_LINUX_ENABLED
void* vMBMasterScanReadSlaveDevTask(void *p_arg)    
#endif
{
    sMBSlaveDev*       psMBSlaveDev   = NULL;
    sMBMasterInfo*     psMBMasterInfo = (sMBMasterInfo*)p_arg;
    sMBMasterDevsInfo* psMBDevsInfo   = &psMBMasterInfo->sMBDevsInfo;  //从设备状态信息

    UCHAR ucMaxAddr = psMBDevsInfo->ucSlaveDevMaxAddr;
    UCHAR ucMinAddr = psMBDevsInfo->ucSlaveDevMinAddr;

#if MB_MASTER_DTU_ENABLED     //GPRS模块首次上电
    if(psMBMasterInfo->bDTUEnable)
    {
        psMBDevsInfo->psMBDTUDev200->xOnLine = TRUE;
        psMBDevsInfo->psMBDTUDev247->xOnLine = TRUE;

        vDTUDevInit(psMBMasterInfo);
    }
#endif
    //(void)vMBTimeDly(3, 0);
    while(1)
    {
        (void)vMBTimeDly(0, MB_SCAN_SLAVE_INTERVAL_MS);
        if(psMBMasterInfo->eMBState == STATE_NOT_INITIALIZED){continue;}

        //debug("**********************************************\n");
        for(psMBSlaveDev = psMBDevsInfo->psMBSlaveDevsList; psMBSlaveDev != NULL; psMBSlaveDev = psMBSlaveDev->pNext)
        {
            if(psMBSlaveDev->xOnLine == FALSE)   //如果设备不在线
            {
                vMBDevTest(psMBMasterInfo, psMBSlaveDev);  //测试
            }
        }
        //debug("--------------vMBMasterScanReadSlaveDevTask------------\n");
        
        /*********************************轮询从设备***********************************/
        for(psMBSlaveDev = psMBDevsInfo->psMBSlaveDevsList; psMBSlaveDev != NULL; psMBSlaveDev = psMBSlaveDev->pNext)
        {
            if( psMBSlaveDev->xOnLine == TRUE && psMBSlaveDev->ucDevAddr <= ucMaxAddr && psMBSlaveDev->ucDevAddr >= ucMinAddr )
            {
                if(psMBSlaveDev->xStateTestRequest == TRUE) //测试请求
                {
                    vMBDevCurStateTest(psMBMasterInfo, psMBSlaveDev);  //检测从设备是否掉线
                }
                if(psMBSlaveDev->xOnLine == TRUE && psMBSlaveDev->ucOfflineTimes == 0) //在线且不处于延时阶段
                {
                    vMBMasterScanReadSlaveDev(psMBMasterInfo, psMBSlaveDev);
                }
                //debug("vMBMasterScanSlaveDevTask  %d  psMBSlaveDev->xOnLine %d\n", psMBSlaveDev->ucDevAddr, psMBSlaveDev->xOnLine);
            }
        }
    }
}

#if MB_UCOSIII_ENABLED
void vMBMasterScanWriteSlaveDevTask(void *p_arg)
#elif MB_LINUX_ENABLED
void* vMBMasterScanWriteSlaveDevTask(void *p_arg)   
#endif
{
    sMBSlaveDev*       psMBSlaveDev   = NULL;
    sMBMasterInfo*     psMBMasterInfo = (sMBMasterInfo*)p_arg;
    sMBMasterDevsInfo* psMBDevsInfo   = &psMBMasterInfo->sMBDevsInfo;  //从设备状态信息

    UCHAR ucMaxAddr = psMBDevsInfo->ucSlaveDevMaxAddr;
    UCHAR ucMinAddr = psMBDevsInfo->ucSlaveDevMinAddr;

#if MB_MASTER_DTU_ENABLED     //GPRS模块首次上电
    if(psMBMasterInfo->bDTUEnable)
    {
        psMBDevsInfo->psMBDTUDev200->xOnLine = TRUE;
        psMBDevsInfo->psMBDTUDev247->xOnLine = TRUE;

        vDTUDevInit(psMBMasterInfo);
    }
#endif

    (void)vMBTimeDly(3, 0);
    while(1)
    {
        (void)vMBTimeDly(0, MB_SCAN_SLAVE_INTERVAL_MS);
        if(psMBMasterInfo->eMBState == STATE_NOT_INITIALIZED){continue;}

        /*********************************轮询从设备***********************************/
        for(psMBSlaveDev = psMBDevsInfo->psMBSlaveDevsList; psMBSlaveDev != NULL; psMBSlaveDev = psMBSlaveDev->pNext)
        {
            if(psMBSlaveDev->xOnLine == TRUE && psMBSlaveDev->ucOfflineTimes == 0 && 
               psMBSlaveDev->ucDevAddr <= ucMaxAddr && psMBSlaveDev->ucDevAddr >= ucMinAddr)
            {
                {
                    vMBMasterScanWriteSlaveDev(psMBMasterInfo, psMBSlaveDev);
                }
            }
        }
        //debug("+++++++++++++++++++vMBMasterScanWriteSlaveDevTask++++++++++++++++++\n");
        
#if MB_MASTER_DTU_ENABLED    //GPRS模块功能支持，特殊处理
        if(psMBMasterInfo->bDTUEnable == TRUE)
        {
            vDTUScanDev(psMBMasterInfo);
        }
#endif
    }
}

/**********************************************************************
 * @brief   创建主栈轮询从设备任务
 * @param   *p_arg    
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
BOOL xMBMasterCreateScanTask(sMBMasterInfo* psMBMasterInfo)
{   
#if MB_UCOSIII_ENABLED
    OS_ERR err = OS_ERR_NONE;
    CPU_STK_SIZE stk_size = MB_MASTER_SCAN_TASK_STK_SIZE; 
    sMBMasterTask* psMBTask = &psMBMasterInfo->sMBTask;
    
    OS_PRIO  prio = psMBTask->ucMasterScanReadPrio;
    OS_TCB*  p_tcb = (OS_TCB*)(&psMBTask->sMasterScanReadTCB);  
    CPU_STK* p_stk_base = (CPU_STK*)(psMBTask->usMasterScanReadStk);
    
    OSTaskCreate(p_tcb, "vMBMasterScanReadSlaveDevTask", vMBMasterScanReadSlaveDevTask, (void*)psMBMasterInfo, prio, p_stk_base,
                 stk_size/10u, stk_size, 0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    
    prio = psMBTask->ucMasterScanWritePrio;
    p_tcb = (OS_TCB*)(&psMBTask->sMasterScanWriteTCB);  
    p_stk_base = (CPU_STK*)(psMBTask->usMasterScanWriteStk); 
    
    OSTaskCreate(p_tcb, "vMBMasterScanWriteSlaveDevTask", vMBMasterScanWriteSlaveDevTask, (void*)psMBMasterInfo, prio, p_stk_base,
                 stk_size/10u, stk_size, 0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    
    return (err == OS_ERR_NONE);

#elif MB_LINUX_ENABLED
    if(pthread_create(&psMBMasterInfo->sMBTask.sMBScanReadTask, NULL, vMBMasterScanReadSlaveDevTask, (void*)psMBMasterInfo) < 0)  //创建线程
    {
        return FALSE;
    }
    if(pthread_create(&psMBMasterInfo->sMBTask.sMBScanWriteTask, NULL, vMBMasterScanWriteSlaveDevTask, (void*)psMBMasterInfo) < 0)  //创建线程
    {
        return FALSE;
    }
    return TRUE;
#endif
}

#endif
