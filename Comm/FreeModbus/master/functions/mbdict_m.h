#ifndef _USER_MB_DICT_M_H
#define _USER_MB_DICT_M_H

#include "mbframe.h"
#include "mbconfig.h"
#include "port.h"

#if MB_UCOSIII_ENABLED

#elif MB_LINUX_ENABLED

#include <time.h>

#endif
/* ----------------------Master Defines -------------------------------------*/

#define MB_HEART_BEAT_DELAY_MS    10   //主栈心跳延时

typedef enum      /* 测试模式 */        
{
	READ_REG_IN,
    READ_REG_HOLD,
	WRITE_REG_HOLD,
}eMasterCmdMode;

typedef enum   /*轮询模式*/
{
    SCAN_WRITE    = 0,     //轮询写
    SCAN_READ     = 1,     //轮询读
}eScanMode;

typedef struct        /* 主栈保持寄存器数据结构 */
{
	USHORT            usAddr;            //地址
    UCHAR             ucDataType;        //数据类型
    volatile USHORT   usPreVal;          //先前值
    LONG              lMinVal;           //最小值
    LONG              lMaxVal;           //最大值
    UCHAR             ucAccessMode;      //访问权限
    float             fTransmitMultiple; //传输因子
    void*             pvValue;           //变量指针    
}sMasterRegHoldData;     		

typedef struct        /* 主栈字典保持寄存器数据结构 */
{
	USHORT    usAddr;             //地址
    UCHAR     ucDataType;         //数据类型
    LONG      lMinVal;            //最小值
    LONG      lMaxVal;            //最大值
    UCHAR     ucAccessMode;       //访问权限
    float     fTransmitMultiple;  //传输因子
    void*     pvValue;            //变量指针    
}sMasterRegInData; 

typedef struct       /* 主栈字典线圈数据结构 */
{
    USHORT          usAddr;           //地址
    volatile UCHAR  ucPreVal;         //先前值
    UCHAR           ucAccessMode;     //访问权限
	UCHAR*          pvValue;          //变量指针   
}sMasterBitCoilData;  

typedef struct       /* 主栈字典离散量数据结构 */
{
    USHORT   usAddr;           //地址
    UCHAR    ucAccessMode;     //访问权限
	UCHAR*   pvValue;          //变量指针      
}sMasterBitDiscData;  

typedef struct   /* 用于测试从栈的指令数据结构 */       
{
    USHORT          usAddr;        //测试点位地址
    USHORT          usValue;       //数值
    eMasterCmdMode  eCmdMode;      //模式
    BOOL            xCheckVal;     //测试时比较数值
}sMBTestDevCmd; 

#if MB_MASTER_HEART_BEAT_ENABLED

typedef struct   /* 从设备心跳帧数据结构 */       
{
    USHORT          usAddr;             //点位地址
    eMasterCmdMode  eCmdMode;           //模式
    USHORT          usValue;            //数值
    USHORT          usHeartBeatPeriod;  //心跳间隔(s)
    BOOL            xHeartBeatEnable;   //使能
}sMBDevHeartBeat;  

#endif 

typedef struct   /* 主栈字典数据列表结构 */
{
    void*    pvDataBuf;             //协议数据域
	USHORT   usStartAddr;           //起始地址
	USHORT   usEndAddr;             //末尾地址
    USHORT   usDataCount;           //协议点位总数
}sMBDevDataTable;

typedef BOOL (*pxMBDevDataMapIndex)(eDataType eDataType, UCHAR ucProtocolID, USHORT usAddr, USHORT* psIndex); //字典映射函数

typedef struct sMBSlaveDevCommData   /* 从设备通讯字典数据结构 */  
{
	sMBDevDataTable      sMBRegInTable;       //输入寄存器数据表
	sMBDevDataTable      sMBRegHoldTable;     //保持寄存器数据表
	sMBDevDataTable      sMBCoilTable;        //线圈数据表
	sMBDevDataTable      sMBDiscInTable;      //离散量数据表
    sMBTestDevCmd        sMBDevCmdTable;      //用于测试从设备状态命令表
    
#if MB_MASTER_HEART_BEAT_ENABLED
    sMBDevHeartBeat      sMBDevHeartBeat;     //心跳帧
#endif     
    UCHAR                ucProtocolID;        //协议ID
    pxMBDevDataMapIndex  pxDevDataMapIndex;   //字典映射函数
    
    struct sMBSlaveDevCommData*   pNext;      //下一个数据表
}sMBSlaveDevCommData; 

typedef struct sMBSlaveDev   /* 从设备信息列表 */   
{
    UCHAR     ucProtocolID;          //协议ID
    UCHAR     ucDevAddr;             //设备通讯地址
	UCHAR     ucOfflineTimes;        //掉线次数
	BOOL      xOnLine;               //是否在线
	BOOL      xDataReady;            //数据是否准备好
	BOOL      xSynchronized;         //是否同步
    BOOL      xStateTestRequest;     //是否需要状态检测
    BOOL      xDevOnTimeout;         //是否处于延时
    
//    eScanMode eScanMode;             //当前轮询模式
    
#if  MB_UCOSIII_ENABLED
    OS_TMR  sDevOfflineTmr;          //设备掉线定时器

#if MB_MASTER_HEART_BEAT_ENABLED >0
    OS_TMR  sDevHeartBeatTmr;      //心跳间隔定时器
    BOOL    xDevHeartBeatRequest;  //心跳请求
#endif

#elif MB_LINUX_ENABLED
    timer_t sDevOfflineTmr;          //设备掉线定时器
#endif

    sMBSlaveDevCommData* psDevDataInfo;     //从设备数据域
    sMBSlaveDevCommData* psDevCurData;      //从设备当前数据域   
    
    struct sMBSlaveDev*  pNext;             //下一个设备节点
    struct sMBSlaveDev*  pLast;             //尾设备节点
    
    struct sMBMasterInfo* psMBMasterInfo;   //所属的主栈  
}sMBSlaveDev; 

typedef struct    /* 主栈从设备状态结构  */
{
    UCHAR         ucSlaveDevCount;    //从设备总数量        
    UCHAR         ucSlaveDevMinAddr;  //从设备最小通讯地址
	UCHAR         ucSlaveDevMaxAddr;  //从设备最大通讯地址
   
	sMBSlaveDev*  psMBSlaveDevsList;  //当前在线从设备列表
    sMBSlaveDev*  psMBSlaveDevCur;    //当前活动的设备
       
}sMBMasterDevsInfo; 

#endif
