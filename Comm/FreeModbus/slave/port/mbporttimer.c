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
 * File: $Id: porttimer.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */
#include "stddef.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#if MB_UCOSIII_ENABLED

#include "lpc_timer.h"
#endif

#if MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_ASCII_ENABLED > 0

#define MB_SLAVE_PORT_TIMEOUT_US   80    //协议规定50us  但需要根据实际情况调整

/* ----------------------- Start implementation -----------------------------*/
void vMBSlavePortTimersEnable(sMBSlavePort* psMBPort)
{
    sMBSlaveInfo* psMBSlaveInfo = psMBPort->psMBSlaveInfo;
#if MB_UCOSIII_ENABLED		
	OS_ERR err = OS_ERR_NONE;
    (void)OSTmrStart(&psMBPort->sSlavePortTmr, &err);

#elif MB_LINUX_ENABLED
    
	int select_ret;
    fd_set rfds;

    uint32_t i = psMBPort->usTim1Timerout50us * MB_SLAVE_PORT_TIMEOUT_US;

    psMBPort->sSlavePortTv.tv_sec = i / ( 1000*1000 );
    psMBPort->sSlavePortTv.tv_usec = i % (1000*1000 );

    FD_ZERO(&rfds);                                                
    FD_SET(psMBPort->fd, &rfds);

    select_ret = select(psMBPort->fd+1, &rfds, NULL, NULL, &psMBPort->sSlavePortTv);
	if (select_ret > 0)
	{              
        (void)psMBSlaveInfo->pxMBSlaveFrameCBByteReceivedCur(psMBPort->psMBSlaveInfo);
	}
	else
	{             
        psMBSlaveInfo->pxMBSlaveFrameCBTimerExpiredCur(psMBPort->psMBSlaveInfo);
	}
#endif	
}

void vMBSlavePortTimersDisable(sMBSlavePort* psMBPort)
{
#if MB_UCOSIII_ENABLED 	
	OS_ERR err = OS_ERR_NONE;
    (void)OSTmrStop(&psMBPort->sSlavePortTmr, OS_OPT_TMR_NONE, NULL, &err);
#endif	
}

void vSlaveTimeoutInd(void * p_tmr, void * p_arg)  //定时器中断服务函数
{
    sMBSlavePort*      psMBPort = (sMBSlavePort*)p_arg;
	sMBSlaveInfo* psMBSlaveInfo = psMBPort->psMBSlaveInfo;
	
	if(psMBSlaveInfo != NULL)
	{
        psMBSlaveInfo->pxMBSlaveFrameCBTimerExpiredCur(psMBSlaveInfo);
	} 
}

BOOL xMBSlavePortTimersInit(sMBSlavePort* psMBPort, USHORT usTim1Timerout50us)
{
#if MB_UCOSIII_ENABLED 
	OS_ERR err = OS_ERR_NONE;
    OS_TICK  i = (usTim1Timerout50us * MB_SLAVE_PORT_TIMEOUT_US) / (1000000 / TMR_TICK_PER_SECOND);   //50us太快，改为80us

    OSTmrCreate(&psMBPort->sSlavePortTmr, "sSlavePortTmr", i, 0, OS_OPT_TMR_ONE_SHOT, vSlaveTimeoutInd, (void*)psMBPort, &err);
    
    return (err == OS_ERR_NONE);

#elif MB_LINUX_ENABLED   
    uint32_t i = usTim1Timerout50us * MB_SLAVE_PORT_TIMEOUT_US;
    psMBPort->usTim1Timerout50us = usTim1Timerout50us;
                  
    psMBPort->sSlavePortTv.tv_sec = i / ( 1000*1000 );
    psMBPort->sSlavePortTv.tv_usec = i % (1000*1000 );

    return TRUE;
#endif
}

#endif
