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

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

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

#if MB_SLAVE_RTU_ENABLED == 1
#include "mbrtu.h"
#endif
#if MB_SLAVE_ASCII_ENABLED == 1
#include "mbascii.h"
#endif
#if MB_SLAVE_TCP_ENABLED == 1
#include "mbtcp.h"
#endif


#if MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_ASCII_ENABLED > 0

#ifndef MB_PORT_HAS_CLOSE
#define MB_PORT_HAS_CLOSE 0
#endif

#define MB_SLAVE_POLL_INTERVAL_MS           30

/* ----------------------- Static variables ---------------------------------*/ 
static sMBSlaveInfo*      psMBSlaveList = NULL; 

/* Functions pointer which are initialized in eMBInit( ). Depending on the
 * mode (RTU or ASCII) the are set to the correct implementations.
 */
static peMBSlaveFrameSend         peMBSlaveFrameSendCur;
static peMBSlaveFrameReceive      peMBSlaveFrameReceiveCur;
static pvMBSlaveFrameStart        pvMBSlaveFrameStartCur;
static pvMBSlaveFrameStop         pvMBSlaveFrameStopCur;
static pvMBSlaveFrameClose        pvMBSlaveFrameCloseCur;

/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 */
pxMBSlaveFrameCBByteReceived      pxMBSlaveFrameCBByteReceivedCur;
pxMBSlaveFrameCBTransmitterEmpty  pxMBSlaveFrameCBTransmitterEmptyCur;
pxMBSlaveFrameCBTimerExpired      pxMBSlaveFrameCBTimerExpiredCur;

pvMBSlaveFrameReceiveCallback     pvMBSlaveReceiveCallback;
pvMBSlaveFrameSendCallback        pvMBSlaveSendCallback;

/* An array of Modbus functions handlers which associates Modbus function
 * codes with implementing functions.
 */
static xMBSlaveFunctionHandler xFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0                     
    {MB_FUNC_OTHER_REPORT_SLAVEID, eMBSlaveFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0              //读输入寄存器(0x04)
    {MB_FUNC_READ_INPUT_REGISTER, eMBSlaveFuncReadInputRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0            //读保持寄存器(0x03)
    {MB_FUNC_READ_HOLDING_REGISTER, eMBSlaveFuncReadHoldingRegister},
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0  //写多个保持寄存器(0x10)
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBSlaveFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0           //写单个保持寄存器(0x06)
    {MB_FUNC_WRITE_REGISTER, eMBSlaveFuncWriteHoldingRegister},
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0       //读多个保持寄存器(0x17)
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBSlaveFuncReadWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0              //读线圈(0x01)
    {MB_FUNC_READ_COILS, eMBSlaveFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0              //写单个线圈(0x05)
    {MB_FUNC_WRITE_SINGLE_COIL, eMBSlaveFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0    //写多个线圈(0x0F)
    {MB_FUNC_WRITE_MULTIPLE_COILS, eMBSlaveFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0    //读离散量(0x02)
    {MB_FUNC_READ_DISCRETE_INPUTS, eMBSlaveFuncReadDiscreteInputs},
#endif
};

/**********************************************************************
 * @brief  MODBUS协议栈初始化
 * @param  eMode           MODBUS模式:    RTU模式   ASCII模式   TCP模式  
 * @param  pcSlaveAddr     从站地址
 * @param  *psMBSlaveUart  UART配置
 * @return eMBErrorCode    错误码
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlaveInit(sMBSlaveInfo* psMBSlaveInfo)
{
    eMBErrorCode           eStatus = MB_ENOERR;
    sMBSlavePort*         psMBPort = &psMBSlaveInfo->sMBPort;
    sMBSlaveCommInfo* psMBCommInfo = &psMBSlaveInfo->sMBCommInfo;
    
#if MB_UCOSIII_ENABLED
    UCHAR ucSlaveAddr = *psMBCommInfo->pcSlaveAddr;
#elif MB_LINUX_ENABLED
    UCHAR ucSlaveAddr = psMBCommInfo->ucSlaveAddr;
#endif
    /* check preconditions */
    if( (ucSlaveAddr == MB_ADDRESS_BROADCAST) ||
        (ucSlaveAddr < MB_ADDRESS_MIN) || (ucSlaveAddr > MB_ADDRESS_MAX) )
    {
       return MB_EINVAL;
    }
    else
    {
        switch (psMBSlaveInfo->eMode)
        {

        case MB_RTU:
#if MB_SLAVE_RTU_ENABLED > 0
    /* 协议栈核心函数指针的赋值，包括Modbus协议栈的使能和禁止、报文的接收和响应、
        3.5T定时器中断回调函数、串口发送和接收中断回调函数 */
            pvMBSlaveFrameStartCur      = eMBSlaveRTUStart;                              // 开启协议栈 
            pvMBSlaveFrameStopCur       = eMBSlaveRTUStop;                               // 终止协议栈 
            peMBSlaveFrameSendCur       = eMBSlaveRTUSend;                               // 发送消息帧 
            peMBSlaveFrameReceiveCur    = eMBSlaveRTUReceive;                            // 接收消息帧 
        
            pvMBSlaveFrameCloseCur      = MB_PORT_HAS_CLOSE ? vMBSlavePortClose : NULL;  // 关闭协议栈串口
        
            pxMBSlaveFrameCBByteReceivedCur     = xMBSlaveRTUReceiveFSM;                 //接收状态机，串口接受中断最终调用此函数接收数据
            pxMBSlaveFrameCBTransmitterEmptyCur = xMBSlaveRTUTransmitFSM;                //发送状态机，串口发送中断最终调用此函数发送数据
            pxMBSlaveFrameCBTimerExpiredCur     = xMBSlaveRTUTimerT35Expired;            //T35超时， 报文到达间隔检查，定时器中断函数最终调                                                                                //用次函数完成定时器中断
            eStatus = eMBSlaveRTUInit(psMBSlaveInfo);
#endif
            break;
        case MB_ASCII:
#if MB_SLAVE_ASCII_ENABLED > 0
            pvMBSlaveFrameStartCur = eMBASCIIStart;
            pvMBSlaveFrameStopCur = eMBASCIIStop;
            peMBSlaveFrameSendCur = eMBASCIISend;
            peMBSlaveFrameReceiveCur = eMBASCIIReceive;
            pvMBSlaveFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBPortClose : NULL;
            pxMBSlaveFrameCBByteReceived = xMBASCIIReceiveFSM;
            pxMBSlaveFrameCBTransmitterEmpty = xMBASCIITransmitFSM;
            pxMBSlaveFrameCBTimerExpired = xMBASCIITimerT1SExpired;

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
                psMBSlaveInfo->eMBState = STATE_DISABLED;    //modbus协议栈初始化状态,在此初始化为禁止
            }
        }
    }
    return MB_ENOERR;
}

#if MB_SLAVE_TCP_ENABLED > 0

eMBErrorCode eMBSlaveTCPInit( USHORT ucTCPPort )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( (eStatus = eMBTCPDoInit(ucTCPPort)) != MB_ENOERR )
    {
        eMBState = STATE_DISABLED;
    }
    else if( !xMBPortEventInit() )
    {
        /* Port dependent event module initalization failed. */
        eStatus = MB_EPORTERR;
    }
    else
    {
        pvMBFrameStartCur = eMBTCPStart;
        pvMBFrameStopCur = eMBTCPStop;
        peMBFrameReceiveCur = eMBTCPReceive;
        peMBFrameSendCur = eMBTCPSend;
        pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBTCPPortClose : NULL;
        ucMBAddress = MB_TCP_PSEUDO_ADDRESS;
        eMBState = STATE_DISABLED;
    }
    return eStatus;
}
#endif

/**********************************************************************
 * @brief  MODBUS协议栈关闭
 * @return eMBErrorCode    错误码
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlaveClose( sMBSlaveInfo* psMBSlaveInfo )
{
    sMBSlavePort* psMBPort = &psMBSlaveInfo->sMBPort;
    
    if(psMBSlaveInfo->eMBState == STATE_DISABLED)
    {
        if(pvMBSlaveFrameCloseCur != NULL)
        {
            pvMBSlaveFrameCloseCur(psMBPort);
        }
    }
    else
    {
        return MB_EILLSTATE;
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  MODBUS协议栈使能
 * @return eMBErrorCode    协议栈错误
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlaveEnable(sMBSlaveInfo* psMBSlaveInfo)
{
    if(psMBSlaveInfo->eMBState == STATE_DISABLED)
    {
        /* Activate the protocol stack. */
        pvMBSlaveFrameStartCur(psMBSlaveInfo);       //激活协议栈
        psMBSlaveInfo->eMBState = STATE_ENABLED;    //设置Modbus协议栈工作状态eMBState为STATE_ENABLED
    }
    else
    {
        return MB_EILLSTATE;
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  MODBUS协议栈禁止
 * @return eMBErrorCode    协议栈错误
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlaveDisable( sMBSlaveInfo* psMBSlaveInfo )
{
    if(psMBSlaveInfo->eMBState == STATE_ENABLED)
    {
        pvMBSlaveFrameStopCur(psMBSlaveInfo);
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
 * @brief  MODBUS协议栈轮询
 *          1. 检查协议栈状态是否使能，eMBState初值为STATE_NOT_INITIALIZED，
 *             在eMBInit()函数中被赋值为STATE_DISABLED,在eMBEnable函数中被赋值为STATE_ENABLE;
 *          2. 轮询EV_FRAME_RECEIVED事件发生，若EV_FRAME_RECEIVED事件发生，
 *             接收一帧报文数据，上报EV_EXECUTE事件，解析一帧报文，响应(发送)一帧数据给主机;
 * @return eMBErrorCode    协议栈错误
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBSlavePoll(sMBSlaveInfo* psMBSlaveInfo)
{
    static UCHAR   *pucMBFrame;            //接收和发送报文数据缓存区
    static UCHAR    ucRcvAddress;         //modbus从机地址
    static UCHAR    ucFunctionCode;       //功能码
    static USHORT   usLength;             //报文长度
    static eMBException eException;       //错误码响应 枚举
    USHORT          i;

	eMBSlaveEventType      eEvent;              //错误码
    eMBErrorCode           eStatus = MB_ENOERR;
    sMBSlavePort*         psMBPort = &psMBSlaveInfo->sMBPort;
    sMBSlaveCommInfo* psMBCommInfo = &psMBSlaveInfo->sMBCommInfo;

    /* Check if the protocol stack is ready. */
    if(psMBSlaveInfo->eMBState != STATE_ENABLED)      //检查协议栈是否使能
    {
        return MB_EILLSTATE;             //协议栈未使能，返回协议栈无效错误码
    }
     /* 检查是否有事件发生。 若没有事件发生，将控制权交还主调函数. 否则，将处理该事件 */
    if(xMBSlavePortEventGet(psMBPort, &eEvent) == TRUE)              
    {
        switch (eEvent)
        {
        case EV_READY:
            break;

        case EV_FRAME_RECEIVED:                    //接收到一帧数据，此事件发生

            /*CRC校验、提取地址、有效数据指针和有效数据长度*/
            eStatus = peMBSlaveFrameReceiveCur(psMBSlaveInfo, &ucRcvAddress, &pucMBFrame, &usLength); /*ucRcvAddress 主站要读取的从站的地址，
                                                                                                     ucMBFrame 指向PDU的头部，usLength PDU的长度*/    
		    if(eStatus == MB_ENOERR)
            {
                /* Check if the frame is for us. If not ignore the frame. */
                if( (ucRcvAddress == psMBCommInfo->ucSlaveAddr) || (ucRcvAddress == MB_ADDRESS_BROADCAST) )
                {
                    (void)xMBSlavePortEventPost(psMBPort, EV_EXECUTE);      //修改事件标志为EV_EXECUTE执行事件
                }
            }
			if(pvMBSlaveReceiveCallback != NULL)
            {
                pvMBSlaveReceiveCallback((void*)psMBSlaveInfo);
            }
		break;
			
        case EV_EXECUTE:	
		    ucFunctionCode = *(pucMBFrame + MB_PDU_FUNC_OFF);              //提取功能码
            eException = MB_EX_ILLEGAL_FUNCTION;
		
            for(i = 0; i < MB_FUNC_HANDLERS_MAX; i++)
            {
                /* No more function handlers registered. Abort. */
                if( xFuncHandlers[i].ucFunctionCode == 0 )
                {
                    break;
                }
                else if(xFuncHandlers[i].ucFunctionCode == ucFunctionCode)
                {
                    eException = xFuncHandlers[i].pxHandler(psMBSlaveInfo, pucMBFrame, &usLength); //该结构体将功能码和相应功能的处理函数捆绑在一起。 
                    break;                                                            
                }
            }
            /*若不是广播命令，则需要发出响应。*/
            if( ucRcvAddress != MB_ADDRESS_BROADCAST )     
            {
                if(eException != MB_EX_NONE)
                {
                    /*发生异常，建立一个错误报告帧*/
                    usLength = 0;
                    *(pucMBFrame + (usLength++)) = (UCHAR)(ucFunctionCode | MB_FUNC_ERROR);    //响应发送数据帧的第二个字节，功能码最高位置1
                    *(pucMBFrame + (usLength++)) = (UCHAR)eException;                                 //响应发送数据帧的第三个字节为错误码标识
                }
                 /* eMBRTUSend()进行必要的发送预设后，禁用RX，使能TX。发送操作由USART_DATA（UDR空）中断实现。*/			
                eStatus = peMBSlaveFrameSendCur(psMBSlaveInfo, psMBCommInfo->ucSlaveAddr, pucMBFrame, usLength); //modbus从机响应函数,发送响应给主机
            }
        break;
        case EV_FRAME_SENT:
			vMBSlavePortSerialEnable(psMBPort, TRUE, FALSE);      //使能接收，禁止发送
        
            if(pvMBSlaveSendCallback != NULL)
            {
                pvMBSlaveSendCallback((void*)psMBSlaveInfo);
            }
        break;	
        }
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  MODBUS注册节点
 * @param  psMBSlaveInfo  从栈信息块   
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBSlaveRegistNode(sMBSlaveInfo* psMBSlaveInfo, sMBSlaveNodeInfo* psSlaveNode)
{
    sMBSlaveInfo*         psMBInfo = NULL;
    sMBSlavePort*         psMBPort = &psMBSlaveInfo->sMBPort;   //从栈硬件接口信息
	sMBSlaveTask*         psMBTask = &psMBSlaveInfo->sMBTask;   //从栈状态机任务信息
    sMBSlaveCommInfo* psMBCommInfo = &psMBSlaveInfo->sMBCommInfo;   //从栈通讯信息
    
    if(psMBSlaveInfo == NULL || psSlaveNode == NULL)
    {
        return FALSE;
    }
	if( (psMBInfo = psMBSlaveFindNodeByPort(psSlaveNode->pcMBPortName)) == NULL)
    {
        psMBSlaveInfo->pNext = NULL;
        psMBSlaveInfo->eMode = psSlaveNode->eMode;
        
        /***************************硬件接口设置***************************/
        psMBPort = (sMBSlavePort*)(&psMBSlaveInfo->sMBPort);
        if(psMBPort != NULL)
        {
            psMBPort->psMBSlaveInfo = psMBSlaveInfo;
            psMBPort->psMBSlaveUart = psSlaveNode->psSlaveUart;
            psMBPort->pcMBPortName  = psSlaveNode->pcMBPortName;
        }
        
        /***************************通讯参数信息设置***************************/
        psMBCommInfo = (sMBSlaveCommInfo*)(&psMBSlaveInfo->sMBCommInfo);
        if(psMBCommInfo != NULL)
        {
            psMBCommInfo->ucSlaveAddr = psSlaveNode->ucSlaveAddr;
        }
        /***************************从栈状态机任务块设置***************************/
#if MB_UCOSIII_ENABLED
        psMBTask   = (sMBSlaveTask*)(&psMBSlaveInfo->sMBTask);
        if(psMBTask != NULL)
        {

            psMBTask->ucSlavePollPrio = psSlaveNode->ucSlavePollPrio;
        }
#endif
        /*******************************创建从栈状态机任务*************************/
        if(xMBSlaveCreatePollTask(psMBSlaveInfo) == FALSE)  
        {
            return FALSE;
        }
	    if(psMBSlaveList == NULL)  //注册节点
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
 * @brief  MODBUS注册通讯数据表
 * @param  psMBSlaveInfo  从栈信息块   
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
void vMBSlaveRegistCommData(sMBSlaveInfo* psMBSlaveInfo, sMBSlaveCommData* psSlaveCurData)
{
    psMBSlaveInfo->sMBCommInfo.psSlaveCurData = psSlaveCurData;
}

/**********************************************************************
 * @brief  MODBUS通过硬件找到所属从栈
 * @param  pcMBPortName    硬件名称
 * @return sMBMasterInfo   从栈信息块
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
sMBSlaveInfo* psMBSlaveFindNodeByPort(const CHAR* pcMBPortName)
{
	sMBSlaveInfo*  psMBSlaveInfo = NULL;

	for( psMBSlaveInfo = psMBSlaveList; psMBSlaveInfo != NULL; psMBSlaveInfo = psMBSlaveInfo->pNext )
	{
        if( strcmp(psMBSlaveInfo->sMBPort.pcMBPortName, pcMBPortName) == 0 )
        {
            return psMBSlaveInfo;
        }
	}
	return psMBSlaveInfo;		
}

/**********************************************************************
 * @brief   从栈状态机
 * @param   *p_arg    
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
#if MB_UCOSIII_ENABLED
void vMBSlavePollTask(void *p_arg)
{
    sMBSlaveInfo* psMBSlaveInfo = (sMBSlaveInfo*)p_arg;;
    if(eMBSlaveInit(psMBSlaveInfo) == MB_ENOERR && eMBSlaveEnable(psMBSlaveInfo) == MB_ENOERR)
    {
        while(1)
        {
//          (void)OSTimeDlyHMSM(0, 0, 0, MB_SLAVE_POLL_INTERVAL_MS, OS_OPT_TIME_HMSM_STRICT, &err);
            (void)eMBSlavePoll(psMBSlaveInfo);
        }
    }
}
#elif MB_LINUX_ENABLED
void* vMBSlavePollTask(void *p_arg)
{
    sMBSlaveInfo* psMBSlaveInfo = (sMBSlaveInfo*)p_arg;;
    if(eMBSlaveInit(psMBSlaveInfo) == MB_ENOERR && eMBSlaveEnable(psMBSlaveInfo) == MB_ENOERR)
    {
        while(1)
        {
//          (void)OSTimeDlyHMSM(0, 0, 0, MB_SLAVE_POLL_INTERVAL_MS, OS_OPT_TIME_HMSM_STRICT, &err);
            (void)eMBSlavePoll(psMBSlaveInfo);
        }
    }
    return NULL;
}
#endif
/**********************************************************************
 * @brief  MODBUS创建主栈状态机任务
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
    
    OS_PRIO             prio = psMBTask->ucSlavePollPrio;
    OS_TCB*            p_tcb = (OS_TCB*)(&psMBTask->sSlavePollTCB);  
    CPU_STK*      p_stk_base = (CPU_STK*)(psMBTask->usSlavePollStk);
    
    OSTaskCreate(p_tcb, "vMBSlavePollTask", vMBSlavePollTask, (void*)psMBSlaveInfo, prio, p_stk_base, 
                 stk_size/10u, stk_size, 0u, 0u, 0u, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ), &err);
    return (err == OS_ERR_NONE);
#elif MB_LINUX_ENABLED
    int ret = pthread_create(&psMBSlaveInfo->sMBTask.sMBPollTask, NULL, vMBSlavePollTask, (void*)psMBSlaveInfo);    //创建线程
    return ret == 0;
#endif
}

#endif
