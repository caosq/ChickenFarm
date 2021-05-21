/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
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
 * File: $Id: mb.h,v 1.17 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _MB_H
#define _MB_H

#include "mbport.h"
#include "mbdict.h"

#if MB_UCOSIII_ENABLED

#if MB_SLAVE_TCP_ENABLED
#include "sockets.h"
#endif

#elif MB_LINUX_ENABLED

#if MB_SLAVE_TCP_ENABLED
#include <netinet/in.h>
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif 

/*! \defgroup modbus Modbus
 * \code #include "mb.h" \endcode
 *
 * This module defines the interface for the application. It contains
 * the basic functions and types required to use the Modbus protocol stack.
 * A typical application will want to call eMBInit() first. If the device
 * is ready to answer network requests it must then call eMBEnable() to activate
 * the protocol stack. In the main loop the function eMBPoll() must be called
 * periodically. The time interval between pooling depends on the configured
 * Modbus timeout. If an RTOS is available a separate task should be created
 * and the task should always call the function eMBPoll().
 *
 * \code
 * // Initialize protocol stack in RTU mode for a slave with address 10 = 0x0A
 * eMBInit( MB_RTU, 0x0A, 38400, MB_PAR_EVEN );
 * // Enable the Modbus Protocol Stack.
 * eMBEnable(  );
 * for( ;; )
 * {
 *     // Call the main polling loop of the Modbus protocol stack.
 *     eMBPoll(  );
 *     ...
 * }
 * \endcode
 */
/* ----------------------- Defines ------------------------------------------*/
/*! \ingroup modbus
 * \brief Use the default Modbus TCP port (502)
 */
#define MB_TCP_PORT_USE_DEFAULT       0   
#define MB_SLAVE_POLL_TASK_STK_SIZE   256

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_INIT,              /*!< Receiver is in initial state. */
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_ERROR              /*!< If the frame is invalid. */
} eMBSlaveRcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_XMIT               /*!< Transmitter is in transfer state. */
} eMBSlaveSndState;

typedef struct                 /* master poll task information */ 
{
#if  MB_UCOSIII_ENABLED
    OS_TCB     sSlavePollTCB;
    OS_PRIO    ucSlavePollPrio;
    CPU_STK    usSlavePollStk[MB_SLAVE_POLL_TASK_STK_SIZE];
    
#elif MB_LINUX_ENABLED
    pthread_t  sMBPollTask;               //从栈状态机任务信息
#endif

}sMBSlaveTask;

typedef void (*pvMBSlaveFrameStart)(struct sMBSlaveInfo* psMBSlaveInfo);
typedef void (*pvMBSlaveFrameStop)(struct sMBSlaveInfo* psMBSlaveInfo);
typedef void (*pvMBSlaveFrameGetRequest)(struct sMBSlaveInfo* psMBSlaveInfo);

typedef void (*pvMBSlaveFrameClose)(sMBSlavePort* psMBPort);

typedef eMBErrorCode (*peMBSlaveFrameReceive)(struct sMBSlaveInfo* psMBSlaveInfo, UCHAR *pucRcvAddress, 
                                              UCHAR** pucFrame, USHORT *pusLength);
typedef eMBErrorCode (*peMBSlaveFrameSend)(struct sMBSlaveInfo* psMBSlaveInfo, UCHAR slaveAddress, 
                                           const UCHAR* pucFrame, USHORT usLength);
typedef void (*pvMBSlaveFrameReceiveCallback)(void* p_arg);
typedef void (*pvMBSlaveFrameSendCallback)(void* p_arg);

/* ----------------------- Callback for the protocol stack ------------------*/
/*!
 * \brief Callback function for the porting layer when a new byte is
 *   available.
 *
 * Depending upon the mode this callback function is used by the RTU or
 * ASCII transmission layers. In any case a call to xMBPortSerialGetByte()
 * must immediately return a new character.
 *
 * \return <code>TRUE</code> if a event was posted to the queue because
 *   a new byte was received. The port implementation should wake up the
 *   tasks which are currently blocked on the eventqueue.
 */
typedef BOOL (*pxMBSlaveFrameCBByteReceived)(struct sMBSlaveInfo* psMBSlaveInfo);
typedef BOOL (*pxMBSlaveFrameCBTransmitterEmpty)(struct sMBSlaveInfo* psMBSlaveInfo);
typedef BOOL (*pxMBSlaveFrameCBTimerExpired)(struct sMBSlaveInfo* psMBSlaveInfo);

//extern pxMBSlaveFrameCBTimerExpired  pxMBSlaveFrameCBTimerExpiredCur;

typedef struct sMBSlaveInfo  /* Slave information */
{
    /* Functions pointer which are initialized in eMBInit( ). Depending on the
     * mode (RTU or ASCII) the are set to the correct implementations.
     */
    peMBSlaveFrameSend       peMBSlaveFrameSendCur;
    peMBSlaveFrameReceive    peMBSlaveFrameReceiveCur;
    pvMBSlaveFrameStart      pvMBSlaveFrameStartCur;
    pvMBSlaveFrameStop       pvMBSlaveFrameStopCur;
    pvMBSlaveFrameGetRequest pvMBSlaveFrameGetRequestCur;
    pvMBSlaveFrameClose      pvMBSlaveFrameCloseCur;

    pvMBSlaveFrameReceiveCallback    pvMBSlaveReceiveCallback;
    pvMBSlaveFrameSendCallback       pvMBSlaveSendCallback;
    
    /* Callback functions required by the porting layer. They are called when
     * an external event has happend which includes a timeout or the reception
     * or transmission of a character.
     */
    pxMBSlaveFrameCBByteReceived     pxMBSlaveFrameCBByteReceivedCur;
    pxMBSlaveFrameCBTransmitterEmpty pxMBSlaveFrameCBTransmitterEmptyCur;
    pxMBSlaveFrameCBTimerExpired     pxMBSlaveFrameCBTimerExpiredCur;

    sMBSlavePort      sMBPort;        //从栈硬件接口信息
    sMBSlaveTask      sMBTask;        //从栈状态机任务信息

    sMBSlaveCommInfo  sMBCommInfo;    //从栈通讯信息
    
    eMBMode           eMode;      //MODBUS模式: RTU模式   ASCII模式
    eMBState          eMBState;   //从栈状态
    eMBSlaveSndState  eSndState;  //发送状态
    eMBSlaveRcvState  eRcvState;  //接收状态
    
    USHORT usSndBufferCount;   //发送缓冲区数据量
    USHORT usRcvBufferPos;     //接收缓冲区数据位置
    USHORT usLength;           //报文长度

    UCHAR *pucSndBufferCur;   //当前发送数据缓冲区指针
    UCHAR *pucMBFrame;        //接收和发送报文数据缓存区
    
    UCHAR  ucRcvBuf[MB_SER_PDU_SIZE_MAX];       //接收缓冲区                      
    UCHAR  ucRcvAddress;      //modbus从机地址
 
    struct sMBSlaveInfo*    pNext;     //下一从栈节点
    struct sMBSlaveInfo*    pLast;     //末尾从栈节点
}sMBSlaveInfo;


#if MB_SLAVE_TCP_ENABLED
typedef struct                 /* master poll task information */
{
    UCHAR ucSlaveAddr; 

    pvMBSlaveFrameReceiveCallback    pvMBSlaveReceiveCallback;
    pvMBSlaveFrameSendCallback       pvMBSlaveSendCallback;
#if  MB_UCOSIII_ENABLED
    OS_PRIO    ucSlaveTcpPollPrio;
    OS_PRIO    ucSlaveTcpServerPrio;
    OS_TCB     sSlaveTcpServerTCB;
    CPU_STK    usSlaveTcpServerStk[MB_SLAVE_POLL_TASK_STK_SIZE];
    
#elif MB_LINUX_ENABLED  
    pthread_t sMBSlaveTcpServerTask;  //TCP从栈服务任务信息
#endif 
    struct sockaddr_in sTcpServerAddr;
    sMBSlaveInfo sMBSlaveTcpClients[MB_SLAVE_MAX_TCP_CLIENT];
}sMBSlaveTcpInfo;
#endif

typedef struct    /* 从栈节点配置信息 */
{
   eMBMode eMode;
   const CHAR* pcMBPortName;
   UCHAR ucSlaveAddr;
    
#if MB_SLAVE_TCP_ENABLED
   int iSocketClient;
   int iSocketServer; //Server对应的socket号 
#endif    

#if MB_SLAVE_RTU_ENABLED || MB_SLAVE_ASCII_ENABLED
   sUART_Def* psSlaveUart; 
#endif    
  
#if MB_UCOSIII_ENABLED
   OS_PRIO ucSlavePollPrio;
#endif
    
}sMBSlaveNodeInfo;

/* ----------------------- Function prototypes ------------------------------*/
/*! \ingroup modbus
 * \brief Initialize the Modbus protocol stack.
 *
 * This functions initializes the ASCII or RTU module and calls the
 * init functions of the porting layer to prepare the hardware. Please
 * note that the receiver is still disabled and no Modbus frames are
 * processed until eMBEnable( ) has been called.
 *
 * \param eMode If ASCII or RTU mode should be used.
 * \param ucSlaveAddress The slave address. Only frames sent to this
 *   address or to the broadcast address are processed.
 * \param ucPort The port to use. E.g. 1 for COM1 on windows. This value
 *   is platform dependent and some ports simply choose to ignore it.
 * \param ulBaudRate The baudrate. E.g. 19200. Supported baudrates depend
 *   on the porting layer.
 * \param eParity Parity used for serial transmission.
 *
 * \return If no error occurs the function returns eMBErrorCode::MB_ENOERR.
 *   The protocol is then in the disabled state and ready for activation
 *   by calling eMBEnable( ). Otherwise one of the following error codes 
 *   is returned:
 *    - eMBErrorCode::MB_EINVAL If the slave address was not valid. Valid
 *        slave addresses are in the range 1 - 247.
 *    - eMBErrorCode::MB_EPORTERR IF the porting layer returned an error.
 */
eMBErrorCode  eMBSlaveInit(sMBSlaveInfo* psMBSlaveInfo);

/*! \ingroup modbus
 * \brief Initialize the Modbus protocol stack for Modbus TCP.
 *
 * This function initializes the Modbus TCP Module. Please note that
 * frame processing is still disabled until eMBEnable( ) is called.
 *
 * \param usTCPPort The TCP port to listen on.
 * \return If the protocol stack has been initialized correctly the function
 *   returns eMBErrorCode::MB_ENOERR. Otherwise one of the following error
 *   codes is returned:
 *    - eMBErrorCode::MB_EINVAL If the slave address was not valid. Valid
 *        slave addresses are in the range 1 - 247.
 *    - eMBErrorCode::MB_EPORTERR IF the porting layer returned an error.
 */
eMBErrorCode eMBSlaveTCPInit(sMBSlaveInfo* psMBSlaveInfo);

/*! \ingroup modbus
 * \brief Release resources used by the protocol stack.
 *
 * This function disables the Modbus protocol stack and release all
 * hardware resources. It must only be called when the protocol stack 
 * is disabled. 
 *
 * \note Note all ports implement this function. A port which wants to 
 *   get an callback must define the macro MB_PORT_HAS_CLOSE to 1.
 *
 * \return If the resources where released it return eMBErrorCode::MB_ENOERR.
 *   If the protocol stack is not in the disabled state it returns
 *   eMBErrorCode::MB_EILLSTATE.
 */
eMBErrorCode    eMBSlaveClose(sMBSlaveInfo* psMBSlaveInfo);

/*! \ingroup modbus
 * \brief Enable the Modbus protocol stack.
 *
 * This function enables processing of Modbus frames. Enabling the protocol
 * stack is only possible if it is in the disabled state.
 *
 * \return If the protocol stack is now in the state enabled it returns 
 *   eMBErrorCode::MB_ENOERR. If it was not in the disabled state it 
 *   return eMBErrorCode::MB_EILLSTATE.
 */
eMBErrorCode    eMBSlaveEnable( sMBSlaveInfo* psMBSlaveInfo );

/*! \ingroup modbus
 * \brief Disable the Modbus protocol stack.
 *
 * This function disables processing of Modbus frames.
 *
 * \return If the protocol stack has been disabled it returns 
 *  eMBErrorCode::MB_ENOERR. If it was not in the enabled state it returns
 *  eMBErrorCode::MB_EILLSTATE.
 */
eMBErrorCode    eMBSlaveDisable( sMBSlaveInfo* psMBSlaveInfo );

/*! \ingroup modbus
 * \brief The main pooling loop of the Modbus protocol stack.
 *
 * This function must be called periodically. The timer interval required
 * is given by the application dependent Modbus slave timeout. Internally the
 * function calls xMBPortEventGet() and waits for an event from the receiver or
 * transmitter state machines. 
 *
 * \return If the protocol stack is not in the enabled state the function
 *   returns eMBErrorCode::MB_EILLSTATE. Otherwise it returns 
 *   eMBErrorCode::MB_ENOERR.
 */
eMBErrorCode    eMBSlavePoll( sMBSlaveInfo* psMBSlaveInfo );

/*! \ingroup modbus
 * \brief Configure the slave id of the device.
 *
 * This function should be called when the Modbus function <em>Report Slave ID</em>
 * is enabled ( By defining MB_FUNC_OTHER_REP_SLAVEID_ENABLED in mbconfig.h ).
 *
 * \param ucSlaveID Values is returned in the <em>Slave ID</em> byte of the
 *   <em>Report Slave ID</em> response.
 * \param xIsRunning If TRUE the <em>Run Indicator Status</em> byte is set to 0xFF.
 *   otherwise the <em>Run Indicator Status</em> is 0x00.
 * \param pucAdditional Values which should be returned in the <em>Additional</em>
 *   bytes of the <em> Report Slave ID</em> response.
 * \param usAdditionalLen Length of the buffer <code>pucAdditonal</code>.
 *
 * \return If the static buffer defined by MB_FUNC_OTHER_REP_SLAVEID_BUF in
 *   mbconfig.h is to small it returns eMBErrorCode::MB_ENORES. Otherwise
 *   it returns eMBErrorCode::MB_ENOERR.
 */
eMBErrorCode    eMBSlaveSetID( sMBSlaveInfo* psMBSlaveInfo, UCHAR ucSlaveID, BOOL xIsRunning,
                               UCHAR const *pucAdditional, USHORT usAdditionalLen );


/* ----------------------- Callback -----------------------------------------*/

/*! \defgroup modbus_registers Modbus Registers
 * \code #include "mb.h" \endcode
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
 * \param pucRegBuffer A buffer where the callback function should write
 *   the current value of the modbus registers to.
 * \param usAddress The starting address of the register. Input registers
 *   are in the range 1 - 65535.
 * \param usNRegs Number of registers the callback function must supply.
 *
 * \return The function must return one of the following error codes:
 *   - eMBErrorCode::MB_ENOERR If no error occurred. In this case a normal
 *       Modbus response is sent.
 *   - eMBErrorCode::MB_ENOREG If the application can not supply values
 *       for registers within this range. In this case a 
 *       <b>ILLEGAL DATA ADDRESS</b> exception frame is sent as a response.
 *   - eMBErrorCode::MB_ETIMEDOUT If the requested register block is
 *       currently not available and the application dependent response
 *       timeout would be violated. In this case a <b>SLAVE DEVICE BUSY</b>
 *       exception is sent as a response.
 *   - eMBErrorCode::MB_EIO If an unrecoverable error occurred. In this case
 *       a <b>SLAVE DEVICE FAILURE</b> exception is sent as a response.
 */
eMBErrorCode    eMBSlaveRegInputCB( sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucRegBuffer,
                                    USHORT usAddress, USHORT usNRegs );

/*! \ingroup modbus_registers
 * \brief Callback function used if a <em>Holding Register</em> value is
 *   read or written by the protocol stack. The starting register address
 *   is given by \c usAddress and the last register is given by
 *   <tt>usAddress + usNRegs - 1</tt>.
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
 *   - eMBErrorCode::MB_ENOREG If the application can not supply values
 *       for registers within this range. In this case a 
 *       <b>ILLEGAL DATA ADDRESS</b> exception frame is sent as a response.
 *   - eMBErrorCode::MB_ETIMEDOUT If the requested register block is
 *       currently not available and the application dependent response
 *       timeout would be violated. In this case a <b>SLAVE DEVICE BUSY</b>
 *       exception is sent as a response.
 *   - eMBErrorCode::MB_EIO If an unrecoverable error occurred. In this case
 *       a <b>SLAVE DEVICE FAILURE</b> exception is sent as a response.
 */
eMBErrorCode    eMBSlaveRegHoldingCB( sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucRegBuffer,
                                      USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );

/*! \ingroup modbus_registers
 * \brief Callback function used if a <em>Coil Register</em> value is
 *   read or written by the protocol stack. If you are going to use
 *   this function you might use the functions xMBUtilSetBits(  ) and
 *   xMBUtilGetBits(  ) for working with bitfields.
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
 *   - eMBErrorCode::MB_ETIMEDOUT If the requested register block is
 *       currently not available and the application dependent response
 *       timeout would be violated. In this case a <b>SLAVE DEVICE BUSY</b>
 *       exception is sent as a response.
 *   - eMBErrorCode::MB_EIO If an unrecoverable error occurred. In this case
 *       a <b>SLAVE DEVICE FAILURE</b> exception is sent as a response.
 */
eMBErrorCode eMBSlaveRegCoilsCB(sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucRegBuffer,
                                USHORT usAddress, USHORT usNCoils, eMBBitMode eMode);

/*! \ingroup modbus_registers
 * \brief Callback function used if a <em>Input Discrete Register</em> value is
 *   read by the protocol stack.
 *
 * If you are going to use his function you might use the functions
 * xMBUtilSetBits(  ) and xMBUtilGetBits(  ) for working with bitfields.
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
 *   - eMBErrorCode::MB_ENOREG If no such discrete inputs exists.
 *       In this case a <b>ILLEGAL DATA ADDRESS</b> exception frame is sent 
 *       as a response.
 *   - eMBErrorCode::MB_ETIMEDOUT If the requested register block is
 *       currently not available and the application dependent response
 *       timeout would be violated. In this case a <b>SLAVE DEVICE BUSY</b>
 *       exception is sent as a response.
 *   - eMBErrorCode::MB_EIO If an unrecoverable error occurred. In this case
 *       a <b>SLAVE DEVICE FAILURE</b> exception is sent as a response.
 */
eMBErrorCode eMBSlaveRegDiscreteCB( sMBSlaveInfo* psMBSlaveInfo, UCHAR* pucRegBuffer, 
                                    USHORT usAddress,USHORT usNDiscrete );

/************************************************************************! 
 *! \ingroup modbus
 *\brief These functions are register node for Modbus Slave
 *************************************************************************/
BOOL xMBSlaveRegistNode( sMBSlaveInfo* psMBSlaveInfo, sMBSlaveNodeInfo* psSlaveNode);

void vMBSlaveRegistCommData(sMBSlaveInfo* psMBSlaveInfo, sMBSlaveCommData* psSlaveCurData);

void vMBSlaveSetAddr(sMBSlaveInfo* psMBSlaveInfo, UCHAR ucSlaveAddr);

sMBSlaveInfo* psMBSlaveFindNodeByPort(const CHAR* pcMBPortName);
									 
BOOL xMBSlaveCreatePollTask(sMBSlaveInfo* psMBSlaveInfo);									 

#ifdef __cplusplus
}
#endif
#endif
