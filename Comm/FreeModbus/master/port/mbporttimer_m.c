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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb_m.h"
#include "mbrtu_m.h"
#include "mbconfig.h"
#include "mbport_m.h"

#if MB_UCOSIII_ENABLED

#include "lpc_timer.h"

#endif

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0

void vMasterTimeoutInd(void * p_tmr, void * p_arg)
{
    sMBMasterPort*   psMBPort = (sMBMasterPort*)p_arg;
	sMBMasterInfo*   psMBMasterInfo = psMBPort->psMBMasterInfo;
	
	if(psMBMasterInfo != NULL)
	{
		pxMBMasterFrameCBTimerExpiredCur(psMBMasterInfo);
	} 
}

void vMBsMasterPortTmrsEnable(sMBMasterPort* psMBPort)
{
#if MB_UCOSIII_ENABLED
    OS_ERR err = OS_ERR_NONE;
    vMBMasterSetCurTimerMode(psMBPort, MB_TMODE_T35);
	
    (void)OSTmrStart(&psMBPort->sMasterPortTmr, &err);
	
	if( OSTmrStateGet(&psMBPort->sConvertDelayTmr, &err) == OS_TMR_STATE_RUNNING)
	{
	    (void)OSTmrStop(&psMBPort->sConvertDelayTmr, OS_OPT_TMR_NONE, NULL, &err);
	}
	if( OSTmrStateGet(&psMBPort->sRespondTimeoutTmr, &err) == OS_TMR_STATE_RUNNING)
	{
		(void)OSTmrStop(&psMBPort->sRespondTimeoutTmr, OS_OPT_TMR_NONE, NULL, &err);
	}
#elif MB_LINUX_ENABLED
    int select_ret;
    fd_set rfds;      // 读/写文件描述符集

    FD_ZERO(&rfds);                                                
    FD_SET(psMBPort->psMBMasterUart->fd, &rfds);

    select_ret = select(psMBPort->psMBMasterUart->fd, &rfds, NULL, NULL, &psMBPort->sMasterPortTv);

	if (select_ret > 0)
	{              
		(void)pxMBMasterFrameCBByteReceivedCur(psMBPort->psMBMasterInfo);
	}
	else
	{
        pxMBMasterFrameCBTimerExpiredCur(psMBPort->psMBMasterInfo);
	}
#endif
}

void vMBsMasterPortTmrsConvertDelayEnable(sMBMasterPort* psMBPort)
{
#if MB_UCOSIII_ENABLED

	OS_ERR err = OS_ERR_NONE;
	vMBMasterSetCurTimerMode(psMBPort, MB_TMODE_CONVERT_DELAY);

    (void)OSTmrStart(&psMBPort->sConvertDelayTmr, &err);
	
	if( OSTmrStateGet(&psMBPort->sMasterPortTmr, &err) == OS_TMR_STATE_RUNNING)
	{
        (void)OSTmrStop(&psMBPort->sMasterPortTmr, OS_OPT_TMR_NONE, NULL, &err);
	}
	if( OSTmrStateGet(&psMBPort->sRespondTimeoutTmr, &err) == OS_TMR_STATE_RUNNING)
	{
		(void)OSTmrStop(&psMBPort->sRespondTimeoutTmr, OS_OPT_TMR_NONE, NULL, &err);
	} 
#elif MB_LINUX_ENABLED
    
    int select_ret;
    fd_set rfds;      // 读/写文件描述符集

    FD_ZERO(&rfds);                                                
    FD_SET(psMBPort->psMBMasterUart->fd, &rfds);

    select_ret = select(psMBPort->psMBMasterUart->fd, &rfds, NULL, NULL, &psMBPort->sConvertDelayTv);
	if (select_ret == 0)
	{             
		pxMBMasterFrameCBTimerExpiredCur(psMBPort->psMBMasterInfo);
	}
#endif
}

void vMBsMasterPortTmrsRespondTimeoutEnable(sMBMasterPort* psMBPort)	
{
#if MB_UCOSIII_ENABLED
	OS_ERR err = OS_ERR_NONE;
	 vMBMasterSetCurTimerMode(psMBPort, MB_TMODE_RESPOND_TIMEOUT);
 
	(void)OSTmrStart(&psMBPort->sRespondTimeoutTmr, &err);
	
	if( OSTmrStateGet(&psMBPort->sMasterPortTmr, &err) == OS_TMR_STATE_RUNNING)
	{
		 (void)OSTmrStop(&psMBPort->sMasterPortTmr, OS_OPT_TMR_NONE, NULL, &err);
	}
	if( OSTmrStateGet(&psMBPort->sConvertDelayTmr, &err) == OS_TMR_STATE_RUNNING)
	{
		(void)OSTmrStop(&psMBPort->sConvertDelayTmr, OS_OPT_TMR_NONE, NULL, &err);
	}
#elif MB_LINUX_ENABLED
    
	int select_ret;
    fd_set rfds;      // 读/写文件描述符集

    FD_ZERO(&rfds);                                                
    FD_SET(psMBPort->psMBMasterUart->fd, &rfds);

    select_ret = select(psMBPort->psMBMasterUart->fd, &rfds, NULL, NULL, &psMBPort->sConvertDelayTv);
	if (select_ret > 0)
	{              
		(void)pxMBMasterFrameCBByteReceivedCur(psMBPort->psMBMasterInfo);
	}
	else
	{
        pxMBMasterFrameCBTimerExpiredCur(psMBPort->psMBMasterInfo);
	}
#endif	
}


void vMBsMasterPortTmrsDisable(sMBMasterPort* psMBPort)
{
#if MB_UCOSIII_ENABLED
	OS_ERR err = OS_ERR_NONE;
    if( OSTmrStateGet(&psMBPort->sMasterPortTmr, &err) == OS_TMR_STATE_RUNNING)
	{
		 (void)OSTmrStop(&psMBPort->sMasterPortTmr, OS_OPT_TMR_NONE, NULL, &err);
	}
	if( OSTmrStateGet(&psMBPort->sConvertDelayTmr, &err) == OS_TMR_STATE_RUNNING)
	{
		(void)OSTmrStop(&psMBPort->sConvertDelayTmr, OS_OPT_TMR_NONE, NULL, &err);
	} 
	if( OSTmrStateGet(&psMBPort->sRespondTimeoutTmr, &err) == OS_TMR_STATE_RUNNING)
	{
		(void)OSTmrStop(&psMBPort->sRespondTimeoutTmr, OS_OPT_TMR_NONE, NULL, &err);
    }
#endif
}


/* Set Modbus Master current timer mode.*/
void vMBMasterSetCurTimerMode(sMBMasterPort* psMBPort, eMBMasterTimerMode eMBTimerMode)
{
	psMBPort->eCurTimerMode = eMBTimerMode;
}

BOOL xMBsMasterPortTmrsInit(sMBMasterPort* psMBPort, USHORT usTim1Timerout50us)
{
#if MB_UCOSIII_ENABLED    
	OS_ERR err = OS_ERR_NONE;
	OS_TICK i = (OS_TICK)( (usTim1Timerout50us*80) / (1000000/TMR_TICK_PER_SECOND) );
	
	OSTmrCreate(&psMBPort->sMasterPortTmr,       //主定时器
			    "sMasterPortTmr",
			    i,      
			    0,
			    OS_OPT_TMR_ONE_SHOT,
			    vMasterTimeoutInd,
			    (void*)psMBPort,
			    &err);           
	if(err != OS_ERR_NONE){return FALSE;}
    
	i = MB_MASTER_DELAY_MS_CONVERT * TMR_TICK_PER_SECOND / 1000  ; 
	OSTmrCreate( &(psMBPort->sConvertDelayTmr),
			      "sConvertDelayTmr",
			      i ,
			      0,
			      OS_OPT_TMR_ONE_SHOT,
			      vMasterTimeoutInd,
			      (void*)psMBPort,
			      &err);
	if(err != OS_ERR_NONE){return FALSE;}
	
	i = MB_MASTER_TIMEOUT_MS_RESPOND * TMR_TICK_PER_SECOND / 1000  ;    //主栈等待从栈响应定时器
	OSTmrCreate( &(psMBPort->sRespondTimeoutTmr),
			      "sRespondTimeoutTmr",
			      i,
			      0,
			      OS_OPT_TMR_ONE_SHOT,
			      vMasterTimeoutInd,
			      (void*)psMBPort,
			      &err);
    return err == OS_ERR_NONE ? TRUE:FALSE; 
    
#elif MB_LINUX_ENABLED   

    int i = usTim1Timerout50us * 50;
                  
    psMBPort->sMasterPortTv.tv_sec = i / ( 1000*1000 );
    psMBPort->sMasterPortTv.tv_usec = i % (1000*1000 );
                  
    i = MB_MASTER_DELAY_MS_CONVERT * 1000;              
    psMBPort->sConvertDelayTv.tv_sec = i / ( 1000*1000 );
    psMBPort->sConvertDelayTv.tv_usec = i % (1000*1000 ); 

	i = MB_MASTER_TIMEOUT_MS_RESPOND * 1000;    //主栈等待从栈响应定时器             
    psMBPort->sRespondTimeoutTv.tv_sec = i / ( 1000*1000 );
    psMBPort->sRespondTimeoutTv.tv_usec = i % (1000*1000 );

    return TRUE;
#endif    
}

#endif
