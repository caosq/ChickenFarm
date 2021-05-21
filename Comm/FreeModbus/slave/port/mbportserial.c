/*
  * FreeModbus Libary: LPC214X Port
  * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>
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
 *
 * File: $Id: portserial.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "port.h"
#include "mb.h"
#include "mbport.h"

#if MB_UCOSIII_ENABLED
#include "mbdriver.h"

#elif MB_LINUX_ENABLED
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#endif

#if MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED

/* ----------------------- Start implementation -----------------------------*/
void vMBSlavePortSerialEnable(sMBSlavePort* psMBPort, BOOL xRxEnable, BOOL xTxEnable)
{
    sMBSlaveInfo* psMBSlaveInfo = psMBPort->psMBSlaveInfo;
#if MB_UCOSIII_ENABLED	
    const sUART_Def* psMBSlaveUart = psMBPort->psMBSlaveUart;
    
	UART_FIFOReset(psMBSlaveUart->ID, (UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS | UART_FCR_TRG_LEV2));
	if(xRxEnable)
	{
         UART_IntConfig(psMBSlaveUart->ID, UART_INTCFG_RBR, ENABLE); 		//开启接收中断
		 MB_SendOrRecive(psMBSlaveUart, UART_RX_EN);
	}
	else
	{
		 UART_IntConfig(psMBSlaveUart->ID, UART_INTCFG_RBR, DISABLE);    //关闭接收中断
		 MB_SendOrRecive(psMBSlaveUart, UART_TX_EN);
	}

	if(xTxEnable)
	{
		UART_IntConfig(psMBSlaveUart->ID, UART_INTCFG_THRE, ENABLE); 	   //开启发送中断
		MB_SendOrRecive(psMBSlaveUart, UART_TX_EN);
		UART_TxCmd(psMBSlaveUart->ID, ENABLE);                           //UART中断
	}
	else
	{
		UART_IntConfig(psMBSlaveUart->ID, UART_INTCFG_THRE, DISABLE); 	//关闭接收中断
		MB_SendOrRecive(psMBSlaveUart, UART_RX_EN);
		UART_TxCmd(psMBSlaveUart->ID, DISABLE);                           
	}
#elif MB_LINUX_ENABLED

    int select_ret = 0;
    fd_set rfds;

    if(xRxEnable)
    {
        //ioctl(psMBPort->psMBSlaveUart->fd, TCFLSH, TCIFLUSH);
        //tcflush(psMBPort->psMBSlaveUart->fd, TCIFLUSH);
        FD_ZERO(&rfds);
        FD_SET(psMBPort->fd, &rfds);

        select_ret = select(psMBPort->fd+1, &rfds, NULL, NULL, NULL);
        if (select_ret > 0)
        {
            if(psMBSlaveInfo->pxMBSlaveFrameCBByteReceivedCur != NULL)
            {
                (void)psMBSlaveInfo->pxMBSlaveFrameCBByteReceivedCur(psMBPort->psMBSlaveInfo);
            }
        }
        else
        {
             xMBSlavePortEventPost(psMBPort, EV_ERROR_RCV);
        }
    }
    if(xTxEnable)
    {
        //tcflush(psMBPort->psMBSlaveUart->fd, TCOFLUSH);
        //ioctl(psMBPort->psMBSlaveUart->fd, TCFLSH, TCOFLUSH);
    }
#endif 
}

void vMBSlavePortClose(sMBSlavePort* psMBPort)
{
#if MB_UCOSIII_ENABLED 	
    const sUART_Def* psMBSlaveUart = psMBPort->psMBSlaveUart;
    
	UART_IntConfig(psMBSlaveUart->ID, UART_INTCFG_THRE|UART_INTCFG_RBR, DISABLE);
	UART_TxCmd(psMBSlaveUart->ID, DISABLE);
#elif MB_LINUX_ENABLED
    
    close(psMBPort->fd);
#endif  
}

BOOL xMBSlavePortSerialInit(sMBSlavePort* psMBPort)
{
#if MB_UCOSIII_ENABLED
    return xMB_UartInit(psMBPort->psMBSlaveUart);
	
#elif MB_LINUX_ENABLED 
    psMBPort->fd = open(psMBPort->pcMBPortName, O_RDWR | O_NOCTTY | O_NDELAY);
    if(psMBPort->fd < 0) {
        return 0;
    }
    return xMB_UartInit(psMBPort->psMBSlaveUart, psMBPort->fd);
#endif  
}

BOOL xMBSlavePortSerialPutByte(sMBSlavePort* psMBPort, UCHAR ucByte)
{
#if MB_UCOSIII_ENABLED
//	UCHAR h, l;
	const sUART_Def* psMBSlaveUart = psMBPort->psMBSlaveUart;
//	h=ucByte >> 4 ;
//	l=ucByte % 16 ;	
//	h= (h<10)? h+48: h+87;
//	l= (l<10)? l+48: l+87;	
//	
//    debug("TX:%c%c\n", h,l);
	
	UART_SendByte(psMBSlaveUart->ID, ucByte);
#endif
    return TRUE;
}

BOOL xMBSlavePortSerialGetByte(sMBSlavePort* psMBPort, UCHAR* pucByte)
{
#if MB_UCOSIII_ENABLED
	//UCHAR h, l;
	const sUART_Def* psMBSlaveUart = psMBPort->psMBSlaveUart;
	*pucByte = UART_ReceiveByte(psMBSlaveUart->ID);
	
//	h=(*pucByte)>> 4 ;
//	l=(*pucByte) % 16 ;	
//	h= (h<10)? h+48: h+87;
//	l= (l<10)? l+48: l+87;	
//	
//    debug("RX:%c%c\n", h,l);
#endif
    return TRUE;
}

BOOL xMBSlavePortSerialWriteBytes(sMBSlavePort* psMBPort, UCHAR* pucSndBuf, USHORT usSndBytes)
{
#if MB_LINUX_ENABLED
    ssize_t ret = write(psMBPort->fd, pucSndBuf, usSndBytes);
    return ret == usSndBytes ? TRUE : FALSE;
#endif
}

BOOL xMBSlavePortSerialReadBytes(const sMBSlavePort* psMBPort, UCHAR* pucRcvBuf, USHORT* psReadBytes)
{
#if MB_LINUX_ENABLED
    ssize_t sReadBytes = 0;
    *psReadBytes = 0;

    while( (sReadBytes = read(psMBPort->fd, pucRcvBuf + *psReadBytes, 255)) > 0)
    {
        *psReadBytes += (USHORT)sReadBytes;
    }
    return *psReadBytes > 0 ? TRUE:FALSE;
#endif
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvSlaveUARTTxReadyISR(const sMBSlavePort* psMBPort)
{
    sMBSlaveInfo* psMBSlaveInfo = psMBPort->psMBSlaveInfo;
    if(psMBSlaveInfo != NULL && psMBSlaveInfo->pxMBSlaveFrameCBTransmitterEmptyCur != NULL)
    {
        (void)psMBSlaveInfo->pxMBSlaveFrameCBTransmitterEmptyCur(psMBSlaveInfo);
    } 
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvSlaveUARTRxISR(const sMBSlavePort* psMBPort)
{
    sMBSlaveInfo* psMBSlaveInfo = psMBPort->psMBSlaveInfo;
    if(psMBSlaveInfo != NULL && psMBSlaveInfo->pxMBSlaveFrameCBByteReceivedCur != NULL)
    {
        (void)psMBSlaveInfo->pxMBSlaveFrameCBByteReceivedCur(psMBSlaveInfo);
    }
}

#endif

