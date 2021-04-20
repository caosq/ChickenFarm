#ifndef _MB_SCAN_M_H
#define _MB_SCAN_M_H

#include "port.h"
#include "mb_m.h"

BOOL xMBMasterCreateScanSlaveDevTask(sMBMasterInfo* psMBMasterInfo);

eMBMasterReqErrCode
eMBMasterScanHoldingRegister(sMBSlaveDev* psMBSlaveDev, BOOL xWriteEn, BOOL xReadEn, BOOL xCheckPreValue, ULONG ulTimeOut);

eMBMasterReqErrCode eMBMasterScanReadInputRegister(sMBSlaveDev* psMBSlaveDev, ULONG ulTimeOut);

eMBMasterReqErrCode
eMBMasterReqWriteHoldReg(sMBSlaveDev* psMBSlaveDev, USHORT usRegAddr, USHORT usNRegs,
                         USHORT* pusDataBuffer, ULONG ulTimeOut);
#endif
