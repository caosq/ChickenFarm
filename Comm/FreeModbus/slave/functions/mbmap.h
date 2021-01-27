#ifndef _USER_MB_MAP_H
#define _USER_MB_MAP_H

#include "mb.h"
#include "mbdict.h"

//主栈从设备字典申请 
#ifndef SLAVE_DEV_DICT_ALLOC
#define SLAVE_DEV_DICT_ALLOC

#define SLAVE_PBUF_INDEX_ALLOC() \
        void*               pvDataBuf   = NULL; \
        sMBSlaveDataTable*  psDataTable = NULL;  \
        uint16_t            usIndex     = 0;
        
//开始数据表申请 
#define SLAVE_BEGIN_DATA_BUF(BUF, TABLE) \
        usIndex = 0; \
        pvDataBuf   = (void*)BUF; \
        psDataTable = (sMBSlaveDataTable*)TABLE;
  
//保持寄存器数据申请  
#define SLAVE_REG_HOLD_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        vMBSlaveRegDataInit((sMBSlaveRegData*)pvDataBuf + usIndex, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
        usIndex++;
        
//输入寄存器数据申请  
#define SLAVE_REG_IN_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        vMBSlaveRegDataInit((sMBSlaveRegData*)pvDataBuf + usIndex, arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
        usIndex++;

//线圈数据申请  
#define SLAVE_COIL_BIT_DATA(arg1, arg2, arg3) \
        vMBSlaveBitDataInit((sMBSlaveBitData*)pvDataBuf + usIndex, arg1, arg2, arg3); \
        usIndex++;

//离散量数据申请  
#define SLAVE_DISC_BIT_DATA(arg1, arg2, arg3) \
        vMBSlaveBitDataInit((sMBSlaveBitData*)pvDataBuf + usIndex, arg1, arg2, arg3); \
        usIndex++;

//结束数据表申请  
#define SLAVE_END_DATA_BUF(usStartAddr, usEndAddr)\
        vMBSlaveDevDataTableInit(psDataTable, (void*)pvDataBuf, usStartAddr, \
                                  usEndAddr, usIndex);     \
        usIndex = 0;     
#endif

eMBErrorCode 
eMBSlaveRegInMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usAddress, sMBSlaveRegData** pvRegInValue);

eMBErrorCode 
eMBSlaveRegHoldMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usRegHoldAddr, sMBSlaveRegData** pvRegHoldValue);

eMBErrorCode 
eMBSlaveCoilsMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usCoilAddr, sMBSlaveBitData** pvCoilValue);

eMBErrorCode 
eMBSlaveDiscreteMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usDiscreteAddr, sMBSlaveBitData** pvDiscreteValue);

void vMBSlaveRegDataInit(sMBSlaveRegData* pData, USHORT usAddr, UCHAR ucDataType, LONG lMinVal, 
                         LONG lMaxVal, UCHAR ucAccessMode, float fTransmitMultiple, void* pvValue);                                 

void vMBSlaveBitDataInit(sMBSlaveBitData* pData, USHORT usAddr, UCHAR ucAccessMode, void* pvValue);

void vMBSlaveDevDataTableInit(sMBSlaveDataTable* pDataTable, void* pvDataBuf, USHORT usStartAddr, 
                              USHORT usEndAddr, USHORT usDataCount); 

#endif