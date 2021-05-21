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
 * File: $Id: mbfunccoils.c,v 1.8 2007/02/18 23:47:16 wolti Exp $
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
#include "mbbits.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_READ_COILCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
#define MB_PDU_FUNC_READ_COILCNT_MAX        ( 0x07D0 )

#define MB_PDU_FUNC_WRITE_ADDR_OFF          ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_VALUE_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE              ( 4 )

#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF      ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF   ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF   ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF    ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN      ( 5 )
#define MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX   ( 0x07B0 )


/* ----------------------- Start implementation -----------------------------*/

#if MB_FUNC_READ_COILS_ENABLED
/***********************************************************************************
 * @brief 读线圈功能函数
 * @param pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException 
eMBSlaveFuncReadCoils(sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucFrame, USHORT* usLen)
{
    USHORT usRegAddress, usCoilCount;
    UCHAR  ucNBytes;
    UCHAR *pucFrameCur;

    eMBErrorCode eRegStatus;
    if( *usLen == (MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN) )
    {
        usRegAddress  = (USHORT)( *(pucFrame + MB_PDU_FUNC_READ_ADDR_OFF) << 8 );
        usRegAddress |= (USHORT)( *(pucFrame + MB_PDU_FUNC_READ_ADDR_OFF + 1) );
        usRegAddress++;

        usCoilCount  = (USHORT)( *(pucFrame + MB_PDU_FUNC_READ_COILCNT_OFF) << 8 );
        usCoilCount |= (USHORT)( *(pucFrame + MB_PDU_FUNC_READ_COILCNT_OFF + 1) );

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( (usCoilCount >= 1) && (usCoilCount < MB_PDU_FUNC_READ_COILCNT_MAX) )
        {
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = pucFrame + MB_PDU_FUNC_OFF;
            *usLen = MB_PDU_FUNC_OFF;

            /* First byte contains the function code. */
            *pucFrameCur++ = MB_FUNC_READ_COILS;
            *usLen += 1;

            /* Test if the quantity of coils is a multiple of 8. If not last
             * byte is only partially field with unused coils set to zero. */
            if( (usCoilCount & 0x0007) != 0 )
            {
                ucNBytes = (UCHAR)(usCoilCount/8 + 1);
            }
            else
            {
                ucNBytes = (UCHAR)(usCoilCount/8);
            }
            *pucFrameCur++ = ucNBytes;
            *usLen += 1;

            eRegStatus = eMBSlaveRegCoilsCB(psMBSlaveInfo, pucFrameCur, usRegAddress, usCoilCount, MB_BIT_READ);    //回调函数
                    
            /* If an error occured convert it into a Modbus exception. */
            if(eRegStatus != MB_ENOERR)
            {
                return prveMBSlaveError2Exception(eRegStatus);
            }
            else
            {
                *usLen += ucNBytes;;
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

#if MB_FUNC_WRITE_COIL_ENABLED
/***********************************************************************************
 * @brief 写单个线圈功能函数
 * @param pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException 
eMBSlaveFuncWriteCoil(sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucFrame, USHORT* usLen)
{
    USHORT          usRegAddress;
    eMBErrorCode    eRegStatus;

    UCHAR ucBuf[2] = {0};

    if(*usLen == MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN)
    {
        usRegAddress  = (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_ADDR_OFF) << 8 );
        usRegAddress |= (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_ADDR_OFF + 1) );
        usRegAddress++;

        if( (*(pucFrame + MB_PDU_FUNC_WRITE_VALUE_OFF + 1) == 0x00) &&
            ((*(pucFrame + MB_PDU_FUNC_WRITE_VALUE_OFF) == 0xFF) || (*(pucFrame + MB_PDU_FUNC_WRITE_VALUE_OFF) == 0x00)) )
        {
            ucBuf[1] = 0;
            if( *(pucFrame + MB_PDU_FUNC_WRITE_VALUE_OFF) == 0xFF )
            {
                ucBuf[0] = 1;
            }
            else
            {
                ucBuf[0] = 0;
            }
            eRegStatus = eMBSlaveRegCoilsCB(psMBSlaveInfo, &ucBuf[0], usRegAddress, 1, MB_BIT_WRITE);

            /* If an error occured convert it into a Modbus exception. */
            if(eRegStatus != MB_ENOERR)
            {
                return prveMBSlaveError2Exception(eRegStatus);
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

#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED
/***********************************************************************************
 * @brief 写多个线圈功能函数
 * @param pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException 
eMBSlaveFuncWriteMultipleCoils(sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucFrame, USHORT* usLen)
{
    USHORT usCoilAddress, usCoilCnt;
    UCHAR  ucByteCount, ucByteCountVerify;

    eMBErrorCode    eRegStatus;

    if( *usLen > ( MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN ) )
    {
        usCoilAddress  = (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_MUL_ADDR_OFF) << 8 );
        usCoilAddress |= (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1) );

        usCoilCnt  = (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF) << 8 );
        usCoilCnt |= (USHORT)( *(pucFrame + MB_PDU_FUNC_WRITE_MUL_COILCNT_OFF + 1) );

        ucByteCount = *(pucFrame + MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF);

        /* Compute the number of expected bytes in the request. */
        if( (usCoilCnt & 0x0007) != 0 )
        {
            ucByteCountVerify = (UCHAR)(usCoilCnt/8 + 1);
        }
        else
        {
            ucByteCountVerify = (UCHAR)(usCoilCnt/8);
        }

        if( (usCoilCnt >= 1) && (usCoilCnt <= MB_PDU_FUNC_WRITE_MUL_COILCNT_MAX) &&
            (ucByteCountVerify == ucByteCount) )
        {
            eRegStatus = eMBSlaveRegCoilsCB(psMBSlaveInfo, pucFrame + MB_PDU_FUNC_WRITE_MUL_VALUES_OFF,
                                            usCoilAddress, usCoilCnt, MB_BIT_WRITE);

            /* If an error occured convert it into a Modbus exception. */
            if(eRegStatus != MB_ENOERR)
            {
                return prveMBSlaveError2Exception(eRegStatus);
            }
            else
            {
                /* The response contains the function code, the starting address
                 * and the quantity of registers. We reuse the old values in the 
                 * buffer because they are still valid. */
                *usLen = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
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

#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED || MB_FUNC_READ_COILS_ENABLED
/***********************************************************************************
 * @brief 线圈状态寄存器回调函数（读、连续读、写、连续写）
 * @param pucRegBuffer  位组成一个字节，起始寄存器对应的位处于该字节pucRegBuffer的最低位LSB。
 *                      如果回调函数要写这个缓冲区，没有用到的线圈（例如不是8个一组的线圈状态）对应的位的数值必须设置位0。
 * 
 * @param usAddress     第一个线圈地址
 * @param usNCoils      请求的线圈个数
 * 
 * @param eMode         如果该参数为eMBRegisterMode::MB_REG_WRITE，用户的应用数值将从pucRegBuffer中得到更新。
 *                      如果该参数为eMBRegisterMode::MB_REG_READ，用户需要将当前的应用数据存储在pucRegBuffer中
 * 
 * @return eMBErrorCode 错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBErrorCode eMBSlaveRegCoilsCB(sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucRegBuffer, 
                                USHORT usAddress, USHORT usNCoils, eMBBitMode eMode)
{
    USHORT COIL_START, COIL_END;
    sMBSlaveDataTable* psMBCoilTable = &psMBSlaveInfo->sMBCommInfo.psSlaveCurData->sMBCoilTable;  //从栈通讯协议表
    eMBErrorCode  eStatus = MB_ENOERR;

    if( (psMBCoilTable == NULL) || (psMBCoilTable->pvDataBuf == NULL) ||
        (psMBCoilTable->usDataCount == 0)) //非空且数据点不为0
    {
        return MB_ENOREG;
    }
    COIL_START = psMBCoilTable->usStartAddr;
    COIL_END = psMBCoilTable->usEndAddr;

    if( (usAddress >= COIL_START) && (usAddress + usNCoils -1 <= COIL_END) )
    {
		switch (eMode)
        {
        /* read current coil values from the protocol stack. */
        case MB_BIT_READ:
            eStatus = eMBSlaveUtilGetBits(psMBSlaveInfo, pucRegBuffer, usAddress, usNCoils, CoilData);
        break;

        case MB_BIT_WRITE:
            eStatus = eMBSlaveUtilSetBits(psMBSlaveInfo, pucRegBuffer, usAddress, usNCoils, CoilData);
        break;
        }
    }
    else
    {
        return MB_ENOREG;
    }
    return eStatus;
}

#endif
