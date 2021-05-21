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
 * File: $Id: mbutils.c,v 1.6 2007/02/18 23:49:07 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/

#include "mbproto.h"
#include "mbframe.h"
#include "mbbits_m.h"
#include "mbmap_m.h"

/* ----------------------- Defines ------------------------------------------*/
#define BITS_UCHAR      8

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED

/***********************************************************************************
 * @brief  线圈和离散量置位函数
 * @param  ucByteBuf     Modbus的PDU缓冲区数据指针
 * @param  usAddress     起始地址
 * @param  ucNBits       个数
 * @param  eDataType     数据类型（线圈、离散量）
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBErrorCode eMBMasterUtilSetBits(sMBMasterInfo* psMBMasterInfo, UCHAR* ucByteBuf, USHORT usAddress, 
                                  USHORT usNBits, eDataType eDataType, eMBBitMode eMode)
{
    USHORT  usNPreBits, iNReg, iBits, i;
	UCHAR   ucBit;
	
    eMBErrorCode        eStatus            = MB_ENOERR;
    sMasterBitCoilData* pucBitCoilData     = NULL;
	sMasterBitDiscData* pucBitDiscreteData = NULL;
	
    UCHAR  ucMBDestAddr = ucMBMasterGetDestAddr(psMBMasterInfo);        //从设备通讯地址
    
    iNReg = (USHORT)(usNBits / BITS_UCHAR) + 1;
    usNPreBits = (USHORT)(usNBits % BITS_UCHAR);

    switch(eMode)
    {
    case MB_BIT_READ:
        while(iNReg > 0)
	    {
	        if( (iNReg == 1) && (usNPreBits > 0) ) //未满8个bit
	    	{
	    	    iBits = usNPreBits;
	    	}
	    	else    //超过8bits
	    	{
	    	    iBits = BITS_UCHAR;
	    	}
            for(i=0; i<iBits; i++)
            {
            	switch(eDataType)
            	{	
                case CoilData:                  
#if MB_FUNC_READ_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED || MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED
                
                    pucBitCoilData = NULL;
                    (void)eMBMasterCoilMap(psMBMasterInfo, ucMBDestAddr, usAddress, &pucBitCoilData);   //扫描，找到对应点位   
                    if( (pucBitCoilData != NULL) && (pucBitCoilData->pvValue != NULL) && (pucBitCoilData->ucAccessMode != WO))
                    {
                         ucBit = (UCHAR)( ((*(UCHAR*)ucByteBuf) & (1 << i) ) >> i );
                        if( (pucBitCoilData->ucAccessMode == RW) && (*(UCHAR*)pucBitCoilData->pvValue != (UCHAR)pucBitCoilData->ucPreVal) &&
                            (psMBMasterInfo->eMBRunMode == STATE_SCAN_DEV) )
                        {
                            pucBitCoilData->ucPreVal = (UCHAR)ucBit;
                            usAddress++;
                            continue;    //此时实际点位有更新，所以不更新实际点位，只更新先前值
                        }
                        *(UCHAR*)(pucBitCoilData->pvValue) = (UCHAR)ucBit;
                        pucBitCoilData->ucPreVal  = (UCHAR)ucBit;
                    }
#endif                    
                break;
                    
                case DiscInData:             
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED
                    pucBitDiscreteData = NULL;
                    (void)eMBMasterDiscreteMap(psMBMasterInfo, ucMBDestAddr, usAddress, &pucBitDiscreteData);
                    if( (pucBitDiscreteData != NULL) && (pucBitDiscreteData->pvValue != NULL) && (pucBitDiscreteData->ucAccessMode != WO))
                    {
                        ucBit = (UCHAR)( ((*(UCHAR*)ucByteBuf) & (1 << i) ) >> i );	
                        (*(UCHAR*)(pucBitDiscreteData->pvValue)) = (UCHAR)ucBit;
                    }
#endif  
                break;
				default:break;			
                }	
                usAddress++;
            }	
        iNReg--;
        ucByteBuf++;	
    }
    break;
    
    case MB_BIT_WRITE:
        while(iNReg > 0)
	    {
            switch(eDataType)
            {	
            case CoilData:                  
#if MB_FUNC_READ_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED || MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED
                
                (void)eMBMasterCoilMap(psMBMasterInfo, ucMBDestAddr, usAddress, &pucBitCoilData);   //扫描，找到对应点位   
                if( (pucBitCoilData != NULL) && (pucBitCoilData->pvValue != NULL) && (pucBitCoilData->ucAccessMode != RO))
                {
                    pucBitCoilData->ucPreVal  =  *(UCHAR*)(pucBitCoilData->pvValue);
                }
#endif                
            break;
                    
            case DiscInData:             
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED
                
                (void)eMBMasterDiscreteMap(psMBMasterInfo, ucMBDestAddr, usAddress, &pucBitDiscreteData);
                if( (pucBitDiscreteData != NULL) && (pucBitDiscreteData->pvValue != NULL) && (pucBitDiscreteData->ucAccessMode != WO))
                {
                    *(UCHAR*)(pucBitDiscreteData->pvValue) = *(UCHAR*)(pucBitDiscreteData->pvValue);
                }
#endif  
            break;
            default:break;			
            }
            usAddress++;            
            iNReg--;
        }  
    break;
    }
    return eStatus; 
}

/***********************************************************************************
 * @brief  线圈和离散量获取函数
 * @param  ucByteBuf     Modbus的PDU缓冲区数据指针
 * @param  usAddress     起始地址
 * @param  ucNBits       个数
 * @param  eDataType     数据类型（线圈、离散量）
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBErrorCode eMBMasterUtilGetBits(sMBMasterInfo* psMBMasterInfo, UCHAR* ucByteBuf, 
	                              USHORT usAddress, UCHAR ucNBits, eDataType eDataType)
{
    USHORT          usNPreBits, iNReg, iBits, i;
	
    eMBErrorCode        eStatus            = MB_ENOERR;
    sMasterBitCoilData* pucBitCoilData     = NULL;
	sMasterBitDiscData* pucBitDiscreteData = NULL;

    UCHAR  ucMBDestAddr = ucMBMasterGetDestAddr(psMBMasterInfo);        //从设备通讯地址
    
	iNReg = (USHORT)(ucNBits / BITS_UCHAR) + 1;    //8bit为1个reg
    usNPreBits = (USHORT)(ucNBits % BITS_UCHAR); //剩余的bits

	while (iNReg > 0)
	{
		*ucByteBuf = 0;
		if( (iNReg == 1) && (usNPreBits > 0) ) //未满8个bit
		{
			iBits = usNPreBits;
		}
		else    //超过8bits
		{
			iBits = BITS_UCHAR;
		}
		
        for (i = 0; i < iBits; i++)
        {
            switch(eDataType)
            {
#if MB_FUNC_READ_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED || MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED
                
				case CoilData:
				    //通过映射找到对应的点位
                    (void)eMBMasterCoilMap(psMBMasterInfo, ucMBDestAddr, usAddress, &pucBitCoilData);
				
                    if( (pucBitCoilData != NULL) && (pucBitCoilData->pvValue != NULL) && (pucBitCoilData->ucAccessMode != WO) )
                    {
                     	if( (*(UCHAR*)pucBitCoilData->pvValue) > 0 )
                     	{
                     		*ucByteBuf |= (1 << i);
                     	}			
                    }
                    else
                    {
                        *ucByteBuf |= (0 << i);
                    }
                break;
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
				case DiscInData:
				    (void)eMBMasterDiscreteMap(psMBMasterInfo, ucMBDestAddr, usAddress, &pucBitDiscreteData);
				
					if( (pucBitDiscreteData != NULL) && (pucBitDiscreteData->pvValue != NULL) && (pucBitDiscreteData->ucAccessMode != WO))
				    {
				     	if( (*(UCHAR*)pucBitDiscreteData->pvValue) > 0)
				     	{
				     		*ucByteBuf |= (1 << i);
				     	}			
				    }
				    else
				    {
				     	*ucByteBuf |= (0 << i);
				    }
				break;
#endif
				default:break;			
            }	
            usAddress++;
        }
		iNReg--;
		ucByteBuf++;	
	}
    return eStatus;
}
#endif
