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
 * File: $Id: mbfuncholding_m.c,v 1.60 2013/09/02 14:13:40 Armink Add Master Functions  Exp $
 */
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
#include "mbmap_m.h"
#include "mbscan_m.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_REQ_READ_ADDR_OFF                ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REQ_READ_REGCNT_OFF              ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_READ_SIZE                    ( 4 )
#define MB_PDU_FUNC_READ_REGCNT_MAX             ( 0x007D )
#define MB_PDU_FUNC_READ_BYTECNT_OFF            ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READ_VALUES_OFF             ( MB_PDU_DATA_OFF + 1 )
#define MB_PDU_FUNC_READ_SIZE_MIN               ( 1 )

#define MB_PDU_REQ_WRITE_ADDR_OFF               ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_REQ_WRITE_VALUE_OFF              ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_WRITE_SIZE                   ( 4 )
#define MB_PDU_FUNC_WRITE_ADDR_OFF              ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_WRITE_VALUE_OFF             ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE                  ( 4 )

#define MB_PDU_REQ_WRITE_MUL_ADDR_OFF           ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REQ_WRITE_MUL_REGCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_WRITE_MUL_BYTECNT_OFF        ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_REQ_WRITE_MUL_VALUES_OFF         ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_REQ_WRITE_MUL_SIZE_MIN           ( 5 )
#define MB_PDU_REQ_WRITE_MUL_REGCNT_MAX         ( 0x0078 )
#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF          ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE              ( 4 )

#define MB_PDU_REQ_READWRITE_READ_ADDR_OFF      ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REQ_READWRITE_READ_REGCNT_OFF    ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF     ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF   ( MB_PDU_DATA_OFF + 6 )
#define MB_PDU_REQ_READWRITE_WRITE_BYTECNT_OFF  ( MB_PDU_DATA_OFF + 8 )
#define MB_PDU_REQ_READWRITE_WRITE_VALUES_OFF   ( MB_PDU_DATA_OFF + 9 )
#define MB_PDU_REQ_READWRITE_SIZE_MIN           ( 9 )
#define MB_PDU_FUNC_READWRITE_READ_BYTECNT_OFF  ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READWRITE_READ_VALUES_OFF   ( MB_PDU_DATA_OFF + 1 )
#define MB_PDU_FUNC_READWRITE_SIZE_MIN          ( 1 )

/* ----------------------- Start implementation -----------------------------*/
#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED

#if MB_FUNC_WRITE_HOLDING_ENABLED
 /***********************************************************************************
 * @brief  主栈写单个保持寄存器
 * @param  ucSndAddr      从栈地址
 * @param  usRegAddr      起始地址
 * @param  usRegData      数值
 * @param  lTimeOut       主栈等待时间（0则一直等待）
 * @return error          错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode
eMBMasterReqWriteHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usRegAddr, USHORT usRegData, ULONG ulTimeOutMs)
{
    UCHAR *pucMBFrame = NULL;

    sMBMasterDevsInfo*    psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;   //从设备状态信息
    sMBMasterPort*        psMBPort     = &psMBMasterInfo->sMBPort;      //硬件结构

    if( (ucSndAddr < psMBDevsInfo->ucSlaveDevMinAddr) || (ucSndAddr > psMBDevsInfo->ucSlaveDevMaxAddr) )
    {
        return MB_MRE_ILL_ARG;
    }
    else if ( xMBMasterRunResTake(psMBPort, ulTimeOutMs) == FALSE )
    {
        return MB_MRE_MASTER_BUSY;
    }
    else
    {
        vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
        vMBMasterSetDestAddress(psMBMasterInfo, ucSndAddr);
        *(pucMBFrame + MB_PDU_FUNC_OFF)                = MB_FUNC_WRITE_REGISTER;
        *(pucMBFrame + MB_PDU_REQ_WRITE_ADDR_OFF)      = usRegAddr >> 8;
        *(pucMBFrame + MB_PDU_REQ_WRITE_ADDR_OFF + 1)  = (UCHAR)usRegAddr;
        *(pucMBFrame + MB_PDU_REQ_WRITE_VALUE_OFF)     = usRegData >> 8;
        *(pucMBFrame + MB_PDU_REQ_WRITE_VALUE_OFF + 1) = (UCHAR)usRegData ;

        vMBMasterSetPDUSndLength( psMBMasterInfo, MB_PDU_SIZE_MIN + MB_PDU_REQ_WRITE_SIZE );

        (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_FRAME_SENT);
        return eMBMasterWaitRequestFinish(psMBPort);
    }
}

/***********************************************************************************
 * @brief  写单个保持寄存器功能函数
 * @param  pucFrame      Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException
eMBMasterFuncWriteHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR* pucFrame, USHORT* usLen)
{
    USHORT          usRegAddress;
    eMBErrorCode    eRegStatus = MB_ENOERR;

    if( *usLen == ( MB_PDU_SIZE_MIN + MB_PDU_FUNC_WRITE_SIZE ) )
    {
        usRegAddress  = (USHORT)( (*(pucFrame + MB_PDU_FUNC_WRITE_ADDR_OFF)) << 8 );
        usRegAddress |= (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_ADDR_OFF + 1) );
        usRegAddress++;

        /* Make callback to update the value. */
        eRegStatus = eMBMasterRegHoldingCB( psMBMasterInfo, pucFrame + MB_PDU_FUNC_WRITE_VALUE_OFF,
                                            usRegAddress, 1, MB_REG_WRITE );
        /* If an error occured convert it into a Modbus exception. */
        if(eRegStatus != MB_ENOERR)
        {
            return prveMBMasterError2Exception(eRegStatus);
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        return MB_EX_ILLEGAL_DATA_VALUE;
    }
    return MB_EX_NONE;
}
#endif

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED
 /***********************************************************************************
 * @brief  主栈写多个保持寄存器
 * @param  ucSndAddr      从栈地址
 * @param  usRegAddr      起始地址
 * @param  usNRegs        个数
 * @param  pusDataBuffer  数值数据指针
 * @param  lTimeOut       主栈等待时间（0则一直等待）
 * @return error          错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode eMBMasterReqWriteMultipleHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr,
                                     USHORT usRegAddr, USHORT usNRegs, USHORT* pusDataBuffer, ULONG ulTimeOut)
{
    UCHAR*  pucMBFrame = NULL;
    USHORT  usRegIndex = 0;

    sMBMasterDevsInfo*  psMBDevsInfo  = &psMBMasterInfo->sMBDevsInfo;   //从设备状态信息
    sMBMasterPort*      psMBPort      = &psMBMasterInfo->sMBPort;      //硬件结构

    if( (ucSndAddr < psMBDevsInfo->ucSlaveDevMinAddr) || (ucSndAddr > psMBDevsInfo->ucSlaveDevMaxAddr) )
    {
        return MB_MRE_ILL_ARG;
    }
    else if(xMBMasterRunResTake(psMBPort, ulTimeOut) == FALSE)
    {
        return MB_MRE_MASTER_BUSY;
    }
    else
    {
        vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
        vMBMasterSetDestAddress(psMBMasterInfo, ucSndAddr);
        *(pucMBFrame + MB_PDU_FUNC_OFF)                     = MB_FUNC_WRITE_MULTIPLE_REGISTERS;
        *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_ADDR_OFF)       = usRegAddr >> 8;
        *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_ADDR_OFF + 1)   = (UCHAR)usRegAddr;
        *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_REGCNT_OFF)     = usNRegs >> 8;
        *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_REGCNT_OFF + 1) = (UCHAR)usNRegs ;
        *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_BYTECNT_OFF)    = (UCHAR)usNRegs * 2;
        pucMBFrame += MB_PDU_REQ_WRITE_MUL_VALUES_OFF;

        while( usNRegs > usRegIndex)
        {
            *pucMBFrame++ = (UCHAR)( *(pusDataBuffer + usRegIndex) >> 8);
            *pucMBFrame++ = (UCHAR)( *(pusDataBuffer + (usRegIndex++)) );
        }
        vMBMasterSetPDUSndLength(psMBMasterInfo, MB_PDU_SIZE_MIN + MB_PDU_REQ_WRITE_MUL_SIZE_MIN + 2*usNRegs);
        vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);

//	    debug("ucSlaveAddr %d  eMBMasterReqWriteMultipleHoldingRegister    \n",ucSndAddr);

        (void) xMBMasterPortEventPost(psMBPort, EV_MASTER_FRAME_SENT);
        return eMBMasterWaitRequestFinish(psMBPort);
    }
}

/***********************************************************************************
 * @brief  写多个保持寄存器功能函数
 * @param  pucFrame      Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException
eMBMasterFuncWriteMultipleHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR* pucFrame, USHORT* usLen)
{
    UCHAR  ucRegByteCount;
    USHORT usRegAddress, usRegCount;

    UCHAR        *pucMBFrame = NULL;
    eMBErrorCode  eRegStatus = MB_ENOERR;
    /* If this request is broadcast, the *usLen is not need check. */
    if( ( *usLen == MB_PDU_SIZE_MIN + MB_PDU_FUNC_WRITE_MUL_SIZE ) || xMBMasterRequestIsBroadcast(psMBMasterInfo) )
    {
        vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
        usRegAddress = ( USHORT )( (*(pucMBFrame + MB_PDU_REQ_WRITE_MUL_ADDR_OFF )) << 8 );
        usRegAddress |= ( USHORT )( *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_ADDR_OFF + 1) );
        usRegAddress++;

        usRegCount  = (USHORT)( (*(pucMBFrame + MB_PDU_REQ_WRITE_MUL_REGCNT_OFF )) << 8 );
        usRegCount |= (USHORT)( *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_REGCNT_OFF + 1) );
        ucRegByteCount = *(pucMBFrame + MB_PDU_REQ_WRITE_MUL_BYTECNT_OFF);

        if(ucRegByteCount == 2*usRegCount)
        {
            /* Make callback to update the register values. */
            eRegStatus = eMBMasterRegHoldingCB(psMBMasterInfo, pucFrame + MB_PDU_REQ_WRITE_MUL_VALUES_OFF,
                                               usRegAddress, usRegCount, MB_REG_WRITE);
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
        /* Can't be a valid request because the length is incorrect. */
        return MB_EX_ILLEGAL_DATA_VALUE;
    }
    return MB_EX_NONE;
}
#endif

#if MB_FUNC_READ_HOLDING_ENABLED
 /***********************************************************************************
 * @brief  主栈读保持寄存器
 * @param  ucSndAddr      从栈地址
 * @param  usRegAddr      起始地址
 * @param  lTimeOut       主栈等待时间（0则一直等待）
 * @return error          错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode
eMBMasterReqReadHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr,
                                USHORT usRegAddr, USHORT usNRegs, ULONG ulTimeOut)
{
    UCHAR* pucMBFrame = NULL;

    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;      //硬件结构
    sMBMasterDevsInfo* psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;  //从设备状态信息

    if( (ucSndAddr < psMBDevsInfo->ucSlaveDevMinAddr) || (ucSndAddr > psMBDevsInfo->ucSlaveDevMaxAddr) )
    {
        return MB_MRE_ILL_ARG;
    }
    else if(xMBMasterRunResTake(psMBPort, ulTimeOut) == FALSE)
    {
        return MB_MRE_MASTER_BUSY;
    }
    else
    {
        vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
        vMBMasterSetDestAddress(psMBMasterInfo, ucSndAddr);
        *(pucMBFrame + MB_PDU_FUNC_OFF)                = MB_FUNC_READ_HOLDING_REGISTER;
        *(pucMBFrame + MB_PDU_REQ_READ_ADDR_OFF)       = usRegAddr >> 8;
        *(pucMBFrame + MB_PDU_REQ_READ_ADDR_OFF + 1)   = (UCHAR)usRegAddr;
        *(pucMBFrame + MB_PDU_REQ_READ_REGCNT_OFF)     = usNRegs >> 8;
        *(pucMBFrame + MB_PDU_REQ_READ_REGCNT_OFF + 1) = (UCHAR)usNRegs;

        psMBMasterInfo->usRcvRequestBytes = usNRegs * 2 + 5;

        vMBMasterSetPDUSndLength(psMBMasterInfo, MB_PDU_SIZE_MIN + MB_PDU_REQ_READ_SIZE);
        (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_FRAME_SENT);

//        debug("eMBMasterReqWriteMultipleHoldingRegister  ucSlaveAddr %d usRegAddr %d usNRegs %d\n",ucSndAddr, usRegAddr, usNRegs);
        return eMBMasterWaitRequestFinish(psMBPort);
    }
}

/***********************************************************************************
 * @brief  读保持寄存器功能函数
 * @param  pucFrame      Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException
eMBMasterFuncReadHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR  *pucRcvFrame, USHORT *usLen)
{
    USHORT usRegAddress, usRegCount;

    UCHAR *pucMBSndFrame = NULL;
    eMBErrorCode eRegStatus = MB_ENOERR;

    /* If this request is broadcast, and it's read mode. This request don't need execute. */
    if(xMBMasterRequestIsBroadcast(psMBMasterInfo))
    {
        return MB_EX_NONE;
    }
    else if( *usLen >= MB_PDU_SIZE_MIN + MB_PDU_FUNC_READ_SIZE_MIN )
    {
        vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBSndFrame);
        usRegAddress  = (USHORT)( (*(pucMBSndFrame + MB_PDU_REQ_READ_ADDR_OFF)) << 8 );
        usRegAddress |= (USHORT)( *(pucMBSndFrame + MB_PDU_REQ_READ_ADDR_OFF + 1) ) ;
        usRegAddress++;

        usRegCount  = (USHORT)( (*(pucMBSndFrame + MB_PDU_REQ_READ_REGCNT_OFF)) << 8 );
        usRegCount |= (USHORT)( *(pucMBSndFrame + MB_PDU_REQ_READ_REGCNT_OFF + 1) );

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception.
         */

        //debug("eMBMasterFuncReadHoldingRegister usRegCount %d  code %d \n",  usRegCount, *(pucMBSndFrame));

        if( (usRegCount >= 1) && ( 2*usRegCount == *(pucRcvFrame + MB_PDU_FUNC_READ_BYTECNT_OFF)) )
        {
            /* Make callback to fill the buffer. */
            eRegStatus = eMBMasterRegHoldingCB(psMBMasterInfo, pucRcvFrame + MB_PDU_FUNC_READ_VALUES_OFF,
                                               usRegAddress, usRegCount, MB_REG_READ);
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
        /* Can't be a valid request because the length is incorrect. */
        return MB_EX_ILLEGAL_DATA_VALUE;
    }
    return MB_EX_NONE;
}
#endif

#if MB_FUNC_READWRITE_HOLDING_ENABLED
 /***********************************************************************************
 * @brief  主栈读写多个保持寄存器
 * @param  ucSndAddr            从栈地址
 * @param  usReadRegAddr        读起始地址
 * @param  usNReadRegs          个数
 * @param  pusDataBuffer        数值数据指针
  * @param  usWriteRegAddr      写起始地址
 * @param  usNWriteRegs         个数
 * @param  lTimeOut             主栈等待时间（0则一直等待）
 * @return error                错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode
eMBMasterReqReadWriteMultipleHoldingRegister( sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr,
                                              USHORT usReadRegAddr, USHORT usNReadRegs, USHORT * pusDataBuffer,
                                              USHORT usWriteRegAddr, USHORT usNWriteRegs, ULONG ulTimeOut )
{
    UCHAR  *pucMBFrame = NULL;
    USHORT  usRegIndex = 0;

    sMBMasterDevsInfo*    psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;    //从设备状态信息
    sMBMasterPort*            psMBPort = &psMBMasterInfo->sMBPort;      //硬件结构

    if( (ucSndAddr < psMBDevsInfo->ucSlaveDevMinAddr) || (ucSndAddr > psMBDevsInfo->ucSlaveDevMaxAddr) )
    {
        return MB_MRE_ILL_ARG;
    }
    else if ( xMBMasterRunResTake(psMBPort, ulTimeOut) == FALSE )
    {
        return MB_MRE_MASTER_BUSY;
    }
    else
    {
        vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
        vMBMasterSetDestAddress(psMBMasterInfo, ucSndAddr);
        *(pucMBFrame + MB_PDU_FUNC_OFF)                           = MB_FUNC_READWRITE_MULTIPLE_REGISTERS;

        *(pucMBFrame + MB_PDU_REQ_READWRITE_READ_ADDR_OFF)        = usReadRegAddr >> 8;
        *(pucMBFrame + MB_PDU_REQ_READWRITE_READ_ADDR_OFF + 1)    = (UCHAR)usReadRegAddr;

        *(pucMBFrame + MB_PDU_REQ_READWRITE_READ_REGCNT_OFF)      = usNReadRegs >> 8;
        *(pucMBFrame + MB_PDU_REQ_READWRITE_READ_REGCNT_OFF + 1)  = (UCHAR)usNReadRegs ;

        *(pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF)       = usWriteRegAddr >> 8;
        *(pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF + 1)   = (UCHAR)usWriteRegAddr;

        *(pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF)     = usNWriteRegs >> 8;
        *(pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF + 1) = (UCHAR)usNWriteRegs ;

        *(pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_BYTECNT_OFF)    = (UCHAR)usNWriteRegs * 2;
        pucMBFrame += MB_PDU_REQ_READWRITE_WRITE_VALUES_OFF;

        while( usNWriteRegs > usRegIndex)
        {
            *pucMBFrame++ = (UCHAR)( *(pusDataBuffer + usRegIndex) >> 8);
            *pucMBFrame++ = (UCHAR)( *(pusDataBuffer + (usRegIndex++)) );
        }
        vMBMasterSetPDUSndLength(psMBMasterInfo, MB_PDU_SIZE_MIN + MB_PDU_REQ_READWRITE_SIZE_MIN + 2*usNWriteRegs);

        (void) xMBMasterPortEventPost(psMBPort, EV_MASTER_FRAME_SENT);
        return eMBMasterWaitRequestFinish(psMBPort);
    }
}

/***********************************************************************************
 * @brief 读写保持寄存器功能函数
 * @param pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException
eMBMasterFuncReadWriteMultipleHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR *pucFrame, USHORT *usLen)
{
    USHORT usRegReadAddress, usRegReadCount, usRegWriteAddress, usRegWriteCount;
    UCHAR        *pucMBFrame = NULL;
    eMBErrorCode  eRegStatus = MB_ENOERR;
    /* If this request is broadcast, and it's read mode. This request don't need execute. */
    if ( xMBMasterRequestIsBroadcast(psMBMasterInfo) )
    {
        return MB_EX_NONE;
    }
    else if( *usLen >= MB_PDU_SIZE_MIN + MB_PDU_FUNC_READWRITE_SIZE_MIN )
    {
        vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
        usRegReadAddress  = (USHORT)( (*(pucMBFrame + MB_PDU_REQ_READWRITE_READ_ADDR_OFF)) << 8U );
        usRegReadAddress |= (USHORT)( *(pucMBFrame + MB_PDU_REQ_READWRITE_READ_ADDR_OFF + 1) );
        usRegReadAddress++;

        usRegReadCount  = (USHORT)( (*(pucMBFrame + MB_PDU_REQ_READWRITE_READ_REGCNT_OFF)) << 8U );
        usRegReadCount |= (USHORT)( *(pucMBFrame + MB_PDU_REQ_READWRITE_READ_REGCNT_OFF + 1) );

        usRegWriteAddress  = (USHORT)( (*(pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF)) << 8U );
        usRegWriteAddress |= (USHORT)( *(pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF + 1) );
        usRegWriteAddress++;

        usRegWriteCount  = (USHORT)( (*(pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF)) << 8U );
        usRegWriteCount |= (USHORT)( *(pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF + 1) );

        if( (2 * usRegReadCount) == *(pucFrame + MB_PDU_FUNC_READWRITE_READ_BYTECNT_OFF) )
        {
            /* Make callback to update the register values. */
            eRegStatus = eMBMasterRegHoldingCB(psMBMasterInfo, pucMBFrame + MB_PDU_REQ_READWRITE_WRITE_VALUES_OFF,
                                           usRegWriteAddress, usRegWriteCount, MB_REG_WRITE );

            if(eRegStatus == MB_ENOERR)
            {
                /* Make the read callback. */
                eRegStatus = eMBMasterRegHoldingCB(psMBMasterInfo, pucFrame + MB_PDU_FUNC_READWRITE_READ_VALUES_OFF,
                                              usRegReadAddress, usRegReadCount, MB_REG_READ);
            }
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
    return MB_EX_NONE;
}
#endif

#if MB_FUNC_WRITE_HOLDING_ENABLED || MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED \
    || MB_FUNC_READ_HOLDING_ENABLED || MB_FUNC_READWRITE_HOLDING_ENABLED
/**
 * Modbus master holding register callback function.
 *
 * @param pucRegBuffer holding register buffer
 * @param usAddress holding register address
 * @param usNRegs holding register number
 * @param eMode read or write
 *
 * @return result
 */
eMBErrorCode eMBMasterRegHoldingCB(sMBMasterInfo* psMBMasterInfo, UCHAR* pucRegBuffer, USHORT usAddress,
        USHORT usNRegs, eMBRegisterMode eMode)
{
    USHORT iRegIndex, REG_HOLDING_START, REG_HOLDING_END;

    USHORT usRegHoldValue = 0;
    SHORT  sRegHoldValue = 0;
    int8_t cRegHoldValue = 0;
    
    sMasterRegHoldData* pvRegHoldValue  = NULL;
    sMBSlaveDev* psMBSlaveDevCur = NULL;
    sMBDevDataTable *psMBRegHoldTable = NULL;
    UCHAR ucMBDestAddr = 1;
    if(psMBMasterInfo->eMBRunMode != STATE_SCAN_DEV) //非轮询从设备模式
    {
        return MB_ENOERR;
    }
    psMBSlaveDevCur = psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur;    //当前从设备

    if(psMBSlaveDevCur == NULL) //从设备模式
    {
        return MB_ENOERR;
    }
    psMBRegHoldTable = &psMBSlaveDevCur->psDevCurData->sMBRegHoldTable; //从设备通讯协议表
    ucMBDestAddr = ucMBMasterGetDestAddr(psMBMasterInfo);  //从设备通讯地址

    if(psMBSlaveDevCur->ucDevAddr != ucMBDestAddr) //如果当前从设备地址与要轮询从设备地址不一致，则更新从设备
    {
        psMBSlaveDevCur = psMBMasterGetDev(psMBMasterInfo, ucMBDestAddr);
        psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDevCur;
        psMBRegHoldTable = &psMBSlaveDevCur->psDevCurData->sMBRegHoldTable;
    }
    if( (psMBRegHoldTable == NULL) || (psMBRegHoldTable->pvDataBuf  == NULL) ||
        (psMBRegHoldTable->usDataCount == 0)) //非空且数据点不为0
    {
        return MB_ENOREG;
    }
    REG_HOLDING_START = psMBRegHoldTable->usStartAddr;
    REG_HOLDING_END = psMBRegHoldTable->usEndAddr;

    /* if mode is read, the master will write the received date to buffer. */

    /* it already plus one in modbus function method. */
    usAddress--;
    if ( (usAddress < REG_HOLDING_START) || (usAddress + usNRegs-1 > REG_HOLDING_END))
    {
        return MB_ENOREG;
    }
    iRegIndex = usAddress ;

    switch (eMode)
    {
    /* read current register values from the protocol stack. */
    case MB_REG_READ:
        while (usNRegs > 0)
        {
            pvRegHoldValue = NULL;
            (void)eMBMasterRegHoldingMap(psMBMasterInfo, ucMBDestAddr, iRegIndex, &pvRegHoldValue);     //扫描字典中变量，找出对应的变量

            usRegHoldValue = ( (USHORT)(*pucRegBuffer++) ) << 8;
            usRegHoldValue |=( (USHORT)(*pucRegBuffer++) ) & 0xFF;

            if( (pvRegHoldValue != NULL) && (pvRegHoldValue->pvValue != NULL) && (pvRegHoldValue->ucAccessMode != WO) )
            {
                if( (pvRegHoldValue->ucTmitMult != 0) && (pvRegHoldValue->ucTmitMult != 1) )
                {
                    usRegHoldValue = (USHORT)(usRegHoldValue / pvRegHoldValue->ucTmitMult);     //传输因子
                }
                if (pvRegHoldValue->ucDataType == uint16)
                {
                    if( (usRegHoldValue >= (USHORT)pvRegHoldValue->usMinVal ) && (usRegHoldValue <= (USHORT)pvRegHoldValue->usMaxVal))
                    {
                        if( (pvRegHoldValue->ucAccessMode == RW) && (*(USHORT*)pvRegHoldValue->pvValue != (USHORT)pvRegHoldValue->usPreVal) &&
                            (psMBMasterInfo->eMBRunMode == STATE_SCAN_DEV) )
                        {
                            pvRegHoldValue->usPreVal = (USHORT)usRegHoldValue;
                            iRegIndex++;
                            usNRegs--;
                            continue;    //此时命令更新了，所以不更新实际点位，只更新先前值
                        }
                        *(USHORT*)pvRegHoldValue->pvValue = (USHORT)usRegHoldValue;    //更新对应点位
                        pvRegHoldValue->usPreVal = (USHORT)usRegHoldValue;
                    }
                }
                else if(pvRegHoldValue->ucDataType == uint8)
                {
                    if( ((UCHAR)usRegHoldValue >= (UCHAR)pvRegHoldValue->usMinVal ) && ((UCHAR)usRegHoldValue <= (UCHAR)pvRegHoldValue->usMaxVal) )
                    {
                        if( (pvRegHoldValue->ucAccessMode == RW) && (*(UCHAR*)pvRegHoldValue->pvValue != (UCHAR)pvRegHoldValue->usPreVal) &&
                            (psMBMasterInfo->eMBRunMode == STATE_SCAN_DEV) )
                        {
                            pvRegHoldValue->usPreVal = (UCHAR)usRegHoldValue;
                            iRegIndex++;
                            usNRegs--;
                            continue;    //此时命令更新了，所以不更新实际点位，只更新先前值
                        }
                        *(UCHAR*)pvRegHoldValue->pvValue = (UCHAR)usRegHoldValue;
                        pvRegHoldValue->usPreVal = (USHORT)usRegHoldValue;
                    }
                }
                else if (pvRegHoldValue->ucDataType == int16)
                {
                    sRegHoldValue = (SHORT)usRegHoldValue;
                    if( (sRegHoldValue >= (SHORT)pvRegHoldValue->usMinVal ) && (sRegHoldValue <= (SHORT)pvRegHoldValue->usMaxVal) )
                    {
                         if( (pvRegHoldValue->ucAccessMode == RW) && (*(SHORT*)pvRegHoldValue->pvValue != (SHORT)pvRegHoldValue->usPreVal) &&
                             (psMBMasterInfo->eMBRunMode == STATE_SCAN_DEV) )
                         {
                             pvRegHoldValue->usPreVal = (USHORT)sRegHoldValue;
                             iRegIndex++;
                             usNRegs--;
                             continue;    //此时实际点位有更新，所以不更新实际点位，只更新先前值
                         }
                         *(SHORT*)pvRegHoldValue->pvValue = (SHORT)sRegHoldValue;
                         pvRegHoldValue->usPreVal = (USHORT)sRegHoldValue;
                    }
                }
                else if(pvRegHoldValue->ucDataType == int8)
                {
                    cRegHoldValue = (int8_t)usRegHoldValue;
                    if( (cRegHoldValue >= (int8_t)pvRegHoldValue->usMinVal ) && (cRegHoldValue <= (int8_t)pvRegHoldValue->usMaxVal) )
                    {
                        if( (pvRegHoldValue->ucAccessMode == RW) && (*(int8_t*)pvRegHoldValue->pvValue != (int8_t)pvRegHoldValue->usPreVal) &&
                            (psMBMasterInfo->eMBRunMode == STATE_SCAN_DEV) )
                        {
                            pvRegHoldValue->usPreVal = (USHORT)cRegHoldValue;
                            iRegIndex++;
                            usNRegs--;
                            continue;    //此时命令更新了，所以不更新实际点位，只更新先前值
                        }
                        *(int8_t*)pvRegHoldValue->pvValue = (int8_t)cRegHoldValue;
                        pvRegHoldValue->usPreVal = (USHORT)cRegHoldValue;
                    }
                }
            }
            iRegIndex++;
            usNRegs--;
        }
    break;

    /* write current register values with new values from the protocol stack. */
    case MB_REG_WRITE:
        while (usNRegs > 0)
        {
            pvRegHoldValue = NULL;
            (void)eMBMasterRegHoldingMap(psMBMasterInfo, ucMBDestAddr, iRegIndex, &pvRegHoldValue);     //扫描字典中变量，找出对应的变量
            if( (pvRegHoldValue != NULL) && (pvRegHoldValue->pvValue != NULL) && (pvRegHoldValue->ucAccessMode != RO) )
            {
                if (pvRegHoldValue->ucDataType == uint16)
                {
                    usRegHoldValue = *(USHORT*)pvRegHoldValue->pvValue;
                    if( (usRegHoldValue >= (USHORT)pvRegHoldValue->usMinVal ) && (usRegHoldValue <= (USHORT)pvRegHoldValue->usMaxVal))
                    {
                        pvRegHoldValue->usPreVal = (USHORT)usRegHoldValue;		 //更新对应点位
                    }
                }
                else if(pvRegHoldValue->ucDataType == uint8)
                {
                    usRegHoldValue = *(UCHAR*)pvRegHoldValue->pvValue;
                    if( ((UCHAR)usRegHoldValue >= (UCHAR)pvRegHoldValue->usMinVal ) && ((UCHAR)usRegHoldValue <= (UCHAR)pvRegHoldValue->usMaxVal) )
                    {
                        pvRegHoldValue->usPreVal = (USHORT)usRegHoldValue;
                    }
                }
                else if (pvRegHoldValue->ucDataType == int16)
                {
                    sRegHoldValue = *(SHORT*)pvRegHoldValue->pvValue;
                    if( (sRegHoldValue >= (SHORT)pvRegHoldValue->usMinVal ) && (sRegHoldValue <= (SHORT)pvRegHoldValue->usMaxVal) )
                    {
                        pvRegHoldValue->usPreVal = (USHORT)sRegHoldValue;
                    }
                }
                else if(pvRegHoldValue->ucDataType == int8)
                {
                    cRegHoldValue = *(int8_t*)pvRegHoldValue->pvValue;
                    if( (cRegHoldValue >= (int8_t)pvRegHoldValue->usMinVal ) && (cRegHoldValue <= (int8_t)pvRegHoldValue->usMaxVal) )
                    {
                        pvRegHoldValue->usPreVal = (USHORT)cRegHoldValue;
                    }
                }
            }
            iRegIndex++;
            usNRegs--;
        }
    break;
    }
    return MB_ENOERR;
}
#endif

#endif

