/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
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
 * File: $Id: mb_m.h,v 1.60 2013/09/03 10:20:05 Armink Add Master Functions $
 */

#ifndef _MB_M_H
#define _MB_M_H

#include "port.h"
#include "mbport_m.h"
#include "mbproto.h"
#include "mbframe.h"
#include "mbdict_m.h"
#include "mbutils.h"
#include "mbconfig.h"
#include "mbdebug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RT_WAITING_FOREVER              0              /**< Block forever until get resource. */

/*! \defgroup modbus Modbus
 * \code #include "mb_m.h" \endcode
 *
 * This module defines the interface for the application. It contains
 * the basic functions and types required to use the Modbus Master protocol stack.
 * A typical application will want to call eMBMasterInit() first. If the device
 * is ready to answer network requests it must then call eMBEnable() to activate
 * the protocol stack. In the main loop the function eMBMasterPoll() must be called
 * periodically. The time interval between pooling depends on the configured
 * Modbus timeout. If an RTOS is available a separate task should be created
 * and the task should always call the function eMBMasterPoll().
 *
 * \code
 * // Initialize protocol stack in RTU mode for a Master
 * eMBMasterInit( MB_RTU, 38400, MB_PAR_EVEN );
 * // Enable the Modbus Protocol Stack.
 * eMBMasterEnable(  );
 * for( ;; )
 * {
 *     // Call the main polling loop of the Modbus Master protocol stack.
 *     eMBMasterPoll(  );
 *     ...
 * }
 * \endcode
 */

/* ----------------------- Defines ------------------------------------------*/

/*! \ingroup modbus
 * \brief Use the default Modbus Master TCP port (502)
 */
#define MB_MASTER_TCP_PORT_USE_DEFAULT   0

#define MB_MASTER_POLL_TASK_STK_SIZE        160
#define MB_MASTER_SCAN_TASK_STK_SIZE        160
#define MB_MASTER_HEART_BEAT_TASK_STK_SIZE  160

//#define MB_MASTER_WAITING_DELAY             1000    //主栈等待响应时间
#define MB_MASTER_HEART_BEAT_DELAY_MS       100   //心跳延时

/* ----------------------- Type definitions ---------------------------------*/

/*! \ingroup modbus
 * \brief Errorcodes used by all function in the Master request.
 */
typedef enum
{
    MB_MRE_NO_ERR,        /*!< no error. */
    MB_MRE_NO_REG,        /*!< illegal register address. */
    MB_MRE_ILL_ARG,       /*!< illegal argument. */
    MB_MRE_REV_DATA,      /*!< receive data error. */
    MB_MRE_TIMEDOUT,      /*!< timeout error occurred. */
    MB_MRE_MASTER_BUSY,   /*!< master is busy now. */
    MB_MRE_EXE_FUN,       /*!< execute function error. */
	MB_MRE_EPORTERR,      /*!< porting layer error. */
    MB_MRE_ENORES,        /*!< insufficient resources. */
    MB_MRE_EIO,           /*!< I/O error. */
    MB_MRE_EILLSTATE,     /*!< protocol stack in illegal state. */
    MB_MRE_ETIMEDOUT      /*!< timeout error occurred. */
}eMBMasterReqErrCode;

typedef enum
{
    STATE_M_RX_IDLE,    /*!< Receiver is in idle state. */
    STATE_M_RX_INIT,    /*!< Receiver is in initial state. */
    STATE_M_RX_RCV,     /*!< Frame is being received. */
    STATE_M_RX_ERROR,   /*!< If the frame is invalid. */
}eMBMasterRcvState;

typedef enum
{
    STATE_M_TX_IDLE,    /*!< Transmitter is in idle state. */
    STATE_M_TX_XMIT,    /*!< Transmitter is in transfer state. */
    STATE_M_TX_XFWR,    /*!< Transmitter is in transfer finish and wait receive state. */
}eMBMasterSndState;

typedef enum
{
    STATE_HEART_BEAT,  //心跳模式
    STATE_SCAN_DEV,    //主栈轮询从设备模式
    STATE_SCAN_DTU,    //主栈轮询GPRS模块模式
    STATE_TEST_DEV,    //主栈测试从设备模式
    STATE_SYSN_DEV,    //主栈同步从设备数据模式
}eMasterRunMode;


typedef struct                 /* master poll task information */ 
{
#if MB_UCOSIII_ENABLED
    OS_TCB   sMasterPollTCB;
    OS_TCB   sMasterScanReadTCB;
    OS_TCB   sMasterScanWriteTCB;

    OS_PRIO  ucMasterPollPrio;
    OS_PRIO  ucMasterScanReadPrio;
    OS_PRIO  ucMasterScanWritePrio;

    CPU_STK  usMasterPollStk[MB_MASTER_POLL_TASK_STK_SIZE];
    CPU_STK  usMasterScanReadStk[MB_MASTER_SCAN_TASK_STK_SIZE];
    CPU_STK  usMasterScanWriteStk[MB_MASTER_SCAN_TASK_STK_SIZE];
    
#elif MB_LINUX_ENABLED
    pthread_t sMBPollTask;       //主栈状态机任务信息
    pthread_t sMBScanReadTask;   //主栈轮询读任务信息
    pthread_t sMBScanWriteTask;  //主栈轮询写任务信息
#endif

}sMBMasterTask;

/* Functions pointer which are initialized in eMBInit( ). Depending on the
 * mode (RTU or ASCII) the are set to the correct implementations.
 * Using for Modbus Master,Add by Armink 20130813
 */
typedef void (*pvMBMasterFrameStart) (struct sMBMasterInfo* psMBMasterInfo);
typedef void (*pvMBMasterFrameStop) (struct sMBMasterInfo* psMBMasterInfo);
typedef void (*pvMBMasterFrameClose) (sMBMasterPort* psMBPort);

typedef eMBErrorCode (*peMBMasterFrameReceive) (struct sMBMasterInfo* psMBMasterInfo, UCHAR* pucRcvAddress,
                                                UCHAR** pucFrame, USHORT* pusLength);
typedef eMBErrorCode (*peMBMasterFrameSend) (struct sMBMasterInfo* psMBMasterInfo, UCHAR slaveAddress,
                                             UCHAR *pucFrame, USHORT usLength);

typedef void(*pvMBMasterFrameReceiveCallback) (void* p_arg);
typedef void(*pvMBMasterFrameSendCallback) (void* p_arg);

/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 * Using for Modbus Master,Add by Armink 20130813
 */
typedef BOOL (*pxMBMasterFrameCBByteReceived) (struct sMBMasterInfo* psMBMasterInfo);
typedef BOOL (*pxMBMasterFrameCBTransmitterEmpty) (struct sMBMasterInfo* psMBMasterInfo);
typedef BOOL (*pxMBMasterFrameCBTimerExpired) (struct sMBMasterInfo* psMBMasterInfo);

typedef struct sMBMasterInfo  /* master information */
{
    USHORT usSndPDULength;     //PDU数据域长度
    USHORT usSndBufferCount;   //发送缓冲区数据量
    USHORT usRcvBufferPos;     //接收缓冲区数据位置
    USHORT usRcvRequestBytes;  //需要接收的数据字节数
    USHORT usLength;           //数据长度

    UCHAR *pucSndBufferCur;    //当前发送数据缓冲区指针
    UCHAR *pucMasterPDUCur;    //当前发送帧PDU数据域指针
    UCHAR *pucMBRcvFrame;     //接收缓存区
    UCHAR *pucMBSndFrame;     //发送缓存区

    UCHAR  ucMBDestAddr;       //当前从设备地址
    
    BOOL   bDTUEnable;
    BOOL   xFrameIsBroadcast;  //是否为广播帧
    
    eMBMode  eMode;     //MODBUS模式:RTU模式 ASCII模式 TCP模式
    eMBState eMBState;  //主栈状态
    eMBMasterRcvState eRcvState; //接收状态
    eMBMasterSndState eSndState; //发送状态
    
    eMBMasterErrorEventType eCurErrorType; //当前错误类型
    eMasterRunMode eMBRunMode;   //主栈模式
    
    sMBMasterPort     sMBPort;      //主栈硬件接口信息
    sMBMasterDevsInfo sMBDevsInfo;  //主栈从设备信息
    sMBMasterTask     sMBTask;      //主栈状态机任务信息

    /* Functions pointer which are initialized in eMBInit( ). Depending on the
     * mode (RTU or ASCII) the are set to the correct implementations.
     * Using for Modbus Master,Add by Armink 20130813
     */
    peMBMasterFrameSend     peMBMasterFrameSendCur;
    pvMBMasterFrameStart    pvMBMasterFrameStartCur;
    pvMBMasterFrameStop     pvMBMasterFrameStopCur;
    peMBMasterFrameReceive  peMBMasterFrameReceiveCur;
    pvMBMasterFrameClose    pvMBMasterFrameCloseCur;

    /* Callback functions required by the porting layer. They are called when
     * an external event has happend which includes a timeout or the reception
     * or transmission of a character.
     * Using for Modbus Master,Add by Armink 20130813
     */
    pxMBMasterFrameCBByteReceived      pxMBMasterFrameCBByteReceivedCur;
    pxMBMasterFrameCBTransmitterEmpty  pxMBMasterFrameCBTransmitterEmptyCur ;
    pxMBMasterFrameCBTimerExpired      pxMBMasterFrameCBTimerExpiredCur;

    pvMBMasterFrameReceiveCallback     pvMBMasterReceiveCallback;
    pvMBMasterFrameSendCallback        pvMBMasterSendCallback;

    UCHAR ucRTUSndBuf[MB_PDU_SIZE_MAX];         //发送缓冲区
    UCHAR ucRTURcvBuf[MB_SER_PDU_SIZE_MAX];     //接收缓冲区

    UCHAR  BitCoilByteValList[MB_PDU_SIZE_MAX];
    USHORT RegHoldValList[MB_PDU_SIZE_MAX];

    struct sMBMasterInfo* pNext;  //下一主栈节点
    struct sMBMasterInfo* pLast;  //末尾主栈节点
}sMBMasterInfo;

typedef struct                 /* 主栈节点配置信息 */
{
    eMBMode     eMode;
    sUART_Def*  psMasterUart;

    const CHAR* pcMBPortName;

    UCHAR ucMinAddr;
    UCHAR ucMaxAddr;
    BOOL  bDTUEnable;
    
#if MB_UCOSIII_ENABLED
    OS_PRIO  ucMasterPollPrio;
    OS_PRIO  ucMasterScanReadPrio;
    OS_PRIO  ucMasterScanWritePrio;
#endif

}sMBMasterNodeInfo;

/* ----------------------- Function prototypes ------------------------------*/

/************************************************************************! 
 *\ingroup modbus
 *\brief Initialize the Modbus Master protocol stack.
 *
 * This functions initializes the ASCII or RTU module and calls the
 * init functions of the porting layer to prepare the hardware. Please
 * note that the receiver is still disabled and no Modbus frames are
 * processed until eMBMasterEnable( ) has been called.
 *
 * \param The information of master node including the state of stack, 
 * the communicate port,the dictionary of master and all slave devices' state
 *   
 * \return If no error occurs the function returns eMBErrorCode::MB_ENOERR.
 *   The protocol is then in the disabled state and ready for activation
 *   by calling eMBMasterEnable( ). Otherwise one of the following error codes
 *   is returned:
 *    - eMBErrorCode::MB_EPORTERR IF the porting layer returned an error.
 *************************************************************************************/
eMBErrorCode eMBMasterInit( sMBMasterInfo* psMBMasterInfo);

/************************************************************************! 
 *! \ingroup modbus
 * \brief Release resources used by the protocol stack.
 *
 * This function disables the Modbus Master protocol stack and release all
 * hardware resources. It must only be called when the protocol stack 
 * is disabled. 
 *
 * \param The information of master node including the state of stack, 
 * the communicate port,the dictionary of master and all slave devices' state
 * 
 * \note Note all ports implement this function. A port which wants to 
 *   get an callback must define the macro MB_PORT_HAS_CLOSE to 1.
 *
 * \return If the resources where released it return eMBErrorCode::MB_ENOERR.
 *   If the protocol stack is not in the disabled state it returns
 *   eMBErrorCode::MB_EILLSTATE.
 *****************************************************************************/
eMBErrorCode    eMBMasterClose( sMBMasterInfo* psMBMasterInfo );

/************************************************************************! 
 *! \ingroup modbus
 * \brief Enable the Modbus Master protocol stack.
 *
 * This function enables processing of Modbus Master frames. Enabling the protocol
 * stack is only possible if it is in the disabled state.
 *
 * \param The information of master node including the state of stack, 
 * the communicate port,the dictionary of master and all slave devices' state
 * 
 * \return If the protocol stack is now in the state enabled it returns 
 *   eMBErrorCode::MB_ENOERR. If it was not in the disabled state it 
 *   return eMBErrorCode::MB_EILLSTATE.
 */
eMBErrorCode    eMBMasterEnable( sMBMasterInfo* psMBMasterInfo );

/************************************************************************! 
 *! \ingroup modbus
 * \brief Disable the Modbus Master protocol stack.
 *
 * This function disables processing of Modbus frames.
  *
 * \param The information of master node including the state of stack, 
 * the communicate port,the dictionary of master and all slave devices' state
 * 
 * \return If the protocol stack has been disabled it returns 
 *  eMBErrorCode::MB_ENOERR. If it was not in the enabled state it returns
 *  eMBErrorCode::MB_EILLSTATE.
 */
eMBErrorCode    eMBMasterDisable( sMBMasterInfo* psMBMasterInfo );

/************************************************************************! 
 *! \ingroup modbus
 * \brief The main pooling loop of the Modbus Master protocol stack.
 *
 * This function must be called periodically. The timer interval required
 * is given by the application dependent Modbus slave timeout. Internally the
 * function calls xMBMasterPortEventGet() and waits for an event from the receiver or
 * transmitter state machines. 
 *
 * \param The information of master node including the state of stack, 
 * the communicate port,the dictionary of master and all slave devices' state
 * 
 * \return If the protocol stack is not in the enabled state the function
 *   returns eMBErrorCode::MB_EILLSTATE. Otherwise it returns 
 *   eMBErrorCode::MB_ENOERR.
 *************************************************************************/
eMBErrorCode eMBMasterPoll( sMBMasterInfo* psMBMasterInfo );


/* ----------------------- Callback -----------------------------------------*/

/************************************************************************! 
 *! \defgroup modbus_master registers Modbus Registers
 * \code #include "mb_m.h" \endcode
 * The protocol stack does not internally allocate any memory for the
 * registers. This makes the protocol stack very small and also usable on
 * low end targets. In addition the values don't have to be in the memory
 * and could for example be stored in a flash.<br>
 * Whenever the protocol stack requires a value it calls one of the callback
 * function with the register address and the number of registers to read
 * as an argument. The application should then read the actual register values
 * (for example the ADC voltage) and should store the result in the supplied
 * buffer.<br>
 * If the protocol stack wants to update a register value because a write
 * register function was received a buffer with the new register values is
 * passed to the callback function. The function should then use these values
 * to update the application register values.
 */

/*! \ingroup modbus_registers
 * \brief Callback function used if the value of a <em>Input Register</em>
 *   is required by the protocol stack. The starting register address is given
 *   by \c usAddress and the last register is given by <tt>usAddress +
 *   usNRegs - 1</tt>.
 *
 * \param The information of master node including the state of stack, 
 * the communicate port,the dictionary of master and all slave devices' state
 * 
 * \param pucRegBuffer A buffer where the callback function should write
 *   the current value of the modbus registers to.
 * \param usAddress The starting address of the register. Input registers
 *   are in the range 1 - 65535.
 * \param usNRegs Number of registers the callback function must supply.
 *
 * \return The function must return one of the following error codes:
 *   - eMBErrorCode::MB_ENOERR If no error occurred. In this case a normal
 *       Modbus response is sent.
 *   - eMBErrorCode::MB_ENOREG If the application does not map an coils
 *       within the requested address range. In this case a
 *       <b>ILLEGAL DATA ADDRESS</b> is sent as a response.
 *************************************************************************/
eMBErrorCode eMBMasterRegInputCB( sMBMasterInfo* psMBMasterInfo, UCHAR* pucRegBuffer, 
                                  USHORT usAddress, USHORT usNRegs );

/************************************************************************! 
 *! \ingroup modbus_registers
 * \brief Callback function used if a <em>Holding Register</em> value is
 *   read or written by the protocol stack. The starting register address
 *   is given by \c usAddress and the last register is given by
 *   <tt>usAddress + usNRegs - 1</tt>.
 *
 * \param The information of master node including the state of stack, 
 * the communicate port,the dictionary of master and all slave devices' state
 * 
 * \param pucRegBuffer If the application registers values should be updated the
 *   buffer points to the new registers values. If the protocol stack needs
 *   to now the current values the callback function should write them into
 *   this buffer.
 * \param usAddress The starting address of the register.
 * \param usNRegs Number of registers to read or write.
 * \param eMode If eMBRegisterMode::MB_REG_WRITE the application register
 *   values should be updated from the values in the buffer. For example
 *   this would be the case when the Modbus master has issued an
 *   <b>WRITE SINGLE REGISTER</b> command.
 *   If the value eMBRegisterMode::MB_REG_READ the application should copy
 *   the current values into the buffer \c pucRegBuffer.
 *
 * \return The function must return one of the following error codes:
 *   - eMBErrorCode::MB_ENOERR If no error occurred. In this case a normal
 *       Modbus response is sent.
 *   - eMBErrorCode::MB_ENOREG If the application does not map an coils
 *       within the requested address range. In this case a
 *       <b>ILLEGAL DATA ADDRESS</b> is sent as a response.
 *************************************************************************/
eMBErrorCode eMBMasterRegHoldingCB( sMBMasterInfo* psMBMasterInfo, UCHAR* pucRegBuffer, 
                                    USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );


/************************************************************************! 
 *! \ingroup modbus_registers
 * \brief Callback function used if a <em>Coil Register</em> value is
 *   read or written by the protocol stack. If you are going to use
 *   this function you might use the functions xMBUtilSetBits(  ) and
 *   xMBUtilGetBits(  ) for working with bitfields.
 *
 * \param The information of master node including the state of stack, 
 * the communicate port,the dictionary of master and all slave devices' state
 * 
 * \param pucRegBuffer The bits are packed in bytes where the first coil
 *   starting at address \c usAddress is stored in the LSB of the
 *   first byte in the buffer <code>pucRegBuffer</code>.
 *   If the buffer should be written by the callback function unused
 *   coil values (I.e. if not a multiple of eight coils is used) should be set
 *   to zero.
 * \param usAddress The first coil number.
 * \param usNCoils Number of coil values requested.
 * \param eMode If eMBRegisterMode::MB_REG_WRITE the application values should
 *   be updated from the values supplied in the buffer \c pucRegBuffer.
 *   If eMBRegisterMode::MB_REG_READ the application should store the current
 *   values in the buffer \c pucRegBuffer.
 *
 * \return The function must return one of the following error codes:
 *   - eMBErrorCode::MB_ENOERR If no error occurred. In this case a normal
 *       Modbus response is sent.
 *   - eMBErrorCode::MB_ENOREG If the application does not map an coils
 *       within the requested address range. In this case a
 *       <b>ILLEGAL DATA ADDRESS</b> is sent as a response.
 *************************************************************************/
eMBErrorCode eMBMasterRegCoilsCB(sMBMasterInfo* psMBMasterInfo, UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBBitMode eMode);

/************************************************************************! 
 *! \ingroup modbus_registers
 * \brief Callback function used if a <em>Input Discrete Register</em> value is
 *   read by the protocol stack.
 *
 * If you are going to use his function you might use the functions
 * xMBUtilSetBits(  ) and xMBUtilGetBits(  ) for working with bitfields.
 *
 * \param The information of master node including the state of stack, 
 * the communicate port,the dictionary of master and all slave devices' state
 * 
 * \param pucRegBuffer The buffer should be updated with the current
 *   coil values. The first discrete input starting at \c usAddress must be
 *   stored at the LSB of the first byte in the buffer. If the requested number
 *   is not a multiple of eight the remaining bits should be set to zero.
 * \param usAddress The starting address of the first discrete input.
 * \param usNDiscrete Number of discrete input values.
 * \return The function must return one of the following error codes:
 *   - eMBErrorCode::MB_ENOERR If no error occurred. In this case a normal
 *       Modbus response is sent.
 *   - eMBErrorCode::MB_ENOREG If the application does not map an coils
 *       within the requested address range. In this case a
 *       <b>ILLEGAL DATA ADDRESS</b> is sent as a response.
 *************************************************************************/
eMBErrorCode eMBMasterRegDiscreteCB( sMBMasterInfo* psMBMasterInfo, UCHAR * pucRegBuffer, 
                                     USHORT usAddress, USHORT usNDiscrete );

/************************************************************************! 
 *! \ingroup modbus
 *\brief These functions are interface for Modbus Master
 *************************************************************************/
void vMBMasterSetCurTimerMode( sMBMasterPort* psMBPort, eMBMasterTimerMode eMBTimerMode );
BOOL xMBMasterRequestIsBroadcast( const sMBMasterInfo* psMBMasterInfo );

void vMBMasterGetRTUSndBuf(sMBMasterInfo* psMBMasterInfo, UCHAR** pucFrame);
void vMBMasterGetPDUSndBuf(sMBMasterInfo* psMBMasterInfo, UCHAR** pucFrame);

USHORT usMBMasterGetPDUSndLength( const sMBMasterInfo* psMBMasterInfo );
void vMBMasterSetPDUSndLength( sMBMasterInfo* psMBMasterInfo, USHORT SendPDULength );

UCHAR ucMBMasterGetDestAddr( const sMBMasterInfo* psMBMasterInfo );
void vMBMasterSetDestAddress( sMBMasterInfo* psMBMasterInfo, UCHAR Address );

eMBMasterRcvState usMBMasterGetRcvState(const sMBMasterInfo* psMBMasterInfo);
eMBMasterSndState usMBMasterGetSndState(const sMBMasterInfo* psMBMasterInfo);

//eMasterRunMode eMBMasterGetCBRunInMode(const sMBMasterInfo* psMBMasterInfo);
//void vMBMasterSetCBRunInScanMode(sMBMasterInfo* psMBMasterInfo);

eMBMasterErrorEventType eMBMasterGetErrorType( const sMBMasterInfo* psMBMasterInfo );
void vMBMasterSetErrorType( sMBMasterInfo* psMBMasterInfo, eMBMasterErrorEventType errorType );

eMBMasterEventType xMBMasterPortCurrentEvent( const sMBMasterPort* psMBPort );
eMBMasterReqErrCode eMBMasterWaitRequestFinish( sMBMasterPort* psMBPort );

void vMBMasterPortLock(sMBMasterPort* psMBPort);
void vMBMasterPortUnLock(sMBMasterPort* psMBPort);
/************************************************************************! 
 *! \ingroup modbus
 *\brief These functions are register node for Modbus Master
 *************************************************************************/
BOOL xMBMasterRegistNode(sMBMasterInfo* psMBMasterInfo, sMBMasterNodeInfo* psMasterNode);

sMBMasterInfo* psMBMasterFindNodeByPort(const CHAR* pcMBPortName);
									 
BOOL xMBMasterCreatePollTask(sMBMasterInfo* psMBMasterInfo);									 

/************************************************************************! 
 *! \ingroup modbus
 *\brief These functions are for Modbus Master slave device state
 *************************************************************************/
BOOL xMBMasterRegistDev(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBNewDev); 
 
sMBSlaveDev* psMBMasterGetDev(const sMBMasterInfo* psMBMasterInfo, UCHAR Address);

BOOL xMBMasterRemoveDev(sMBMasterInfo* psMBMasterInfo, UCHAR Address);

/************************************************************************! 
 *! \ingroup modbus
 *\brief These functions are for Modbus Master slave device timer
 *************************************************************************/
BOOL xMBMasterInitDevTimer(sMBSlaveDev* psMBDev, USHORT usTimerSec); 
 
void vMBMasterDevOfflineTmrEnable(sMBSlaveDev* psMBDev);

void vMBMasterDevOfflineTmrDel(sMBSlaveDev* psMBDev);

#ifdef __cplusplus
}
#endif
#endif
