#ifndef _MB_TEST_M_H
#define _MB_TEST_M_H

#include "mb_m.h"

void vMBDevTest(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev);
void vMBDevCurStateTest(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev);

BOOL xMBMasterCreateDevHeartBeatTask(sMBMasterInfo* psMBMasterInfo);

BOOL xMBMasterDevOfflineTmrEnable(sMBSlaveDev* psMBDev, LONG lOffline_S);

#endif
