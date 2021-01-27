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
 * File: $Id: portevent_m.c v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Modbus includes ----------------------------------*/

#include "mbconfig.h"
#include "mb_m.h"
#include "mbport_m.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0

/* ----------------------- Start implementation -----------------------------*/

/**********************************************************************
 * @brief  modbus协议栈事件初始化函数
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBMasterPortEventInit(sMBMasterPort* psMBPort)
{
#if  MB_UCOSIII_ENABLED
    OS_ERR err = OS_ERR_NONE;
	
    OSSemCreate(&psMBPort->sMBIdleSem, "sMBIdleSem", 0, &err);             //主栈空闲消息量
	OSSemCreate(&psMBPort->sMBEventSem, "sMBEventSem", 0, &err);           //主栈事件消息量
    OSSemCreate(&psMBPort->sMBWaitFinishSem, "sMBWaitFinishSem", 0, &err); //主栈错误消息量

    psMBPort->xEventInQueue = FALSE;
	psMBPort->xWaitFinishInQueue = FALSE;

    return (err == OS_ERR_NONE);

#elif MB_LINUX_ENABLED
    int ret = 0;

    ret = sem_init(&psMBPort->sMBIdleSem, 0, 0);      //主栈空闲消息量
	ret = sem_init(&psMBPort->sMBEventSem, 0, 0);      //主栈事件消息量
    ret = sem_init(&psMBPort->sMBWaitFinishSem, 0, 0); //主栈错误消息

    psMBPort->xEventInQueue = FALSE;
	psMBPort->xWaitFinishInQueue = FALSE;

    return (ret >= 0);
#endif
}

/**********************************************************************
 * @brief  modbus主栈事件发送
 * @param  eEvent  当前事件
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBMasterPortEventPost(sMBMasterPort* psMBPort, eMBMasterEventType eEvent)
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
 * @brief  获取modbus主栈最新的事件
 * @param  eEvent  当前事件
 * @return BOOL   
 * @author laoc
 * @date 2019.01.22
 *********************************************************************/
BOOL xMBMasterPortEventGet(sMBMasterPort* psMBPort, eMBMasterEventType* eEvent)
{
    BOOL xEventHappened = FALSE;

#if  MB_UCOSIII_ENABLED
	CPU_TS ts  = 0;
    OS_ERR err = OS_ERR_NONE;
    
    (void)OSSemPend(&psMBPort->sMBEventSem, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	(void)OSSemSet(&psMBPort->sMBEventSem, 0, &err);

#elif MB_LINUX_ENABLED

    sem_wait(&psMBPort->sMBEventSem);
#endif
    if(psMBPort->xEventInQueue)
    {
		switch(psMBPort->eQueuedEvent)
		{
		case EV_MASTER_READY:
			*eEvent = EV_MASTER_READY;
			break;
		case EV_MASTER_FRAME_RECEIVED:
			*eEvent = EV_MASTER_FRAME_RECEIVED;
			break;
		case EV_MASTER_EXECUTE:
			*eEvent = EV_MASTER_EXECUTE;
			break;
		case EV_MASTER_FRAME_SENT:
			*eEvent = EV_MASTER_FRAME_SENT;
			break;
		case EV_MASTER_ERROR_PROCESS:
			*eEvent = EV_MASTER_ERROR_PROCESS;
			break;
		case EV_MASTER_ERROR_RECEIVE_DATA:
			*eEvent = EV_MASTER_ERROR_RECEIVE_DATA;
			break;
		case EV_MASTER_ERROR_RESPOND_DATA:
			*eEvent = EV_MASTER_ERROR_RESPOND_DATA;
			break;
		default: break;
		}
        psMBPort->xEventInQueue = FALSE;
        xEventHappened = TRUE;
    }
    return xEventHappened;
}
/**
 * This function is initialize the OS resource for modbus master.
 * Note:The resource is define by OS.If you not use OS this function can be empty.
 *
 */
void vMBMasterOsResInit( void )
{
   
}

/**
 * This function is take Mobus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be just return TRUE.
 *
 * @param lTimeOut the waiting time.
 *
 * @return resource taked result
 */
BOOL xMBMasterRunResTake( LONG lTimeOut )
{
    /*If waiting time is 0 .It will wait forever */
//	usTimeOut = lTimeOut;
    return  TRUE ;
}

/**
 * This function is release Mobus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be empty.
 *
 */
void vMBMasterRunResRelease( void )
{
    /* release resource */ 
}

/**
 * This is modbus master respond timeout error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddr destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBRespondTimeout(sMBMasterPort* psMBPort, UCHAR ucDestAddr, 
	                                const UCHAR* pucPDUData, USHORT ucPDULength) 
{
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
#if  MB_UCOSIII_ENABLED
	 OS_ERR err = OS_ERR_NONE;	
    
    (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_RESPOND_TIMEOUT);
	psMBPort->xWaitFinishInQueue = TRUE;

	(void)OSSemPost(&psMBPort->sMBWaitFinishSem, OS_OPT_POST_ALL, &err);		
    /* You can add your code under here. */

#elif MB_LINUX_ENABLED

    (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_RESPOND_TIMEOUT);
    psMBPort->xWaitFinishInQueue = TRUE;

    sem_post(&psMBPort->sMBWaitFinishSem);
#endif
}

/**
 * This is modbus master receive data error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddr destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBReceiveData(sMBMasterPort* psMBPort, UCHAR ucDestAddr, 
	                             const UCHAR* pucPDUData, USHORT ucPDULength) 
{
#if  MB_UCOSIII_ENABLED
	/**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    OS_ERR err = OS_ERR_NONE;
    
    (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_RECEIVE_DATA);
	psMBPort->xWaitFinishInQueue = TRUE;
    
    (void)OSSemPost(&psMBPort->sMBWaitFinishSem, OS_OPT_POST_ALL, &err);

#elif MB_LINUX_ENABLED

    (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_RECEIVE_DATA);
    psMBPort->xWaitFinishInQueue = TRUE;

    sem_post(&psMBPort->sMBWaitFinishSem);
#endif
}
		
void vMBMasterErrorCBRespondData( sMBMasterPort* psMBPort, UCHAR ucDestAddr, 
                                  const UCHAR* pucPDUData, USHORT ucPDULength ) 
{    
    (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_RESPOND_DATA);
    vMBsMasterPortTmrsRespondTimeoutEnable(psMBPort);
}

/**
 * This is modbus master execute function error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddr destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBExecuteFunction( sMBMasterPort* psMBPort, UCHAR ucDestAddr, 
                                      const UCHAR* pucPDUData, USHORT ucPDULength ) 
{
#if  MB_UCOSIII_ENABLED
	/**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    OS_ERR err = OS_ERR_NONE;
    
    (void)xMBMasterPortEventPost( psMBPort, EV_MASTER_ERROR_EXECUTE_FUNCTION );
	psMBPort->xWaitFinishInQueue = TRUE;	
	
    (void)OSSemPost(&psMBPort->sMBWaitFinishSem, OS_OPT_POST_ALL, &err );

#elif MB_LINUX_ENABLED

    (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_EXECUTE_FUNCTION);
    psMBPort->xWaitFinishInQueue = TRUE;
    
    sem_post(&psMBPort->sMBWaitFinishSem);
#endif
}

/**
 * This is modbus master request process success callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 */
void vMBMasterCBRequestSuccess(sMBMasterPort* psMBPort) 
{
#if  MB_UCOSIII_ENABLED
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    OS_ERR err = OS_ERR_NONE;
    
    (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_PROCESS_SUCCESS);
    psMBPort->xWaitFinishInQueue = TRUE;
    
    (void)OSSemPost(&psMBPort->sMBWaitFinishSem, OS_OPT_POST_ALL, &err);

#elif MB_LINUX_ENABLED

    (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_PROCESS_SUCCESS);
    psMBPort->xWaitFinishInQueue = TRUE;
    
    sem_post(&psMBPort->sMBWaitFinishSem);
#endif
}

/**
 * This function is wait for modbus master request finish and return result.
 * Waiting result include request process success, request respond timeout,
 * receive data error and execute function error.You can use the above callback function.
 * @note If you are use OS, you can use OS's event mechanism. Otherwise you have to run
 * much user custom delay for waiting.
 *
 * @return request error code
 */
eMBMasterReqErrCode eMBMasterWaitRequestFinish(sMBMasterPort* psMBPort) 
{
    eMBMasterReqErrCode  eErrStatus = MB_MRE_NO_ERR;

#if  MB_UCOSIII_ENABLED
    CPU_TS ts = 0;
    OS_ERR err = OS_ERR_NONE;
    
    (void)OSSemPend(&psMBPort->sMBWaitFinishSem, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	(void)OSSemSet(&psMBPort->sMBWaitFinishSem, 0, &err);
#elif MB_LINUX_ENABLED
    sem_wait(&psMBPort->sMBWaitFinishSem);
#endif
    
	if(psMBPort->xWaitFinishInQueue)
    {
        switch(psMBPort->eQueuedEvent)
        {
        case EV_MASTER_PROCESS_SUCCESS:
        	break;
        case EV_MASTER_ERROR_RESPOND_TIMEOUT:
        {
//            myprintf(" EV_MASTER_ERROR_RESPOND_TIMEOUT \n"); 
        	eErrStatus = MB_MRE_TIMEDOUT;
        	break;
        }
        case EV_MASTER_ERROR_RECEIVE_DATA:
        {
//            myprintf(" EV_MASTER_ERROR_RECEIVE_DATA \n");
        	eErrStatus = MB_MRE_REV_DATA;
        	break;
        }
        case EV_MASTER_ERROR_EXECUTE_FUNCTION:
        {
//            myprintf(" EV_MASTER_ERROR_EXECUTE_FUNCTION \n");
        	eErrStatus = MB_MRE_EXE_FUN;
        	break;
        }	
        default:	
        	break;
		}
	}
	psMBPort->xWaitFinishInQueue = FALSE;

#if  MB_UCOSIII_ENABLED

    (void)OSSemPost(&psMBPort->sMBIdleSem, OS_OPT_POST_ALL, &err);
#elif MB_LINUX_ENABLED

    sem_post(&psMBPort->sMBIdleSem);
#endif
    return eErrStatus;
}

/**********************************************************************
 * @brief  返回modbus主栈最新的事件
 *********************************************************************/
eMBMasterEventType xMBMasterPortCurrentEvent( const sMBMasterPort* psMBPort )
{
	return psMBPort->eQueuedEvent;       	
}

/***********************************************************************************
 * @brief  错误代码转异常码
 * @param  eMBErrorCode  mb错误代码
 * @return eMBException  异常码
 * @author laoc
 * @date 2019.01.22
 *************************************************************************************/
eMBException prveMBMasterError2Exception( eMBErrorCode eErrorCode )
{
    eMBException    eStatus;

    switch (eErrorCode)
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

/**********************************************************************
 * @brief modbus主栈互锁
 *********************************************************************/
void vMBMasterPortLock(sMBMasterPort* psMBPort)
{
#if  MB_UCOSIII_ENABLED
    OS_ERR err = OS_ERR_NONE;
    
	(void)OSSemPend(&psMBPort->sMBIdleSem, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    (void)OSSemSet(&psMBPort->sMBIdleSem, 0, &err); 
#elif MB_LINUX_ENABLED

    sem_wait(&psMBPort->sMBIdleSem);
#endif   	
}
 
/**********************************************************************
 * @brief modbus主栈释放锁
 *********************************************************************/
void vMBMasterPortUnLock(sMBMasterPort* psMBPort)
{
#if  MB_UCOSIII_ENABLED
    OS_ERR err = OS_ERR_NONE;
	(void)OSSemPost(&psMBPort->sMBIdleSem, OS_OPT_POST_ALL, &err);

#elif MB_LINUX_ENABLED

    sem_post(&psMBPort->sMBIdleSem);
#endif	
}

#endif
