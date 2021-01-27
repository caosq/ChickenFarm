/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbfuncholding.c,v 1.12 2007/02/18 23:48:22 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbconfig.h"
#include "mbutils.h"
#include "mbmap.h"

#if MB_SLAVE_CPN_ENABLED > 0 

#define S_VAL_CPN_START               500
#define S_VAL_CPN_NREGS               600

/* ----------------------- Start implementation -----------------------------*/

#if MB_FUNC_CPN_WRITE_ENABLED > 0
/***********************************************************************************
 * @brief 写CPN变量功能函数
 * @param pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException eMBSlaveFuncWriteCPNValue(sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucFrame, USHORT* usLen)
{
	UCHAR          ucValCount;
    UCHAR          *pucFrameCur;

    eMBException    eStatus = MB_EX_NONE;
    eMBErrorCode    eRegStatus;
	
	if( (*usLen >= MB_CPN_PDU_SIZE_MIN) && (*usLen <= MB_CPN_PDU_SIZE_MAX) )
    {
	    ucValCount = (UCHAR)( *(pucFrame + MB_CPN_PDU_VALUE_COUNT_OFF) );
       	
		/* Set the current PDU data pointer to the beginning. */
        pucFrameCur = pucFrame;
        *usLen = MB_CPN_PDU_FUNC_OFF;
		
		/* First byte contains the function code. */
        *pucFrameCur++ = MB_FUNC_CPN_WRITE - MB_CPN_FUNC_CODE_OFF_TO_REAL;
        *usLen += 1;
		
		/* Second byte in the response contain the number of values. */
        *pucFrameCur++ = ucValCount;
        *usLen += 1;
		
		/* The source address and destination address should exchange in the response  */
		
//		/* Third byte in the response contains the source address.*/ 
//        *pucFrameCur++ = ucDestAddr;
//        *usLen += 1;
//		
//		/* Four byte in the response contains the destination address.*/ 
//        *pucFrameCur++ = ucSourAddr;

        *usLen += 2;
		pucFrameCur += 2;
		
		/* Make callback to fill the buffer. */
        eRegStatus = eMBSlaveWriteCPNCB(psMBSlaveInfo, pucFrameCur, ucValCount, usLen );

        /* If an error occured convert it into a Modbus exception. */
		if( eRegStatus != MB_ENOERR )
		{
			eStatus = prveMBSlaveError2Exception(eRegStatus);
		}
		else
		{
			
		}
	}
	else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}

/***********************************************************************************
 * @brief  写CPN变量回调函数（读、连续读）
 * @param  pucRegBuffer  回调函数将变量当前值写入缓冲区
 * @param  ucValCount    变量个数
 * @param  usLen         缓冲区长度
 * @return eMBErrorCode 错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBErrorCode 
eMBSlaveWriteCPNCB(sMBSlaveInfo* psMBSlaveInfo, UCHAR * pucRegBuffer, UCHAR ucValCount, USHORT * usLen )
{
	float pfNum;
   
	USHORT          iValBitIndex, iValDataBitIndex, iNVal;
	USHORT          VAL_CPN_START, VAL_CPN_NREGS, VAL_CPN_DEFINE_SIZE;
    USHORT          usValCPNStart, usValCPNBytes, usValCPNType;
    
    ULONG           ulValCPNName, ulValCPNValue;
	LONG            lValCPNValue;
	SHORT           sValCPNValue;
    int8_t          cValCPNValue;
	
    UCHAR *pcNum = NULL;
	eMBErrorCode               eStatus      = MB_ENOERR;
    sMBSlaveCPNData*           pvCPNValue   = NULL;
    const sMBSlaveDataTable*   psCPNBuf = psMBSlaveInfo->sMBCommInfo.psSlaveCurData->psMBCPNTable;  //从栈通讯协议表
    
    VAL_CPN_START        = S_VAL_CPN_START;
    VAL_CPN_NREGS        = S_VAL_CPN_NREGS;
	VAL_CPN_DEFINE_SIZE  = MB_CPN_VALUE_DEFINE_SIZE;	

    if((ucValCount <= VAL_CPN_NREGS))
    {
        iValDataBitIndex = VAL_CPN_DEFINE_SIZE * ucValCount;
		*usLen += iValDataBitIndex;
		
        for ( iNVal = 0; iNVal < ucValCount; iNVal++ )
        {
			iValBitIndex = VAL_CPN_DEFINE_SIZE * iNVal + MB_CPN_VALUE_DATA_NAME_OFF;
			
			ulValCPNName =   ((ULONG)(*(pucRegBuffer + iValBitIndex)))
			                |(( (ULONG)(*(pucRegBuffer + iValBitIndex + 1)) ) << 8 )
			                |(( (ULONG)(*(pucRegBuffer + iValBitIndex + 2)) ) << 16)
			                |(( (ULONG)(*(pucRegBuffer + iValBitIndex + 3)) ) << 24)  ;
			
			iValBitIndex = VAL_CPN_DEFINE_SIZE * iNVal + MB_CPN_VALUE_DATA_BYTES_OFF;
			usValCPNBytes =   ( (USHORT)(*(pucRegBuffer + iValBitIndex)) )
			                | ( ((USHORT)(*(pucRegBuffer + iValBitIndex + 1)))<< 8 )  ;
			
			iValBitIndex = VAL_CPN_DEFINE_SIZE * iNVal + MB_CPN_VALUE_DATA_TYPE_OFF;
			usValCPNType =    ( (USHORT)(*(pucRegBuffer + iValBitIndex)) )
			                | ( ((USHORT)(*(pucRegBuffer + iValBitIndex + 1)))<< 8 )  ;
						
			(void)eMBSlaveCPNMap(psMBSlaveInfo, ulValCPNName, &pvCPNValue);
			
			if( (pvCPNValue != NULL) && (pvCPNValue->pvValue != NULL))
			{
				switch( pvCPNValue->ucValType )  //分数据类型处理
				{
					case CPN_UINT8:   
						ulValCPNValue = *(UCHAR*)(pucRegBuffer + iValDataBitIndex);
						iValDataBitIndex += 1;
						*usLen += 1;
                    
					    if( (pvCPNValue->fTransmitMultiple != 0.0) && (pvCPNValue->fTransmitMultiple != 1.0))  //传输因子
						{
                            ulValCPNValue = (USHORT)((float)ulValCPNValue / (float)pvCPNValue->fTransmitMultiple);     //传输因子
						}
					break;
					
					case CPN_UINT16:
						ulValCPNValue =  ( *(USHORT*)(pucRegBuffer + iValDataBitIndex) ) 
							                |( *(USHORT*)(pucRegBuffer + iValDataBitIndex + 1)  << 8 );
						iValDataBitIndex += 2;
						*usLen += 2;
					    
					    if( (pvCPNValue->fTransmitMultiple != 0.0) && (pvCPNValue->fTransmitMultiple != 1.0))  //传输因子
						{
							 ulValCPNValue = (USHORT)((float)ulValCPNValue / (float)pvCPNValue->fTransmitMultiple);     //传输因子
						}
					break;
					
					case CPN_UINT32:
						ulValCPNValue =  ( *(ULONG*)(pucRegBuffer + iValDataBitIndex) ) 
											|( *(ULONG*)(pucRegBuffer + iValDataBitIndex + 1) << 8 )
											|( *(ULONG*)(pucRegBuffer + iValDataBitIndex + 2) << 16)
											|( *(ULONG*)(pucRegBuffer + iValDataBitIndex + 3) << 24);
						iValDataBitIndex += 4;
						*usLen += 4;
						 if( (pvCPNValue->fTransmitMultiple != 0.0) && (pvCPNValue->fTransmitMultiple != 1.0))  //传输因子
						{
							 ulValCPNValue = (USHORT)((float)ulValCPNValue / (float)pvCPNValue->fTransmitMultiple);     //传输因子
						}
					break;
						
					case CPN_FLOAT:		
						pcNum = (UCHAR*)&pfNum;	
						* pcNum      = *( pucRegBuffer + iValDataBitIndex ); 
						*(pcNum + 1) = *( pucRegBuffer + iValDataBitIndex + 1);
						*(pcNum + 2) = *( pucRegBuffer + iValDataBitIndex + 2);
						*(pcNum + 3) = *( pucRegBuffer + iValDataBitIndex + 3);
						
                        if( (pvCPNValue->fTransmitMultiple != 0.0) && (pvCPNValue->fTransmitMultiple != 1.0))  //传输因子 
                        {
                            ulValCPNValue = (USHORT)((float)ulValCPNValue / (float)pvCPNValue->fTransmitMultiple);     //传输因子
                        } 
                        
						iValDataBitIndex += 4;
						*usLen += 4;	
					break;
					default:break;
				}
				switch (pvCPNValue->ucDataType )
				{
					case uint8:
						if( (ulValCPNValue >= (UCHAR)pvCPNValue->lMinVal) && (ulValCPNValue <= (UCHAR)pvCPNValue->lMaxVal) )
						{
							if( ulValCPNValue != *(UCHAR*)pvCPNValue->pvValue )  //更新值
							{
								(*(UCHAR*)pvCPNValue->pvValue)=(UCHAR)ulValCPNValue ;
							}
						}
						else
						{
							eStatus = MB_EINVAL;
							return eStatus;
						}	
					break;
					case uint16:
						if( (ulValCPNValue >= (USHORT)pvCPNValue->lMinVal) && (ulValCPNValue <= (USHORT)pvCPNValue->lMaxVal) )
						{
							if( ulValCPNValue != *(USHORT*)pvCPNValue->pvValue )
							{
								(*(USHORT*)pvCPNValue->pvValue)=(USHORT)ulValCPNValue;
							}
						}
						else
						{
							eStatus = MB_EINVAL;
							return eStatus;
						}
					break;
					case uint32:
						if( (ulValCPNValue >= (ULONG)pvCPNValue->lMinVal) && (ulValCPNValue <= (ULONG)pvCPNValue->lMaxVal) )
						{
							if( ulValCPNValue != *(ULONG*)pvCPNValue->pvValue )
							{
								*(ULONG*)pvCPNValue->pvValue=(ULONG)ulValCPNValue ;
							}
						}
						else
						{
							eStatus = MB_EINVAL;
							return eStatus;
						}
						
					break;
					case int8:
						cValCPNValue = (int8_t)ulValCPNValue;
					
						if( (cValCPNValue >= (int8_t)pvCPNValue->lMinVal) && (cValCPNValue <= (int8_t)pvCPNValue->lMaxVal) )
						{
							if( cValCPNValue != *(int8_t*)pvCPNValue->pvValue )
							{
								*(int8_t*)pvCPNValue->pvValue=(int8_t)cValCPNValue ;
							}
						}
						else
						{
							eStatus = MB_EINVAL;
							return eStatus;
						}
					break;
					case int16:
						
						sValCPNValue = (SHORT)ulValCPNValue;
					
						 if( (sValCPNValue >= (SHORT)pvCPNValue->lMinVal) && (sValCPNValue <= (SHORT)pvCPNValue->lMaxVal) )
						{
							if( sValCPNValue != *(SHORT*)pvCPNValue->pvValue )
							{
								*(SHORT*)pvCPNValue->pvValue=(SHORT)sValCPNValue;
							}
						}
						else
						{
							eStatus = MB_EINVAL;
							return eStatus;
						}	
					break;
					case int32:
						lValCPNValue = (LONG)ulValCPNValue;
					
						 if( (lValCPNValue >= (LONG)pvCPNValue->lMinVal) && (lValCPNValue <= (LONG)pvCPNValue->lMaxVal) )
						{
							if( lValCPNValue != *(LONG*)pvCPNValue->pvValue )
							{
								*(LONG*)pvCPNValue->pvValue=(LONG)lValCPNValue ;
							}
						}
						else
						{
							eStatus = MB_EINVAL;
							return eStatus;
						}	
					break;
					default: break;
				}	 
			}
			else
			{
				eStatus = MB_ENOREG;
				return eStatus;
			}	
        }
    }
    else
	{
		eStatus = MB_EINVAL;
		return eStatus;
	}
    return eStatus;
}
#endif

#if MB_FUNC_CPN_READ_ENABLED > 0
/***********************************************************************************
 * @brief 读CPN变量功能函数
 * @param pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException eMBSlaveFuncReadCPNValue(sMBSlaveInfo* psMBSlaveInfo, UCHAR * pucFrame, USHORT * usLen )
{
	UCHAR          ucValCount; 	
    UCHAR          *pucFrameCur;
    USHORT         usDatabytes;
    
    eMBErrorCode    eRegStatus;
    eMBException    eStatus = MB_EX_NONE;
   
	if( (*usLen >= MB_CPN_PDU_SIZE_MIN) && (*usLen <= MB_CPN_PDU_SIZE_MAX) )
    {
	    ucValCount = (UCHAR)( *(pucFrame + MB_CPN_PDU_VALUE_COUNT_OFF) );
    
		/* Set the current PDU data pointer to the beginning. */
        pucFrameCur = pucFrame;
        *usLen = MB_CPN_PDU_FUNC_OFF;
		
		/* First byte contains the function code. */
        *pucFrameCur++ = MB_FUNC_CPN_READ - MB_CPN_FUNC_CODE_OFF_TO_REAL;
        *usLen += 1;
		
		/* Second byte in the response contain the number of values. */
        *pucFrameCur++ = ucValCount;
        *usLen += 1;
		
//		/* The source address and destination address should exchange in the response  */
//		
//		/* Third byte in the response contains the source address.*/ 
//        *pucFrameCur++ = ucDestAddr;
//        *usLen += 1;
//		
//		/* Four byte in the response contains the destination address.*/ 
//        *pucFrameCur++ = ucSourAddr;
        
		*usLen += 2;
		pucFrameCur += 2;
		
		/* Make callback to fill the buffer. */
        eRegStatus = eMBSlaveReadCPNCB(psMBSlaveInfo, pucFrameCur, ucValCount, usLen);
		
         /* If an error occured convert it into a Modbus exception. */
		if(eRegStatus != MB_ENOERR)
		{
			eStatus = prveMBSlaveError2Exception(eRegStatus);
		}	
	}
	else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}

/***********************************************************************************
 * @brief  读CPN变量回调函数（读、连续读）
 * @param  pucRegBuffer  回调函数将变量当前值写入缓冲区
 * @param  ucValCount    变量个数
 * @param  usLen         缓冲区长度
 * @return eMBErrorCode 错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBErrorCode eMBSlaveReadCPNCB(sMBSlaveInfo* psMBSlaveInfo, UCHAR * pucRegBuffer, UCHAR ucValCount, USHORT * usLen )
{
	float fNum;
    UCHAR *pcNum = NULL;
    UCHAR iExchange;
	USHORT          iValBitIndex, iValDataBitIndex, iNVal;
	USHORT          VAL_CPN_START, VAL_CPN_NREGS, VAL_CPN_DEFINE_SIZE;
	USHORT          usValCPNStart, usValCPNBytes, usValCPNType;
    
    ULONG            ulValCPNName;
	ULONG            ulValCPNValue;
    
    eMBErrorCode                eStatus = MB_ENOERR;
    sMBSlaveCPNData*         pvCPNValue = NULL;
	const sMBSlaveDataTable*   psCPNBuf = psMBSlaveInfo->sMBCommInfo.psSlaveCurData->psMBCPNTable;  //从栈通讯协议表
     	
    VAL_CPN_START        = S_VAL_CPN_START;
    VAL_CPN_NREGS        = S_VAL_CPN_NREGS;
	VAL_CPN_DEFINE_SIZE  = MB_CPN_VALUE_DEFINE_SIZE;	

    
    if ((ucValCount <= VAL_CPN_NREGS))
    {
        iValDataBitIndex = VAL_CPN_DEFINE_SIZE * ucValCount ;

		*usLen += iValDataBitIndex;

		for ( iNVal = 0; iNVal < ucValCount; iNVal++ )
        {
			iValBitIndex = VAL_CPN_DEFINE_SIZE * iNVal + MB_CPN_VALUE_DATA_NAME_OFF ;
			ulValCPNName =   (  (ULONG)(*(pucRegBuffer + iValBitIndex)) )                          //CPN变量名称
			                |(( (ULONG)(*(pucRegBuffer + iValBitIndex + 1)) ) << 8 )
			                |(( (ULONG)(*(pucRegBuffer + iValBitIndex + 2)) ) << 16)
			                |(( (ULONG)(*(pucRegBuffer + iValBitIndex + 3)) ) << 24)  ;
			
			iValBitIndex = VAL_CPN_DEFINE_SIZE * iNVal + MB_CPN_VALUE_DATA_BYTES_OFF;
			usValCPNBytes =   ( (USHORT)(*(pucRegBuffer + iValBitIndex)) )                        //CPN变量长度
			                | ( ((USHORT)(*(pucRegBuffer + iValBitIndex + 1)))<< 8 )  ;
			
			iValBitIndex = VAL_CPN_DEFINE_SIZE * iNVal + MB_CPN_VALUE_DATA_TYPE_OFF;
			usValCPNType =    ( (USHORT)(*(pucRegBuffer + iValBitIndex)) )                  //CPN变量类型
			                | ( ((USHORT)(*(pucRegBuffer + iValBitIndex + 1)))<< 8 )  ;
		
			(void)eMBSlaveCPNMap(psMBSlaveInfo, ulValCPNName, &pvCPNValue);                //扫描CPN变量字典
			
			if( (pvCPNValue != NULL) && (pvCPNValue->pvValue != NULL))
			{
				switch (pvCPNValue->ucDataType )                                          //根据数据类型进行指针转换
				{
					case uint8:
						ulValCPNValue = (ULONG)(*(UCHAR*)pvCPNValue->pvValue) ;
					break;
					case uint16:
						ulValCPNValue = (ULONG)(*(USHORT*)pvCPNValue->pvValue) ;
					break;
					case uint32:
						ulValCPNValue = (ULONG)(*(ULONG*)pvCPNValue->pvValue);
					break;
					
					case int8:
						ulValCPNValue = (ULONG)(*(int8_t*)pvCPNValue->pvValue) ;
					break;
					case int16:
						ulValCPNValue = (ULONG)(*(SHORT*)pvCPNValue->pvValue) ;
					break;
					case int32:
						ulValCPNValue = (ULONG)(*(LONG*)pvCPNValue->pvValue) ;
					break;

                   default: break;					
				}
				switch(pvCPNValue->ucValType )
				{
					case CPN_UINT8:
                        if( (pvCPNValue->fTransmitMultiple != 0.0) && (pvCPNValue->fTransmitMultiple != 1.0))  //传输因子 
                        {
                            ulValCPNValue = (USHORT)((float)ulValCPNValue * (float)pvCPNValue->fTransmitMultiple);     //传输因子
                        }
						*( pucRegBuffer + iValDataBitIndex ) = (UCHAR)( ulValCPNValue & 0xFF );;
						iValDataBitIndex += 1;
						*usLen += 1;
						break;
                        
					case CPN_UINT16:
                        if( (pvCPNValue->fTransmitMultiple != 0) && (pvCPNValue->fTransmitMultiple != 1))  //传输因子 
                        {
                            ulValCPNValue *= pvCPNValue->fTransmitMultiple;
                        }
						*( pucRegBuffer + iValDataBitIndex )    = (UCHAR)( ulValCPNValue & 0xFF );
					    *( pucRegBuffer + iValDataBitIndex + 1) = (UCHAR)( ulValCPNValue >> 8 );
						iValDataBitIndex += 2;
						*usLen += 2;
						break;
					
					case CPN_UINT32:
						if( (pvCPNValue->fTransmitMultiple != 0) && (pvCPNValue->fTransmitMultiple != 1))  //传输因子 
                        {
                            ulValCPNValue *= pvCPNValue->fTransmitMultiple;
                        }
						*( pucRegBuffer + iValDataBitIndex )    = (UCHAR)( ulValCPNValue & 0xFF ) ;
					    *( pucRegBuffer + iValDataBitIndex + 1) = (UCHAR)( ulValCPNValue >> 8 );
						*( pucRegBuffer + iValDataBitIndex + 2) = (UCHAR)( ulValCPNValue >> 16 );
						*( pucRegBuffer + iValDataBitIndex + 3) = (UCHAR)( ulValCPNValue >> 24 );
						iValDataBitIndex += 4;
						*usLen += 4;
					    break;
							
					case CPN_FLOAT:	
                        if( (pvCPNValue->fTransmitMultiple != 0.0) && (pvCPNValue->fTransmitMultiple != 1.0))  //传输因子 
						{
							fNum = (float)( ulValCPNValue / (float)pvCPNValue->fTransmitMultiple );	
						}											
						pcNum = (UCHAR*)(&fNum);															
						*( pucRegBuffer + iValDataBitIndex )    = *pcNum;
						*( pucRegBuffer + iValDataBitIndex + 1) = *(pcNum + 1);
						*( pucRegBuffer + iValDataBitIndex + 2) = *(pcNum + 2);
						*( pucRegBuffer + iValDataBitIndex + 3) = *(pcNum + 3);
						
						iValDataBitIndex += 4;
						*usLen += 4;
					    break;
					default:break;	
				}	
			}
            else
			{
				switch(usValCPNType)
				{
					case CPN_UINT8:
					    ulValCPNValue = 0;
						*( pucRegBuffer + iValDataBitIndex ) = (UCHAR)( ulValCPNValue & 0xFF );;
						iValDataBitIndex += 1;
						*usLen += 1;
						break;
					case CPN_UINT16:
						ulValCPNValue = 0;
						*( pucRegBuffer + iValDataBitIndex )    = (UCHAR)( ulValCPNValue & 0xFF );
					    *( pucRegBuffer + iValDataBitIndex + 1) = (UCHAR)( ulValCPNValue >> 8 );
						iValDataBitIndex += 2;
						*usLen += 2;
						break;
					
					case CPN_UINT32:
						ulValCPNValue = 0;
						*( pucRegBuffer + iValDataBitIndex )    = (UCHAR)( ulValCPNValue & 0xFF ) ;
					    *( pucRegBuffer + iValDataBitIndex + 1) = (UCHAR)( ulValCPNValue >> 8 );
						*( pucRegBuffer + iValDataBitIndex + 2) = (UCHAR)( ulValCPNValue >> 16 );
						*( pucRegBuffer + iValDataBitIndex + 3) = (UCHAR)( ulValCPNValue >> 24 );
						iValDataBitIndex += 4;
						*usLen += 4;
					    break;
							
					case CPN_FLOAT:	
                 		ulValCPNValue = 0;	
                        if( (pvCPNValue->fTransmitMultiple != 0.0) && (pvCPNValue->fTransmitMultiple != 1.0))  //传输因子 
						{
							fNum = (float)( ulValCPNValue / (float)pvCPNValue->fTransmitMultiple );	
						}									
						pcNum = (UCHAR*)(&fNum);
																			
						*( pucRegBuffer + iValDataBitIndex )    = *pcNum;
						*( pucRegBuffer + iValDataBitIndex + 1) = *(pcNum + 1);
						*( pucRegBuffer + iValDataBitIndex + 2) = *(pcNum + 2);
						*( pucRegBuffer + iValDataBitIndex + 3) = *(pcNum + 3);
						
						iValDataBitIndex += 4;
						*usLen += 4;
					    break;
					default:break;	
					}
				return MB_ENOREG;	
			}			
        }
    }
    else
	{
		return MB_EINVAL;
	}
    return eStatus;
}
#endif

#endif
