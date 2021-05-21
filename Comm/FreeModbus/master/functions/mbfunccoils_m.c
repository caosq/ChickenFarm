/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
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
 * File: $Id: mbfunccoils_m.c,v 1.60 2013/10/12 15:10:12 Armink Add Master Functions
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb_m.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbconfig.h"
#include "mbfunc_m.h"
#include "mbbits_m.h"
#include "mbdict_m.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_REQ_READ_ADDR_OFF            ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REQ_READ_COILCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_READ_SIZE                ( 4 )
#define MB_PDU_FUNC_READ_COILCNT_OFF        ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READ_VALUES_OFF         ( MB_PDU_DATA_OFF + 1 )
#define MB_PDU_FUNC_READ_SIZE_MIN           ( 1 )

#define MB_PDU_REQ_WRITE_ADDR_OFF           ( MB_PDU_DATA_OFF )
#define MB_PDU_REQ_WRITE_VALUE_OFF          ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_WRITE_SIZE               ( 4 )
#define MB_PDU_FUNC_WRITE_ADDR_OFF          ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_VALUE_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE              ( 4 )

#define MB_PDU_REQ_WRITE_MUL_ADDR_OFF       ( MB_PDU_DATA_OFF )
#define MB_PDU_REQ_WRITE_MUL_COILCNT_OFF    ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_WRITE_MUL_BYTECNT_OFF    ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_REQ_WRITE_MUL_VALUES_OFF     ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_REQ_WRITE_MUL_SIZE_MIN       ( 5 )
#define MB_PDU_REQ_WRITE_MUL_COILCNT_MAX    ( 0x07B0 )
#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF      ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF   ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE          ( 5 )

/* ----------------------- Start implementation -----------------------------*/
#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED
 
#if MB_FUNC_READ_COILS_ENABLED
/***********************************************************************************
 * @brief  主栈读线圈
 * @param  psMBMasterInfo  主栈信息块
 * @param  ucSndAddr      从栈地址
 * @param  usCoilAddr     线圈起始地址
 * @param  usNCoils       线圈个数
 * @param  lTimeOut       主栈等待时间（0则一直等待）
 * @return error          错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode
eMBMasterReqReadCoils(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr,
                      USHORT usCoilAddr, USHORT usNCoils, ULONG ulTimeOut)
{
    UCHAR  *pucMBFrame  = NULL;
	sMBMasterDevsInfo*  psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;          //从设备状态表
    sMBMasterPort*      psMBPort     = &psMBMasterInfo->sMBPort;
	
    if( (ucSndAddr < psMBDevsInfo->ucSlaveDevMinAddr) || (ucSndAddr > psMBDevsInfo->ucSlaveDevMaxAddr) ) 
	{
        return MB_MRE_ILL_ARG;
	}		
    else if (xMBMasterRunResTake(psMBPort, ulTimeOut) == FALSE)
    {
        return MB_MRE_MASTER_BUSY;
	}		
    else
    {
		vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
		vMBMasterSetDestAddress(psMBMasterInfo, ucSndAddr);
		
		*(pucMBFrame + MB_PDU_FUNC_OFF)                 = MB_FUNC_READ_COILS;     //功能码01
		*(pucMBFrame + MB_PDU_REQ_READ_ADDR_OFF)        = usCoilAddr >> 8;        //线圈地址高位
        *(pucMBFrame + MB_PDU_REQ_READ_ADDR_OFF + 1)    = (UCHAR)usCoilAddr;             //线圈地址低位
		*(pucMBFrame + MB_PDU_REQ_READ_COILCNT_OFF )    = usNCoils >> 8;          //线圈个数高位
        *(pucMBFrame + MB_PDU_REQ_READ_COILCNT_OFF + 1) = (UCHAR)usNCoils;               //线圈个数低位
		
		vMBMasterSetPDUSndLength( psMBMasterInfo, MB_PDU_SIZE_MIN + MB_PDU_REQ_READ_SIZE );
        
		(void) xMBMasterPortEventPost( psMBPort, EV_MASTER_FRAME_SENT );     //主栈发送请求
        return eMBMasterWaitRequestFinish(psMBPort);                     //等待数据响应，会阻塞线程
    }
}

/***********************************************************************************
 * @brief  读线圈功能函数
 * @param  psMBMasterInfo  主栈信息块
 * @param  pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen          缓冲区长度
 * @return eMBException   异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException
eMBMasterFuncReadCoils( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen )
{
    
    UCHAR  ucByteCount;
    USHORT usCoilAddress, usCoilCount;
    UCHAR*       pucMBFrame = NULL;
    eMBErrorCode eRegStatus = MB_ENOERR;

    /* If this request is broadcast, and it's read mode. This request don't need execute. */
    if ( xMBMasterRequestIsBroadcast(psMBMasterInfo) )
    {
        return MB_EX_NONE;
    }
    else if ( *usLen >= MB_PDU_SIZE_MIN + MB_PDU_FUNC_READ_SIZE_MIN )
    {
    	vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
		
        usCoilAddress  = (USHORT)( *(pucMBFrame + MB_PDU_REQ_READ_ADDR_OFF) << 8 );   //赋值地址
        usCoilAddress |= (USHORT)( *(pucMBFrame + MB_PDU_REQ_READ_ADDR_OFF + 1) );
        usCoilAddress++;

        usCoilCount  = (USHORT)( *(pucMBFrame + MB_PDU_REQ_READ_COILCNT_OFF) << 8 );  //线圈数量
        usCoilCount |= (USHORT)( *(pucMBFrame + MB_PDU_REQ_READ_COILCNT_OFF + 1) );

        /* Test if the quantity of coils is a multiple of 8. If not last
         * byte is only partially field with unused coils set to zero. */
        if( (usCoilCount & 0x0007) != 0 )
        {
        	ucByteCount = (UCHAR)(usCoilCount/8 + 1);
        }
        else
        {
        	ucByteCount = (UCHAR)(usCoilCount/8);
        }

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( (usCoilCount >= 1) && (ucByteCount == *(pucFrame + MB_PDU_FUNC_READ_COILCNT_OFF)) )
        {
        	/* Make callback to fill the buffer. */
            eRegStatus = eMBMasterRegCoilsCB(psMBMasterInfo, pucFrame + MB_PDU_FUNC_READ_VALUES_OFF,
                                              usCoilAddress, usCoilCount, MB_BIT_READ);  //回调函数

            /* If an error occured convert it into a Modbus exception. */
            if(eRegStatus != MB_ENOERR)
            {
                return prveMBMasterError2Exception(eRegStatus);
            }
        }
        else
        {
            return MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid read coil register request because the length
         * is incorrect. */
        return MB_EX_ILLEGAL_DATA_VALUE;
    }
    return MB_EX_NONE;
}
#endif

#if MB_FUNC_WRITE_COIL_ENABLED
/***********************************************************************************
 * @brief  主栈写单个线圈
 * @param  psMBMasterInfo  主栈信息块
 * @param  ucSndAddr      从栈地址
 * @param  usCoilAddr     线圈起始地址
 * @param  usNCoils       线圈个数
 * @param  usMBBitData      线圈数值
 * @param  lTimeOut       主栈等待时间（0则一直等待）
 * @return error          错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode
eMBMasterReqWriteCoil(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr,
                      USHORT usCoilAddr, USHORT usMBBitData, ULONG ulTimeOut)
{
    UCHAR  *pucMBFrame  = NULL;
	sMBMasterDevsInfo*  psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;          //从设备状态表
    sMBMasterPort*      psMBPort     = &psMBMasterInfo->sMBPort;
    	
    if( (ucSndAddr < psMBDevsInfo->ucSlaveDevMinAddr) || (ucSndAddr > psMBDevsInfo->ucSlaveDevMaxAddr) ) 
	{
        return MB_MRE_ILL_ARG;
	}		
    else if( (usMBBitData != 0xFF00) && (usMBBitData != 0x0000) ) 
	{
        return MB_MRE_ILL_ARG;
	}
    else if(xMBMasterRunResTake(psMBPort, ulTimeOut) == FALSE )
	{
        return MB_MRE_MASTER_BUSY;
	}
    else
    {
		vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
		vMBMasterSetDestAddress(psMBMasterInfo, ucSndAddr);
		
		*(pucMBFrame + MB_PDU_FUNC_OFF)                = MB_FUNC_WRITE_SINGLE_COIL;       //功能码05
		*(pucMBFrame + MB_PDU_REQ_WRITE_ADDR_OFF)      = usCoilAddr >> 8;                 //线圈地址高位
        *(pucMBFrame + MB_PDU_REQ_WRITE_ADDR_OFF + 1)  = (UCHAR)usCoilAddr;               //线圈地址低位
		*(pucMBFrame + MB_PDU_REQ_WRITE_VALUE_OFF )    = usMBBitData >> 8;                //线圈个数高位
        *(pucMBFrame + MB_PDU_REQ_WRITE_VALUE_OFF + 1) = (UCHAR)usMBBitData;              //线圈个数低位
		
		vMBMasterSetPDUSndLength( psMBMasterInfo, MB_PDU_SIZE_MIN + MB_PDU_REQ_WRITE_SIZE );

		(void) xMBMasterPortEventPost(psMBPort, EV_MASTER_FRAME_SENT);       //主栈发送请求
        return eMBMasterWaitRequestFinish(psMBPort);                   //等待数据响应，会阻塞线程
    }
}

/***********************************************************************************
 * @brief 写单个线圈功能函数
 * @param  psMBMasterInfo  主栈信息块
 * @param pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException
eMBMasterFuncWriteCoil(sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen)
{
    USHORT usRegAddress;
    UCHAR  ucBuf[2];

    eMBErrorCode    eRegStatus = MB_ENOERR;

    if( *usLen == (MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN) )
    {
        usRegAddress = (USHORT)( pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8 );    //赋值地址
        usRegAddress |= (USHORT)( pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1] );
        usRegAddress++;

        if( (*(pucFrame + MB_PDU_FUNC_WRITE_VALUE_OFF + 1) == 0x00) &&
            ( (*(pucFrame + MB_PDU_FUNC_WRITE_VALUE_OFF) == 0xFF) || (*(pucFrame + MB_PDU_FUNC_WRITE_VALUE_OFF) == 0x00) ) )
        {
            ucBuf[1] = 0;
            if( *(pucFrame + MB_PDU_FUNC_WRITE_VALUE_OFF) == 0xFF )
            {
                ucBuf[0] = 1 << (usRegAddress - 1);
            }
            else
            {
                ucBuf[0] = 0;
            }
            eRegStatus = eMBMasterRegCoilsCB(psMBMasterInfo, &ucBuf[0], usRegAddress, 1, MB_BIT_WRITE); //功能回调函数

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR )
            {
                return prveMBMasterError2Exception(eRegStatus);
            }
        }
        else
        {
            return MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid write coil register request because the length
         * is incorrect. */
        return MB_EX_ILLEGAL_DATA_VALUE;
    }
    return MB_EX_NONE;
}
#endif

#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED
 /***********************************************************************************
 * @brief  主栈写多个线圈
 * @param  psMBMasterInfo  主栈信息块
 * @param  ucSndAddr      从栈地址
 * @param  usCoilAddr     线圈起始地址
 * @param  usNCoils       线圈个数
 * @param  pucDataBuffer  线圈数值数据指针
 * @param  lTimeOut       主栈等待时间（0则一直等待）
 * @return error          错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode
eMBMasterReqWriteMultipleCoils(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usCoilAddr,
                                USHORT usNCoils, UCHAR* pucDataBuffer, ULONG ulTimeOut)
{
    UCHAR  ucByteCount = 0;
    USHORT usRegIndex  = 0;
    UCHAR  *pucMBFrame = NULL;
    
	sMBMasterDevsInfo*  psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;          //从设备状态表
    sMBMasterPort*      psMBPort     = &psMBMasterInfo->sMBPort;

    if( (ucSndAddr < psMBDevsInfo->ucSlaveDevMinAddr) || (ucSndAddr > psMBDevsInfo->ucSlaveDevMaxAddr) ) 
	{
        return MB_MRE_ILL_ARG;
	}		
    else if ( usNCoils > MB_PDU_REQ_WRITE_MUL_COILCNT_MAX ) 
	{
        return MB_MRE_ILL_ARG;
	}
    else if ( xMBMasterRunResTake( psMBPort, ulTimeOut ) == FALSE )
	{
        return MB_MRE_MASTER_BUSY;
	}
    else
    {
		vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
		vMBMasterSetDestAddress(psMBMasterInfo, ucSndAddr);
		
		*(pucMBFrame + MB_PDU_FUNC_OFF)                      = MB_FUNC_WRITE_MULTIPLE_COILS;     //功能码15
		*(pucMBFrame + MB_PDU_REQ_WRITE_MUL_ADDR_OFF)        = usCoilAddr >> 8;                  //线圈地址高位
        *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_ADDR_OFF + 1)    = (UCHAR)usCoilAddr;                       //线圈地址低位
		*(pucMBFrame + MB_PDU_REQ_WRITE_MUL_COILCNT_OFF)     = usNCoils >> 8;                    //线圈个数高位
        *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_COILCNT_OFF + 1) = (UCHAR)usNCoils ;                        //线圈个数低位
		
		if( (usNCoils & 0x0007) != 0 )
        {
			ucByteCount = (UCHAR)(usNCoils/8 + 1);
        }
        else
        {
        	ucByteCount = (UCHAR)(usNCoils/8);
        }
		*(pucMBFrame + MB_PDU_REQ_WRITE_MUL_BYTECNT_OFF) = ucByteCount;
		pucMBFrame += MB_PDU_REQ_WRITE_MUL_VALUES_OFF;
		
		while( ucByteCount > usRegIndex)
		{
			*pucMBFrame++ = (UCHAR)( *(pucDataBuffer + (usRegIndex++)) );
		}
		vMBMasterSetPDUSndLength( psMBMasterInfo, MB_PDU_SIZE_MIN + MB_PDU_REQ_WRITE_MUL_SIZE_MIN + ucByteCount );
       
		(void)xMBMasterPortEventPost(psMBPort, EV_MASTER_FRAME_SENT);
        return eMBMasterWaitRequestFinish(psMBPort);
    }
}

/***********************************************************************************
 * @brief 写多个线圈功能函数
 * @param  psMBMasterInfo  主栈信息块
 * @param  pucFrame        Modbus的PDU缓冲区数据指针
 * @param  usLen           缓冲区长度
 * @return eMBException    异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException
eMBMasterFuncWriteMultipleCoils(sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen)
{
    USHORT usRegAddress, usCoilCnt;
    UCHAR  ucByteCount, ucByteCountVerify;
    UCHAR *pucMBFrame = NULL;

    eMBErrorCode    eRegStatus = MB_ENOERR;

    if( (*usLen == MB_PDU_FUNC_WRITE_MUL_SIZE) || xMBMasterRequestIsBroadcast(psMBMasterInfo) )
    {
    	vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
		
        usRegAddress  = (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_MUL_ADDR_OFF) << 8 );   //赋值地址
        usRegAddress |= (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1) );
        usRegAddress++;

        usCoilCnt  = (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF) << 8 );  //线圈数量
        usCoilCnt |= (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF + 1) );

        ucByteCount = *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_BYTECNT_OFF);

        /* Compute the number of expected bytes in the request. */
        if( ( usCoilCnt & 0x0007 ) != 0 )
        {
            ucByteCountVerify = (UCHAR)( usCoilCnt/8 + 1);
        }
        else
        {
            ucByteCountVerify = (UCHAR)( usCoilCnt/8);
        }

        if( (usCoilCnt >= 1) && (ucByteCountVerify == ucByteCount) )
        {
            eRegStatus = eMBMasterRegCoilsCB(psMBMasterInfo, pucMBFrame + MB_PDU_REQ_WRITE_MUL_VALUES_OFF,
                                              usRegAddress, usCoilCnt, MB_BIT_WRITE);     //线圈功能函数
            /* If an error occured convert it into a Modbus exception. */
            if(eRegStatus != MB_ENOERR)
            {
                return prveMBMasterError2Exception(eRegStatus);
            }
        }
        else
        {
            return MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        return MB_EX_ILLEGAL_DATA_VALUE;
    }
    return MB_EX_NONE;
}
#endif

#if MB_FUNC_READ_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED || MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED
/**
 * Modbus master coils callback function.
 *
 * @param pucRegBuffer coils buffer
 * @param usAddress coils address
 * @param usNCoils coils number
 * @param eMode read or write
 *
 * @return result
 */
eMBErrorCode 
eMBMasterRegCoilsCB(sMBMasterInfo* psMBMasterInfo, UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBBitMode eMode)
{
    USHORT COIL_START, COIL_END;
    UCHAR ucMBDestAddr = 0;
    sMBDevDataTable* psCoilTable = NULL;
    sMBSlaveDev* psMBSlaveDevCur = psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur;  //当前从设备

    if(psMBSlaveDevCur == NULL) //从设备模式
    {
        return MB_ENOERR;
    }
    psCoilTable = &psMBSlaveDevCur->psDevCurData->sMBCoilTable; //从设备通讯协议表
    ucMBDestAddr = ucMBMasterGetDestAddr(psMBMasterInfo);        //从设备通讯地址
    
    if(psMBSlaveDevCur->ucDevAddr != ucMBDestAddr) //如果当前从设备地址与要轮询从设备地址不一致，则更新从设备
    {
        psMBSlaveDevCur = psMBMasterGetDev(psMBMasterInfo, ucMBDestAddr);
        psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDevCur;
        psCoilTable = &psMBSlaveDevCur->psDevCurData->sMBCoilTable;
    }
    if( (psCoilTable == NULL) || (psCoilTable->pvDataBuf == NULL) ||
        (psCoilTable->usDataCount == 0)) //非空且数据点不为0
	{
		return MB_ENOREG;
	} 
    COIL_START = psCoilTable->usStartAddr;
    COIL_END = psCoilTable->usEndAddr;

    /* if mode is read,the master will write the received date to buffer. */
  
    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= COIL_START) && (usAddress + usNCoils -1 <= COIL_END))
    {
        return eMBMasterUtilSetBits(psMBMasterInfo, pucRegBuffer, usAddress, usNCoils, CoilData, eMode);
    }
    else
    {
        return MB_ENOREG;
    }
}
#endif

#endif
