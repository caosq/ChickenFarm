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
 * File: $Id: mbrtu_m.c,v 1.60 2013/08/20 11:18:10 Armink Add Master Functions $
 */

#include "mbconfig.h"

#if MB_UCOSIII_ENABLED
#include "os.h"

#elif MB_LINUX_ENABLED
#include <pthread.h>
#endif

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb_m.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbfunc_m.h"
#include "mbmap_m.h"
#include "mbport_m.h"
#include "mbutils.h"

#if MB_MASTER_RTU_ENABLED
#include "mbrtu_m.h"
#endif
#if MB_MASTER_ASCII_ENABLED
#include "mbascii_m.h"
#endif
#if MB_MASTER_TCP_ENABLED
#include "mbtcp_m.h"
#endif

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED || MB_MASTER_TCP_ENABLED

#ifndef MB_PORT_HAS_CLOSE
#define MB_PORT_HAS_CLOSE 0
#endif

#define MB_MASTER_POLL_INTERVAL_MS   100

/* ----------------------- Static variables ---------------------------------*/
static sMBMasterInfo*      psMBMasterList = NULL;    

/* An array of Modbus functions handlers which associates Modbus function
 * codes with implementing functions.
 */
static xMBMasterFunctionHandler xMasterFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
	//TODO Add Master function define
    {MB_FUNC_OTHER_REPORT_SLAVEID, eMBFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0
    {MB_FUNC_READ_INPUT_REGISTER, eMBMasterFuncReadInputRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0
    {MB_FUNC_READ_HOLDING_REGISTER, eMBMasterFuncReadHoldingRegister},
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBMasterFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_REGISTER, eMBMasterFuncWriteHoldingRegister},
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBMasterFuncReadWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0
    {MB_FUNC_READ_COILS, eMBMasterFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0
    {MB_FUNC_WRITE_SINGLE_COIL, eMBMasterFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_COILS, eMBMasterFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
    {MB_FUNC_READ_DISCRETE_INPUTS, eMBMasterFuncReadDiscreteInputs},
#endif
};

/* ----------------------- Start implementation -----------------------------*/

/**********************************************************************
 * @brief  MODBUS协议栈初始化
 * @param  psMBMasterInfo  主栈信息块
 * @return eMBErrorCode    错误码
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBMasterInit(sMBMasterInfo* psMBMasterInfo)
{
    eMBErrorCode eStatus = MB_ENOERR;
    sMBMasterPort* psMBPort= &psMBMasterInfo->sMBPort;
	
	switch(psMBMasterInfo->eMode)
	{
	case MB_RTU:
#if MB_MASTER_RTU_ENABLED
        psMBMasterInfo->pvMBMasterFrameStartCur   = vMBMasterRTUStart;
        psMBMasterInfo->pvMBMasterFrameStopCur    = vMBMasterRTUStop;
        psMBMasterInfo->peMBMasterFrameSendCur    = eMBMasterRTUSend;
        psMBMasterInfo->peMBMasterFrameReceiveCur = eMBMasterRTUReceive;
        psMBMasterInfo->pvMBMasterFrameCloseCur   = MB_PORT_HAS_CLOSE ? vMBMasterPortClose : NULL;
	
        psMBMasterInfo->pxMBMasterFrameCBByteReceivedCur     = xMBMasterRTUReceiveFSM;
        psMBMasterInfo->pxMBMasterFrameCBTransmitterEmptyCur = xMBMasterRTUTransmitFSM;
        psMBMasterInfo->pxMBMasterFrameCBTimerExpiredCur     = xMBMasterRTUTimerExpired;

        psMBMasterInfo->pvMBMasterSendCallback = NULL;
        psMBMasterInfo->pvMBMasterReceiveCallback = NULL;

		eStatus = eMBMasterRTUInit(psMBMasterInfo);
#endif
		break;
    case MB_ASCII:
#if MB_MASTER_ASCII_ENABLED
        psMBMasterInfo->pvMBMasterFrameStartCur   = eMBMasterASCIIStart;
        psMBMasterInfo->pvMBMasterFrameStopCur    = eMBMasterASCIIStop;
        psMBMasterInfo->peMBMasterFrameSendCur    = eMBMasterASCIISend;
        psMBMasterInfo->peMBMasterFrameReceiveCur = eMBMasterASCIIReceive;
        psMBMasterInfo->pvMBMasterFrameCloseCur   = MB_PORT_HAS_CLOSE ? vMBMasterPortClose : NULL;
    
        psMBMasterInfo->pxMBMasterFrameCBByteReceived     = xMBMasterASCIIReceiveFSM;
        psMBMasterInfo->pxMBMasterFrameCBTransmitterEmpty = xMBMasterASCIITransmitFSM;
        psMBMasterInfo->pxMBMasterPortCBTimerExpired      = xMBMasterASCIITimerT1SExpired;

        psMBMasterInfo->pvMBMasterSendCallback = NULL;
        psMBMasterInfo->pvMBMasterReceiveCallback = NULL;

		eStatus = eMBMasterASCIIInit(ucPort, ulBaudRate, eParity );
#endif
		break;
    case MB_TCP:
#if MB_MASTER_TCP_ENABLED
        psMBMasterInfo->pvMBMasterFrameStartCur   = vMBMasterTCPStart;
        psMBMasterInfo->pvMBMasterFrameStopCur    = vMBMasterTCPStop;
        psMBMasterInfo->peMBMasterFrameSendCur    = eMBMasterTCPSend;
        psMBMasterInfo->peMBMasterFrameReceiveCur = eMBMasterTCPReceive;
        //psMBMasterInfo->pvMBMasterFrameCloseCur   = MB_PORT_HAS_CLOSE ? vMBMasterPortClose : NULL;

        psMBMasterInfo->pxMBMasterFrameCBByteReceivedCur     = xMBMasterTCPReceiveFSM;
        psMBMasterInfo->pxMBMasterFrameCBTransmitterEmptyCur = xMBMasterTCPTransmitFSM;
        psMBMasterInfo->pxMBMasterFrameCBTimerExpiredCur     = xMBMasterTCPTimerExpired;

        psMBMasterInfo->pvMBMasterSendCallback = NULL;
        psMBMasterInfo->pvMBMasterReceiveCallback = NULL;

        eStatus = eMBMasterTCPInit(psMBMasterInfo);
#endif
        break;
	default:
		eStatus = MB_EINVAL;
		break;
	}
	if (eStatus == MB_ENOERR)
	{
		if(xMBMasterPortEventInit(psMBPort) == FALSE)
		{
			/* port dependent event module initalization failed. */
			eStatus = MB_EPORTERR;
		}
		else
		{
			psMBMasterInfo->eMBState = STATE_DISABLED;
		}
		/* initialize the OS resource for modbus master. */
		vMBMasterOsResInit();
	}
	return eStatus;
}

/**********************************************************************
 * @brief  MODBUS协议栈关闭
 * @param  psMBMasterInfo  主栈信息块
 * @return eMBErrorCode    错误码
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBMasterClose(sMBMasterInfo* psMBMasterInfo)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;

    if( psMBMasterInfo->eMBState == STATE_DISABLED )
    {
        if(psMBMasterInfo->pvMBMasterFrameCloseCur != NULL )
        {
            psMBMasterInfo->pvMBMasterFrameCloseCur(psMBPort);
        }
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

/**********************************************************************
 * @brief  MODBUS协议栈使能
 * @param  psMBMasterInfo  主栈信息块
 * @return eMBErrorCode    协议栈错误
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBMasterEnable(sMBMasterInfo* psMBMasterInfo)
{
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;

#if  MB_UCOSIII_ENABLED
    OS_ERR err = OS_ERR_NONE;
    if( psMBMasterInfo->eMBState == STATE_DISABLED )
    {
        psMBMasterInfo->pvMBMasterFrameStartCur(psMBMasterInfo);
        psMBMasterInfo->eMBState = STATE_ENABLED;
    }
    else
    {
        return MB_EILLSTATE;
    }
#elif MB_LINUX_ENABLED
    if(psMBMasterInfo->eMBState == STATE_DISABLED)
    {
        /* Activate the protocol stack. */
        psMBMasterInfo->pvMBMasterFrameStartCur(psMBMasterInfo);
        psMBMasterInfo->eMBState = STATE_ENABLED;
    }
    else
    {
        return MB_EILLSTATE;
    }
#endif  
    return MB_ENOERR; 
}

/**********************************************************************
 * @brief  MODBUS协议栈禁止
 * @param  psMBMasterInfo  主栈信息块
 * @return eMBErrorCode    协议栈错误
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBMasterDisable(sMBMasterInfo* psMBMasterInfo)
{
    eMBErrorCode eStatus = MB_ENOERR;

    if( psMBMasterInfo->eMBState == STATE_ENABLED )
    {
        psMBMasterInfo->pvMBMasterFrameStopCur(psMBMasterInfo);
        psMBMasterInfo->eMBState = STATE_DISABLED;
        eStatus = MB_ENOERR;
    }
    else if(psMBMasterInfo->eMBState == STATE_DISABLED)
    {
        eStatus = MB_ENOERR;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

/**********************************************************************
 * @brief  MODBUS协议栈轮询
 *          1. 检查协议栈状态是否使能，eMBState初值为STATE_NOT_INITIALIZED，
 *             在eMBInit()函数中被赋值为STATE_DISABLED,在eMBEnable函数中被赋值为STATE_ENABLE;
 *          2. 轮询EV_FRAME_RECEIVED事件发生，若EV_FRAME_RECEIVED事件发生，
 *             接收一帧报文数据，上报EV_EXECUTE事件，解析一帧报文，响应(发送)一帧数据给主机;
 * @param  psMBMasterInfo  主栈信息块
 * @return eMBErrorCode    协议栈错误
 * @author laoc
 * @date 2019.01.22
*********************************************************************/
eMBErrorCode eMBMasterPoll(sMBMasterInfo* psMBMasterInfo)
{
    UCHAR i, j, ucFunctionCode, ucRcvAddress;

    eMBException eException;

    eMBMasterEventType      eEvent;
    eMBMasterErrorEventType errorType;
    
    eMBErrorCode       eStatus      = MB_ENOERR;  
	sMBMasterPort*     psMBPort     = &psMBMasterInfo->sMBPort;   //硬件结构
	sMBMasterDevsInfo* psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;   //从设备状态表

    /* Check if the protocol stack is ready. */
    if(psMBMasterInfo->eMBState != STATE_ENABLED)
    {
        return MB_EILLSTATE;
    }
    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event. */
    if( xMBMasterPortEventGet(psMBPort, &eEvent) == TRUE )
    {
        //debug("eMBMasterPoll\n");
        switch (eEvent)
        {
        case EV_MASTER_READY:
        break;
        case EV_MASTER_FRAME_RECEIVED:   
            eStatus = psMBMasterInfo->peMBMasterFrameReceiveCur(psMBMasterInfo, &ucRcvAddress, &psMBMasterInfo->pucMBRcvFrame,
                                                                &psMBMasterInfo->usLength);
			/* Check if the frame is for us. If not ,send an error process event. */
            if((eStatus == MB_ENOERR) && (ucRcvAddress == ucMBMasterGetDestAddr(psMBMasterInfo)))
			{
                psMBMasterInfo->pucMasterPDUCur = psMBMasterInfo->pucMBRcvFrame;
				(void) xMBMasterPortEventPost(psMBPort, EV_MASTER_EXECUTE);
			}
			else
			{
				vMBMasterSetErrorType(psMBMasterInfo, EV_ERROR_RECEIVE_DATA);
                (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_PROCESS);
			}
        break;  
        case EV_MASTER_EXECUTE:
            ucFunctionCode = *(psMBMasterInfo->pucMBRcvFrame + MB_PDU_FUNC_OFF);
            eException = MB_EX_NONE;
            /* If receive frame has exception .The receive function code highest bit is 1.*/
            if(ucFunctionCode >> 7) 
			{
                eException = (eMBException)( *(psMBMasterInfo->pucMBRcvFrame + MB_PDU_DATA_OFF) );
            }
			else
			{
				for (i = 0; i < MB_FUNC_HANDLERS_MAX; i++)
				{
					/* No more function handlers registered. Abort. */
					if (xMasterFuncHandlers[i].ucFunctionCode == 0)	
					{
						break;
					}
					else if (xMasterFuncHandlers[i].ucFunctionCode == ucFunctionCode)
					{
						/* If master request is broadcast,
						 * the master need execute function for all slave.
						 */
						if(xMBMasterRequestIsBroadcast(psMBMasterInfo)) 
						{
                            psMBMasterInfo->usLength = usMBMasterGetPDUSndLength(psMBMasterInfo);
							for(j = psMBDevsInfo->ucSlaveDevMinAddr; j <= psMBDevsInfo->ucSlaveDevMaxAddr; j++)
							{
								vMBMasterSetDestAddress(psMBMasterInfo, j);
                                eException = xMasterFuncHandlers[i].pxHandler(psMBMasterInfo, psMBMasterInfo->pucMBRcvFrame,
                                                                              &psMBMasterInfo->usLength);
							}
						}
						else 
						{
                            eException = xMasterFuncHandlers[i].pxHandler(psMBMasterInfo, psMBMasterInfo->pucMBRcvFrame,
                                                                          &psMBMasterInfo->usLength);
						}
						break;
					}
				}
			}
            /* If master has exception ,Master will send error process.Otherwise the Master is idle.*/
            if (eException != MB_EX_NONE) 
			{
            	vMBMasterSetErrorType(psMBMasterInfo, EV_ERROR_EXECUTE_FUNCTION);
            	(void) xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_PROCESS );
            }
            else 
			{
            	vMBMasterCBRequestSuccess(psMBPort);
                vMBMasterRunResRelease(psMBPort);
            }
        break;
        case EV_MASTER_FRAME_SENT:     //主栈发送请求
            //psMBMasterInfo->sMBPort.xMBIsFinished = FALSE;  /* Master is busy now. */
            vMBMasterGetPDUSndBuf(psMBMasterInfo, &psMBMasterInfo->pucMBSndFrame);	
            eStatus = psMBMasterInfo->peMBMasterFrameSendCur(psMBMasterInfo,ucMBMasterGetDestAddr(psMBMasterInfo),
                                              psMBMasterInfo->pucMBSndFrame, usMBMasterGetPDUSndLength(psMBMasterInfo));    //发送数据帧
		break;
        case EV_MASTER_ERROR_PROCESS:    //主栈处理错误
        	/* Execute specified error process callback function. */
			errorType = eMBMasterGetErrorType(psMBMasterInfo);
            vMBMasterGetPDUSndBuf(psMBMasterInfo, &psMBMasterInfo->pucMBSndFrame);
			switch(errorType) 
			{
			    case EV_ERROR_RESPOND_TIMEOUT:    //等待超时               
                    vMBMasterErrorCBRespondTimeout(psMBPort, ucMBMasterGetDestAddr(psMBMasterInfo),psMBMasterInfo->pucMBRcvFrame,
                                                   usMBMasterGetPDUSndLength(psMBMasterInfo));
			    	break;
			    case EV_ERROR_RECEIVE_DATA:      //接收数据出错
                    vMBMasterErrorCBReceiveData(psMBPort, ucMBMasterGetDestAddr(psMBMasterInfo), psMBMasterInfo->pucMBRcvFrame,
                                                usMBMasterGetPDUSndLength(psMBMasterInfo));
			    	break;
			    case EV_ERROR_EXECUTE_FUNCTION:   //处理数据出错
                    vMBMasterErrorCBExecuteFunction(psMBPort, ucMBMasterGetDestAddr(psMBMasterInfo), psMBMasterInfo->pucMBRcvFrame,
                                                    usMBMasterGetPDUSndLength(psMBMasterInfo));
			    	break;
			}
            vMBMasterRunResRelease(psMBPort);
        break;	
		default:break;
        }
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  MODBUS注册节点
 * @param  psMBMasterInfo  主栈信息块   
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBMasterRegistNode(sMBMasterInfo* psMBMasterInfo, sMBMasterNodeInfo* psMasterNode)
{
    sMBMasterInfo* psMBInfo = NULL;
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;      //主栈硬件接口信息
    sMBMasterTask* psMBTask = &psMBMasterInfo->sMBTask;      //主栈状态机任务信息
    sMBMasterDevsInfo* psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;  //主栈从设备信息
    
    if(psMBMasterInfo == NULL || psMasterNode == NULL || psMasterNode->pcMBPortName == NULL)
    {
        return FALSE;
    }
	if((psMBInfo = psMBMasterFindNodeByPort(psMasterNode->pcMBPortName)) == NULL)
    {
        psMBMasterInfo->pNext = NULL;
        psMBMasterInfo->eMode = psMasterNode->eMode;
       
        /***************************硬件接口设置***************************/
        psMBPort = (sMBMasterPort*)(&psMBMasterInfo->sMBPort);
        if(psMBPort != NULL)
        {
            psMBPort->psMBMasterInfo = psMBMasterInfo;
#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED
            psMBPort->psMBMasterUart = psMasterNode->psMasterUart;
#endif              
            psMBPort->pcMBPortName = psMasterNode->pcMBPortName;
        }
        /***************************从设备列表设置***************************/
        psMBDevsInfo = (sMBMasterDevsInfo*)(&psMBMasterInfo->sMBDevsInfo);
        if(psMBDevsInfo != NULL)
        {
            psMBDevsInfo->ucSlaveDevMinAddr = psMasterNode->ucMinAddr;
            psMBDevsInfo->ucSlaveDevMaxAddr = psMasterNode->ucMaxAddr;
        }
        /***************************主栈状态机任务块设置***************************/
        psMBTask = (sMBMasterTask*)(&psMBMasterInfo->sMBTask);
        if(psMBTask != NULL)
        {
#if MB_UCOSIII_ENABLED
            psMBTask->ucMasterPollPrio = psMasterNode->ucMasterPollPrio;
            psMBTask->ucMasterScanReadPrio = psMasterNode->ucMasterScanReadPrio;
            psMBTask->ucMasterScanWritePrio = psMasterNode->ucMasterScanWritePrio;            
#endif
        }
#if MB_MASTER_DTU_ENABLED > 0         
        /******************************GPRS模块功能支持****************************/
        psMBMasterInfo->bDTUEnable = psMasterNode->bDTUEnable;
#endif   
        /*******************************创建主栈状态机任务*************************/
        if(xMBMasterCreatePollTask(psMBMasterInfo) == FALSE)
        {
            return FALSE;
        }
        /*******************************创建主栈轮询任务*************************/
        if(xMBMasterCreateScanTask(psMBMasterInfo) == FALSE)
        {
            return FALSE;
        }
	    if(psMBMasterList == NULL)   //注册节点
	    {
            psMBMasterList = psMBMasterInfo;
	    }
	    else if(psMBMasterList->pLast != NULL)
	    {
            psMBMasterList->pLast->pNext = psMBMasterInfo;
	    }
        psMBMasterList->pLast = psMBMasterInfo;
        
        return TRUE;
    }
    return FALSE;
}

/**********************************************************************
 * @brief  MODBUS通过硬件找到所属主栈
 * @param  pcMBPortName    硬件名称
 * @return sMBMasterInfo   主栈信息块
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
sMBMasterInfo* psMBMasterFindNodeByPort(const CHAR* pcMBPortName)
{
	sMBMasterInfo* psMBMasterInfo = NULL;
	
    if(pcMBPortName == NULL)
    {
        return NULL;
    }
    for(psMBMasterInfo = psMBMasterList; psMBMasterInfo != NULL && psMBMasterInfo->sMBPort.pcMBPortName != NULL;
        psMBMasterInfo = psMBMasterInfo->pNext)
	{    
        if(strcmp(psMBMasterInfo->sMBPort.pcMBPortName, pcMBPortName) == 0)
        {
        	return psMBMasterInfo;
        }
	}
	return psMBMasterInfo;		
}

/**********************************************************************
 * @brief   主栈状态机
 * @param   *p_arg    
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
#if MB_UCOSIII_ENABLED
void vMBMasterPollTask(void *p_arg)
{
    sMBMasterInfo* psMBMasterInfo = (sMBMasterInfo*)p_arg;
    while(1)
    {
        //(void)vMBTimeDly(0, MB_MASTER_POLL_INTERVAL_MS);
        (void)eMBMasterPoll(psMBMasterInfo);
    }
}
#elif MB_LINUX_ENABLED
void* vMBMasterPollTask(void *p_arg)
{
    sMBMasterInfo* psMBMasterInfo = (sMBMasterInfo*)p_arg;
    while(1)
    {
//        debug("vMBMasterPollTask\n");
       // (void)vMBTimeDly(0, MB_MASTER_POLL_INTERVAL_MS);
        (void)eMBMasterPoll(psMBMasterInfo);      
    }
    return NULL;
}
#endif
/**********************************************************************
 * @brief  MODBUS创建主栈状态机任务
 * @param  psMBMasterInfo  主栈信息块   
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBMasterCreatePollTask(sMBMasterInfo* psMBMasterInfo)
{
#if MB_UCOSIII_ENABLED
    OS_ERR err = OS_ERR_B;
    
    CPU_STK_SIZE   stk_size = MB_MASTER_SCAN_TASK_STK_SIZE; 
    sMBMasterTask* psMBTask = &psMBMasterInfo->sMBTask;
    
    OS_PRIO prio = psMBTask->ucMasterPollPrio;
    OS_TCB* p_tcb = (OS_TCB*)(&psMBTask->sMasterPollTCB);  
    CPU_STK* p_stk_base = (CPU_STK*)(psMBTask->usMasterPollStk);
    
    if(eMBMasterInit(psMBMasterInfo) == MB_ENOERR && eMBMasterEnable(psMBMasterInfo) == MB_ENOERR )
    {
        OSTaskCreate(p_tcb, "vMBMasterPollTask", vMBMasterPollTask, (void*)psMBMasterInfo, prio, p_stk_base,
                 stk_size/10u, stk_size, 0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ), &err);
    }
    return (err == OS_ERR_NONE);

#elif MB_LINUX_ENABLED
    int ret = -1;

    if(eMBMasterInit(psMBMasterInfo) == MB_ENOERR && eMBMasterEnable(psMBMasterInfo) == MB_ENOERR )
    {
        ret = pthread_create(&psMBMasterInfo->sMBTask.sMBPollTask, NULL, vMBMasterPollTask, (void*)psMBMasterInfo);    //创建线程
    }
    return ret == 0;
#endif
}

/**********************************************************************
 * @brief   获取从设备
 * @param   psMBMasterInfo  主栈信息块
 * @param   Address         从设备地址   
 * @return	sMBSlaveDev 
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
sMBSlaveDev* psMBMasterGetDev(const sMBMasterInfo* psMBMasterInfo, UCHAR ucDevAddr)
{
    sMBSlaveDev*  psMBDev = NULL;
    const sMBMasterDevsInfo* psMBDevsInfo = &(psMBMasterInfo->sMBDevsInfo);    //从设备状态信息
    
    if( psMBDevsInfo->psMBSlaveDevsList == NULL)   //无任何结点
    {
        return NULL;
    }
    else
    {
        for(psMBDev = psMBDevsInfo->psMBSlaveDevsList; psMBDev != NULL; psMBDev = psMBDev->pNext)
        {
            if(psMBDev->ucDevAddr == ucDevAddr)
            {
                return psMBDev;
            }
        }
    }
    return psMBDev;
}

/**********************************接口函数******************************************/

/* Get whether the Modbus Master is run in master mode.*/
eMasterRunMode eMBMasterGetCBRunInMode(const sMBMasterInfo* psMBMasterInfo)
{
	return psMBMasterInfo->eMBRunMode;
}
/* Set whether the Modbus Master is run in master mode.*/
void vMBMasterSetCBRunInScanMode(sMBMasterInfo* psMBMasterInfo)
{
	psMBMasterInfo->eMBRunMode = STATE_SCAN_DEV;
}
/* Get Modbus Master send destination address. */
UCHAR ucMBMasterGetDestAddr(const sMBMasterInfo* psMBMasterInfo)
{
	return psMBMasterInfo->ucMBDestAddr;
}
/* Set Modbus Master send destination address. */
void vMBMasterSetDestAddress(sMBMasterInfo* psMBMasterInfo, UCHAR Address)
{
	psMBMasterInfo->ucMBDestAddr = Address;
}
/* Get Modbus Master current error event type. */
eMBMasterErrorEventType eMBMasterGetErrorType(const sMBMasterInfo* psMBMasterInfo)
{
	return psMBMasterInfo->eCurErrorType;
}
/* Set Modbus Master current error event type. */
void vMBMasterSetErrorType(sMBMasterInfo* psMBMasterInfo, eMBMasterErrorEventType errorType)
{
	psMBMasterInfo->eCurErrorType = errorType;
}

/* Get Modbus Master send RTU's buffer address pointer.*/
void vMBMasterGetRTUSndBuf(sMBMasterInfo* psMBMasterInfo, UCHAR** pucFrame)
{
    *pucFrame = (UCHAR*)psMBMasterInfo->ucRTUSndBuf;
}

/* Get Modbus Master send PDU's buffer address pointer.*/
void vMBMasterGetPDUSndBuf(sMBMasterInfo* psMBMasterInfo, UCHAR** pucFrame)
{
    *pucFrame = (UCHAR*) &(psMBMasterInfo->ucRTUSndBuf[MB_SER_PDU_PDU_OFF]);
}

/* Set Modbus Master send PDU's buffer length.*/
void vMBMasterSetPDUSndLength(sMBMasterInfo* psMBMasterInfo, USHORT SendPDULength)
{
    psMBMasterInfo->usSndPDULength = SendPDULength;
}

/* Get Modbus Master send PDU's buffer length.*/
USHORT usMBMasterGetPDUSndLength(const sMBMasterInfo* psMBMasterInfo)
{
    return psMBMasterInfo->usSndPDULength;
}

/* The master request is broadcast? */
BOOL xMBMasterRequestIsBroadcast(const sMBMasterInfo* psMBMasterInfo)
{
    return psMBMasterInfo->xFrameIsBroadcast;
}

/* Get Modbus Master Receive State.*/
eMBMasterRcvState usMBMasterGetRcvState(const sMBMasterInfo* psMBMasterInfo)
{
    return psMBMasterInfo->eRcvState;
}

/* Get Modbus Master send State.*/
eMBMasterSndState usMBMasterGetSndState(const sMBMasterInfo* psMBMasterInfo)
{
    return psMBMasterInfo->eSndState;
}
#endif

/**********************************************************************
 * @brief   注册设备进设备链表
 * @param   psMBMasterInfo  主栈信息块 
 * @param   Address         从设备地址 
 * @return	sMBSlaveDev
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
BOOL xMBMasterRegistDev(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBNewDev)
{
    sMBSlaveDev*       psMBDev      = NULL;
    sMBMasterDevsInfo* psMBDevsInfo = NULL;
    
    if(psMBMasterInfo == NULL || psMBNewDev == NULL)
    {
        return FALSE;
    }
    psMBDevsInfo =&psMBMasterInfo->sMBDevsInfo;    //从设备信息
    //psMBNewDev->psMBMasterInfo = psMBMasterInfo;
    psMBNewDev->pNext = NULL;
    
    if(psMBDevsInfo->psMBSlaveDevsList == NULL)   //无任何结点
    {
        psMBDevsInfo->psMBSlaveDevsList = psMBNewDev;
    }
    else //有节点
    {
        psMBDev = psMBDevsInfo->psMBSlaveDevsList->pLast;   //尾节点
        psMBDev->pNext = psMBNewDev;
    }
    psMBDevsInfo->psMBSlaveDevsList->pLast = psMBNewDev;
    psMBDevsInfo->ucSlaveDevCount++;
   
    return TRUE;
}
