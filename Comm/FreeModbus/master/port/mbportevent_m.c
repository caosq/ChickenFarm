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
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>

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

    ret = pthread_mutex_init(&psMBPort->mutex, NULL);//初始化互斥量


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

//    debug("xMBMasterPortEventPost\n");
    return ret >= 0;
#endif
}

// 时间 time 自加 ms 毫秒
void time_add_ms(struct timeval *time, uint ms)
{
    time->tv_usec += ms * 1000; // 微秒 = 毫秒 * 1000
    if(time->tv_usec >= 1000000) // 进位，1000 000 微秒 = 1 秒
    {
        time->tv_sec += time->tv_usec / 1000000;
        time->tv_usec %= 1000000;
    }
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

    //if(psMBPort->xEventInQueue)
    //{
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
		default: break;
		}
        psMBPort->xEventInQueue = FALSE;
        xEventHappened = TRUE;
//        debug("xMBMasterPortEventGet %d\n", *eEvent);
    //}
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
BOOL xMBMasterRunResTake(sMBMasterPort* psMBPort, ULONG lTimeOutMs)
{
//    struct timeval time;

//    clock_gettime(&time, NULL);
 //   time_add_ms(&time, 200);

//    psMBPort->sMasterWaitFinishTv.tv_sec = time.tv_sec;
//    psMBPort->sMasterWaitFinishTv.tv_usec = time.tv_usec;


#if  MB_UCOSIII_ENABLED
    OS_ERR err = OS_ERR_NONE;

    (void)OSSemPend(&psMBPort->sMBIdleSem, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    (void)OSSemSet(&psMBPort->sMBIdleSem, 0, &err);
#elif MB_LINUX_ENABLED

    pthread_mutex_lock(&psMBPort->mutex);//加锁 若有线程获得锁，则会阻塞

   // tcflush(psMBPort->psMBMasterUart->fd, TCIOFLUSH);

    //psMBPort->psMBMasterInfo->usRcvBufferPos = read(psMBPort->psMBMasterUart->fd, psMBPort->psMBMasterInfo->ucRTURcvBuf, 255);

    debug(" xMBMasterRunResTake %d\n", psMBPort->psMBMasterInfo->usRcvBufferPos);


    //vMBTimeDly(0, 200);

    psMBPort->xMBIsFinished = FALSE;

    //int ret = sem_timedwait(&psMBPort->sMBIdleSem, &psMBPort->sMasterWaitFinishTv);
   // sem_wait(&psMBPort->sMBIdleSem);

    return TRUE;
#endif
}

/**
 * This function is release Mobus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be empty.
 *
 */
void vMBMasterRunResRelease(sMBMasterPort* psMBPort)
{
    /* release resource */ 

#if  MB_UCOSIII_ENABLED

    (void)OSSemPost(&psMBPort->sMBIdleSem, OS_OPT_POST_ALL, &err);
#elif MB_LINUX_ENABLED

    //sem_post(&psMBPort->sMBIdleSem);

    pthread_mutex_unlock(&psMBPort->mutex);//解锁

#endif
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

   // (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_RESPOND_TIMEOUT);
    psMBPort->eQueuedEvent = EV_MASTER_ERROR_RESPOND_TIMEOUT;
    psMBPort->xWaitFinishInQueue = TRUE;
    psMBPort->xMBIsFinished = TRUE;

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

  //  (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_RECEIVE_DATA);
    psMBPort->eQueuedEvent = EV_MASTER_ERROR_RECEIVE_DATA;
    psMBPort->xWaitFinishInQueue = TRUE;
    psMBPort->xMBIsFinished = TRUE;

    sem_post(&psMBPort->sMBWaitFinishSem);

#endif
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

    //(void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_EXECUTE_FUNCTION);
    psMBPort->eQueuedEvent = EV_MASTER_ERROR_EXECUTE_FUNCTION;
    psMBPort->xWaitFinishInQueue = TRUE;
    psMBPort->xMBIsFinished = TRUE;

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

   // (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_PROCESS_SUCCESS);

    psMBPort->eQueuedEvent = EV_MASTER_PROCESS_SUCCESS;
    psMBPort->xWaitFinishInQueue = TRUE;
    psMBPort->xMBIsFinished = TRUE;
    
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
    uint8_t ucDlyCount = 0;


#if  MB_UCOSIII_ENABLED
    CPU_TS ts = 0;
    OS_ERR err = OS_ERR_NONE;
    
    (void)OSSemPend(&psMBPort->sMBWaitFinishSem, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	(void)OSSemSet(&psMBPort->sMBWaitFinishSem, 0, &err);

#elif MB_LINUX_ENABLED
    //int ret = sem_timedwait(&psMBPort->sMBWaitFinishSem, &psMBPort->sMasterWaitFinishTv);

    while(psMBPort->xMBIsFinished == FALSE && ucDlyCount < 10)
    {
        (void)vMBTimeDly(0, 400);
        ucDlyCount++;
    }

    if(ucDlyCount == 10)
    {
        psMBPort->xMBIsFinished = TRUE;
        //sem_post(&psMBPort->sMBIdleSem);

        sem_post(&psMBPort->sMBEventSem);

        //read(psMBPort->psMBMasterUart->fd, psMBPort->psMBMasterInfo->ucRTURcvBuf, 255);

        pthread_mutex_unlock(&psMBPort->mutex);//解锁


        return MB_MRE_TIMEDOUT;
    }
    //sem_wait(&psMBPort->sMBWaitFinishSem);

#endif
    
/*    if(ret == -1)
    {
        psMBPort->xWaitFinishInQueue = FALSE;
       //psMBPort->eQueuedEvent = EV_MASTER_ERROR_RESPOND_TIMEOUT;
        eErrStatus = MB_MRE_TIMEDOUT;
#if  MB_UCOSIII_ENABLED

        (void)OSSemPost(&psMBPort->sMBIdleSem, OS_OPT_POST_ALL, &err);
#elif MB_LINUX_ENABLED

        sem_post(&psMBPort->sMBIdleSem);
 //       sem_post(&psMBPort->sMBEventSem);
        return eErrStatus;
    #endif

    //    (void)xMBMasterPortEventPost(psMBPort, EV_MASTER_ERROR_RESPOND_TIMEOUT);
    }*/

    //if(psMBPort->xWaitFinishInQueue)
    //{
        switch(psMBPort->eQueuedEvent)
        {
        case EV_MASTER_PROCESS_SUCCESS:
        	break;
        case EV_MASTER_ERROR_RESPOND_TIMEOUT:
        {
            debug(" EV_MASTER_ERROR_RESPOND_TIMEOUT \n");
        	eErrStatus = MB_MRE_TIMEDOUT;

            //read(psMBPort->psMBMasterUart->fd, psMBPort->psMBMasterInfo->ucRTURcvBuf, 255);
        	break;
        }
        case EV_MASTER_ERROR_RECEIVE_DATA:
        {
            debug(" EV_MASTER_ERROR_RECEIVE_DATA \n");
        	eErrStatus = MB_MRE_REV_DATA;
        	break;
        }
        case EV_MASTER_ERROR_EXECUTE_FUNCTION:
        {
            debug(" EV_MASTER_ERROR_EXECUTE_FUNCTION \n");
        	eErrStatus = MB_MRE_EXE_FUN;


           // read(psMBPort->psMBMasterUart->fd, psMBPort->psMBMasterInfo->ucRTURcvBuf, 255);


        	break;
        }	
        default:	
        	break;
		}
    //}
	psMBPort->xWaitFinishInQueue = FALSE;


    //tcflush(psMBPort->psMBMasterUart->fd, TCIOFLUSH);

    read(psMBPort->psMBMasterUart->fd, psMBPort->psMBMasterInfo->ucRTURcvBuf, 255);

    debug(" eMBMasterWaitRequestFinish %d\n" ,psMBPort->eQueuedEvent);

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
