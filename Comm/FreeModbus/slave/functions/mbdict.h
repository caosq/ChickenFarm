#ifndef _MB_DICT_H
#define _MB_DICT_H

#include "mbconfig.h"
#include "mbframe.h"

#define SLAVE_PROTOCOL_TYPE_ID    0

typedef struct        /* 从栈寄存器数据结构 */
{
    USHORT  usAddr;        //地址
    USHORT  usMinVal;      //最小值
    USHORT  usMaxVal;      //最大值
    UCHAR   ucTmitMult;    //传输因子
    UCHAR   ucDataType;    //数据类型
    UCHAR   ucAccessMode;  //访问权限
    void*   pvValue;       //变量指针     
}sMBSlaveRegData;     		

typedef struct       /* 从栈线圈和离散量数据结构 */
{
    USHORT usAddr;        //地址
    UCHAR  ucAccessMode;  //访问权限
	UCHAR* pvValue;       //变量指针     
}sMBSlaveBitData;  

typedef struct   /* 从栈字典数据列表结构 */
{
    void*    pvDataBuf;             //协议数据域
	USHORT   usStartAddr;           //起始地址
	USHORT   usEndAddr;             //末尾地址
    USHORT   usDataCount;           //协议点位总数
}sMBSlaveDataTable;

typedef BOOL (*pxMBSlaveDataMapIndex)(eDataType eDataType, USHORT usAddr, USHORT* psIndex); //字典映射函数

typedef struct            /*从栈通讯字典数据结构*/  
{
#if MB_FUNC_READ_INPUT_ENABLED
    sMBSlaveDataTable      sMBRegInTable;       //输入寄存器数据表
#endif

#if MB_FUNC_WRITE_HOLDING_ENABLED || MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED \
    || MB_FUNC_READ_HOLDING_ENABLED || MB_FUNC_READWRITE_HOLDING_ENABLED

    sMBSlaveDataTable sMBRegHoldTable;     //保持寄存器数据表
#endif

#if MB_FUNC_READ_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED || MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED
    sMBSlaveDataTable sMBCoilTable;        //线圈数据表
#endif

#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED
    sMBSlaveDataTable      sMBDiscInTable;      //离散量数据表
#endif

#if MB_FUNC_READ_INPUT_ENABLED
    uint16_t  *pRegInIndex;       //输入寄存器数据域映射
#endif

#if MB_FUNC_WRITE_HOLDING_ENABLED || MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED \
    || MB_FUNC_READ_HOLDING_ENABLED || MB_FUNC_READWRITE_HOLDING_ENABLED

    uint16_t  *pRegHoldIndex;     //保持寄存器数据域映射
#endif

#if MB_FUNC_READ_COILS_ENABLED || MB_FUNC_WRITE_COIL_ENABLED || MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED
    uint16_t  *pBitCoilIndex;     //线圈数据域映射
#endif

#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
     uint16_t  *pBitDiscIndex;     //离散量数据表数据域映射
#endif
    pxMBSlaveDataMapIndex  pxSlaveDataMapIndex; //从栈字典映射函数
                              
}sMBSlaveCommData; 

typedef struct          /*从栈通讯参数信息*/   
{
    UCHAR              ucSlaveAddr;        //从栈通讯地址
    BOOL               xDataReady;         //数据是否准备好
    sMBSlaveCommData*  psSlaveCurData;     //从栈当前数据域
}sMBSlaveCommInfo; 

#endif
