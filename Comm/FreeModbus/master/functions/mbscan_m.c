#include "mbport_m.h"
#include "mb_m.h"
#include "mbframe.h"

#include "mbfunc_m.h"
#include "mbdict_m.h"
#include "mbtest_m.h"
#include "mbscan_m.h"

#if MB_UCOSIII_ENABLED

#elif MB_LINUX_ENABLED
#include <pthread.h>
#endif

#define MB_SCAN_SLAVE_DELAY_MS             200    //主栈扫描从设备
#define MB_SCAN_SLAVE_INTERVAL_MS          200

#define MB_SCAN_MAX_REG_INTERVAL           10    //寄存器轮询地址最大间隔
#define MB_SCAN_MAX_REG_NUM                50    //寄存器轮询最大数量

#define MB_SCAN_MAX_BIT_INTERVAL           80    //线圈轮询地址最大间隔
#define MB_SCAN_MAX_BIT_NUM                400   //线圈轮询最大数量

#if MB_FUNC_READ_INPUT_ENABLED > 0
/***********************************************************************************
 * @brief  轮询输入寄存器
 * @param  ucSndAddr            从栈地址
 * @return eMBMasterReqErrCode  错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterScanReadInputRegister( sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr )
{
    USHORT iIndex, iStartAddr, iLastAddr, iCount;
    
    eMBMasterReqErrCode         eStatus = MB_MRE_NO_ERR;
    sMasterRegInData*   psRegInputValue = NULL;
    sMBSlaveDev*        psMBSlaveDevCur = psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur ;   //当前从设备
    sMBDevDataTable*     psMBRegInTable = &psMBSlaveDevCur->psDevCurData->sMBRegInTable;  //从设备通讯协议表

    iLastAddr = 0;
	iStartAddr = 0;
	iCount = 0;
  
    if(psMBSlaveDevCur->ucDevAddr != ucSndAddr) //如果当前从设备地址与要轮询从设备地址不一致，则更新从设备
    {
        psMBSlaveDevCur = psMBMasterGetDev(psMBMasterInfo, ucSndAddr);
        psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDevCur;
        psMBRegInTable = &psMBSlaveDevCur->psDevCurData->sMBRegInTable;
    }
	if( (psMBRegInTable->pvDataBuf == NULL) || (psMBRegInTable->usDataCount == 0) ) //非空且数据点不为0
	{
		return eStatus;
	}

	for(iIndex = 0; iIndex < psMBRegInTable->usDataCount ; iIndex++)    //轮询
	{
		psRegInputValue = (sMasterRegInData*)psMBRegInTable->pvDataBuf + iIndex;

	    /***************************** 读输入寄存器 **********************************/
		if( (psRegInputValue->usAddr - iLastAddr + 1) > MB_SCAN_MAX_REG_INTERVAL)    //地址间隔超过最大间隔，则发送读请求
		{
			if( iCount > 0)
			{
				eStatus = eMBMasterReqReadInputRegister(psMBMasterInfo, ucSndAddr, iStartAddr, iCount, MB_MASTER_WAITING_DELAY);
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
        	eStatus = eMBMasterReqReadInputRegister(psMBMasterInfo, ucSndAddr, iStartAddr, iCount, MB_MASTER_WAITING_DELAY);
            iCount = 0; 
        }
        iLastAddr = psRegInputValue->usAddr;		
	}
	return eStatus;
}
#endif


#if MB_FUNC_READ_HOLDING_ENABLED > 0 || MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0 || MB_FUNC_WRITE_HOLDING_ENABLED > 0
/***********************************************************************************
 * @brief  写保持寄存器
 *************************************************************************************/
eMBMasterReqErrCode 
eMBMasterReqWriteHoldReg(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usRegAddr, USHORT usNRegs, USHORT* pusDataBuffer, LONG lTimeOut)
{
    if(usNRegs == 1)      //写单个寄存器
    {
        return eMBMasterReqWriteHoldingRegister(psMBMasterInfo, ucSndAddr, usRegAddr, *(USHORT*)pusDataBuffer, lTimeOut);
    }
    else if(usNRegs > 1) //写多个寄存器
    {
        return eMBMasterReqWriteMultipleHoldingRegister(psMBMasterInfo, ucSndAddr, usRegAddr, usNRegs, pusDataBuffer, lTimeOut);
    }
    return MB_MRE_NO_ERR;
}

/***********************************************************************************
 * @brief  轮询保持寄存器
 * @param  ucSndAddr            从栈地址
 * @param  xCheckPreValue       检查先前值
 * @return eMBMasterReqErrCode  错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterScanHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, BOOL xWriteEn, BOOL xReadEn, BOOL xCheckPreValue)
{
    BOOL   bStarted;
    USHORT iIndex, iReadStartRegAddr, iWriteStartRegAddr, iLastAddr, iReadCount, iWriteCount, iRegs, nRegs;
	
    USHORT usRegHoldValue = 0;
    SHORT  sRegHoldValue  = 0;
	int8_t cRegHoldValue  =0;

	eMBMasterReqErrCode eStatus        = MB_MRE_NO_ERR;
    sMasterRegHoldData* psRegHoldValue = NULL;
    
    sMBSlaveDev*     psMBSlaveDevCur  = psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur;     //当前从设备
    sMBDevDataTable* psMBRegHoldTable = &psMBSlaveDevCur->psDevCurData->sMBRegHoldTable; //从设备通讯协议表
	
	iLastAddr = 0;
	iReadStartRegAddr = 0;
    iWriteStartRegAddr =0;
    
    iReadCount = 0;
	iWriteCount = 0;
    
	iRegs = 0;
	bStarted = FALSE;
	
//    debug("ucSlaveAddr %d  eMBMasterScanWriteHoldingRegister\n", ucSndAddr);
 
    if(psMBSlaveDevCur->ucDevAddr != ucSndAddr) //如果当前从设备地址与要轮询从设备地址不一致，则更新从设备
    {
        psMBSlaveDevCur = psMBMasterGetDev(psMBMasterInfo, ucSndAddr);
        psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDevCur;
        psMBRegHoldTable = &psMBSlaveDevCur->psDevCurData->sMBRegHoldTable;
    }
    if( (psMBRegHoldTable->pvDataBuf  == NULL) || (psMBRegHoldTable->usDataCount == 0)) //非空且数据点不为0
	{
		return eStatus;
	}
    
	for(iIndex = 0; iIndex < psMBRegHoldTable->usDataCount; iIndex++)  //轮询
	{
		psRegHoldValue = (sMasterRegHoldData*)psMBRegHoldTable->pvDataBuf + iIndex;
		
		/******************* 写保持寄存器***************************/
        if(xWriteEn)
        {
            if( (psRegHoldValue != NULL) && (psRegHoldValue->pvValue != NULL) && (psRegHoldValue->ucAccessMode != RO) )   //寄存器非只读
		    {
		    	switch (psRegHoldValue->ucDataType)
                {	
                case uint16:	
                    usRegHoldValue = *(USHORT*)psRegHoldValue->pvValue;
                    if( psRegHoldValue->fTransmitMultiple != 1.0f)
		    	    {
		    	    	usRegHoldValue = (USHORT)( (float)usRegHoldValue * (float)psRegHoldValue->fTransmitMultiple ); //传输因子
		    	    }
		    	    if( (USHORT)psRegHoldValue->usPreVal != usRegHoldValue || xCheckPreValue == FALSE )  //变量变化且或者不检查是否变化
                    {		
                        if( (usRegHoldValue >= (USHORT)psRegHoldValue->lMinVal) && (usRegHoldValue <= (USHORT)psRegHoldValue->lMaxVal) )
                        {
                        	psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)usRegHoldValue;
                            iWriteCount++;
                        }
                   	}	
                break;     
                case uint8: 		
                   	usRegHoldValue = *(UCHAR*)psRegHoldValue->pvValue;
                    if( psRegHoldValue->fTransmitMultiple != 1.0f)
		    		{
		    			usRegHoldValue =  (UCHAR)( (float)usRegHoldValue * (float)psRegHoldValue->fTransmitMultiple ); //传输因子
		    		}
                   	if( (USHORT)psRegHoldValue->usPreVal != usRegHoldValue || xCheckPreValue == FALSE )  //变量变化且或者不检查是否变化
                   	{
                        if( (usRegHoldValue >= (USHORT)psRegHoldValue->lMinVal) && (usRegHoldValue <= (USHORT)psRegHoldValue->lMaxVal) )
                        {
                            psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)usRegHoldValue;
                            iWriteCount++;
                        }			
                   	}	
                break;    
                case int16:		
                   	sRegHoldValue = *(SHORT*)psRegHoldValue->pvValue;
                    if( psRegHoldValue->fTransmitMultiple != 1.0f)
		    		{
		                sRegHoldValue = (SHORT)( (float)sRegHoldValue * (float)psRegHoldValue->fTransmitMultiple ); //传输因子
		    		}
                   	if( (SHORT)psRegHoldValue->usPreVal != sRegHoldValue || xCheckPreValue == FALSE )  //变量变化且或者不检查是否变化
                   	{
                        if( (sRegHoldValue >= (SHORT)psRegHoldValue->lMinVal) && (sRegHoldValue <= (SHORT)psRegHoldValue->lMaxVal) )
                        {
                            psMBMasterInfo->RegHoldValList[iWriteCount] = (USHORT)sRegHoldValue;
                            iWriteCount++;
                        }
                   	}		
                break; 
                case int8:
                    cRegHoldValue = *(int8_t*)psRegHoldValue->pvValue;
                    if( psRegHoldValue->fTransmitMultiple != 1.0f)
		    	    {
		    	        cRegHoldValue = (int8_t)( (float)cRegHoldValue * (float)psRegHoldValue->fTransmitMultiple ); //传输因子
		    	    }
                    if( (int8_t)psRegHoldValue->usPreVal != cRegHoldValue || xCheckPreValue == FALSE )  //变量变化且或者不检查是否变化
                    {
                        if( (cRegHoldValue >= (int8_t)psRegHoldValue->lMinVal) && (cRegHoldValue <= (int8_t)psRegHoldValue->lMaxVal) )
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
            if( psRegHoldValue->usAddr != iLastAddr+1 && iWriteCount>0 && iRegs>1 )    //地址不连续，则发送写请求
            {
       	        if(iRegs == iWriteCount)    //地址不连续且当前寄存器也发生了变化
       	        {
                    nRegs = iWriteCount-1;
       	            eStatus = eMBMasterReqWriteHoldReg(psMBMasterInfo, ucSndAddr, iWriteStartRegAddr, iWriteCount-1, 
       	                                              (USHORT*)psMBMasterInfo->RegHoldValList, MB_MASTER_WAITING_DELAY);	//写寄存器
                    iWriteCount = 1;    //记录当前位置
       	            iRegs = 1;
       	            bStarted = TRUE;
       	            iWriteStartRegAddr = psRegHoldValue->usAddr;
                    
      	            psMBMasterInfo->RegHoldValList[0] = psMBMasterInfo->RegHoldValList[nRegs];
       	        }
       	        else                         //地址不连续但当前寄存器也没有变化
       	        {
                    nRegs = iWriteCount;
       	            eStatus = eMBMasterReqWriteHoldReg(psMBMasterInfo, ucSndAddr, iWriteStartRegAddr, iWriteCount, 
       	                                              (USHORT*)psMBMasterInfo->RegHoldValList, MB_MASTER_WAITING_DELAY);	//写寄存器
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
                    eStatus = eMBMasterReqWriteHoldReg(psMBMasterInfo, ucSndAddr, iWriteStartRegAddr, iWriteCount, 
                                                      (USHORT*)psMBMasterInfo->RegHoldValList, MB_MASTER_WAITING_DELAY);	//写寄存器
                    iWriteCount = 0;
                    iRegs = 0;
                    bStarted = FALSE;                
                }	
		    }
        }
        /***************************** 读保持寄存器 **********************************/
        if(xReadEn)
        {        
            if( (psRegHoldValue->usAddr - iLastAddr + 1) > MB_SCAN_MAX_REG_INTERVAL)    //地址间隔超过最大间隔，则发送读请求
            {
                if(iReadCount > 0)
                {
                    eStatus = eMBMasterReqReadHoldingRegister(psMBMasterInfo, ucSndAddr, iReadStartRegAddr, iReadCount, MB_MASTER_WAITING_DELAY);
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
                eStatus = eMBMasterReqReadHoldingRegister(psMBMasterInfo, ucSndAddr, iReadStartRegAddr, iReadCount, MB_MASTER_WAITING_DELAY);
                iReadCount = 0;	 
            }
        }
        iLastAddr = psRegHoldValue->usAddr;
	}
    return eStatus;
}
#endif

#if MB_FUNC_READ_COILS_ENABLED > 0  || MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0 || MB_FUNC_WRITE_COIL_ENABLED > 0
/*************************************************************************************
 * @brief  写线圈
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterWriteCoil(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usCoilAddr,
                                       USHORT usNCoils, UCHAR* pucDataBuffer, LONG lTimeOut)
{
    eMBMasterReqErrCode eStatus = MB_MRE_NO_ERR;
    USHORT usMBBitData = 0;
    
    if(usNCoils == 1)      //写单个线圈
    {
        usMBBitData = (*(UCHAR*)pucDataBuffer >0) ? 0xFF00 : 0x0000;
        
//      debug("eMBMasterWriteCoil ucSndAddr  %d usCoilAddr %d  usMBBitData %d %d %d\n\n",ucSndAddr,
//                usCoilAddr, usMBBitData, *(UCHAR*)pucDataBuffer, pSystem->psModularRoofList[0]->xErrClean);

        eStatus = eMBMasterReqWriteCoil(psMBMasterInfo, ucSndAddr, usCoilAddr, usMBBitData, lTimeOut);   
    }
    else if(usNCoils > 1)  //写多个线圈
    {
        eStatus = eMBMasterReqWriteMultipleCoils(psMBMasterInfo, ucSndAddr, usCoilAddr, usNCoils, (UCHAR*)pucDataBuffer, lTimeOut);	      
    }
    return eStatus;
}

/***********************************************************************************
 * @brief  轮询线圈
 * @param  ucSndAddr            从栈地址
 * @return eMBMasterReqErrCode  错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterScanCoils(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, BOOL xWriteEn, BOOL xReadEn, BOOL xCheckPreValue)
{
	UCHAR  iBitInByte, cByteValue;
    BOOL bStarted;
    USHORT iIndex, iLastAddr, iWriteStartCoilAddr, iReadStartCoilAddr, iReadCount, iWriteBytes, iWriteBits, iBits;
  
	eMBMasterReqErrCode      eStatus = MB_MRE_NO_ERR;
	sMasterBitCoilData*  psCoilValue = NULL;

    sMBSlaveDev*     psMBSlaveDevCur = psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur;     //当前从设备 
    sMBDevDataTable* psMBCoilTable   = &psMBSlaveDevCur->psDevCurData->sMBCoilTable;    //从设备通讯协议表
   
	iLastAddr = 0;
	iWriteStartCoilAddr = 0;
    iReadStartCoilAddr = 0;
	iReadCount = 0;
	iWriteBits = 0;
	iWriteBytes = 0;
    iBitInByte = 0;
	iBits = 0;
    cByteValue = 0;
	bStarted = FALSE;
	
//    debug("ucSlaveAddr %d  eMBMasterScanWriteCoils\n", ucSndAddr);
    
    if(psMBSlaveDevCur->ucDevAddr != ucSndAddr) //如果当前从设备地址与要轮询从设备地址不一致，则更新从设备
    {
        psMBSlaveDevCur = psMBMasterGetDev(psMBMasterInfo, ucSndAddr);
        psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDevCur;
        psMBCoilTable = &psMBSlaveDevCur->psDevCurData->sMBCoilTable;
    }
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
        if(xWriteEn)
        {
            if ( (psCoilValue->pvValue != NULL) && (psCoilValue->ucAccessMode != RO) )   //非只读
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
		    	        eStatus = eMBMasterWriteCoil(psMBMasterInfo, ucSndAddr, iWriteStartCoilAddr, iWriteBits-1, 
		    	                                     psMBMasterInfo->BitCoilByteValList, MB_MASTER_WAITING_DELAY);	     //写线圈
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
		    	        eStatus = eMBMasterWriteCoil(psMBMasterInfo, ucSndAddr, iWriteStartCoilAddr, iWriteBits, 
                                                     psMBMasterInfo->BitCoilByteValList, MB_MASTER_WAITING_DELAY);	//写线圈
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
                    if( (iBits > iWriteBits) || (iIndex == psMBCoilTable->usDataCount-1) || (iWriteBits >= MB_SCAN_MAX_BIT_NUM) ) 
                    {        
                        eStatus = eMBMasterWriteCoil(psMBMasterInfo, ucSndAddr, iWriteStartCoilAddr, iWriteBits, 
                                                     psMBMasterInfo->BitCoilByteValList, MB_MASTER_WAITING_DELAY);	//写线圈	
                        iWriteBits = 0;
                        iWriteBytes = 0;
                        iBits = 0;
                        bStarted = FALSE;
                        cByteValue = 0;
                        iBitInByte = 0;    
                    }
                }                
		    }
        }
        /***************************** 读线圈 **********************************/
        if(xReadEn)
        {
            if( (psCoilValue->usAddr - iLastAddr + 1) > MB_SCAN_MAX_BIT_INTERVAL)     //地址间隔超过最大间隔，则发送读请求
            {
                if(iReadCount>0)
                {
                    eStatus = eMBMasterReqReadCoils(psMBMasterInfo, ucSndAddr, iReadStartCoilAddr, iReadCount, MB_MASTER_WAITING_DELAY);       
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
            if( (psCoilValue->ucAccessMode == WO || iIndex == psMBCoilTable->usDataCount-1 || iReadCount >= MB_SCAN_MAX_BIT_NUM) && (iReadCount>0) )  
            {
                eStatus = eMBMasterReqReadCoils(psMBMasterInfo, ucSndAddr, iReadStartCoilAddr, iReadCount, MB_MASTER_WAITING_DELAY);
                if(eStatus == MB_MRE_ETIMEDOUT)
                {
                    return eStatus;
                }             
                iReadCount = 0;
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
eMBMasterReqErrCode eMBMasterScanReadDiscreteInputs( sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr )
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
				eStatus = eMBMasterReqReadDiscreteInputs(psMBMasterInfo, ucSndAddr, iStartAddr, iCount, MB_MASTER_WAITING_DELAY);	
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
void vMBMasterScanSlaveDevData(sMBMasterInfo* psMBMasterInfo, UCHAR ucSlaveAddr, BOOL xWriteEn, BOOL xReadEn, BOOL xCheckPreValue)
{
    eMBMasterReqErrCode errorCode    = MB_MRE_NO_ERR;
    sMBSlaveDev*    psMBSlaveDevCur  = psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur;     //当前从设备

#if MB_FUNC_READ_HOLDING_ENABLED > 0 || MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0 || MB_FUNC_WRITE_HOLDING_ENABLED > 0	
    errorCode = eMBMasterScanHoldingRegister(psMBMasterInfo, ucSlaveAddr, xWriteEn, xReadEn, xCheckPreValue); //保持寄存器
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDevCur->xStateTestRequest = TRUE;
        psMBSlaveDevCur->xSynchronized = FALSE;
        return;
    }         
#endif
					
#if MB_FUNC_READ_COILS_ENABLED > 0  || MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0 || MB_FUNC_WRITE_COIL_ENABLED > 0
    errorCode = eMBMasterScanCoils(psMBMasterInfo, ucSlaveAddr, xWriteEn, xReadEn, xCheckPreValue);           //线圈
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDevCur->xStateTestRequest = TRUE;
        psMBSlaveDevCur->xSynchronized = FALSE;
        return;
    }     
#endif
					
#if MB_FUNC_READ_INPUT_ENABLED > 0				
    errorCode = eMBMasterScanReadInputRegister(psMBMasterInfo, ucSlaveAddr);	  //读输入寄存器
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDevCur->xStateTestRequest = TRUE;
        psMBSlaveDevCur->xSynchronized = FALSE;
        return;
    }         
#endif	
				
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
    errorCode = eMBMasterScanReadDiscreteInputs(psMBMasterInfo, ucSlaveAddr);   //读离散量
    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDevCur->xStateTestRequest = TRUE;
        psMBSlaveDevCur->xSynchronized = FALSE;
        return;
    }     
#endif 
/*    if(errorCode == MB_MRE_TIMEDOUT)
    {
        psMBSlaveDevCur->xStateTestRequest = TRUE;
        psMBSlaveDevCur->xSynchronized = FALSE;
        return;
//        debug("vMBMasterScanSlaveDevData ucSlaveAddr %d  errorCode %d\n", ucSlaveAddr, errorCode);
    }
   */
}

/**********************************************************************
 * @brief   主栈轮询某个从设备
 * @param   psMBMasterInfo  主栈信息块 
 * @param   psMBSlaveDev    从设备
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
void vMBMasterScanSlaveDev(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev)
{
    sMBMasterDevsInfo*  psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;      //从设备列表
    UCHAR               ucSlaveAddr  = psMBSlaveDev->ucDevAddr;           //通讯地址

    psMBDevsInfo->psMBSlaveDevCur = psMBSlaveDev;                         //当前从设备
    if(psMBDevsInfo->psMBSlaveDevCur->psDevCurData == NULL)               //数据表为空则不进行轮询
    {
        return;
    }
    if( (psMBSlaveDev != NULL) && (psMBSlaveDev->xOnLine == TRUE) )       //如果设备在线则进行轮询
    {
        if(psMBSlaveDev->xDataReady == TRUE)         //从设备数据准备好了才同步上来
        {	 	    
            if(psMBSlaveDev->xSynchronized == FALSE) //重新上线的话，同步所有数据，先写后读
            {
                vMBMasterScanSlaveDevData(psMBMasterInfo, ucSlaveAddr, TRUE, TRUE, FALSE);   //同步从设备数据            
                psMBSlaveDev->xSynchronized = TRUE;                              //同步完成
            }
            else   //同步完成后，先写后读
            {
                vMBMasterScanSlaveDevData(psMBMasterInfo, ucSlaveAddr, TRUE, TRUE, FALSE);  //根据实际通讯需要灵活调整，可以只写有变化数据，也可轮询写             
            }
        }
        else  //从设备数据未好，则只进行写不读
        {
            if(psMBSlaveDev->xSynchronized == FALSE) 
            {
                vMBMasterScanSlaveDevData(psMBMasterInfo, ucSlaveAddr, TRUE, FALSE, FALSE);   //同步从设备数据  
                psMBSlaveDev->xSynchronized = TRUE;  //同步完成
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
void vMBMasterScanSlaveDevTask(void *p_arg)
{
	USHORT msReadInterval = MB_SCAN_SLAVE_INTERVAL_MS;

    sMBSlaveDev*       psMBSlaveDev   = NULL;
    sMBMasterInfo*     psMBMasterInfo = (sMBMasterInfo*)p_arg;
    sMBMasterDevsInfo* psMBDevsInfo   = &psMBMasterInfo->sMBDevsInfo;  //从设备状态信息

    UCHAR ucMaxAddr = psMBDevsInfo->ucSlaveDevMaxAddr;
    UCHAR ucMinAddr = psMBDevsInfo->ucSlaveDevMinAddr;

    while(1)
	{
//        debug("****************************************************************************\n");
        (void)vMBTimeDly(0, msReadInterval);    
 
#if MB_MASTER_DTU_ENABLED > 0    //GPRS模块功能支持，特殊处理      
        if( (psMBMasterInfo->bDTUEnable == TRUE) && (psMBMasterInfo->pvDTUScanDevCallBack != NULL))
        {
             psMBMasterInfo->pvDTUScanDevCallBack(psMBMasterInfo);
        }   
#endif
		/*********************************测试从设备***********************************/
		for(psMBSlaveDev = psMBDevsInfo->psMBSlaveDevsList; psMBSlaveDev != NULL; psMBSlaveDev = psMBSlaveDev->pNext)
        {
            if(psMBSlaveDev->xOnLine == FALSE)   //如果设备不在线
            {
                vMBDevTest(psMBMasterInfo, psMBSlaveDev);  //测试  
            }
        }
        /*********************************轮询从设备***********************************/
        for(psMBSlaveDev = psMBDevsInfo->psMBSlaveDevsList; psMBSlaveDev != NULL; psMBSlaveDev = psMBSlaveDev->pNext)
        {    
            if( psMBSlaveDev->xOnLine == TRUE  && psMBSlaveDev->ucDevAddr <= ucMaxAddr && psMBSlaveDev->ucDevAddr >= ucMinAddr )
            {
                if(psMBSlaveDev->xStateTestRequest == TRUE) //测试请求
                {
                    vMBDevCurStateTest(psMBMasterInfo, psMBSlaveDev);  //检测从设备是否掉线
                }
                if( (psMBSlaveDev->xOnLine == TRUE) && (psMBSlaveDev->ucOfflineTimes == 0) ) //在线且不处于延时阶段
                {                  
                    vMBMasterScanSlaveDev(psMBMasterInfo, psMBSlaveDev);
                }
//                debug("vMBDevCurStateTest  %d  psMBSlaveDev->xOnLine %d\n", psMBSlaveDev->ucDevAddr, psMBSlaveDev->xOnLine);
            }          
        }    
	}
}
#elif MB_LINUX_ENABLED
void* vMBMasterScanSlaveDevTask(void *p_arg)
{
    sMBSlaveDev*       psMBSlaveDev   = NULL;
    sMBMasterInfo*     psMBMasterInfo = (sMBMasterInfo*)p_arg;
    sMBMasterDevsInfo* psMBDevsInfo   = &psMBMasterInfo->sMBDevsInfo;  //从设备状态信息

    UCHAR ucMaxAddr = psMBDevsInfo->ucSlaveDevMaxAddr;
    UCHAR ucMinAddr = psMBDevsInfo->ucSlaveDevMinAddr;

    while(1)
    {
        (void)vMBTimeDly(0, MB_SCAN_SLAVE_INTERVAL_MS);
        if(psMBMasterInfo->eMBState == STATE_NOT_INITIALIZED)
        {
            continue;
        }
#if MB_MASTER_DTU_ENABLED > 0    //GPRS模块功能支持，特殊处理
        if( (psMBMasterInfo->bDTUEnable == TRUE) && (psMBMasterInfo->pvDTUScanDevCallBack != NULL))
        {
             psMBMasterInfo->pvDTUScanDevCallBack(psMBMasterInfo);
        }
#endif
        /*********************************测试从设备***********************************/

        //debug("**********************************************\n");
        for(psMBSlaveDev = psMBDevsInfo->psMBSlaveDevsList; psMBSlaveDev != NULL; psMBSlaveDev = psMBSlaveDev->pNext)
        {
            if(psMBSlaveDev->xOnLine == FALSE)   //如果设备不在线
            {
                vMBDevTest(psMBMasterInfo, psMBSlaveDev);  //测试
            }
        }
        /*********************************轮询从设备***********************************/
        for(psMBSlaveDev = psMBDevsInfo->psMBSlaveDevsList; psMBSlaveDev != NULL; psMBSlaveDev = psMBSlaveDev->pNext)
        {
            if( psMBSlaveDev->xOnLine == TRUE  && psMBSlaveDev->ucDevAddr <= ucMaxAddr && psMBSlaveDev->ucDevAddr >= ucMinAddr )
            {
                if(psMBSlaveDev->xStateTestRequest == TRUE) //测试请求
                {
                    vMBDevCurStateTest(psMBMasterInfo, psMBSlaveDev);  //检测从设备是否掉线
                }
                if( (psMBSlaveDev->xOnLine == TRUE) && (psMBSlaveDev->ucOfflineTimes == 0) ) //在线且不处于延时阶段
                {
                    vMBMasterScanSlaveDev(psMBMasterInfo, psMBSlaveDev);
                }
//                debug("vMBMasterScanSlaveDevTask  %d  psMBSlaveDev->xOnLine %d\n", psMBSlaveDev->ucDevAddr, psMBSlaveDev->xOnLine);
            }
        }
    }
}
#endif

/**********************************************************************
 * @brief   创建主栈轮询从设备任务
 * @param   *p_arg    
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
BOOL xMBMasterCreateScanSlaveDevTask(sMBMasterInfo* psMBMasterInfo)
{   
#if MB_UCOSIII_ENABLED
    OS_ERR               err = OS_ERR_NONE;
    CPU_STK_SIZE    stk_size = MB_MASTER_SCAN_TASK_STK_SIZE; 
    
    sMBMasterTask* psMBTask = &psMBMasterInfo->sMBTask;
    
    OS_PRIO             prio = psMBTask->ucMasterScanPrio;
    OS_TCB*            p_tcb = (OS_TCB*)(&psMBTask->sMasterScanTCB);  
    CPU_STK*      p_stk_base = (CPU_STK*)(psMBTask->usMasterScanStk);
   
    OSTaskCreate(p_tcb, "vMBMasterScanSlaveDevTask", vMBMasterScanSlaveDevTask, (void*)psMBMasterInfo, prio, p_stk_base,
                 stk_size/10u, stk_size, 0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
    return (err == OS_ERR_NONE);

#elif MB_LINUX_ENABLED
    int ret = pthread_create(&psMBMasterInfo->sMBTask.sMBScanTask, NULL, vMBMasterScanSlaveDevTask, (void*)psMBMasterInfo);    //创建线程
    return ret == 0;
#endif
}

