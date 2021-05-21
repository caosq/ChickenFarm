#include "stddef.h"
#include "port.h"
#include "mb.h"
#include "mbport.h"

#if MB_UCOSIII_ENABLED
#include "tcp.h"
#include "sys_arch.h"
#include "api.h"

#elif MB_LINUX_ENABLED

#endif

#if MB_SLAVE_TCP_ENABLED

#define MB_SLAVE_RCV_MAX_BYTES    255    //最大读取字节数
#define MB_SLAVE_PORT_TIMEOUT_S   10    //从站等待延时
#define MB_SLAVE_PORT_OFF_TIMES   6    //从站掉线次数判断

//UCHAR rcvbuf[100];

void vMBSlaveTCPPortClose(sMBSlavePort* psMBPort)
{
}
void vMBSlaveTCPPortDisable(sMBSlavePort* psMBPort)
{
}
#if MB_UCOSIII_ENABLED   
BOOL xMBSlaveTCPPortGetRequest(sMBSlavePort* psMBPort)
{
    struct timeval sSlavePortTv; 
    int sReadBytes = 0;
    UCHAR* pucRcvBuf = 0;
    uint32_t i = MB_SLAVE_PORT_TIMEOUT_S;
    
    sMBSlaveInfo* psMBSlaveInfo = psMBPort->psMBSlaveInfo;
    while(psMBPort->xSocketConnected == FALSE)
    {
        (void)vMBTimeDly(0, 500);
    }
    sSlavePortTv.tv_sec = i;
    sSlavePortTv.tv_usec = i % (1000*1000);

    psMBSlaveInfo->usRcvBufferPos = 0;
    pucRcvBuf = &psMBSlaveInfo->ucRcvBuf[psMBSlaveInfo->usRcvBufferPos];
    
    sReadBytes = recv(psMBPort->fd, pucRcvBuf, MB_SLAVE_RCV_MAX_BYTES, 0); 
    //debug("xMBSlaveTCPPortGetRequest sReadBytes %d \n", sReadBytes);
    if(sReadBytes <= 0)
    {
        psMBPort->xSocketConnected = FALSE;
        close(psMBPort->fd);
        debug("close sReadBytes %d \n", sReadBytes);
        xMBSlavePortEventPost(psMBPort, EV_ERROR_RCV);
        return FALSE;
    }
    psMBSlaveInfo->usRcvBufferPos += (USHORT)sReadBytes;
    xMBSlavePortEventPost(psMBPort, EV_FRAME_RECEIVED);
    
    return psMBSlaveInfo->usRcvBufferPos > 0 ? TRUE:FALSE;
}
#elif MB_LINUX_ENABLED     
BOOL xMBSlaveTCPPortGetRequest(sMBSlavePort* psMBPort)
{
    fd_set rfds;
    fd_set efds;
    struct timeval sSlavePortTv;
    
    int select_ret = 0;
    int sReadBytes = 0;
    UCHAR* pucRcvBuf = 0;
    uint32_t i = MB_SLAVE_PORT_TIMEOUT_S;
    sMBSlaveInfo* psMBSlaveInfo = psMBPort->psMBSlaveInfo;

    while(psMBPort->xSocketConnected == FALSE)
    {
        (void)vMBTimeDly(0, 500);
    }
    FD_ZERO(&rfds);
    FD_ZERO(&efds);
    FD_SET(psMBPort->fd, &rfds);
    FD_SET(psMBPort->fd, &efds);

    sSlavePortTv.tv_sec = i;
    sSlavePortTv.tv_usec = i % (1000*1000 );

    select_ret = select(psMBPort->fd+1, &rfds, NULL, &efds, &sSlavePortTv);
    if(select_ret > 0 && FD_ISSET(psMBPort->fd, &rfds))
    {    
        psMBSlaveInfo->usRcvBufferPos = 0;
        pucRcvBuf = &psMBSlaveInfo->ucRcvBuf[psMBSlaveInfo->usRcvBufferPos];
      
        sReadBytes = recv(psMBPort->fd, pucRcvBuf, MB_SLAVE_RCV_MAX_BYTES, 0); 
        if(sReadBytes <= 0)
        {
            psMBPort->xSocketConnected = FALSE;
            close(psMBPort->fd);
            xMBSlavePortEventPost(psMBPort, EV_ERROR_RCV);
            return FALSE;
        }
        psMBSlaveInfo->usRcvBufferPos += (USHORT)sReadBytes;
        xMBSlavePortEventPost(psMBPort, EV_FRAME_RECEIVED);
        psMBPort->iSocketOfflines = 0;
    }
    else if(select_ret < 0)
    {
        psMBPort->xSocketConnected = FALSE;
        close(psMBPort->fd);
        xMBSlavePortEventPost(psMBPort, EV_ERROR_RCV);
        return FALSE;
    }
    else
    { 
        if(psMBPort->xSocketConnected)
        {
            psMBPort->iSocketOfflines++;
        }
        if(psMBPort->iSocketOfflines >= MB_SLAVE_PORT_OFF_TIMES)
        {
            psMBPort->xSocketConnected = FALSE;
            close(psMBPort->fd);     
            psMBPort->iSocketOfflines = 0;
        }
        
        xMBSlavePortEventPost(psMBPort, EV_ERROR_RCV);
        return FALSE;
    }
    return psMBSlaveInfo->usRcvBufferPos > 0 ? TRUE:FALSE;
}
#endif 

BOOL xMBSlaveTCPPortSendResponse(sMBSlavePort* psMBPort, const UCHAR *pucMBTCPFrame, USHORT usTCPLength)
{
    int sSendBytes = send(psMBPort->fd, pucMBTCPFrame, usTCPLength, 0);

    //debug("xMBSlaveTCPPortSendResponse sSendBytes %d \n", sSendBytes);
    return sSendBytes == usTCPLength ?  TRUE : FALSE;
}

#endif
