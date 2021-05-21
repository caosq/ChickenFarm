/*
 * FreeModbus Libary: A portable Modbus implementation for Modbus TCP.
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
 * File: $Id: mbtcp.c,v 1.3 2006/12/07 22:10:34 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mbconfig.h"
#include "mbframe.h"
#include "mbport_m.h"

#if MB_MASTER_TCP_ENABLED
#include "mbtcp_m.h"

#if MB_UCOSIII_ENABLED

#elif MB_LINUX_ENABLED
#include <signal.h>
#include <pthread.h>

#endif

#define MB_MASTER_TCP_PORT_DELAY   35

void vMBMasterTCPSetDev(sMBSlaveDev* psMBSlaveDev, const CHAR* pcMBServerIP, uint16_t uiMBServerPort)
{
    if(psMBSlaveDev !=NULL)
    {
        if(psMBSlaveDev->xSocketConnected == TRUE)
        {
            psMBSlaveDev->xSocketConnected == FALSE;
            close(psMBSlaveDev->iSocketClient);
        }
        psMBSlaveDev->pcMBServerIP = pcMBServerIP;
        psMBSlaveDev->uiMBServerPort = uiMBServerPort;
    }  
}

eMBErrorCode eMBMasterTCPInit(sMBMasterInfo* psMBMasterInfo)
{
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;
    //xMBMasterTCPPortInit(psMBPort);  //socket连接
    xMBMasterPortTmrsInit(psMBPort, MB_MASTER_TCP_PORT_DELAY);

    return MB_ENOERR;
}

void vMBMasterTCPStart(sMBMasterInfo* psMBMasterInfo)
{
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;
    psMBMasterInfo->eSndState = STATE_M_TX_IDLE;
    psMBMasterInfo->eRcvState = STATE_M_RX_IDLE;
    //xMBMasterTCPPortInit(psMBPort);  //socket连接
}

void vMBMasterTCPStop(sMBMasterInfo* psMBMasterInfo)
{
    close(psMBMasterInfo->sMBPort.fd);  
}

eMBErrorCode
eMBMasterTCPReceive(sMBMasterInfo* psMBMasterInfo, UCHAR* pucRcvAddr, UCHAR** pucFrame, USHORT* pusLength)
{
    USHORT usPID;
    UCHAR *pucMBTCPFrame = (UCHAR*)(psMBMasterInfo->ucRTURcvBuf);

    usPID = pucMBTCPFrame[MB_TCP_PID] << 8U;
    usPID |= pucMBTCPFrame[MB_TCP_PID + 1];

    if(usPID == MB_TCP_PROTOCOL_ID)
    {
        *pusLength = pucMBTCPFrame[MB_TCP_LEN + 1] - 1; //00 00 00 00 00 06 01 03 00 04 00 01 => 03 00 04 00 01
        *pucRcvAddr = pucMBTCPFrame[MB_TCP_UID];
        *pucFrame = &pucMBTCPFrame[MB_TCP_FUNC];

        if(psMBMasterInfo->usRcvBufferPos - MB_TCP_FUNC +1 < *pusLength)
        {
            return MB_EIO;
        }
    }
    else
    {
        return MB_EIO;
    }
    if(psMBMasterInfo->pvMBMasterReceiveCallback != NULL)
    {
        //psMBMasterInfo->pvMBMasterReceiveCallback((void*)psMBMasterInfo);
    }
    return MB_ENOERR;
}

eMBErrorCode
eMBMasterTCPSend(sMBMasterInfo* psMBMasterInfo, UCHAR ucSlaveAddr, UCHAR* pucFrame, USHORT usLength)
{
    uint8_t i = 0;
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;
    sMBMasterDevsInfo* psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo; //从设备状态
    sMBSlaveDev* psMBSlaveDevCur = psMBDevsInfo->psMBSlaveDevCur;   //当前从设备

    if(ucSlaveAddr < psMBDevsInfo->ucSlaveDevMinAddr || ucSlaveAddr > psMBDevsInfo->ucSlaveDevMaxAddr)
    {
        return MB_EINVAL;
    }
    if(psMBSlaveDevCur == NULL || psMBSlaveDevCur->ucDevAddr != ucSlaveAddr)
    {
        psMBSlaveDevCur = psMBMasterGetDev(psMBMasterInfo, ucSlaveAddr);
    }
    if(psMBMasterInfo->eRcvState == STATE_M_RX_IDLE)
    {
        psMBPort->psMBSlaveDevCur = psMBSlaveDevCur;
        psMBMasterInfo->pucSndBufferCur = (UCHAR*)pucFrame - MB_TCP_FUNC;
        for(i = 0; i < MB_TCP_FUNC - 1; i++)
        {
            psMBMasterInfo->pucSndBufferCur[i] = 0;
        }
        psMBMasterInfo->pucSndBufferCur[MB_TCP_LEN] = 0; //后续字节数
        psMBMasterInfo->pucSndBufferCur[MB_TCP_LEN + 1] = (UCHAR)usLength + 1; //后续字节数
        psMBMasterInfo->pucSndBufferCur[MB_TCP_UID] = ucSlaveAddr;    //地址
        psMBMasterInfo->usSndBufferCount = usLength + MB_TCP_FUNC;    //总长度

        /* Activate the transmitter. */
        psMBMasterInfo->eSndState = STATE_M_TX_XMIT;  //发送状态
        if(!xMBMasterTCPTransmitFSM(psMBMasterInfo))
        {
            return MB_EIO;
        }
    }
    else
    {
        return MB_EIO;
    }
    if(psMBMasterInfo->pvMBMasterSendCallback != NULL)
    {
        //psMBMasterInfo->pvMBMasterSendCallback((void*)psMBMasterInfo);
    }
    return MB_ENOERR;
}

BOOL xMBMasterTCPTransmitFSM(sMBMasterInfo* psMBMasterInfo)
{
    BOOL xNeedPoll = FALSE;
    sMBMasterPort*  psMBPort = &psMBMasterInfo->sMBPort;

    assert_param(eRcvState == STATE_M_RX_IDLE);
    switch(psMBMasterInfo->eSndState)
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_M_TX_IDLE:
        /* enable receiver/disable transmitter. */
        //vMBMasterPortSerialEnable(psMBPort, TRUE, FALSE);    //发送器处于空闲状态，使能接收，禁止发送
        break;

    case STATE_M_TX_XMIT:        //发送器处于发送状态,在从机发送函数eMBRTUSend中赋值STATE_TX_XMIT
        /* check if we are finished. */
        if(psMBMasterInfo->usSndBufferCount != 0)
        {
            if(xMBMasterTCPPortWriteBytes(psMBPort, psMBMasterInfo->pucSndBufferCur, psMBMasterInfo->usSndBufferCount))
            {
                psMBMasterInfo->usSndBufferCount = 0;
                psMBMasterInfo->eSndState = STATE_M_TX_XFWR;
                vMBsMasterPortTmrsRespondTimeoutEnable(psMBPort);
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            psMBMasterInfo->xFrameIsBroadcast = (psMBMasterInfo->ucRTUSndBuf[MB_SER_PDU_ADDR_OFF] == MB_ADDRESS_BROADCAST) ? TRUE : FALSE;
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */

            //vMBMasterPortSerialEnable(psMBPort, TRUE, FALSE);
            psMBMasterInfo->eSndState = STATE_M_TX_XFWR;

            /* If the frame is broadcast ,master will enable timer of convert delay,
             * else master will enable timer of respond timeout. */
            if ( psMBMasterInfo->xFrameIsBroadcast == TRUE )
            {
                vMBsMasterPortTmrsConvertDelayEnable(psMBPort);
            }
            else
            {
                vMBsMasterPortTmrsRespondTimeoutEnable(psMBPort);
                //debug("vMBsMasterPortTmrsRespondTimeoutEnable\n");
            }
        }
        break;
    default: break;
    }
    return xNeedPoll;
}

BOOL xMBMasterTCPReceiveFSM(sMBMasterInfo* psMBMasterInfo)
{
    USHORT usRcvBytes;
    BOOL  xTaskNeedSwitch = FALSE;
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;

    assert_param(( eSndState == STATE_M_TX_IDLE ) || ( eSndState == STATE_M_TX_XFWR ));//确保没有数据在发送或者主栈没有在等待从栈响应
    switch(psMBMasterInfo->eRcvState)
    {
        /* If we have received a character in the init state we have to
         * wait until the frame is finished.
         */
    case STATE_M_RX_INIT:
        vMBsMasterPortTmrsEnable(psMBPort);
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted.
         */
    case STATE_M_RX_ERROR:     //数据帧被损坏，重启定时器，不保存串口接收的数据
        vMBsMasterPortTmrsEnable(psMBPort);
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE and disable early
         * the timer of respond timeout .
         */
    case STATE_M_RX_IDLE:                                     // 接收器空闲，开始接收，进入STATE_RX_RCV状态
        /* In time of respond timeout,the receiver receive a frame.
         * Disable timer of respond timeout and change the transmiter state to idle.
         */
        vMBsMasterPortTmrsDisable(psMBPort);
        psMBMasterInfo->eSndState = STATE_M_TX_IDLE;

        psMBMasterInfo->usRcvBufferPos = 0;
        psMBMasterInfo->eRcvState = STATE_M_RX_RCV;

        xMBMasterTCPPortReadBytes(psMBPort, &psMBMasterInfo->ucRTURcvBuf[psMBMasterInfo->usRcvBufferPos], &usRcvBytes);
        psMBMasterInfo->usRcvBufferPos += usRcvBytes;
        vMBsMasterPortTmrsEnable(psMBPort);   //重启3.5T定时器
        //debug("xMBMasterTCPReceiveFSM usRcvBufferPos %d \n",  psMBMasterInfo->usRcvBufferPos);

        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_M_RX_RCV:
        if(psMBMasterInfo->usRcvBufferPos < MB_SER_PDU_SIZE_MAX)  //一帧报文的字节数大于最大PDU长度，忽略超出的数据
        {
            xMBMasterTCPPortReadBytes(psMBPort, &psMBMasterInfo->ucRTURcvBuf[psMBMasterInfo->usRcvBufferPos], &usRcvBytes);
            psMBMasterInfo->usRcvBufferPos += usRcvBytes;
        }
        else
        {
            psMBMasterInfo->eRcvState = STATE_M_RX_ERROR;
        }
        vMBsMasterPortTmrsEnable(psMBPort);   //每收到字节，都重启3.5T定时器
        break;
    }
    return xTaskNeedSwitch;
}

BOOL xMBMasterTCPTimerExpired(sMBMasterInfo* psMBMasterInfo)
{
    BOOL xNeedPoll = FALSE;
    BOOL xSndStateNeedChange = TRUE;
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;

    switch (psMBMasterInfo->eRcvState)
    {
    case STATE_M_RX_INIT:    /* Timer t35 expired. Startup phase is finished. */
        xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_READY);
        break;
        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_M_RX_RCV:
        if(psMBMasterInfo->usRcvBufferPos >= MB_SER_PDU_SIZE_MIN)   //接收最少数据数
        {
             xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_FRAME_RECEIVED);   //一帧数据接收完成，上报协议栈事件,接收到一帧完整的数据
 //           debug("EV_MASTER_FRAME_RECEIVED  %d \n", psMBMasterInfo->usRcvBufferPos);
        }
        break;
    case STATE_M_RX_ERROR:   /* An error occured while receiving the frame. */
        vMBMasterSetErrorType(psMBMasterInfo, EV_ERROR_RECEIVE_DATA);
        xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_PROCESS);
        break;

    default:   /* Function called in an illegal state. */
        assert_param( (eRcvState == STATE_M_RX_INIT)  || (eRcvState == STATE_M_RX_RCV) ||
                      (eRcvState == STATE_M_RX_ERRO ) || (eRcvState == STATE_M_RX_IDLE) );
        break;
    }
    psMBMasterInfo->eRcvState = STATE_M_RX_IDLE;   //处理完数据，接收器状态为空闲

    if(xSndStateNeedChange)
    {
        switch (psMBMasterInfo->eSndState)
        {
            /* A frame was send finish and convert delay or respond timeout expired.
             * If the frame is broadcast,The master will idle,and if the frame is not
             * broadcast.Notify the listener process error.*/
        case STATE_M_TX_XFWR:                       //接收超时
            if (psMBMasterInfo->xFrameIsBroadcast == FALSE)
            {
                vMBMasterSetErrorType(psMBMasterInfo, EV_ERROR_RESPOND_TIMEOUT);   /* Function called in an illegal state. */
                xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_PROCESS);   //上报接收数据超时
            }
            break;

        default:
            assert_param( (eSndState == STATE_M_TX_XFWR) || (eSndState == STATE_M_TX_IDLE) );
            break;
        }
        psMBMasterInfo->eSndState = STATE_M_TX_IDLE;
        vMBsMasterPortTmrsDisable(psMBPort);  //当接收到一帧数据后，禁止3.5T定时器，直到接受下一帧数据开始，开始计时

    /* If timer mode is convert delay, the master event then turns EV_MASTER_EXECUTE status. */
        if (psMBPort->eCurTimerMode == MB_TMODE_CONVERT_DELAY)
        {
            xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_EXECUTE);
        }
    }
    return xNeedPoll;
}
#endif
