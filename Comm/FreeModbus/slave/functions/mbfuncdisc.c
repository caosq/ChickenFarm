 /*
  * FreeRTOS Modbus Libary: A Modbus serial implementation for FreeRTOS
  * Copyright (C) 2006 Christian Walter <wolti@sil.at>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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

#if MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_ASCII_ENABLED > 0 

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_READ_DISCCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
#define MB_PDU_FUNC_READ_DISCCNT_MAX        ( 0x07D0 )


/* ----------------------- Start implementation -----------------------------*/

#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
/***********************************************************************************
 * @brief 读离散量功能函数
 * @param pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException 
eMBSlaveFuncReadDiscreteInputs(sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucFrame, USHORT* usLen)
{
    USHORT          usRegAddress, usDiscreteCnt;
    UCHAR           ucNBytes;
    UCHAR          *pucFrameCur;

    eMBErrorCode    eRegStatus;

    if( *usLen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN ) )
    {
        usRegAddress  = (USHORT)( pucFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= (USHORT)( pucFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1] );

        usDiscreteCnt  = (USHORT)( pucFrame[MB_PDU_FUNC_READ_DISCCNT_OFF] << 8 );
        usDiscreteCnt |= (USHORT)( pucFrame[MB_PDU_FUNC_READ_DISCCNT_OFF + 1] );

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( ( usDiscreteCnt >= 1 ) &&
            ( usDiscreteCnt < MB_PDU_FUNC_READ_DISCCNT_MAX ) )
        {
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
            *usLen = MB_PDU_FUNC_OFF;

            /* First byte contains the function code. */
            *pucFrameCur++ = MB_FUNC_READ_DISCRETE_INPUTS;
            *usLen += 1;

            /* Test if the quantity of coils is a multiple of 8. If not last
             * byte is only partially field with unused coils set to zero. */
            if( (usDiscreteCnt & 0x0007) != 0 )
            {
                ucNBytes = (UCHAR)(usDiscreteCnt/8 + 1);
            }
            else
            {
                ucNBytes = (UCHAR)(usDiscreteCnt/8);
            }
            *pucFrameCur++ = ucNBytes;
            *usLen += 1;

            eRegStatus = eMBSlaveRegDiscreteCB(psMBSlaveInfo, pucFrameCur, usRegAddress, usDiscreteCnt);

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR )
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



/*********************************************************************************************
 * @brief 离散输入量回调函数（读、连续读）
 * @param pucRegBuffer  用当前的线圈数据更新这个寄存器，起始寄存器对应的位处于该字节pucRegBuffer的最低位LSB。
 *                      如果回调函数要写这个缓冲区，没有用到的线圈（例如不是8个一组的线圈状态）
 *                      对应的位的数值必须设置为0。
 * 
 * @param usAddress     离散输入的起始地址
 * @param usNDiscrete   离散输入点数量
 * 
 * @return eMBErrorCode 错误码
 * @author laoc
 * @date 2019.01.22
 ********************************************************************************************/
eMBErrorCode 
eMBSlaveRegDiscreteCB(sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    USHORT DISCRETE_INPUT_START, DISCRETE_INPUT_END;
    sMBSlaveDataTable* psMBDiscInTable = &psMBSlaveInfo->sMBCommInfo.psSlaveCurData->sMBDiscInTable;  //从栈通讯协议表
    
    if( (psMBDiscInTable == NULL) || (psMBDiscInTable->pvDataBuf == NULL) ||
        (psMBDiscInTable->usDataCount == 0)) //非空且数据点不为0
    {
        return MB_ENOREG;
    }
    DISCRETE_INPUT_START = psMBDiscInTable->usStartAddr;
    DISCRETE_INPUT_END = psMBDiscInTable->usEndAddr;

    if ( (usAddress >= DISCRETE_INPUT_START) && (usAddress + usNDiscrete -1 <= DISCRETE_INPUT_END) )
    {
        /* read current coil values from the protocol stack. */ 
        return eMBSlaveUtilGetBits(psMBSlaveInfo, pucRegBuffer, usAddress, usNDiscrete, DiscInData);
    }
    else
    {
        return MB_ENOREG;
    }
}
#endif

#endif
