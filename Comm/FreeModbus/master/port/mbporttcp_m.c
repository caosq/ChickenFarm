/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
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
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb_m.h"
#include "mbconfig.h"
#include "mbport_m.h"

#if MB_MASTER_TCP_ENABLED

#if MB_UCOSIII_ENABLED
#include "inet.h"

#elif MB_LINUX_ENABLED
#include "arpa/inet.h"
#endif

#define MB_SLAVE_PORT_TIMEOUT_S   10    //从站等待延时
#define MB_SLAVE_PORT_OFF_TIMES   6    //从站掉线次数判断

void vMBMasterTCPPortInit(sMBMasterPort* psMBPort)     //初始化
{
   sMBSlaveDev* psMBSlaveDevCur = psMBPort->psMBSlaveDevCur;
   if(psMBSlaveDevCur != NULL)
   {
       psMBSlaveDevCur->tSocketServerAddr.sin_family = AF_INET;
       psMBSlaveDevCur->tSocketServerAddr.sin_port   = htons(psMBSlaveDevCur->uiMBServerPort);  /* host to net, short */
       inet_aton(psMBSlaveDevCur->pcMBServerIP, &psMBSlaveDevCur->tSocketServerAddr.sin_addr);
        
       //memset(psMBSlaveDevCur->tSocketServerAddr.sin_zero, 0, 8);
   }
}

BOOL xMBMasterTCPPortConn(sMBMasterPort* psMBPort)  //初始化
{
    int iRet = 0;
    sMBSlaveDev* psMBSlaveDevCur = psMBPort->psMBSlaveDevCur;
    if(psMBSlaveDevCur != NULL)
    {
        psMBSlaveDevCur->iSocketClient = socket(AF_INET, SOCK_STREAM, 0);
        iRet = connect(psMBSlaveDevCur->iSocketClient, (struct sockaddr*)&psMBSlaveDevCur->tSocketServerAddr, sizeof(struct sockaddr));

        if(iRet >= 0)
        {
            psMBSlaveDevCur->xSocketConnected = TRUE;
            psMBSlaveDevCur->iSocketOfflines = 0;
        }
    }
    debug("xMBMasterTCPPortConn iRet %d \n", iRet);
    return iRet < 0 ? FALSE : TRUE;
}

BOOL xMBMasterTCPPortReadBytes(sMBMasterPort* psMBPort, UCHAR* pucRcvBuf, USHORT* psReadBytes)
{
    int sReadBytes = 0;
    sMBSlaveDev* psMBSlaveDevCur = psMBPort->psMBSlaveDevCur;
    *psReadBytes = 0;
    if(psMBSlaveDevCur != NULL)
    {
        sReadBytes = recv(psMBSlaveDevCur->iSocketClient, pucRcvBuf + *psReadBytes, 255, 0);
        debug("xMBMasterTCPPortReadBytes sReadBytes %d \n", sReadBytes);
        if(sReadBytes <= 0)
        {
            if(psMBSlaveDevCur->xSocketConnected)
            {
                psMBSlaveDevCur->iSocketOfflines++;  
            }
            if(psMBSlaveDevCur->iSocketOfflines >= MB_SLAVE_PORT_OFF_TIMES)
            {
                psMBSlaveDevCur->xSocketConnected = FALSE;
                close(psMBSlaveDevCur->iSocketClient);     
                psMBSlaveDevCur->iSocketOfflines = 0;
            }
            return FALSE;
        }
        *psReadBytes += (USHORT)sReadBytes;
    }
    if(sReadBytes >= 0)
    {
        psMBSlaveDevCur->iSocketOfflines = 0;
    }
    return sReadBytes > 0 ? TRUE:FALSE;
}

BOOL xMBMasterTCPPortWriteBytes(sMBMasterPort* psMBPort, const UCHAR *pucMBTCPFrame, USHORT usTCPLength) //初始化
{
    int sSendBytes = 0;
    sMBSlaveDev* psMBSlaveDevCur = psMBPort->psMBSlaveDevCur;
    if(psMBSlaveDevCur != NULL)
    {
        if(psMBSlaveDevCur->xSocketConnected == FALSE)   //重连socket
        {
            vMBMasterTCPPortInit(psMBPort);
            if(!xMBMasterTCPPortConn(psMBPort))
            {
                return FALSE;
            }
            (void)vMBTimeDly(1, 0);
            //return FALSE;
        }
        sSendBytes = send(psMBSlaveDevCur->iSocketClient, pucMBTCPFrame, usTCPLength, 0);
        if(sSendBytes <= 0)
        {
            psMBSlaveDevCur->xSocketConnected = FALSE;
            close(psMBSlaveDevCur->iSocketClient);
            return FALSE;
        }
    }
    if(sSendBytes >= usTCPLength)
    {
        psMBSlaveDevCur->iSocketOfflines = 0;
    }
    debug("xMBMasterTCPPortWriteBytes sSendBytes %d \n", sSendBytes); 
    return sSendBytes >= usTCPLength ? TRUE : FALSE;
}
#endif
