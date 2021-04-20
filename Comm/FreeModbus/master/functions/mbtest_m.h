#ifndef _MB_TEST_M_H
#define _MB_TEST_M_H

#include "mb_m.h"

void vMBDevTest(sMBSlaveDev* psMBSlaveDev);
void vMBDevCurStateTest(sMBSlaveDev* psMBSlaveDev);

BOOL xMBMasterCreateDevHeartBeatTask(sMBMasterInfo* psMBMasterInfo);

BOOL xMBMasterDevOfflineTmrEnable(sMBSlaveDev* psMBDev, LONG lOffline_S);

#endif
