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
 * File: $Id: portevent.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#define TIME_TICK_OUT 0

/* ----------------------- Start implementation -----------------------------*/

/**********************************************************************
 * @brief  modbus协议栈事件初始化函数
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBSlavePortEventInit(sMBSlavePort* psMBPort)
{
#if  MB_UCOSIII_ENABLED    
    OS_ERR err = OS_ERR_NONE;
    
    psMBPort->xEventInQueue = FALSE;
    OSSemCreate(&psMBPort->sMBEventSem, "sMBEventSem", 0, &err);      //从栈事件消息量
    
    return err == OS_ERR_NONE;
#elif MB_LINUX_ENABLED
    int ret = 0;
    psMBPort->xEventInQueue = FALSE;

	ret = sem_init(&psMBPort->sMBEventSem, 0, 0);      //从栈事件消息量
    return ret >= 0;
#endif
}

/**********************************************************************
 * @brief  modbus从栈事件发送
 * @param  eEvent  当前事件
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBSlavePortEventPost(sMBSlavePort* psMBPort, eMBSlaveEventType eEvent)
{
#if MB_UCOSIII_ENABLED
	OS_ERR err = OS_ERR_NONE;
    psMBPort->xEventInQueue = TRUE;
    psMBPort->eQueuedEvent = eEvent;
	
    (void)OSSemPost(&psMBPort->sMBEventSem, OS_OPT_POST_ALL, &err);
    
    return (err == OS_ERR_NONE);
#elif MB_LINUX_ENABLED
    int ret = 0;
    psMBPort->xEventInQueue = TRUE;
    psMBPort->eQueuedEvent = eEvent;

    ret = sem_post(&psMBPort->sMBEventSem);
    return ret >= 0;
#endif    
}

/**********************************************************************
 * @brief  获取modbus从栈最新的事件
 * @param  eEvent  当前事件
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBSlavePortEventGet(sMBSlavePort* psMBPort, eMBSlaveEventType* eEvent)
{
    BOOL xEventHappened = FALSE;
#if  MB_UCOSIII_ENABLED
	CPU_TS ts = 0;
    OS_ERR err = OS_ERR_NONE;
	
	(void)OSSemPend(&psMBPort->sMBEventSem, TIME_TICK_OUT, OS_OPT_PEND_BLOCKING, &ts, &err);    
	(void)OSSemSet(&psMBPort->sMBEventSem, 0, &err);

 #elif MB_LINUX_ENABLED
    sem_wait(&psMBPort->sMBEventSem);
#endif
    if( psMBPort->xEventInQueue )
    {
        *eEvent = psMBPort->eQueuedEvent;
        psMBPort->xEventInQueue = FALSE;
        xEventHappened = TRUE;	
    }
    return xEventHappened;
}

/***********************************************************************************
 * @brief  错误代码转异常码
 * @param  eMBErrorCode  mb错误代码
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException prveMBSlaveError2Exception(eMBErrorCode eErrorCode)
{
    eMBException    eStatus;

    switch(eErrorCode)
    {
        case MB_ENOERR:
            eStatus = MB_EX_NONE;
            break;

        case MB_ENOREG:
            eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;
            break;

		case MB_EINVAL:
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
            break;
		
        case MB_ETIMEDOUT:
            eStatus = MB_EX_SLAVE_BUSY;
            break;

        default:
            eStatus = MB_EX_SLAVE_DEVICE_FAILURE;
            break;
    }
    return eStatus;
}

