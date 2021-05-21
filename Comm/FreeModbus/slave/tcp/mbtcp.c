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
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbport.h"

#if MB_SLAVE_TCP_ENABLED
#include "mbtcp.h"

#if MB_UCOSIII_ENABLED
#include "md_lwip.h"
#include "tcp.h"
#include "sys_arch.h"

#elif MB_LINUX_ENABLED
#include <signal.h>
#include <pthread.h>
#include <netinet/tcp.h>

#endif

/* ----------------------- Defines ------------------------------------------*/
#define BACKLOG     10

char cMBSlaveTcpClientName[MB_SLAVE_MAX_TCP_CLIENT][16];

/* ----------------------- Start implementation -----------------------------*/
#if MB_UCOSIII_ENABLED
void vMBSlaveTcpServerTask(void *p_arg)
{
    OS_ERR err = OS_ERR_NONE;
    struct sockaddr_in tSocketClientAddr;

    int iRet;
    int iSocketServer;
    int iClientNum = 0;

    uint8_t i = 0;
    uint32_t iAddrLen = 0;
    char c1[] = {"MB_TCP_CLIENT_"};

    int keeplive = 1;     //开启keeplive属性
    int keepidle = 5;     //该时间段内无数据交互，则进行探测
    int keepinterval = 5; //探测包间隔
    int keepcount = 3;    //探测次数

    sMBSlaveNodeInfo m_MBSlaveNode;
    sMBSlaveTcpInfo *psMBSlaveTcpInfo = (sMBSlaveTcpInfo*)p_arg;

    m_MBSlaveNode.eMode = MB_TCP;
    m_MBSlaveNode.ucSlaveAddr = psMBSlaveTcpInfo->ucSlaveAddr;

    psMBSlaveTcpInfo->sTcpServerAddr.sin_family      = AF_INET;
    psMBSlaveTcpInfo->sTcpServerAddr.sin_port        = htons(MODBUS_TCP_PORT);  /* host to net, short */
    psMBSlaveTcpInfo->sTcpServerAddr.sin_addr.s_addr = INADDR_ANY;

    memset(psMBSlaveTcpInfo->sTcpServerAddr.sin_zero, 0, 8);

    iSocketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == iSocketServer)
    {
        debug("socket error!\n");
        return;
    }
    iRet = bind(iSocketServer, (const struct sockaddr*)&psMBSlaveTcpInfo->sTcpServerAddr, sizeof(struct sockaddr));
    if (-1 == iRet)
    {
        debug("bind error!\n");
        return;
    }
    iRet = listen(iSocketServer, BACKLOG);
    if (-1 == iRet)
    {
        debug("listen error!\n");
        return;
    }
    for(i = 0; i < MB_SLAVE_MAX_TCP_CLIENT; i++)
    {
        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected = FALSE;
    }
    while(1)
    {
        m_MBSlaveNode.iSocketClient = -1;
        for(i = 0; i < MB_SLAVE_MAX_TCP_CLIENT; i++)
        {
            if(psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected == FALSE)
            {
                iAddrLen = sizeof(struct sockaddr);
                m_MBSlaveNode.iSocketClient = accept(iSocketServer, (struct sockaddr*)&tSocketClientAddr, &iAddrLen);
#if MB_UCOSIII_ENABLED
                m_MBSlaveNode.ucSlavePollPrio = psMBSlaveTcpInfo->ucSlaveTcpPollPrio;
#endif
                break;
            }
        }
        if(m_MBSlaveNode.iSocketClient != -1)
        {
            keeplive = 1;     //开启keeplive属性
            keepidle = 5;     //该时间段内无数据交互，则进行探测
            keepinterval = 5; //探测包间隔
            keepcount = 3;    //探测次数

            setsockopt(m_MBSlaveNode.iSocketClient, SOL_SOCKET, SO_KEEPALIVE, (void*)&keeplive, sizeof(keeplive));
            setsockopt(m_MBSlaveNode.iSocketClient, IPPROTO_TCP, TCP_KEEPIDLE, (void*)&keepidle, sizeof(keepidle));
            setsockopt(m_MBSlaveNode.iSocketClient, IPPROTO_TCP, TCP_KEEPINTVL, (void*)&keepinterval, sizeof(keepinterval));
            setsockopt(m_MBSlaveNode.iSocketClient, IPPROTO_TCP, TCP_KEEPCNT, (void*)&keepcount, sizeof(keepcount));

            if(iClientNum < MB_SLAVE_MAX_TCP_CLIENT)
            {
                c1[14] = '0' + iClientNum;
                strcpy(cMBSlaveTcpClientName[iClientNum], c1);
                m_MBSlaveNode.pcMBPortName = cMBSlaveTcpClientName[iClientNum];

                if(xMBSlaveRegistNode(&psMBSlaveTcpInfo->sMBSlaveTcpClients[iClientNum], &m_MBSlaveNode))
                {
                    psMBSlaveTcpInfo->sMBSlaveTcpClients[iClientNum].sMBPort.xSocketConnected = TRUE;
                    psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.iSocketOfflines = 0;

                    if(psMBSlaveTcpInfo->pvMBSlaveReceiveCallback != NULL)
                    {
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].pvMBSlaveReceiveCallback = psMBSlaveTcpInfo->pvMBSlaveReceiveCallback;
                    }
                    if(psMBSlaveTcpInfo->pvMBSlaveSendCallback != NULL)
                    {
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].pvMBSlaveSendCallback = psMBSlaveTcpInfo->pvMBSlaveSendCallback;
                    }
                }
                debug("vMBSlaveTcpServerTask iClientNum %d \n", iClientNum);
                iClientNum++;
            }
            else
            {
                for(i = 0; i < MB_SLAVE_MAX_TCP_CLIENT; i++)
                {
                    if(psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected == FALSE)
                    {
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.fd = m_MBSlaveNode.iSocketClient;
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected = TRUE;
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.iSocketOfflines = 0;
                        debug("vMBSlaveTcpServerTask xSocketConnected %d %d\n", i, psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected);
                        break;
                    }
                }
            }
        }
        else
        {
            (void)vMBTimeDly(0, 500);
        }
    }
    close(iSocketServer);
}
#elif MB_LINUX_ENABLED
void* vMBSlaveTcpServerTask(void *p_arg)
{
   // OS_ERR err = OS_ERR_NONE;
    struct sockaddr_in tSocketClientAddr;
    
    int iRet;
    int iSocketServer;
    int iClientNum = 0;
    
    uint8_t i = 0;
    uint32_t iAddrLen = 0;
    char c1[] = {"MB_TCP_CLIENT_"};
 
    int keeplive = 1;     //开启keeplive属性
    int keepidle = 5;     //该时间段内无数据交互，则进行探测
    int keepinterval = 5; //探测包间隔
    int keepcount = 3;    //探测次数
    
    sMBSlaveNodeInfo m_MBSlaveNode;
    sMBSlaveTcpInfo *psMBSlaveTcpInfo = (sMBSlaveTcpInfo*)p_arg; 
    
    m_MBSlaveNode.eMode = MB_TCP;
    m_MBSlaveNode.ucSlaveAddr = psMBSlaveTcpInfo->ucSlaveAddr;
    
    psMBSlaveTcpInfo->sTcpServerAddr.sin_family      = AF_INET;
    psMBSlaveTcpInfo->sTcpServerAddr.sin_port        = htons(MODBUS_TCP_PORT);  /* host to net, short */
    psMBSlaveTcpInfo->sTcpServerAddr.sin_addr.s_addr = INADDR_ANY;
   
    memset(psMBSlaveTcpInfo->sTcpServerAddr.sin_zero, 0, 8);

    iSocketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == iSocketServer)
    {
        debug("socket error!\n");
        return NULL;
    }
    iRet = bind(iSocketServer, (const struct sockaddr*)&psMBSlaveTcpInfo->sTcpServerAddr, sizeof(struct sockaddr));
    if (-1 == iRet)
    {
        debug("bind error!\n");
        return NULL;
    }
    iRet = listen(iSocketServer, BACKLOG);
    if (-1 == iRet)
    {
        debug("listen error!\n");
        return NULL;
    } 
    for(i = 0; i < MB_SLAVE_MAX_TCP_CLIENT; i++)
    {
        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected = FALSE;
    }
    while(1)
    {
        m_MBSlaveNode.iSocketClient = -1;
        for(i = 0; i < MB_SLAVE_MAX_TCP_CLIENT; i++)
        {
            if(psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected == FALSE)
            {
                iAddrLen = sizeof(struct sockaddr);
                m_MBSlaveNode.iSocketClient = accept(iSocketServer, (struct sockaddr*)&tSocketClientAddr, &iAddrLen);
#if MB_UCOSIII_ENABLED                
                m_MBSlaveNode.ucSlavePollPrio = psMBSlaveTcpInfo->ucSlaveTcpPollPrio;
#endif                                   
                break;
            }
        }
        if(m_MBSlaveNode.iSocketClient != -1)
        {
            keeplive = 1;     //开启keeplive属性
            keepidle = 5;     //该时间段内无数据交互，则进行探测
            keepinterval = 5; //探测包间隔
            keepcount = 3;    //探测次数
            
            setsockopt(m_MBSlaveNode.iSocketClient, SOL_SOCKET, SO_KEEPALIVE, (void*)&keeplive, sizeof(keeplive));
            setsockopt(m_MBSlaveNode.iSocketClient, IPPROTO_TCP, TCP_KEEPIDLE, (void*)&keepidle, sizeof(keepidle));
            setsockopt(m_MBSlaveNode.iSocketClient, IPPROTO_TCP, TCP_KEEPINTVL, (void*)&keepinterval, sizeof(keepinterval));
            setsockopt(m_MBSlaveNode.iSocketClient, IPPROTO_TCP, TCP_KEEPCNT, (void*)&keepcount, sizeof(keepcount));
            
            if(iClientNum < MB_SLAVE_MAX_TCP_CLIENT)
            {
                c1[14] = '0' + iClientNum;
                strcpy(cMBSlaveTcpClientName[iClientNum], c1);
                m_MBSlaveNode.pcMBPortName = cMBSlaveTcpClientName[iClientNum];

                if(xMBSlaveRegistNode(&psMBSlaveTcpInfo->sMBSlaveTcpClients[iClientNum], &m_MBSlaveNode))
                {
                    psMBSlaveTcpInfo->sMBSlaveTcpClients[iClientNum].sMBPort.xSocketConnected = TRUE;
                    psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.iSocketOfflines = 0;

                    if(psMBSlaveTcpInfo->pvMBSlaveReceiveCallback != NULL)
                    {
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].pvMBSlaveReceiveCallback = psMBSlaveTcpInfo->pvMBSlaveReceiveCallback;
                    }
                    if(psMBSlaveTcpInfo->pvMBSlaveSendCallback != NULL)
                    {
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].pvMBSlaveSendCallback = psMBSlaveTcpInfo->pvMBSlaveSendCallback;
                    } 
                }
                debug("vMBSlaveTcpServerTask iClientNum %d \n", iClientNum);   
                iClientNum++;
            }
            else
            {
                for(i = 0; i < MB_SLAVE_MAX_TCP_CLIENT; i++)
                {
                    if(psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected == FALSE)
                    {
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.fd = m_MBSlaveNode.iSocketClient;
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected = TRUE;
                        psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.iSocketOfflines = 0;
                        debug("vMBSlaveTcpServerTask xSocketConnected %d %d\n", i, psMBSlaveTcpInfo->sMBSlaveTcpClients[i].sMBPort.xSocketConnected); 
                        break;
                    }
                }
            }
        }
        else
        {
            (void)vMBTimeDly(0, 500);
        }
    }
    close(iSocketServer);
}
#endif

BOOL xMBSlaveTCPServerInit(sMBSlaveTcpInfo *psMBSlaveTcpInfo)
{  
 #if MB_UCOSIII_ENABLED  
    OS_ERR err = OS_ERR_NONE;    
    OSTaskCreate((OS_TCB 	*)&psMBSlaveTcpInfo->sSlaveTcpServerTCB,		
				 (CPU_CHAR	*)"vMBSlaveTcpServerTask", 		
                 (OS_TASK_PTR)vMBSlaveTcpServerTask, 			
                 (void		*)psMBSlaveTcpInfo,					
                 (OS_PRIO	 )psMBSlaveTcpInfo->ucSlaveTcpServerPrio,    
                 (CPU_STK   *)&psMBSlaveTcpInfo->usSlaveTcpServerStk[0],	
                 (CPU_STK_SIZE)MB_SLAVE_POLL_TASK_STK_SIZE / 10,	
                 (CPU_STK_SIZE)MB_SLAVE_POLL_TASK_STK_SIZE,	
                 (OS_MSG_QTY )0,					
                 (OS_TICK	 )0,					
                 (void   	*)0,					
                 (OS_OPT     )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	*)&err);
    return err == OS_ERR_NONE;  
#elif MB_LINUX_ENABLED    
 
    int ret = pthread_create(&psMBSlaveTcpInfo->sMBSlaveTcpServerTask, NULL, vMBSlaveTcpServerTask, (void*)psMBSlaveTcpInfo);    //创建线程
    return ret == 0;
#endif 
}

eMBErrorCode eMBTCPDoInit(USHORT ucTCPPort)
{
}

void vMBSlaveTCPStart(sMBSlaveInfo* psMBSlaveInfo)
{
    xMBSlaveTCPPortGetRequest(&psMBSlaveInfo->sMBPort);
}

void vMBSlaveTCPStop(sMBSlaveInfo* psMBSlaveInfo)
{ 
    close(psMBSlaveInfo->sMBPort.fd);
}

void vMBSlaveTCPGetRequest(sMBSlaveInfo* psMBSlaveInfo)
{
    xMBSlaveTCPPortGetRequest(&psMBSlaveInfo->sMBPort);
}

eMBErrorCode eMBSlaveTCPReceive(sMBSlaveInfo* psMBSlaveInfo, UCHAR *pucRcvAddr, UCHAR **pucFrame, USHORT *pusLength)
{
    USHORT usPID;
    UCHAR *pucMBTCPFrame = (UCHAR*)(psMBSlaveInfo->ucRcvBuf);

    usPID = pucMBTCPFrame[MB_TCP_PID] << 8U;
    usPID |= pucMBTCPFrame[MB_TCP_PID + 1];

    if(usPID == MB_TCP_PROTOCOL_ID)
    {
        *pusLength = pucMBTCPFrame[MB_TCP_LEN + 1] - 1; //00 00 00 00 00 06 01 03 00 04 00 01 => 03 00 04 00 01
        *pucRcvAddr = pucMBTCPFrame[MB_TCP_UID];
        *pucFrame = &pucMBTCPFrame[MB_TCP_FUNC];

        //debug("eMBSlaveTCPReceive usRcvBufferPos %d  pusLength  %d\n", psMBSlaveInfo->usRcvBufferPos, *pusLength);
        if(psMBSlaveInfo->usRcvBufferPos - MB_TCP_FUNC + 1 < *pusLength)
        {
            return MB_EIO;
        }
    }
    else
    {
        return MB_EIO;
    }
    if(psMBSlaveInfo->pvMBSlaveReceiveCallback != NULL)
    {
        psMBSlaveInfo->pvMBSlaveReceiveCallback((void*)psMBSlaveInfo);
    }
    return MB_ENOERR;
}

eMBErrorCode eMBSlaveTCPSend(sMBSlaveInfo* psMBSlaveInfo, UCHAR ucSlaveAddr, const UCHAR *pucFrame, USHORT usLength)
{
    sMBSlavePort* psMBPort = &psMBSlaveInfo->sMBPort;
    psMBSlaveInfo->pucSndBufferCur = (UCHAR*)pucFrame - MB_TCP_FUNC;

    psMBSlaveInfo->pucSndBufferCur[MB_TCP_LEN + 1] = (UCHAR)usLength + 1; //后续字节数
    psMBSlaveInfo->pucSndBufferCur[MB_TCP_UID] = ucSlaveAddr;    //地址
    psMBSlaveInfo->usSndBufferCount = usLength + MB_TCP_FUNC;    //总长度

    if(xMBSlaveTCPPortSendResponse(psMBPort, psMBSlaveInfo->pucSndBufferCur, psMBSlaveInfo->usSndBufferCount))
    {
        if(psMBSlaveInfo->pvMBSlaveSendCallback != NULL)
        {
            psMBSlaveInfo->pvMBSlaveSendCallback((void*)psMBSlaveInfo);
        }
    }
    return xMBSlavePortEventPost(psMBPort, EV_FRAME_SENT);
}

#endif
