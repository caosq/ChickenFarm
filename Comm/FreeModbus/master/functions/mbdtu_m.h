#ifndef MBDTU_M_H
#define MBDTU_M_H

#include "mbconfig.h"
#include "mb_m.h"

#ifdef __cplusplus
extern "C" {
#endif

#if MB_MASTER_DTU_ENABLED      //GPRS模块功能支持

void vDTUScanDev(sMBMasterInfo* psMBMasterInfo);

BOOL xMBMasterRegistDTUDev(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBDTUDev247,
                           sMBSlaveDev* psMBDTUDev200);

void vDTUDevInit(sMBMasterInfo* psMBMasterInfo);

#endif

#ifdef __cplusplus
}
#endif

#endif // MBDTU_M_H
