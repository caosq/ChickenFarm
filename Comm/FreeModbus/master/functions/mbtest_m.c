#include <string.h>

#include "mbrtu_m.h"
#include "mbtest_m.h"
#include "mbfunc_m.h"

#if MB_UCOSIII_ENABLED

#elif MB_LINUX_ENABLED
#include <signal.h>
#include <time.h>
#endif


#define MB_MASTER_DEV_OFFLINE_TMR_S      10
#define MB_TEST_RETRY_TIMES              2
#define MB_TEST_OFFLINE_TIMES            2

/**********************************************************************
 * @brief   从设备定时器中断
 * @param   *p_tmr     定时器
 * @param   *p_arg     参数
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
#if  MB_UCOSIII_ENABLED
void vMBMasterDevOfflineTimeout(void * p_tmr, void * p_arg)
{
    sMBSlaveDev* psMBSlaveDev = (sMBSlaveDev*)p_arg;
    psMBSlaveDev->xDevOnTimeout = FALSE; 
//    debug("vMBMasterDevOfflineTimeout  ucDevAddr %d \n", psMBSlaveDev->ucDevAddr);
}
#elif MB_LINUX_ENABLED
void vMBMasterDevOfflineTimeout(union sigval v)
{
    sMBSlaveDev* psMBSlaveDev = (sMBSlaveDev*)v.sival_ptr;
    psMBSlaveDev->xDevOnTimeout = FALSE; 
//    debug("vMBMasterDevOfflineTimeout  ucDevAddr %d \n", psMBSlaveDev->ucDevAddr);
}
#endif
/**********************************************************************
 * @brief   从设备定时器使能
 * @param   psMBDev   从设备状态
 * @return	none
 *********************************************************************/
BOOL xMBMasterDevOfflineTmrEnable(sMBSlaveDev* psMBDev)
{
#if  MB_UCOSIII_ENABLED
    OS_STATE  sTmrState;
    OS_ERR err = OS_ERR_NONE;
	OS_TICK i = 0;
    
    sTmrState = OSTmrStateGet(&psMBDev->sDevOfflineTmr, &err);
    if(sTmrState == OS_TMR_STATE_UNUSED)
    { 
        i = (OS_TICK)(MB_MASTER_DEV_OFFLINE_TMR_S * TMR_TICK_PER_SECOND);  //延时10s
        
        OSTmrCreate(&psMBDev->sDevOfflineTmr, "sDevOfflineTmr", i, 0, OS_OPT_TMR_ONE_SHOT, 
                    vMBMasterDevOfflineTimeout, (void*)psMBDev, &err);//从设备定时器
    }
    if(sTmrState != OS_TMR_STATE_RUNNING)
    {
        (void)OSTmrStart(&psMBDev->sDevOfflineTmr, &err); 
    }
    psMBDev->xDevOnTimeout = TRUE;
    return (err == OS_ERR_NONE);

#elif MB_LINUX_ENABLED
    int32_t ret;
    struct sigevent evp;
    struct itimerspec ts;

    memset (&evp, 0, sizeof(evp));
    evp.sigev_value.sival_ptr = psMBDev;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = vMBMasterDevOfflineTimeout;
    evp.sigev_value.sival_int = 0; //作为handle()的参数

    ret = timer_create(CLOCK_REALTIME, &evp, &psMBDev->sDevOfflineTmr);

    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = MB_MASTER_DEV_OFFLINE_TMR_S;
    ts.it_value.tv_nsec = 0;
    ret = timer_settime(psMBDev->sDevOfflineTmr, TIMER_ABSTIME, &ts, NULL);
    return ret == 0;
#endif
}

/**********************************************************************
 * @brief   主栈对从设备发送命令
 * @param   psMBMasterInfo  主栈信息块
 * @param   psMBSlaveDev    从设备
 * @param   ucSlaveAddr     从设备地址
 * @return	eMBMasterReqErrCode
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
eMBMasterReqErrCode 
eMBDevCmdTest(sMBMasterInfo* psMBMasterInfo, const sMBSlaveDev* psMBSlaveDev, const sMBTestDevCmd* psMBDevCmd)
{
    eMBMasterReqErrCode errorCode = MB_MRE_EILLSTATE;
   
    if(psMBDevCmd == NULL)
    {
        return MB_MRE_ILL_ARG;
    }
    psMBMasterInfo->eMBRunMode = STATE_TEST_DEV; //接口处于测试从设备状态
    if(psMBDevCmd->eCmdMode == WRITE_REG_HOLD)
    {
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0 
        errorCode = eMBMasterReqWriteHoldingRegister(psMBMasterInfo, psMBSlaveDev->ucDevAddr, psMBDevCmd->usAddr, 
                                                     psMBDevCmd->usValue, MB_MASTER_WAITING_DELAY);   //测试从设备
#endif						
    }   
    if(psMBDevCmd->eCmdMode == READ_REG_HOLD)
    {
#if MB_FUNC_READ_HOLDING_ENABLED > 0 
        errorCode = eMBMasterReqReadHoldingRegister(psMBMasterInfo, psMBSlaveDev->ucDevAddr, psMBDevCmd->usAddr, 1, MB_MASTER_WAITING_DELAY);   //测试从设备
#endif						
    }
    if(psMBDevCmd->eCmdMode == READ_REG_IN)
    {				
#if MB_FUNC_READ_INPUT_ENABLED > 0						
        errorCode = eMBMasterReqReadInputRegister(psMBMasterInfo, psMBSlaveDev->ucDevAddr, psMBDevCmd->usAddr, 1, MB_MASTER_WAITING_DELAY);     //测试从设备
#endif						
    }
    psMBMasterInfo->eMBRunMode = STATE_SCAN_DEV;  //退出测试从设备状态

//    debug("eMBDevCmdTest ucDevAddr %d errorCode %d\n", ucSlaveAddr, errorCode);
    return errorCode;
}

/**********************************************************************
 * @brief  主栈对从设备未知状态进行测试
 * @param  psMBMasterInfo  主栈信息块
 * @param  psMBSlaveDev    某从设备状态
 * @param  ucSlaveAddr      从设备地址
 * @return sMBSlaveDev
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
void vMBDevTest(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev)
{
    USHORT    n,usDataVal;

    UCHAR*               pcPDUDataCur = NULL;
    sMBSlaveDevCommData* psMBDevData  = NULL;  //某从设备数据域
    const sMBTestDevCmd* psMBCmd      = NULL;  //某从设备测试命令表

    eMBMasterReqErrCode  errorCode    = MB_MRE_EILLSTATE;

    UCHAR ucMaxAddr = psMBMasterInfo->sMBDevsInfo.ucSlaveDevMaxAddr;
    UCHAR ucMinAddr = psMBMasterInfo->sMBDevsInfo.ucSlaveDevMinAddr;
    
    if(psMBSlaveDev->xDevOnTimeout == TRUE || psMBSlaveDev->ucDevAddr < ucMinAddr || psMBSlaveDev->ucDevAddr > ucMaxAddr)  
    {
        return;   //处于掉线延时测试,则放弃本次测试
    }
    psMBMasterInfo->eMBRunMode = STATE_TEST_DEV;  //接口处于测试从设备状态

    for(psMBDevData = psMBSlaveDev->psDevDataInfo; psMBDevData != NULL; psMBDevData = psMBDevData->pNext)  
    {
        psMBCmd = &psMBDevData->sMBDevCmdTable;
        if(psMBCmd == NULL)
        {
            continue;
        }
        for(n=0; n<MB_TEST_RETRY_TIMES; n++)
        {
    	    errorCode = eMBDevCmdTest(psMBMasterInfo, psMBSlaveDev, psMBCmd);
            if(errorCode == MB_MRE_NO_ERR)
            {
                break;  //证明从设备有反应
            }
        }
        if(errorCode == MB_MRE_NO_ERR) //证明从设备有反应
        {
            pcPDUDataCur = psMBMasterInfo->pucMasterPDUCur + MB_PDU_VALUE_OFF;  //接收帧的数据域
            
            usDataVal  = ( (USHORT)(*pcPDUDataCur++) ) << 8;                  //数据
            usDataVal |= ( (USHORT)(*pcPDUDataCur++) ) & 0xFF;
            
            if(psMBCmd->xCheckVal)  //测试时检测数值
            {
                if(usDataVal == psMBCmd->usValue)
                {
                    psMBSlaveDev->xOnLine           = TRUE;                       //从设备反馈正确，则设备在线
                    psMBSlaveDev->psDevCurData      = psMBDevData;                //从设备当前数据域
                    psMBSlaveDev->ucProtocolID      = psMBDevData->ucProtocolID;  //从设备协议ID
                    psMBSlaveDev->xDataReady        = TRUE;                       //从设备数据准备好
                    psMBSlaveDev->xStateTestRequest = FALSE;                      //状态测试请求                  
                }
                else
                {
                    psMBSlaveDev->xDataReady   = FALSE;
                }
            }
            else
            {
                psMBSlaveDev->xOnLine           = TRUE;                       //从设备反馈正确，则设备在线
                psMBSlaveDev->psDevCurData      = psMBDevData;                //从设备当前数据域
                psMBSlaveDev->ucProtocolID      = psMBDevData->ucProtocolID;  //从设备协议ID
                psMBSlaveDev->xDataReady        = TRUE;                       //从设备数据准备好
                psMBSlaveDev->xStateTestRequest = FALSE;                      //状态测试请求              
            }    
            break; 				
        } 
    }
//    debug("vMBDevTest  ucDevAddr %d errorCode %d\n", psMBSlaveDev->ucDevAddr, errorCode);
    if(errorCode != MB_MRE_NO_ERR) //证明从设备无反应
    {
        (void)xMBMasterDevOfflineTmrEnable(psMBSlaveDev);  
    }
}

/**********************************************************************
 * @brief   主栈对从设备当前状态测试
 * @param   psMBMasterInfo  主栈信息块
 * @param   psMBSlaveDev    从设备
 * @return	none
 * @author  laoc
 * @date    2019.01.22
 *********************************************************************/
void vMBDevCurStateTest(sMBMasterInfo* psMBMasterInfo, sMBSlaveDev* psMBSlaveDev)
{
    USHORT  n, usDataVal;
    
    UCHAR*                pcPDUDataCur = NULL;
    const sMBTestDevCmd*       psMBCmd = NULL;
    eMBMasterReqErrCode  errorCode = MB_MRE_EILLSTATE;

    UCHAR ucMaxAddr = psMBMasterInfo->sMBDevsInfo.ucSlaveDevMaxAddr;
    UCHAR ucMinAddr = psMBMasterInfo->sMBDevsInfo.ucSlaveDevMinAddr;
    
    if( psMBSlaveDev == NULL || psMBSlaveDev->xDevOnTimeout == TRUE || 
        psMBSlaveDev->ucDevAddr < ucMinAddr || psMBSlaveDev->ucDevAddr > ucMaxAddr)  
    {
        return;   //处于掉线延时测试,则放弃本次测试
    }
    psMBCmd = &psMBSlaveDev->psDevCurData->sMBDevCmdTable;  //从设备命令列表
    if(psMBCmd == NULL)
    {
         return;
    }
    /****************************测试设备**********************************/
    psMBMasterInfo->eMBRunMode = STATE_TEST_DEV;  //接口处于测试从设备状态
    
    for(n=0; n<MB_TEST_RETRY_TIMES; n++)
    {
        errorCode = eMBDevCmdTest(psMBMasterInfo, psMBSlaveDev, psMBCmd);	
        if(errorCode == MB_MRE_NO_ERR) 
        {
            break; //证明从设备有反应
        }        
    }
    if(errorCode == MB_MRE_NO_ERR) //证明从设备有反应
    {
        pcPDUDataCur = psMBMasterInfo->pucMasterPDUCur + MB_PDU_VALUE_OFF;  //当前帧的数据域

        usDataVal  = ( (USHORT)(*pcPDUDataCur++) ) << 8;   //数据
        usDataVal |= ( (USHORT)(*pcPDUDataCur++) ) & 0xFF;
        
        if(psMBCmd->xCheckVal)  //测试时检测数值
        {
            if(usDataVal == psMBCmd->usValue)
            {
                psMBSlaveDev->xOnLine           = TRUE;  //从设备反馈正确，则设备在线
                psMBSlaveDev->xDataReady        = TRUE;  //数据准备好
                psMBSlaveDev->xStateTestRequest = FALSE; //状态测试请求
                psMBSlaveDev->ucOfflineTimes    = 0;     //测试次数清零
            }
            else
            {
                psMBSlaveDev->xDataReady     = FALSE;
            }
        }
        else
        {
            psMBSlaveDev->xOnLine        = TRUE;  //从设备反馈正确，则设备在线
            psMBSlaveDev->xDataReady     = TRUE;
            psMBSlaveDev->ucOfflineTimes = 0;     //测试次数清零
        }
//        debug("vMBDevCurStateTest  ucDevAddr %d  xOnLine %d xDataReady %d xSynchronized %d xStateTestRequest %d \n",
//        psMBSlaveDev->ucDevAddr,  psMBSlaveDev->xOnLine,  psMBSlaveDev->xDataReady, psMBSlaveDev->xSynchronized, psMBSlaveDev->xStateTestRequest);
    }
    else  //多次测试仍返回错误
    {
        psMBSlaveDev->xDataReady = FALSE;     
        if(psMBSlaveDev->ucOfflineTimes == MB_TEST_OFFLINE_TIMES)  //前几个周期测试都报故障
        {
            psMBSlaveDev->xOnLine        = FALSE;    //从设备掉线
            psMBSlaveDev->xDataReady     = FALSE;    //从设备准备置位
            psMBSlaveDev->xSynchronized  = FALSE;    //从设备同步置位
            psMBSlaveDev->ucOfflineTimes = 0;        //测试次数清零
           // debug("vMBDevCurStateTest  ucDevAddr %d errorCode %d\n", psMBSlaveDev->ucDevAddr, errorCode);
        }
        else
        {
            psMBSlaveDev->ucOfflineTimes++;
            (void)xMBMasterDevOfflineTmrEnable(psMBSlaveDev);
        }
    }
    psMBMasterInfo->eMBRunMode = STATE_SCAN_DEV;  //退出测试从设备状态    
}
