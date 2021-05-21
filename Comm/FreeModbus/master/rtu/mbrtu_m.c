/*
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2013 China Beijing Armink <armink.ztl@gmail.com>
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
 * File: $Id: mbrtu_m.c,v 1.60 2013/08/17 11:42:56 Armink Add Master Functions $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/

#include "mb_m.h"
#include "mbrtu_m.h"
#include "mbframe.h"
#include "mbconfig.h"
#include "mbdict_m.h"

#if MB_MASTER_RTU_ENABLED
/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/

/**********************************************************************
 * @brief  RTU模式协议栈初始化
 * @param  *Uart           UART配置
 * @return eMBErrorCode    协议栈错误
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode eMBMasterRTUInit(sMBMasterInfo* psMBMasterInfo)
{
    ULONG usTimerT35_50us;
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;

    uint32_t Baud_rate;

#if MB_UCOSIII_ENABLED
    Baud_rate = psMBPort->psMBMasterUart->UARTCfg.Baud_rate;

#elif MB_LINUX_ENABLED
    Baud_rate = psMBPort->psMBMasterUart->baud;
#endif
    if(xMBMasterPortSerialInit(psMBPort) != TRUE)      //串口初始化
    {
        return MB_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        if(Baud_rate > 19200)
        {
            usTimerT35_50us = 35;       /* 1800us. */
        }
        else
        {
            /* The timer reload value for a character is given by:
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            usTimerT35_50us = (7UL * 220000UL) / (2UL * Baud_rate);
        }
        if( xMBMasterPortTmrsInit(psMBPort, (USHORT)usTimerT35_50us) != TRUE )       //t35超时定时器
        {
            return MB_EPORTERR;
        }
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  开始RTU模式协议栈
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
void vMBMasterRTUStart(sMBMasterInfo* psMBMasterInfo)
{
	sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;
	
    psMBMasterInfo->eSndState = STATE_M_TX_IDLE;
    psMBMasterInfo->eRcvState = STATE_M_RX_IDLE;
    //vMBMasterPortSerialEnable(psMBPort, FALSE, TRUE);  //从栈等待数据，开启串口接收，发送未开启
    //vMBsMasterPortTmrsEnable(psMBPort);                //启动定时器
}

/**********************************************************************
 * @brief  关闭RTU模式协议栈
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
void vMBMasterRTUStop(sMBMasterInfo* psMBMasterInfo)
{
	sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;
	
    vMBMasterPortSerialEnable(psMBPort,FALSE, FALSE);
    vMBsMasterPortTmrsDisable(psMBPort);
}

/**********************************************************************
 * @brief  报文接收处理
 *         1. eMBPoll函数轮询到EV_FRAME_RECEIVED事件时,调用peMBFrameReceiveCur()
 *         2. 从一帧数据报文中，取得modbus从机地址给pucRcvAddress、PDU报文的长度给pusLength
 *         3. PDU报文的首地址给pucFrame，函数*形参全部为地址传递
 * @param  pucRcvAddress   从站地址
 * @param  pucFrame        MODBUS数据帧指针
 * @param  pusLength       PDU长度
 * @return eMBErrorCode    协议栈错误 
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode
eMBMasterRTUReceive(sMBMasterInfo* psMBMasterInfo, UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength)
{
    /*  eMBRTUReceive函数完成了CRC校验、帧数据地址和长度的赋值，便于给上层进行处理*/
    if((psMBMasterInfo->usRcvBufferPos >= MB_SER_PDU_SIZE_MIN)  /* Length and CRC check */
        && (usMBCRC16((UCHAR*)psMBMasterInfo->ucRTURcvBuf, psMBMasterInfo->usRcvBufferPos ) == 0))
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pucRcvAddress = psMBMasterInfo->ucRTURcvBuf[MB_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = (USHORT)(psMBMasterInfo->usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC);     //PDU的长度为数据帧-从栈地址-CRC校验

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = (UCHAR*) &(psMBMasterInfo->ucRTURcvBuf[MB_SER_PDU_PDU_OFF]);      //pucFrame指向PDU起始位置
    }
    else
    {
        return MB_EIO;
    }
    if(psMBMasterInfo->pvMBMasterReceiveCallback != NULL)
    {
        psMBMasterInfo->pvMBMasterReceiveCallback((void*)psMBMasterInfo);
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  报文发送处理
 *         1. 对响应报文PDU前面加上从机地址
 *         2. 对响应报文PDU后加上CRC校
 *         3. 使能发送，启动传输
 * @param  ucSlaveAddr   从站地址
 * @param  pucFrame        MODBUS数据帧指针
 * @param  pusLength       PDU长度
 * @return eMBErrorCode    协议栈错误  
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
eMBErrorCode
eMBMasterRTUSend(sMBMasterInfo* psMBMasterInfo, UCHAR ucSlaveAddr, UCHAR* pucFrame, USHORT usLength)
{
    /* 在 eMBRTUSend函数中会调用串口发送数据，在进入串口发送中断后会调用xMBRTUTransmitFSM
     * 发送状态机函数发送应答报文。*/
    USHORT             usCRC16;
    sMBMasterPort*     psMBPort     = &psMBMasterInfo->sMBPort;
    sMBMasterDevsInfo* psMBDevsInfo = &psMBMasterInfo->sMBDevsInfo;   //从设备状态

    //debug("eMBMasterRTUSend ucSlaveAddr %d\n" ,ucSlaveAddr);
    
    if( (ucSlaveAddr < psMBDevsInfo->ucSlaveDevMinAddr) || (ucSlaveAddr > psMBDevsInfo->ucSlaveDevMaxAddr) ) 
	{
		return MB_EINVAL;
	}
    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if(psMBMasterInfo->eRcvState == STATE_M_RX_IDLE)
    {
        /* First byte before the Modbus-PDU is the slave address. */
        psMBMasterInfo->pucSndBufferCur = (UCHAR*)(pucFrame - 1);
        psMBMasterInfo->usSndBufferCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        *(psMBMasterInfo->pucSndBufferCur + MB_SER_PDU_ADDR_OFF) = ucSlaveAddr;        //在协议数据单元前加从机地址
        psMBMasterInfo->usSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usMBCRC16( (UCHAR*) psMBMasterInfo->pucSndBufferCur, psMBMasterInfo->usSndBufferCount );
        psMBMasterInfo->ucRTUSndBuf[psMBMasterInfo->usSndBufferCount++] = ( UCHAR )( usCRC16 & 0xFF );
        psMBMasterInfo->ucRTUSndBuf[psMBMasterInfo->usSndBufferCount++] = ( UCHAR )( usCRC16 >> 8 );

        /* Activate the transmitter. */
        psMBMasterInfo->eSndState = STATE_M_TX_XMIT;               //发送状态
        vMBMasterPortSerialEnable(psMBPort, FALSE, TRUE);  //使能发送，禁止接收

#if MB_UCOSIII_ENABLED
        (void)xMBMasterPortSerialPutByte( psMBPort, (CHAR)(*psMBMasterInfo->pucSndBufferCur) );//启动第一次发送
        psMBMasterInfo->pucSndBufferCur++;
        psMBMasterInfo->usSndBufferCount--;

#elif MB_LINUX_ENABLED
        psMBMasterInfo->usRcvBufferPos = 0;
        xMBMasterRTUTransmitFSM(psMBMasterInfo);  
#endif
    }
    else
    {
        return MB_EIO;
    }
    if(psMBMasterInfo->pvMBMasterSendCallback != NULL)
    {
        psMBMasterInfo->pvMBMasterSendCallback((void*)psMBMasterInfo);
    }
    return MB_ENOERR;
}

/**********************************************************************
 * @brief  串口接收数据
 *         1. 将接收到的数据存入ucRTUBuf[]中
 *         2. usRcvBufferPos为全局变量，表示接收数据的个数
 *         3. 每接收到一个字节的数据，3.5T定时器清0
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBMasterRTUReceiveFSM(sMBMasterInfo* psMBMasterInfo)
{
    /*在串口中断前，状态机为eRcvState=STATE_RX_IDLE，接收状态机开始后，读取uart串口缓存中的数据，并进入STATE_RX_IDLE分支中存储一次数据后开启定时器，
    然后进入STATE_RX_RCV分支继续接收后续的数据，直至定时器超时！如果没有超时的话，状态不会转换，将还可以继续接收数据。超时之后，
    在T3.5超时函数xMBRTUTimerT35Expired 中将发送EV_FRAME_RECEIVED事件。然后eMBPoll函数将会调用eMBRTUReceive函数。*/
    CHAR ucByte;
    USHORT usRcvBytes;
    BOOL  xTaskNeedSwitch = FALSE;
    sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;
	
    assert_param(eSndState == STATE_M_TX_IDLE || eSndState == STATE_M_TX_XFWR);   //确保没有数据在发送或者主栈没有在等待从栈响应
    
    (void)xMBMasterPortSerialGetByte(psMBPort, &ucByte); /* Always read the character. */
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
    case STATE_M_RX_IDLE:    // 接收器空闲，开始接收，进入STATE_RX_RCV状态
    	/* In time of respond timeout,the receiver receive a frame.
    	 * Disable timer of respond timeout and change the transmiter state to idle.
    	 */
    	vMBsMasterPortTmrsDisable(psMBPort);
    	psMBMasterInfo->eSndState = STATE_M_TX_IDLE;

        psMBMasterInfo->usRcvBufferPos = 0;
        psMBMasterInfo->eRcvState = STATE_M_RX_RCV;

#if MB_UCOSIII_ENABLED
        psMBMasterInfo->ucRTURcvBuf[psMBMasterInfo->usRcvBufferPos++] = ucByte;
        vMBsMasterPortTmrsEnable(psMBPort);              //重启3.5T定时器

#elif MB_LINUX_ENABLED
        xMBMasterPortSerialReadBytes(psMBPort, &psMBMasterInfo->ucRTURcvBuf[psMBMasterInfo->usRcvBufferPos], &usRcvBytes);
        psMBMasterInfo->usRcvBufferPos += usRcvBytes;

        //xMBMasterRTUTimerExpired(psMBMasterInfo);
        vMBsMasterPortTmrsEnable(psMBPort);              //重启3.5T定时器

        //debug("xMBMasterRTUReceiveFSM usRcvBufferPos %d \n",  psMBMasterInfo->usRcvBufferPos);
#endif
        break;
        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_M_RX_RCV:
        if(psMBMasterInfo->usRcvBufferPos < MB_SER_PDU_SIZE_MAX)  //一帧报文的字节数大于最大PDU长度，忽略超出的数据
        {
#if MB_UCOSIII_ENABLED
            psMBMasterInfo->ucRTURcvBuf[psMBMasterInfo->usRcvBufferPos++] = ucByte;

#elif MB_LINUX_ENABLED
            xMBMasterPortSerialReadBytes(psMBPort, &psMBMasterInfo->ucRTURcvBuf[psMBMasterInfo->usRcvBufferPos], &usRcvBytes);
            psMBMasterInfo->usRcvBufferPos += usRcvBytes;
#endif
        }
        else
        {
            psMBMasterInfo->eRcvState = STATE_M_RX_ERROR;
        }
        vMBsMasterPortTmrsEnable(psMBPort);                   //每收到一个字节，都重启3.5T定时器
        break;
    }
    return xTaskNeedSwitch;
}

/**********************************************************************
 * @brief  串口发送数据
 *         eMBRTUSend函数中会调用串口发送数据，在进入串口发送中断后会调用xMBRTUTransmitFSM
 *         发送状态机函数发送应答报文。
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBMasterRTUTransmitFSM(sMBMasterInfo* psMBMasterInfo)
{
    BOOL           xNeedPoll = FALSE;
    sMBMasterPort*  psMBPort = &psMBMasterInfo->sMBPort;
	
    assert_param(eRcvState == STATE_M_RX_IDLE);
    switch(psMBMasterInfo->eSndState)
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_M_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vMBMasterPortSerialEnable(psMBPort, TRUE, FALSE); //发送器处于空闲状态，使能接收，禁止发送
        break;
    case STATE_M_TX_XMIT: //发送器处于发送状态,在从机发送函数eMBRTUSend中赋值STATE_TX_XMIT
        if(psMBMasterInfo->usSndBufferCount != 0)  /* check if we are finished. */
        {
#if MB_UCOSIII_ENABLED
            (void)xMBMasterPortSerialPutByte( psMBPort, (CHAR)(*psMBMasterInfo->pucSndBufferCur) );          //发送数据
            psMBMasterInfo->pucSndBufferCur++;  /* next byte in sendbuffer. */
            psMBMasterInfo->usSndBufferCount--;

#elif MB_LINUX_ENABLED
            if(xMBMasterPortSerialWriteBytes(psMBPort, psMBMasterInfo->pucSndBufferCur, psMBMasterInfo->usSndBufferCount) )
            {
                psMBMasterInfo->usSndBufferCount = 0;
                psMBMasterInfo->eSndState = STATE_M_TX_XFWR;
                vMBsMasterPortTmrsRespondTimeoutEnable(psMBPort);
            }
#endif              
        }
        else
        {
            psMBMasterInfo->xFrameIsBroadcast = (psMBMasterInfo->ucRTUSndBuf[MB_SER_PDU_ADDR_OFF] == MB_ADDRESS_BROADCAST ) ? TRUE : FALSE;
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */	
            vMBMasterPortSerialEnable(psMBPort, TRUE, FALSE);
            psMBMasterInfo->eSndState = STATE_M_TX_XFWR;

            /* If the frame is broadcast ,master will enable timer of convert delay,
             * else master will enable timer of respond timeout. */
            if(psMBMasterInfo->xFrameIsBroadcast == TRUE)
            {
                vMBsMasterPortTmrsConvertDelayEnable(psMBPort);
            }
            else
            {
                //debug("xMBMasterRTUTransmitFSM  %d \n", psMBMasterInfo->eSndState);
                vMBsMasterPortTmrsRespondTimeoutEnable(psMBPort);
            }
        }
        break;
	default: break;
    }
    return xNeedPoll;
}

/**********************************************************************
 * @brief  超时响应函数
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBMasterRTUTimerExpired(sMBMasterInfo* psMBMasterInfo)
{
	BOOL xNeedPoll = FALSE;
	sMBMasterPort* psMBPort = &psMBMasterInfo->sMBPort;
	
    //debug("xMBMasterRTUTimerExpired %d \n", psMBMasterInfo->eSndState);
	switch (psMBMasterInfo->eRcvState)
	{
	case STATE_M_RX_INIT:  /* Timer t35 expired. Startup phase is finished. */
		xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_READY);
		break;			 
	case STATE_M_RX_RCV:  /* A frame was received and t35 expired. Notify the listener that a new frame was received. */
        if(psMBMasterInfo->usRcvBufferPos >= MB_SER_PDU_SIZE_MIN)   //接收最少数据数
		{
             xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_FRAME_RECEIVED);   //一帧数据接收完成，上报协议栈事件,接收到一帧完整的数据
 //           debug("EV_MASTER_FRAME_RECEIVED  %d \n", psMBMasterInfo->usRcvBufferPos);
		}
		else
		{
			psMBMasterInfo->eSndState = STATE_M_TX_XFWR;
			//vMBsMasterPortTmrsRespondTimeoutEnable(psMBPort);      //接收数据不完整，重启定时器
			//xSndStateNeedChange = FALSE;
			//debug("EV_MASTER_FRAME_RECEIVED_ERROR******************\n");
        }
		break;	
	case STATE_M_RX_ERROR:  /* An error occured while receiving the frame. */
		vMBMasterSetErrorType(psMBMasterInfo, EV_ERROR_RECEIVE_DATA);
        xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_PROCESS);
		break;
	default:  /* Function called in an illegal state. */
		assert_param( (eRcvState == STATE_M_RX_INIT) || (eRcvState == STATE_M_RX_RCV) ||
				      (eRcvState == STATE_M_RX_ERRO) || (eRcvState == STATE_M_RX_IDLE) );
		break;
	}
	psMBMasterInfo->eRcvState = STATE_M_RX_IDLE;   //处理完数据，接收器状态为空闲

	switch(psMBMasterInfo->eSndState)
	{
		/* A frame was send finish and convert delay or respond timeout expired.
		 * If the frame is broadcast,The master will idle,and if the frame is not
		 * broadcast.Notify the listener process error.*/
	case STATE_M_TX_XFWR:      
		if (psMBMasterInfo->xFrameIsBroadcast == FALSE) 
		{
			vMBMasterSetErrorType(psMBMasterInfo, EV_ERROR_RESPOND_TIMEOUT);
			xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_PROCESS);   //上报接收数据超时
		}
		break;
	default:    /* Function called in an illegal state. */
		assert_param(eSndState == STATE_M_TX_XFWR || eSndState == STATE_M_TX_IDLE);
		break;
	}
	psMBMasterInfo->eSndState = STATE_M_TX_IDLE;
    vMBsMasterPortTmrsDisable(psMBPort);  //当接收到一帧数据后，禁止3.5T定时器，直到接受下一帧数据开始，开始计时

	/* If timer mode is convert delay, the master event then turns EV_MASTER_EXECUTE status. */
    if (psMBPort->eCurTimerMode == MB_TMODE_CONVERT_DELAY)
    {
        xNeedPoll = xMBMasterPortEventPost(psMBPort, EV_MASTER_EXECUTE);
    }
	return xNeedPoll;
}
#endif

