#ifndef _MB_SCAN_M_H
#define _MB_SCAN_M_H

#include "port.h"
#include "mb_m.h"

BOOL xMBMasterCreateScanTask(sMBMasterInfo* psMBMasterInfo);

eMBMasterReqErrCode
eMBMasterScanRegReadHold(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, BOOL xCheckPreValue, ULONG ulTimeOut);

eMBMasterReqErrCode eMBMasterScanReadRegIn(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, ULONG ulTimeOut);

eMBMasterReqErrCode
eMBMasterWriteRegHold(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev, USHORT usRegAddr, USHORT usNRegs,
                      USHORT* pusDataBuffer, ULONG ulTimeOut);
#endif
