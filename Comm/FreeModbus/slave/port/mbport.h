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
 * File: $Id: mbport.h,v 1.17 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _MB_PORT_H
#define _MB_PORT_H

#include "mbproto.h"
#include "mbconfig.h"
#include "mbdriver.h"

#if MB_UCOSIII_ENABLED

#elif MB_LINUX_ENABLED
#include <semaphore.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* ----------------------- Type definitions ---------------------------------*/

typedef enum
{
    EV_READY,                   /*!< Startup finished. */
    EV_FRAME_RECEIVED,          /*!< Frame received. */
    EV_EXECUTE,                 /*!< Execute function. */
    EV_FRAME_SENT,              /*!< Frame sent. */
    EV_ERROR_RCV
} eMBSlaveEventType;

typedef struct                        /* ??????????????????  */
{
    struct sMBSlaveInfo* psMBSlaveInfo; //????????????
    eMBSlaveEventType    eQueuedEvent;  //??????????????????
    const CHAR*          pcMBPortName;  //??????????????????
    BOOL                 xEventInQueue; //????????????????????????

#if MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED 
    sUART_Def* psMBSlaveUart;      //??????????????????????????????

#if MB_UCOSIII_ENABLED
    OS_TMR    sSlavePortTmr;  //????????????3.5?????????????????????
#elif MB_LINUX_ENABLED
    uint16_t  usTim1Timerout50us;
    struct timeval sSlavePortTv;   //????????????3.5????????????
#endif

#endif

#if MB_SLAVE_TCP_ENABLED
    int fd;                //?????????????????????
    int iSocketOfflines;   //????????????
    BOOL xSocketConnected; //socket????????????
#endif

#if MB_UCOSIII_ENABLED
    OS_SEM sMBEventSem;    //???????????????????????????
    
#elif MB_LINUX_ENABLED 
    sem_t sMBEventSem;    //???????????????????????????
#endif

}sMBSlavePort;

/* -----------------------Slave Serial port functions ----------------------------*/
BOOL xMBSlavePortSerialInit(sMBSlavePort* psMBPort);

void vMBSlavePortClose(sMBSlavePort* psMBPort);
void xMBSlavePortSerialClose(sMBSlavePort* psMBPort);
void vMBSlavePortSerialEnable(sMBSlavePort* psMBPort, BOOL xRxEnable, BOOL xTxEnable);

INLINE BOOL xMBSlavePortSerialGetByte(sMBSlavePort* psMBPort, UCHAR* pucByte);
INLINE BOOL xMBSlavePortSerialPutByte(sMBSlavePort* psMBPort, UCHAR ucByte);
INLINE BOOL xMBSlavePortSerialReadBytes(const sMBSlavePort* psMBPort, UCHAR* pucRcvBuf, USHORT* psReadBytes);
INLINE BOOL xMBSlavePortSerialWriteBytes(sMBSlavePort* psMBPort, UCHAR* pucSndBuf, USHORT usSndBytes);

void prvvSlaveUARTTxReadyISR(const sMBSlavePort* psMBPort);
void prvvSlaveUARTRxISR(const sMBSlavePort* psMBPort);

/* -----------------------Master Serial port event functions ----------------------------*/
BOOL xMBSlavePortEventInit(sMBSlavePort* psMBPort);
BOOL xMBSlavePortEventPost(sMBSlavePort* psMBPort, eMBSlaveEventType eEvent);
BOOL xMBSlavePortEventGet(sMBSlavePort* psMBPort, eMBSlaveEventType* eEvent);

/* ----------------------- Timers functions ---------------------------------*/
BOOL xMBSlavePortTimersInit(sMBSlavePort* psMBPort, USHORT usTim1Timerout50us);
void xMBSlavePortTimersClose(sMBSlavePort* psMBPort);

INLINE void vMBSlavePortTimersEnable(sMBSlavePort* psMBPort);
INLINE void vMBSlavePortTimersDisable(sMBSlavePort* psMBPort);

eMBException prveMBSlaveError2Exception(eMBErrorCode eErrorCode);

/* ----------------------- TCP port functions -------------------------------*/
BOOL xMBSlaveTCPPortInit(sMBSlavePort* psMBPort);

void vMBSlaveTCPPortClose(sMBSlavePort* psMBPort);
void vMBSlaveTCPPortDisable(sMBSlavePort* psMBPort);

BOOL xMBSlaveTCPPortGetRequest(sMBSlavePort* psMBPort);
BOOL xMBSlaveTCPPortSendResponse(sMBSlavePort* psMBPort, const UCHAR *pucMBTCPFrame, USHORT usTCPLength);

#ifdef __cplusplus
}
#endif

#endif
