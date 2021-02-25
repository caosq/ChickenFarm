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
 * File: $Id: mbfunc.h,v 1.12 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _MB_FUNC_M_H
#define _MB_FUNC_M_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mbframe.h"
#include "mbproto.h"
#include "mb_m.h"

typedef         eMBException( *pxMBMasterFunctionHandler ) ( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * pusLength );

typedef struct
{
    UCHAR           ucFunctionCode;
    pxMBMasterFunctionHandler pxHandler;
} xMBMasterFunctionHandler;

/************************************************************************! 
 *\brief These Modbus functions are called for user when Modbus run in Master Mode.
 *************************************************************************/
 
#if MB_FUNC_OTHER_REP_SLAVEID_BUF > 0
eMBException    eMBMasterFuncReportSlaveID( UCHAR * pucFrame, USHORT * usLen );
#endif

#if MB_FUNC_READ_INPUT_ENABLED > 0
eMBMasterReqErrCode eMBMasterReqReadInputRegister(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usRegAddr, 
                                                  USHORT usNRegs, ULONG ulTimeOut);
eMBException    eMBMasterFuncReadInputRegister( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen );
#endif


#if MB_FUNC_READ_HOLDING_ENABLED > 0
eMBMasterReqErrCode eMBMasterReqReadHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usRegAddr, 
                                                     USHORT usNRegs, ULONG ulTimeOut);
eMBException    eMBMasterFuncReadHoldingRegister( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen );
#endif


#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
eMBMasterReqErrCode eMBMasterReqWriteHoldingRegister(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usRegAddr, 
                                                     USHORT usRegData, ULONG ulTimeOut);
eMBException    eMBMasterFuncWriteHoldingRegister( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen );
#endif


#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
eMBMasterReqErrCode eMBMasterReqWriteMultipleHoldingRegister( sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usRegAddr, 
                                                              USHORT usNRegs, USHORT * pusDataBuffer, ULONG ulTimeOut );
eMBException    eMBMasterFuncWriteMultipleHoldingRegister( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen );
#endif


#if MB_FUNC_READ_COILS_ENABLED > 0
eMBMasterReqErrCode eMBMasterReqReadCoils( sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, 
                                           USHORT usCoilAddr, USHORT usNCoils, ULONG ulTimeOut );
eMBException    eMBMasterFuncReadCoils( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen );
#endif


#if MB_FUNC_WRITE_COIL_ENABLED > 0
eMBMasterReqErrCode eMBMasterReqWriteCoil( sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, 
                                           USHORT usCoilAddr, USHORT usMBSlaveBitData, ULONG ulTimeOut );
eMBException    eMBMasterFuncWriteCoil( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen );
#endif


#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
eMBMasterReqErrCode eMBMasterReqWriteMultipleCoils( sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, USHORT usCoilAddr, 
                                                    USHORT usNCoils, UCHAR * pucDataBuffer, ULONG ulTimeOut );
eMBException    eMBMasterFuncWriteMultipleCoils( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen );
#endif


#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
eMBMasterReqErrCode eMBMasterReqReadDiscreteInputs( sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, 
                                                    USHORT usDiscreteAddr, USHORT usNDiscreteIn, ULONG ulTimeOut );
eMBException    eMBMasterFuncReadDiscreteInputs( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen );
#endif

#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
eMBMasterReqErrCode eMBMasterReqReadWriteMultipleHoldingRegister( sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr,
		                                                          USHORT usReadRegAddr, USHORT usNReadRegs, USHORT * pusDataBuffer,
                                                                  USHORT usWriteRegAddr, USHORT usNWriteRegs, ULONG ulTimeOut );
eMBException    eMBMasterFuncReadWriteMultipleHoldingRegister( sMBMasterInfo* psMBMasterInfo, UCHAR * pucFrame, USHORT * usLen );
#endif

#ifdef __cplusplus
}
#endif
#endif
