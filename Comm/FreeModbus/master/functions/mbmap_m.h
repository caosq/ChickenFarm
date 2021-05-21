#ifndef _MB_MAP_M_H
#define _MB_MAP_M_H

#include "mb_m.h"

#ifdef __cplusplus
extern "C" {
#endif

//主栈从设备字典申请 
#ifndef MASTER_DEV_DICT_ALLOC
#define MASTER_DEV_DICT_ALLOC

#if  MB_UCOSIII_ENABLED
#define MASTER_PBUF_INDEX_ALLOC \
        void*             pvDataBuf   = NULL; \
        sMBDevDataTable*  psDataTable = NULL;  \
        uint16_t          usIndex     = 0;
#elif MB_LINUX_ENABLED

#define MASTER_DEV_INIT(sMBDevCommData) \
        void*             pvDataBuf   = NULL; \
        sMBDevDataTable*  psDataTable = NULL;  \
        uint16_t          usIndex     = 0; \
        uint16_t*         pDaTableIndex = NULL; \
        vMBMasterCommDataTableInit(&sMBDevCommData);
#endif

//开始数据表申请 
#if  MB_UCOSIII_ENABLED
#define MASTER_BEGIN_DATA_BUF(BUF, TABLE) \
        usIndex = 0; \
        pvDataBuf   = BUF; \
        psDataTable = &TABLE;

#elif   MB_LINUX_ENABLED

#define MASTER_BEGIN_DATA_BUF(BUF, TABLE, TABLE_INDEX) \
        usIndex = 0; \
        pvDataBuf   = BUF; \
        psDataTable = &TABLE; \
        pDaTableIndex = TABLE_INDEX;
#endif

#if MB_UCOSIII_ENABLED
//保持寄存器数据申请  
#define MASTER_REG_HOLD_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
        vMBMasterDevRegHoldDataInit((sMasterRegHoldData*)pvDataBuf + usIndex, \
        arg1, arg2, (USHORT)arg3, (USHORT)arg4, (USHORT)arg5, arg6, arg7, (void*)&arg8); \
        usIndex++;
        
//输入寄存器数据申请  
#define MASTER_REG_IN_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        vMBMasterDevRegInDataInit((sMasterRegInData*)pvDataBuf + usIndex, \
        arg1, arg2, (USHORT)arg3, (USHORT)arg4, arg5, arg6, (void*)&arg7); \
        usIndex++;
        
//线圈数据申请  
#define MASTER_COIL_BIT_DATA(arg1, arg2, arg3, arg4) \
        vMBMasterDevCoilDataInit((sMasterBitCoilData*)pvDataBuf + usIndex, \
        arg1, arg2, arg3, (void*)&arg4); \
        usIndex++;
        
//离散量数据申请  
#define MASTER_DISC_BIT_DATA(arg1, arg2, arg3) \
        vMBMasterDevDiscDataInit((sMasterBitDiscData*)pvDataBuf + usIndex, arg1, arg2, (void*)&arg3); \
        usIndex++;
        
//结束数据表申请  
#define MASTER_END_DATA_BUF(usStartAddr, usEndAddr)\
        vMBMasterDevDataTableInit(psDataTable, pvDataBuf, usStartAddr, usEndAddr, usIndex);     \
        usIndex = 0; 
 
//测试命令初始化申请  
#define MASTER_TEST_CMD_INIT(pCmd, arg1, arg2, arg3, arg4) \
        vMBMasterDevTestCmdInit(pCmd, arg1, arg2, arg3, arg4);

#if MB_MASTER_HEART_BEAT_ENABLED > 0

//心跳帧初始化申请  
#define MASTER_HEART_BEAT_INIT(psDevHeartBeat, arg1, arg2, arg3, arg4, arg5) \
        vMBMasterDevHeartBeatInit((sMBDevHeartBeat*)psDevHeartBeat, \
        arg1, arg2, arg3, arg4, arg5);
#endif

#elif MB_LINUX_ENABLED

//保持寄存器数据申请
#define MASTER_REG_HOLD_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
        vMBMasterDevRegHoldDataInit(static_cast<sMasterRegHoldData*>(pvDataBuf) + usIndex, \
        arg1, arg2, static_cast<USHORT>(arg3), static_cast<USHORT>(arg4), static_cast<USHORT>(arg5), \
        arg6, arg7, static_cast<void*>(&arg8)); \
        usIndex++; \
        if(pDaTableIndex != NULL) \
        pDaTableIndex[arg1] = usIndex;

//输入寄存器数据申请
#define MASTER_REG_IN_DATA(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
        vMBMasterDevRegInDataInit(static_cast<sMasterRegInData*>(pvDataBuf) + usIndex, \
        arg1, arg2, static_cast<USHORT>(arg3), static_cast<USHORT>(arg4), arg5, arg6, static_cast<void*>(&arg7)); \
        pDaTableIndex[arg1] = usIndex; \
        usIndex++; \
        if(pDaTableIndex != NULL) \
        pDaTableIndex[arg1] = usIndex;

//线圈数据申请
#define MASTER_COIL_BIT_DATA(arg1, arg2, arg3, arg4) \
        vMBMasterDevCoilDataInit(static_cast<sMasterBitCoilData*>(pvDataBuf) + usIndex, \
        arg1, arg2, arg3, static_cast<void*>(&arg4)); \
        usIndex++; \
        if(pDaTableIndex != NULL) \
        pDaTableIndex[arg1] = usIndex;

//离散量数据申请
#define MASTER_DISC_BIT_DATA(arg1, arg2, arg3) \
        vMBMasterDevDiscDataInit(static_cast<sMasterBitDiscData*>(pvDataBuf) + usIndex, \
        arg1, arg2, static_cast<void*>(&arg3)); \
        usIndex++; \
        if(pDaTableIndex != NULL) \
        pDaTableIndex[arg1] = usIndex;

//结束数据表申请
#define MASTER_END_DATA_BUF(usStartAddr, usEndAddr)\
        vMBMasterDevDataTableInit(psDataTable, pvDataBuf, usStartAddr, usEndAddr, usIndex);  \
        usIndex = 0;

//测试命令初始化申请
#define MASTER_TEST_CMD_INIT(pCmd, arg1, arg2, arg3, arg4) \
        vMBMasterDevTestCmdInit(&pCmd, arg1, arg2, arg3, arg4);

#if MB_MASTER_HEART_BEAT_ENABLED > 0

//心跳帧初始化申请
#define MASTER_HEART_BEAT_INIT(psDevHeartBeat, arg1, arg2, arg3, arg4, arg5) \
        vMBMasterDevHeartBeatInit(static_cast<sMBDevHeartBeat*>(psDevHeartBeat), arg1, arg2, arg3, arg4, arg5);
#endif

#endif

#endif

typedef eMBMasterReqErrCode (*psMBDevDataMapIndex)(eDataType eDataType, UCHAR ucProtocolID,
                                                   USHORT usAddr, USHORT* psIndex); //字典映射函数


eMBMasterReqErrCode eMBMasterRegInMap(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, 
                                      USHORT usRegAddr, sMasterRegInData ** pvRegInValue);

eMBMasterReqErrCode eMBMasterRegHoldingMap(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, 
                                           USHORT usRegAddr, sMasterRegHoldData ** pvRegHoldValue);

eMBMasterReqErrCode eMBMasterCoilMap(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, 
                                     USHORT usCoilAddr, sMasterBitCoilData ** pvCoilValue);

eMBMasterReqErrCode eMBMasterDiscreteMap(sMBMasterInfo* psMBMasterInfo, UCHAR ucSndAddr, 
                                         USHORT usDiscreteAddr, sMasterBitDiscData ** pvDiscreteValue);


void vMBMasterDevRegHoldDataInit(sMasterRegHoldData* pData, USHORT usAddr, UCHAR ucDataType, USHORT usMinVal, USHORT usMaxVal, 
                                 USHORT usPreVal, UCHAR ucAccessMode, UCHAR ucTmitMult, void* pvValue);

void vMBMasterDevRegInDataInit(sMasterRegInData* pData, USHORT usAddr, UCHAR ucDataType, USHORT usMinVal, 
                               USHORT usMaxVal, UCHAR ucAccessMode, UCHAR ucTmitMult, void* pvValue);   
                              
                              
void vMBMasterDevCoilDataInit(sMasterBitCoilData* pData, USHORT usAddr, 
                              UCHAR ucPreVal, UCHAR ucAccessMode, void* pvValue);

void vMBMasterDevDiscDataInit(sMasterBitDiscData* pData, USHORT usAddr, UCHAR ucAccessMode, void* pvValue);

void vMBMasterDevTestCmdInit(sMBTestDevCmd* pCmd, USHORT usAddr, eMasterCmdMode eCmdMode ,USHORT usValue, BOOL xCheckVal); 

#if MB_MASTER_HEART_BEAT_ENABLED
void vMBMasterDevHeartBeatInit(sMBDevHeartBeat* psDevHeartBeat, USHORT usAddr, eMasterCmdMode eCmdMode, 
                               USHORT usValue, USHORT usHeartBeatPeriod, BOOL xHeartBeatEnable);  
#endif

void vMBMasterDevDataTableInit(sMBDevDataTable* pDataTable, void* pvDataBuf, 
                               USHORT usStartAddr, USHORT usEndAddr, USHORT usDataCount); 

void vMBMasterCommDataTableInit(sMBSlaveDevCommData *pDevCommData);

#ifdef __cplusplus
}
#endif

#endif
