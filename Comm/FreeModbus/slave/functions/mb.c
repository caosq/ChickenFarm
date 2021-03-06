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
 * File: $Id: mb.c,v 1.27 2007/02/18 23:45:41 wolti Exp $
 */
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbfunc.h"
#include "mbport.h"
#include "mbmap.h"

#if MB_UCOSIII_ENABLED
#include "os.h"

#elif MB_LINUX_ENABLED
#include <pthread.h>
#endif

#if MB_SLAVE_RTU_ENABLED
#include "mbrtu.h"
#endif
#if MB_SLAVE_ASCII_ENABLED
#include "mbascii.h"
#endif
#if MB_SLAVE_TCP_ENABLED
#include "mbtcp.h"
#endif

#ifndef MB_PORT_HAS_CLOSE
#define MB_PORT_HAS_CLOSE 0
#endif

#define MB_SLAVE_POLL_INTERVAL_MS           50

/* ----------------------- Static variables ---------------------------------*/ 
static sMBSlaveInfo* psMBSlaveList = NULL; 

/* An array of Modbus functions handlers which associates Modbus function
 * codes with implementing functions.
 */
static xMBSlaveFunctionHandler xFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0                     
    {MB_FUNC_OTHER_REPORT_SLAVEID, eMBSlaveFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0              //??????????????????(0x04)
    {MB_FUNC_READ_INPUT_REGISTER, eMBSlaveFuncReadInputRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0            //??????????????????(0x03)
    {MB_FUNC_READ_HOLDING_REGISTER, eMBSlaveFuncReadHoldingRegister},
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0  //????????????????????????(0x10)
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBSlaveFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0           //????????????????????????(0x06)
    {MB_FUNC_WRITE_REGISTER, eMBSlaveFuncWriteHoldingRegister},
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0       //????????????????????????(0x17)
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBSlaveFuncReadWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0              //?????????(0x01)
    {MB_FUNC_READ_COILS, eMBSlaveFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0              //???????????????(0x05)
    {MB_FUNC_WRITE_SINGLE_COIL, eMBSlaveFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0    //???????????????(0x0F)
    {MB_FUNC_WRITE_MULTIPLE_COILS, eMBSlaveFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0    //????????????(0x02)
    {MB_FUNC_READ_DISCRETE_INPUTS, eMBSlaveFuncReadDiscreteInputs},
#endif
};

/**********************************************************************
 * @brief  MODBUS??????????????????
 * @param  eMode           MODBUS??????:    RTU??????   ASCII??????   TCP??????  
 * @param  pcSlaveAddr     ????????????
 * @param  *psMBSlaveUart  UART??????
 * @return eMBErrorCode    ?????????
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlaveInit(sMBSlaveInfo* psMBSlaveInfo)
{
    eMBErrorCode eStatus = MB_ENOERR;
    sMBSlavePort* psMBPort = &psMBSlaveInfo->sMBPort;
    sMBSlaveCommInfo* psMBCommInfo = &psMBSlaveInfo->sMBCommInfo;
    
    UCHAR ucSlaveAddr = psMBCommInfo->ucSlaveAddr;

    /* check preconditions */
    if(ucSlaveAddr == MB_ADDRESS_BROADCAST ||
       ucSlaveAddr < MB_ADDRESS_MIN || ucSlaveAddr > MB_ADDRESS_MAX)
    {
       return MB_EINVAL;
    }
    else
    {
        switch (psMBSlaveInfo->eMode)
        {
        case MB_RTU:
#if MB_SLAVE_RTU_ENABLED
    /* ?????????????????????????????????????????????Modbus?????????????????????????????????????????????????????????
        3.5T????????????????????????????????????????????????????????????????????? */
            psMBSlaveInfo->pvMBSlaveFrameStartCur   = vMBSlaveRTUStart;      // ???????????????
            psMBSlaveInfo->pvMBSlaveFrameStopCur    = vMBSlaveRTUStop;       // ???????????????
            psMBSlaveInfo->peMBSlaveFrameSendCur    = eMBSlaveRTUSend;       // ???????????????
            psMBSlaveInfo->peMBSlaveFrameReceiveCur = eMBSlaveRTUReceive;    // ???????????????
            psMBSlaveInfo->pvMBSlaveFrameGetRequestCur = vMBSlaveRTUGetRequest; //??????????????????

//            psMBSlaveInfo->pvMBSlaveFrameCloseCur   = MB_PORT_HAS_CLOSE ? vMBSlavePortClose : NULL;  // ?????????????????????
//        
            psMBSlaveInfo->pxMBSlaveFrameCBByteReceivedCur     = xMBSlaveRTUReceiveFSM;       //????????????
            psMBSlaveInfo->pxMBSlaveFrameCBTransmitterEmptyCur = xMBSlaveRTUTransmitFSM;      //????????????
            psMBSlaveInfo->pxMBSlaveFrameCBTimerExpiredCur = xMBSlaveRTUTimerExpired;  //T35??????
            
            psMBSlaveInfo->pvMBSlaveReceiveCallback = NULL;
            psMBSlaveInfo->pvMBSlaveSendCallback = NULL;

            eStatus = eMBSlaveRTUInit(psMBSlaveInfo);
#endif
            break;
        case MB_ASCII:
#if MB_SLAVE_ASCII_ENABLED
            psMBSlaveInfo->pvMBSlaveFrameStartCur = eMBASCIIStart;
            psMBSlaveInfo->pvMBSlaveFrameStopCur = eMBASCIIStop;
            psMBSlaveInfo->peMBSlaveFrameSendCur = eMBASCIISend;
            psMBSlaveInfo->peMBSlaveFrameReceiveCur = eMBASCIIReceive;
        
            psMBSlaveInfo->pvMBSlaveFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBPortClose : NULL;
        
            psMBSlaveInfo->pxMBSlaveFrameCBByteReceived = xMBASCIIReceiveFSM;
            psMBSlaveInfo->pxMBSlaveFrameCBTransmitterEmpty = xMBASCIITransmitFSM;
            psMBSlaveInfo->pxMBSlaveFrameCBTimerExpired = xMBASCIITimerT1SExpired;

            psMBSlaveInfo->pvMBSlaveReceiveCallback = NULL;
            psMBSlaveInfo->pvMBSlaveSendCallback = NULL;

            eStatus = eMBASCIIInit(ucMBAddress, ucPort, ulBaudRate, eParity);
#endif
            break;
        default:break;
        }
        if(eStatus == MB_ENOERR)
        {
            if(xMBSlavePortEventInit(psMBPort) == FALSE)
            {
                /* port dependent event module initalization failed. */
                return MB_EPORTERR;
            }
            else
            {
                psMBSlaveInfo->eMBState = STATE_DISABLED; //modbus????????????????????????,????????????????????????
            }
        }
    }
    return MB_ENOERR;
}

#if MB_SLAVE_TCP_ENABLED
eMBErrorCode eMBSlaveTCPInit(sMBSlaveInfo* psMBSlaveInfo)
{
    psMBSlaveInfo->pvMBSlaveFrameStartCur   = vMBSlaveTCPStart;
    psMBSlaveInfo->pvMBSlaveFrameStopCur    = vMBSlaveTCPStop;
    psMBSlaveInfo->peMBSlaveFrameReceiveCur = eMBSlaveTCPReceive;
    psMBSlaveInfo->peMBSlaveFrameSendCur    = eMBSlaveTCPSend;
    psMBSlaveInfo->pvMBSlaveFrameGetRequestCur = vMBSlaveTCPGetRequest; //??????????????????

    psMBSlaveInfo->pvMBSlaveReceiveCallback = NULL;
    psMBSlaveInfo->pvMBSlaveSendCallback = NULL;

    //pvMBSlaveFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBTCPPortClose : NULL;

    //eStatus = eMBSlaveTCPInit(psMBSlaveInfo);
    if(xMBSlavePortEventInit(&psMBSlaveInfo->sMBPort) == FALSE)
    {
        return MB_EPORTERR; /* port dependent event module initalization failed. */
    }
    else
    {
        psMBSlaveInfo->eMBState = STATE_DISABLED; //modbus????????????????????????,????????????????????????
    }
    return MB_ENOERR;
}
#endif

/**********************************************************************
 * @brief  MODBUS???????????????
 * @return eMBErrorCode    ?????????
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlaveClose( sMBSlaveInfo* psMBSlaveInfo )
{
    sMBSlavePort* psMBPort = &psMBSlaveInfo->sMBPort;
    
    if(psMBSlaveInfo->eMBState == STATE_DISABLED)
    {
        if(psMBSlaveInfo->pvMBSlaveFrameCloseCur != NULL)
        {
            psMBSlaveInfo->pvMBSlaveFrameCloseCur(psMBPort);
        }
    }
    else
    {
        return MB_EILLSTATE;
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  MODBUS???????????????
 * @return eMBErrorCode    ???????????????
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlaveEnable(sMBSlaveInfo* psMBSlaveInfo)
{
    if(psMBSlaveInfo->eMBState == STATE_DISABLED)
    {
        /* Activate the protocol stack. */
        psMBSlaveInfo->pvMBSlaveFrameStartCur(psMBSlaveInfo);
        psMBSlaveInfo->eMBState = STATE_ENABLED; //??????Modbus?????????????????????eMBState???STATE_ENABLED
    }
    else
    {
        return MB_EILLSTATE;
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  MODBUS???????????????
 * @return eMBErrorCode    ???????????????
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlaveDisable( sMBSlaveInfo* psMBSlaveInfo )
{
    if(psMBSlaveInfo->eMBState == STATE_ENABLED)
    {
        psMBSlaveInfo->pvMBSlaveFrameStopCur(psMBSlaveInfo);
        psMBSlaveInfo->eMBState = STATE_DISABLED;
        
        return MB_ENOERR;
    }
    else if(psMBSlaveInfo->eMBState == STATE_DISABLED)
    {
        return MB_ENOERR;
    }
    else
    {
        return MB_EILLSTATE;
    }
}

/**********************************************************************
 * @brief  MODBUS???????????????
 *          1. ????????????????????????????????????eMBState?????????STATE_NOT_INITIALIZED???
 *             ???eMBInit()?????????????????????STATE_DISABLED,???eMBEnable?????????????????????STATE_ENABLE??
 *          2. ??????EV_FRAME_RECEIVED??????????????????EV_FRAME_RECEIVED???????????????
 *             ?????????????????????????????????EV_EXECUTE????????????????????????????????????(??????)???????????????????????
 * @return eMBErrorCode    ???????????????
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlavePoll(sMBSlaveInfo* psMBSlaveInfo)
{
    UCHAR ucFunctionCode; //?????????

    eMBException eException; //??????????????? ??????
    USHORT  i;

    eMBSlaveEventType eEvent;   //?????????
    eMBErrorCode      eStatus = MB_ENOERR;
    sMBSlavePort*     psMBPort = &psMBSlaveInfo->sMBPort;
    sMBSlaveCommInfo* psMBCommInfo = &psMBSlaveInfo->sMBCommInfo;

    /* Check if the protocol stack is ready. */
    if(psMBSlaveInfo->eMBState != STATE_ENABLED)      //???????????????????????????
    {
        return MB_EILLSTATE;             //???????????????????????????????????????????????????
    }
    if(xMBSlavePortEventGet(psMBPort, &eEvent) == TRUE)  /* ?????????????????????????????? ??????????????????????????????????????????????????????. ??????????????????????????? */
    {
        switch(eEvent)
        {
        case EV_READY:
            psMBSlaveInfo->pvMBSlaveFrameGetRequestCur(psMBSlaveInfo);
            break;
        case EV_FRAME_RECEIVED:   //???????????????????????????????????????
            /*CRC???????????????????????????????????????????????????????????????*/
            eStatus = psMBSlaveInfo->peMBSlaveFrameReceiveCur(psMBSlaveInfo, &psMBSlaveInfo->ucRcvAddress, &psMBSlaveInfo->pucMBFrame,
                                                              &psMBSlaveInfo->usLength); /*ucRcvAddress ????????????????????????????????????*/
		    if(eStatus == MB_ENOERR)
            {
                /* Check if the frame is for us. If not ignore the frame. */
                if((psMBSlaveInfo->ucRcvAddress == psMBCommInfo->ucSlaveAddr) || (psMBSlaveInfo->ucRcvAddress == MB_ADDRESS_BROADCAST))
                {
                    (void)xMBSlavePortEventPost(psMBPort, EV_EXECUTE);   //?????????????????????EV_EXECUTE????????????
                }
            }
            else
            {
                (void)xMBSlavePortEventPost(psMBPort, EV_ERROR_RCV);
            }
		break;	
        case EV_EXECUTE:	
            ucFunctionCode = *(psMBSlaveInfo->pucMBFrame + MB_PDU_FUNC_OFF);    //???????????????
            eException = MB_EX_ILLEGAL_FUNCTION;
		
            for(i = 0; i < MB_FUNC_HANDLERS_MAX; i++)
            {
                if( xFuncHandlers[i].ucFunctionCode == 0 )
                {
                    break; /* No more function handlers registered. Abort. */
                }
                else if(xFuncHandlers[i].ucFunctionCode == ucFunctionCode)
                {
                    eException = xFuncHandlers[i].pxHandler(psMBSlaveInfo, psMBSlaveInfo->pucMBFrame,
                                                            &psMBSlaveInfo->usLength); //????????????????????????????????????????????????????????????????????????
                    break;                                                            
                }
            }
            /*????????????????????????????????????????????????*/
            if(psMBSlaveInfo->ucRcvAddress != MB_ADDRESS_BROADCAST)
            {
                if(eException != MB_EX_NONE)
                {
                    /*??????????????????????????????????????????*/
                    psMBSlaveInfo->usLength = 0;
                    *(psMBSlaveInfo->pucMBFrame + (psMBSlaveInfo->usLength++)) = (UCHAR)(ucFunctionCode | MB_FUNC_ERROR);    //???????????????????????????????????????????????????????????????1
                    *(psMBSlaveInfo->pucMBFrame + (psMBSlaveInfo->usLength++)) = (UCHAR)eException;                                 //?????????????????????????????????????????????????????????
                }
                 /* eMBRTUSend()???????????????????????????????????????RX?????????TX??????????????????USART_DATA???UDR?????????????????????*/			
                eStatus = psMBSlaveInfo->peMBSlaveFrameSendCur(psMBSlaveInfo, psMBCommInfo->ucSlaveAddr, psMBSlaveInfo->pucMBFrame,
                                                               psMBSlaveInfo->usLength); //modbus??????????????????,?????????????????????
            }
        break;
        case EV_FRAME_SENT:
            psMBSlaveInfo->pvMBSlaveFrameGetRequestCur(psMBSlaveInfo);
        break;
        case EV_ERROR_RCV:
            psMBSlaveInfo->pvMBSlaveFrameGetRequestCur(psMBSlaveInfo);
        break;
        }
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  MODBUS????????????
 * @param  psMBSlaveInfo  ???????????????   
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBSlaveRegistNode(sMBSlaveInfo* psMBSlaveInfo, sMBSlaveNodeInfo* psSlaveNode)
{
    sMBSlaveInfo* psMBInfo = NULL;
    sMBSlavePort* psMBPort = &psMBSlaveInfo->sMBPort;   //????????????????????????
    sMBSlaveTask* psMBTask = &psMBSlaveInfo->sMBTask;   //???????????????????????????
    sMBSlaveCommInfo* psMBCommInfo = &psMBSlaveInfo->sMBCommInfo;   //??????????????????
    
    if(psMBSlaveInfo == NULL || psSlaveNode == NULL || psSlaveNode->pcMBPortName == NULL)
    {
        return FALSE;
    }
    if((psMBInfo = psMBSlaveFindNodeByPort(psSlaveNode->pcMBPortName)) == NULL)
    {
        psMBSlaveInfo->eMode = psSlaveNode->eMode;
        psMBSlaveInfo->pNext = NULL;

        /***************************??????????????????***************************/
        psMBPort = (sMBSlavePort*)(&psMBSlaveInfo->sMBPort);
        if(psMBPort != NULL)
        {
            psMBPort->psMBSlaveInfo = psMBSlaveInfo;
            psMBPort->pcMBPortName  = psSlaveNode->pcMBPortName;
#if MB_SLAVE_TCP_ENABLED            
            psMBPort->fd = psSlaveNode->iSocketClient;
#endif        
#if MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED
            psMBPort->psMBSlaveUart = psSlaveNode->psSlaveUart;
#endif
        }
        /***************************????????????????????????***************************/
        psMBCommInfo = (sMBSlaveCommInfo*)(&psMBSlaveInfo->sMBCommInfo);
        if(psMBCommInfo != NULL)
        {
            psMBCommInfo->ucSlaveAddr = psSlaveNode->ucSlaveAddr;
        }
        /***************************??????????????????????????????***************************/
#if MB_UCOSIII_ENABLED
        psMBTask =(sMBSlaveTask*)(&psMBSlaveInfo->sMBTask);
        if(psMBTask != NULL)
        {
            psMBTask->ucSlavePollPrio = psSlaveNode->ucSlavePollPrio;
        }
#endif
        /*******************************???????????????????????????*************************/
        if(xMBSlaveCreatePollTask(psMBSlaveInfo) == FALSE)  
        {
            return FALSE;
        }
	    if(psMBSlaveList == NULL)  //????????????
	    {
            psMBSlaveList = psMBSlaveInfo;
	    }
	    else if(psMBSlaveList->pLast != NULL)
	    {
            psMBSlaveList->pLast->pNext = psMBSlaveInfo;
	    }
        psMBSlaveList->pLast = psMBSlaveInfo;
        
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************
 * @brief  MODBUS?????????????????????
 * @param  psMBSlaveInfo  ???????????????   
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
void vMBSlaveRegistCommData(sMBSlaveInfo* psMBSlaveInfo, sMBSlaveCommData* psSlaveCurData)
{
    psMBSlaveInfo->sMBCommInfo.psSlaveCurData = psSlaveCurData;
}

/**********************************************************************
 * @brief  MODBUS????????????????????????
 * @param  psMBSlaveInfo  ???????????????
 * @return BOOL
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
void vMBSlaveSetAddr(sMBSlaveInfo* psMBSlaveInfo, UCHAR ucSlaveAddr)
{
    psMBSlaveInfo->sMBCommInfo.ucSlaveAddr = ucSlaveAddr;
}

/**********************************************************************
 * @brief  MODBUS??????????????????????????????
 * @param  pcMBPortName    ????????????
 * @return sMBMasterInfo   ???????????????
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
sMBSlaveInfo* psMBSlaveFindNodeByPort(const CHAR* pcMBPortName)
{
	sMBSlaveInfo*  psMBSlaveInfo = NULL;

    if(pcMBPortName == NULL)
    {
        return NULL;
    }
    for(psMBSlaveInfo = psMBSlaveList; psMBSlaveInfo != NULL && psMBSlaveInfo->sMBPort.pcMBPortName != NULL;
        psMBSlaveInfo = psMBSlaveInfo->pNext)
	{
        if(strcmp(psMBSlaveInfo->sMBPort.pcMBPortName, pcMBPortName) == 0)
        {
            return psMBSlaveInfo;
        }
	}
	return psMBSlaveInfo;		
}

/**********************************************************************
 * @brief   ???????????????
 * @param   *p_arg    
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
#if MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED

#if MB_UCOSIII_ENABLED
void vMBSlavePollTask(void *p_arg) 
    
#elif MB_LINUX_ENABLED
void* vMBSlavePollTask(void *p_arg)
#endif    
{
    sMBSlaveInfo* psMBSlaveInfo = (sMBSlaveInfo*)p_arg;
    if(eMBSlaveInit(psMBSlaveInfo) == MB_ENOERR && eMBSlaveEnable(psMBSlaveInfo) == MB_ENOERR)
    {
        while(1)
        {
            (void)vMBTimeDly(0, MB_SLAVE_POLL_INTERVAL_MS);
            (void)eMBSlavePoll(psMBSlaveInfo);
        }
    }
}
#endif

#if MB_SLAVE_TCP_ENABLED

#if MB_UCOSIII_ENABLED
void vMBSlaveTCPPollTask(void *p_arg) 
    
#elif MB_LINUX_ENABLED
void* vMBSlaveTCPPollTask(void *p_arg)
#endif 
{
    sMBSlaveInfo* psMBSlaveInfo = (sMBSlaveInfo*)p_arg;
    if(eMBSlaveTCPInit(psMBSlaveInfo) == MB_ENOERR && eMBSlaveEnable(psMBSlaveInfo) == MB_ENOERR)
    {
        while(1)
        {
            (void)eMBSlavePoll(psMBSlaveInfo);
        }
    }
    //return NULL;
}
#endif
/**********************************************************************
 * @brief  MODBUS???????????????????????????
 * @param  psMBMasterInfo  psMBSlaveInfo   
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBSlaveCreatePollTask(sMBSlaveInfo* psMBSlaveInfo)
{
#if MB_UCOSIII_ENABLED    
    OS_ERR err = OS_ERR_NONE;
    
    CPU_STK_SIZE  stk_size = MB_SLAVE_POLL_TASK_STK_SIZE; 
    sMBSlaveTask* psMBTask = &psMBSlaveInfo->sMBTask;
    
    OS_PRIO prio = psMBTask->ucSlavePollPrio;
    OS_TCB* p_tcb = (OS_TCB*)(&psMBTask->sSlavePollTCB);  
    CPU_STK* p_stk_base = (CPU_STK*)(psMBTask->usSlavePollStk);
    
if(psMBSlaveInfo->eMode == MB_RTU || psMBSlaveInfo->eMode == MB_ASCII)
    {
#if MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED
         OSTaskCreate(p_tcb, "vMBSlavePollTask", vMBSlavePollTask, (void*)psMBSlaveInfo, prio, p_stk_base, 
                      stk_size/10u, stk_size, 0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
#endif
    }
    else if(psMBSlaveInfo->eMode == MB_TCP)
    {
#if MB_SLAVE_TCP_ENABLED
         OSTaskCreate(p_tcb, "vMBSlavePollTask", vMBSlaveTCPPollTask, (void*)psMBSlaveInfo, prio, p_stk_base, 
                      stk_size/10u, stk_size, 0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
#endif
    }   
    return (err == OS_ERR_NONE);
    
#elif MB_LINUX_ENABLED
    int ret = -1;

    if(psMBSlaveInfo->eMode == MB_RTU || psMBSlaveInfo->eMode == MB_ASCII)
    {
#if MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED
        ret = pthread_create(&psMBSlaveInfo->sMBTask.sMBPollTask, NULL, vMBSlavePollTask, (void*)psMBSlaveInfo);
#endif
    }
    else if(psMBSlaveInfo->eMode == MB_TCP)
    {
#if MB_SLAVE_TCP_ENABLED
        ret = pthread_create(&psMBSlaveInfo->sMBTask.sMBPollTask, NULL, vMBSlaveTCPPollTask, (void*)psMBSlaveInfo);
#endif
    }
    return ret == 0;
#endif
}


