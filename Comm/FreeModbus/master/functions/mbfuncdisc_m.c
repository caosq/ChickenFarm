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
 * File: $Id: mbfuncdisc_m.c,v 1.60 2013/10/15 8:48:20 Armink Add Master Functions  Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
//#include "mb.h"
#include "mb_m.h"
#include "mbframe.h"
#include "mbbits_m.h"
#include "mbproto.h"
#include "mbconfig.h"
#include "mbfunc_m.h"
#include "mbdict_m.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_REQ_READ_ADDR_OFF            ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REQ_READ_DISCCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_READ_SIZE                ( 4 )
#define MB_PDU_FUNC_READ_DISCCNT_OFF        ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READ_VALUES_OFF         ( MB_PDU_DATA_OFF + 1 )
#define MB_PDU_FUNC_READ_SIZE_MIN           ( 1 )

/* ----------------------- Start implementation -----------------------------*/
#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED

#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED
 /***********************************************************************************
 * @brief  主栈读离散量
 * @param  ucSndAddr      从栈地址
 * @param  usDiscreteAddr 起始地址
 * @param  usNDiscreteIn  个数
 * @param  lTimeOut       主栈等待时间（0则一直等待）
 * @return error          错误码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBMasterReqErrCode
eMBMasterReqReadDiscreteInputs(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usDiscreteAddr, USHORT usNDiscreteIn, ULONG ulTimeOut)
{
    UCHAR  *pucMBFrame = NULL;
    sMBMasterDevsInfo*  psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;  //从设备状态信息
    sMBMasterPort*      psMBPort     = &psMBMasterInfo->sMBPort;      //硬件结构
	
    if( (ucSndAddr < psMBDevsInfo->ucSlaveDevMinAddr) || (ucSndAddr > psMBDevsInfo->ucSlaveDevMaxAddr) ) 
	{
        return MB_MRE_ILL_ARG;
	}		
    else if ( xMBMasterRunResTake(psMBPort, ulTimeOut ) == FALSE )
	{
        return MB_MRE_MASTER_BUSY;
	}
    else
    {
		vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
		vMBMasterSetDestAddress(psMBMasterInfo, ucSndAddr);
        
		*(pucMBFrame + MB_PDU_FUNC_OFF)                 = MB_FUNC_READ_DISCRETE_INPUTS;
		*(pucMBFrame + MB_PDU_REQ_READ_ADDR_OFF)        = usDiscreteAddr >> 8;
        *(pucMBFrame + MB_PDU_REQ_READ_ADDR_OFF + 1)    = (UCHAR)usDiscreteAddr;
		*(pucMBFrame + MB_PDU_REQ_READ_DISCCNT_OFF)     = usNDiscreteIn >> 8;
        *(pucMBFrame + MB_PDU_REQ_READ_DISCCNT_OFF + 1) = (UCHAR)usNDiscreteIn;
        
		vMBMasterSetPDUSndLength( psMBMasterInfo, MB_PDU_SIZE_MIN + MB_PDU_REQ_READ_SIZE );
        
		(void) xMBMasterPortEventPost(psMBPort, EV_MASTER_FRAME_SENT);
        return eMBMasterWaitRequestFinish(psMBPort);
    }
}

/***********************************************************************************
 * @brief 读离散量功能函数
 * @param pucFrame       Modbus的PDU缓冲区数据指针
 * @param  usLen         缓冲区长度
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException
eMBMasterFuncReadDiscreteInputs(sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen)
{
    USHORT usRegAddress, usDiscreteCnt;

    UCHAR  ucNBytes;
    UCHAR *pucMBFrame;

    eMBErrorCode    eRegStatus;

    /* If this request is broadcast, and it's read mode. This request don't need execute. */
    if (xMBMasterRequestIsBroadcast(psMBMasterInfo))
    {
        return MB_EX_NONE;
    }
    else if( *usLen >= MB_PDU_SIZE_MIN + MB_PDU_FUNC_READ_SIZE_MIN )
    {
    	vMBMasterGetPDUSndBuf(psMBMasterInfo, &pucMBFrame);
        usRegAddress = (USHORT)( pucMBFrame[MB_PDU_REQ_READ_ADDR_OFF] << 8 );
        usRegAddress |= (USHORT)( pucMBFrame[MB_PDU_REQ_READ_ADDR_OFF + 1] );
        usRegAddress++;

        usDiscreteCnt = (USHORT)( pucMBFrame[MB_PDU_REQ_READ_DISCCNT_OFF] << 8 );
        usDiscreteCnt |= (USHORT)( pucMBFrame[MB_PDU_REQ_READ_DISCCNT_OFF + 1] );

        /* Test if the quantity of coils is a multiple of 8. If not last
         * byte is only partially field with unused coils set to zero. */
        if( ( usDiscreteCnt & 0x0007 ) != 0 )
        {
            ucNBytes = (UCHAR)( usDiscreteCnt / 8 + 1 );
        }
        else
        {
            ucNBytes = (UCHAR)( usDiscreteCnt / 8 );
        }

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
		if ((usDiscreteCnt >= 1) && ucNBytes == pucFrame[MB_PDU_FUNC_READ_DISCCNT_OFF])
        {
	       	/* Make callback to fill the buffer. */
            eRegStatus = eMBMasterRegDiscreteCB(psMBMasterInfo, &pucFrame[MB_PDU_FUNC_READ_VALUES_OFF], usRegAddress, usDiscreteCnt);

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

/**
 * Modbus master discrete callback function.
 *
 * @param pucRegBuffer discrete buffer
 * @param usAddress discrete address
 * @param usNDiscrete discrete number
 *
 * @return result
 */
eMBErrorCode eMBMasterRegDiscreteCB(sMBMasterInfo* psMBMasterInfo, UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    USHORT          DISCRETE_INPUT_START, DISCRETE_INPUT_END;
   
    sMBSlaveDev*         psMBSlaveDevCur = psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur ;    //当前从设备
    sMBDevDataTable*     psMBDiscInTable = &psMBSlaveDevCur->psDevCurData->sMBDiscInTable;  //从设备通讯协议表
    UCHAR                   ucMBDestAddr = ucMBMasterGetDestAddr(psMBMasterInfo);           //从设备通讯地址
    
    if(psMBSlaveDevCur->ucDevAddr != ucMBDestAddr) //如果当前从设备地址与要轮询从设备地址不一致，则更新从设备
    {
        psMBSlaveDevCur = psMBMasterGetDev(psMBMasterInfo, ucMBDestAddr);
        psMBMasterInfo->sMBDevsInfo.psMBSlaveDevCur = psMBSlaveDevCur;
        psMBDiscInTable = &psMBSlaveDevCur->psDevCurData->sMBDiscInTable;
    } 
    if( (psMBDiscInTable == NULL) || (psMBDiscInTable->pvDataBuf == NULL) ||
        (psMBDiscInTable->usDataCount == 0)) //非空且数据点不为0
	{
		return MB_ENOREG;
	} 
    DISCRETE_INPUT_START = psMBDiscInTable->usStartAddr;
    DISCRETE_INPUT_END   = psMBDiscInTable->usEndAddr;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ( (usAddress >= DISCRETE_INPUT_START) && (usAddress + usNDiscrete -1 <= DISCRETE_INPUT_END) )
    {
        return eMBMasterUtilSetBits(psMBMasterInfo, pucRegBuffer, usAddress, usNDiscrete, DiscInData, MB_BIT_READ);
	}   
    else
    {
        return MB_ENOREG;
    }
}
#endif

#endif
