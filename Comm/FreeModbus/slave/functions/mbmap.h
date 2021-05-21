#ifndef _MB_MAP_H
#define _MB_MAP_H

#include "mb.h"
#include "mbdict.h"

#ifdef __cplusplus
extern "C" {
#endif

//主栈从设备字典申请 
#ifndef SLAVE_DEV_DICT_ALLOC
#define SLAVE_DEV_DICT_ALLOC

#if  MB_UCOSIII_ENABLED
#define SLAVE_PBUF_INDEX_ALLOC \
        void*               pvDataBuf   = NULL; \
        sMBSlaveDataTable*  psDataTable = NULL;  \
        uint16_t            usIndex     = 0;

#elif MB_LINUX_ENABLED
#define SLAVE_PBUF_INDEX_ALLOC \
        void*               pvDataBuf   = NULL; \
        sMBSlaveDataTable*  psDataTable = NULL;  \
        uint16_t            usIndex     = 0; \
        uint16_t*           pDaTableIndex = NULL;
#endif
        
//开始数据表申请
#if  MB_UCOSIII_ENABLED
#define SLAVE_BEGIN_DATA_BUF(BUF, TABLE) \
        usIndex = 0; \
        pvDataBuf   = BUF; \
        psDataTable = &TABLE;
#elif   MB_LINUX_ENABLED

#define SLAVE_BEGIN_DATA_BUF(BUF, TABLE, TABLE_INDEX) \
        usIndex = 0; \
        pvDataBuf   = BUF; \
        psDataTable = &TABLE; \
        pDaTableIndex = TABLE_INDEX;
#endif

#if MB_UCOSIII_ENABLED
//保持寄存器数据申请  
#define SLAVE_REG_HOLD_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        vMBSlaveRegDataInit((sMBSlaveRegData*)pvDataBuf + usIndex, arg1, arg2, arg3, arg4, arg5, arg6, (void*)&arg7); \
        usIndex++;
        
//输入寄存器数据申请  
#define SLAVE_REG_IN_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        vMBSlaveRegDataInit((sMBSlaveRegData*)pvDataBuf + usIndex, arg1, arg2, arg3, arg4, arg5, arg6, (void*)&arg7); \
        usIndex++;

//线圈数据申请  
#define SLAVE_COIL_BIT_DATA(arg1, arg2, arg3) \
        vMBSlaveBitDataInit((sMBSlaveBitData*)pvDataBuf + usIndex, arg1, arg2, (void*)&arg3); \
        usIndex++;

//离散量数据申请  
#define SLAVE_DISC_BIT_DATA(arg1, arg2, arg3) \
        vMBSlaveBitDataInit((sMBSlaveBitData*)pvDataBuf + usIndex, arg1, arg2, (void*)&arg3); \
        usIndex++;

//结束数据表申请  
#define SLAVE_END_DATA_BUF(usStartAddr, usEndAddr)\
        vMBSlaveDevDataTableInit(psDataTable, (void*)pvDataBuf, usStartAddr, \
                                  usEndAddr, usIndex);     \
        usIndex = 0;

#elif MB_LINUX_ENABLED
//保持寄存器数据申请
#define SLAVE_REG_HOLD_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        vMBSlaveRegDataInit(static_cast<sMBSlaveRegData*>(pvDataBuf) + usIndex, arg1, arg2, arg3, arg4, arg5, arg6, static_cast<void*>(&arg7)); \
        usIndex++; \
        if(pDaTableIndex != NULL) \
        pDaTableIndex[arg1] = usIndex;

//输入寄存器数据申请
#define SLAVE_REG_IN_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        vMBSlaveRegDataInit(static_cast<sMBSlaveRegData*>(pvDataBuf) + usIndex, arg1, arg2, arg3, arg4, arg5, arg6, static_cast<void*>(&arg8)); \
        usIndex++; \
        if(pDaTableIndex != NULL) \
        pDaTableIndex[arg1] = usIndex;

//线圈数据申请
#define SLAVE_COIL_BIT_DATA(arg1, arg2, arg3) \
        vMBSlaveBitDataInit(static_cast<sMBSlaveBitData*>(pvDataBuf) + usIndex, arg1, arg2, static_cast<void*>(&arg3)); \
        usIndex++; \
        if(pDaTableIndex != NULL) \
        pDaTableIndex[arg1] = usIndex;

//离散量数据申请
#define SLAVE_DISC_BIT_DATA(arg1, arg2, arg3) \
        vMBSlaveBitDataInit(static_cast<sMBSlaveBitData*>(pvDataBuf) + usIndex, arg1, arg2, static_cast<void*>(&arg3)); \
        usIndex++; \
        if(pDaTableIndex != NULL) \
        pDaTableIndex[arg1] = usIndex;

//结束数据表申请
#define SLAVE_END_DATA_BUF(usStartAddr, usEndAddr)\
        vMBSlaveDevDataTableInit(psDataTable, pvDataBuf, usStartAddr, usEndAddr, usIndex); \
        usIndex = 0;
#endif

#endif

eMBErrorCode 
eMBSlaveRegInMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usAddress, sMBSlaveRegData** pvRegInValue);

eMBErrorCode 
eMBSlaveRegHoldMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usRegHoldAddr, sMBSlaveRegData** pvRegHoldValue);

eMBErrorCode 
eMBSlaveCoilsMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usCoilAddr, sMBSlaveBitData** pvCoilValue);

eMBErrorCode 
eMBSlaveDiscreteMap(sMBSlaveInfo* psMBSlaveInfo, USHORT usDiscreteAddr, sMBSlaveBitData** pvDiscreteValue);

void vMBSlaveRegDataInit(sMBSlaveRegData* pData, USHORT usAddr, UCHAR ucDataType, USHORT usMinVal, 
                         USHORT usMaxVal, UCHAR ucAccessMode, UCHAR ucTmitMult, void* pvValue);                                 

void vMBSlaveBitDataInit(sMBSlaveBitData* pData, USHORT usAddr, UCHAR ucAccessMode, void* pvValue);

void vMBSlaveDevDataTableInit(sMBSlaveDataTable* pDataTable, void* pvDataBuf, USHORT usStartAddr,
                              USHORT usEndAddr, USHORT usDataCount) ;

#ifdef __cplusplus
}
#endif

#endif
