#ifndef _MB_PORT_M_H
#define _MB_PORT_M_H

#include "port.h"
#include "mbproto.h"
#include "mbconfig.h"
#include "mbdriver.h"
#include "mbdict_m.h"

#if MB_UCOSIII_ENABLED

#elif MB_LINUX_ENABLED
#include <semaphore.h>
#include <termios.h>
#include <netinet/in.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! \ingroup modbus
 *  \brief TimerMode is Master 3 kind of Timer modes.
 */
typedef enum
{
	MB_TMODE_T35,                   /*!< Master receive frame T3.5 timeout. */
	MB_TMODE_RESPOND_TIMEOUT,       /*!< Master wait respond for slave. */
	MB_TMODE_CONVERT_DELAY          /*!< Master sent broadcast ,then delay sometime.*/
}eMBMasterTimerMode;

typedef enum
{
    EV_MASTER_READY                    = 1<<0,  /*!< Startup finished. */
    EV_MASTER_FRAME_RECEIVED           = 1<<1,  /*!< Frame received. */
    EV_MASTER_EXECUTE                  = 1<<2,  /*!< Execute function. */
    EV_MASTER_FRAME_SENT               = 1<<3,  /*!< Frame sent. */
    EV_MASTER_ERROR_PROCESS            = 1<<4,  /*!< Frame error process. */
    EV_MASTER_PROCESS_SUCCESS          = 1<<5,  /*!< Request process success. */
    EV_MASTER_ERROR_RESPOND_TIMEOUT    = 1<<6,  /*!< Request respond timeout. */
    EV_MASTER_ERROR_RECEIVE_DATA       = 1<<7,  /*!< Request receive data error. */
    EV_MASTER_ERROR_EXECUTE_FUNCTION   = 1<<8,  /*!< Request execute function error. */
} eMBMasterEventType;

typedef enum
{
    EV_ERROR_RESPOND_TIMEOUT,         /*!< Slave respond timeout. */
    EV_ERROR_RECEIVE_DATA,            /*!< Receive frame data error. */
    EV_ERROR_EXECUTE_FUNCTION,        /*!< Execute function error. */
} eMBMasterErrorEventType;

typedef struct                        /* 主栈接口定义  */
{
    eMBMasterEventType  eQueuedEvent; //主栈接口事件
    eMBMasterTimerMode  eCurTimerMode;//当前接口定时器模式
    sMBSlaveDev* psMBSlaveDevCur;     //接口当前轮询的从设备
     
    int    fd;                        //接口对应文件号
    const  CHAR* pcMBPortName;        //主栈接口名称
    BOOL   xEventInQueue;             //主栈接口有新事件
    BOOL   xWaitFinishInQueue;        //主栈接口有新错误事件
    BOOL   xMBIsFinished;             //主栈接口处理完成 
   
    uint16_t usTim1Timerout50us;
    
#if MB_MASTER_RTU_ENABLED && MB_UCOSIII_ENABLED 
    OS_TMR sConvertDelayTmr;          //主栈接口转换延时定时器
    OS_TMR sRespondTimeoutTmr;        //主栈接口等待响应定时器
    OS_TMR sMasterPortTmr;            //主栈接口3.5字符间隔定时器
#endif
    
#if MB_MASTER_TCP_ENABLED || MB_LINUX_ENABLED
    struct timeval sMasterPortTv; 
    struct timeval sRespondTimeoutTv;
#endif

#if MB_MASTER_RTU_ENABLED && MB_LINUX_ENABLED
    struct timeval sConvertDelayTv;
#endif

#if MB_UCOSIII_ENABLED
    OS_MUTEX sMBIdleMutex;            //主栈接口空闲消息量
    OS_SEM sMBEventSem;               //主栈接口事件消息量
    OS_SEM sMBWaitFinishSem;          //主栈接口等待消息量
#elif MB_LINUX_ENABLED
    pthread_mutex_t mutex;
    sem_t sMBEventSem;                //主栈接口事件消息量
    sem_t sMBWaitFinishSem;           //主栈接口等待消息量
#endif

#if MB_MASTER_RTU_ENABLED || MB_MASTER_ASCII_ENABLED
    sUART_Def* psMBMasterUart;   //主栈接口通讯串口结构
#endif
    struct sMBMasterInfo* psMBMasterInfo;  //所属的主栈  
}sMBMasterPort;

/* -----------------------Master Serial port functions ----------------------------*/
BOOL xMBMasterPortSerialInit(sMBMasterPort* psMBPort);

void vMBMasterPortClose(sMBMasterPort* psMBPort);
void xMBMasterPortSerialClose(sMBMasterPort* psMBPort);
void vMBMasterPortSerialEnable(sMBMasterPort* psMBPort, BOOL xRxEnable, BOOL xTxEnable);

INLINE BOOL xMBMasterPortSerialGetByte(const sMBMasterPort* psMBPort, UCHAR* pucByte);
INLINE BOOL xMBMasterPortSerialPutByte(sMBMasterPort* psMBPort, UCHAR ucByte);
INLINE BOOL xMBMasterPortSerialWriteBytes(sMBMasterPort* psMBPort, UCHAR* pucSndBufferCur, USHORT usBytes);
INLINE BOOL xMBMasterPortSerialReadBytes(const sMBMasterPort* psMBPort, UCHAR* pucRcvBuf, USHORT* psReadBytes);

void prvvMasterUARTTxReadyISR(const sMBMasterPort* psMBPort);
void prvvMasterUARTRxISR(const sMBMasterPort* psMBPort);

/* -----------------------Master TCP port functions ----------------------------*/
void vMBMasterTCPPortInit(sMBMasterPort* psMBPort);
BOOL xMBMasterTCPPortConn(sMBMasterPort* psMBPort);

BOOL xMBMasterTCPPortReadBytes(sMBMasterPort* psMBPort, UCHAR* pucRcvBuf, USHORT* psReadBytes);
BOOL xMBMasterTCPPortWriteBytes(sMBMasterPort* psMBPort, const UCHAR *pucMBTCPFrame, USHORT usTCPLength);

/* -----------------------Master Serial port event functions ----------------------------*/

BOOL xMBMasterPortEventInit(sMBMasterPort* psMBPort);
BOOL xMBMasterPortEventPost(sMBMasterPort* psMBPort, eMBMasterEventType eEvent);
BOOL xMBMasterPortEventGet(sMBMasterPort* psMBPort, eMBMasterEventType* peEvent);

void vMBMasterOsResInit(void);

BOOL xMBMasterRunResTake(sMBMasterPort* psMBPort, ULONG lTimeOutMs);
void vMBMasterRunResRelease(sMBMasterPort* psMBPort);

/* ----------------------- Timers functions ---------------------------------*/
BOOL xMBMasterPortTmrsInit(sMBMasterPort* psMBPort, USHORT usTim1Timerout50us);
void xMBMasterPortTmrsClose(sMBMasterPort* psMBPort);

INLINE void vMBsMasterPortTmrsEnable(sMBMasterPort* psMBPort);
INLINE void vMBsMasterPortTmrsConvertDelayEnable(sMBMasterPort* psMBPort);
INLINE void vMBsMasterPortTmrsRespondTimeoutEnable(sMBMasterPort* psMBPort);
INLINE void vMBsMasterPortTmrsDisable(sMBMasterPort* psMBPort);

/* ----------------- Callback for the master error process ------------------*/

void vMBMasterErrorCBRespondTimeout(sMBMasterPort* psMBPort, UCHAR ucDestAddr, 
	                                const UCHAR* pucPDUData, USHORT ucPDULength);
void vMBMasterErrorCBReceiveData(sMBMasterPort* psMBPort, UCHAR ucDestAddr, 
	                             const UCHAR* pucPDUData, USHORT ucPDULength);
void vMBMasterErrorCBExecuteFunction(sMBMasterPort* psMBPort, UCHAR ucDestAddr, 
                                     const UCHAR* pucPDUData, USHORT ucPDULength);
void vMBMasterErrorCBRespondData(sMBMasterPort* psMBPort, UCHAR ucDestAddr, 
                                 const UCHAR* pucPDUData, USHORT ucPDULength);
void vMBMasterCBRequestSuccess(sMBMasterPort* psMBPort);

eMBException prveMBMasterError2Exception(eMBErrorCode eErrorCode);

#ifdef __cplusplus
}
#endif

#endif
