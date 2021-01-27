#include "datamonitor.h"
#include "stdint.h"
#include "stddef.h"
#include <unistd.h>

sMonitorDataList* DataMonitor::g_psMonitorDataList = nullptr;
DataMonitor*      DataMonitor::g_pDataMonitor = nullptr;
uint16_t          DataMonitor::g_usMonitorID = 0;

Monitor::Monitor()
{

}

void Monitor::setValRange(int32_t iMaxVal, int32_t iMinVal)
{
    this->m_iMaxVal = iMaxVal;
    this->m_iMinVal = iMinVal;
}

void Monitor::setValType(eDataType emDataType)
{
    this->m_eDataType = emDataType;
}

void Monitor::setValue(uint32_t uiVal)
{
    uint8_t  ucValue = 0;
    uint16_t usValue = 0;

    int8_t   cValue  = 0;
    int16_t  sValue  = 0;

    if(m_pvVal != nullptr && uiVal <= m_iMaxVal && uiVal >= m_iMinVal)
    {
        if(m_eDataType == Uint8t)
        {
            ucValue = uiVal;
            m_uiDataVal = uint32_t(ucValue);
            *static_cast<uint8_t*>(m_pvVal) = ucValue;
        }
        else if(m_eDataType == Uint16t)
        {
            usValue = uiVal;
            m_uiDataVal = uint32_t(usValue);
            *static_cast<uint16_t*>(m_pvVal) = usValue;
        }
        else if(m_eDataType == Int8t)
        {
            cValue = uiVal;
            m_uiDataVal = uint32_t(cValue);
            *static_cast<int8_t*>(m_pvVal) = cValue;
        }
        else if(m_eDataType == Int16t)
        {
            sValue = uiVal;
            m_uiDataVal = uint32_t(sValue);
            *static_cast<int16_t*>(m_pvVal) = sValue;
        }
    }
}

uint32_t Monitor::getCurVal()
{
    return m_uiDataVal;
}

DataMonitor::DataMonitor()
{

}

DataMonitor* DataMonitor::getInstance()
{
    if(g_pDataMonitor == nullptr)
    {
        g_pDataMonitor = new DataMonitor();
    }
    return g_pDataMonitor;
}

void* DataMonitor::monitorPollTask(void *pvArg)
{
    uint8_t  ucValue = 0;
    uint16_t usValue = 0;

    int8_t   cValue  = 0;
    int16_t  sValue  = 0;
    uint32_t uiDataVal = 0;

    sMonitorList* pmMonitorList = static_cast<sMonitorList*>(pvArg);
    Monitor*      pMonitor = nullptr;

    while(1)
    {
        for(; pmMonitorList != nullptr; pmMonitorList = pmMonitorList->pNext)
        {
            pMonitor = pmMonitorList->pMonitor;
            if(pMonitor->m_eDataType == Uint8t)
            {
                ucValue = *static_cast<uint8_t*>(pMonitor->m_pvVal);
                uiDataVal = uint32_t(ucValue);
            }
            else if(pMonitor->m_eDataType == Uint16t)
            {
                usValue = *static_cast<uint16_t*>(pMonitor->m_pvVal);
                uiDataVal = uint32_t(usValue);
            }
            else if(pMonitor->m_eDataType == Int8t)
            {
                cValue = *static_cast<int8_t*>(pMonitor->m_pvVal);
                uiDataVal = uint32_t(cValue);
            }
            else if(pMonitor->m_eDataType == Int16t)
            {
                sValue = *static_cast<int16_t*>(pMonitor->m_pvVal);
                uiDataVal = uint32_t(sValue);
            }
            if(pMonitor->m_uiDataVal != uiDataVal)
            {
                emit pMonitor->valChanged(pMonitor->m_uiDataVal);
            }
        }
    }
}

Monitor* DataMonitor::monitorRegist(void* pvVal, eDataType eDataType, int32_t iMaxVal, int32_t iMinVal)
{
    uint8_t  ucValue;
    uint16_t usValue;

    int8_t   cValue;
    int16_t  sValue;

    Monitor*          pMonitor = new Monitor();
    sMonitorList*     pmMonitorList     = nullptr;
    sMonitorDataList* pmMonitorDataList = nullptr;

    if(pvVal == nullptr)
    {
        return nullptr;
    }
    if(g_usMonitorID/MONITOR_DATA_MAX_NUM == 0)
    {
        g_usMonitorID = 0;
        pmMonitorDataList = new sMonitorDataList;

        if(g_psMonitorDataList == nullptr)
        {
            g_psMonitorDataList = pmMonitorDataList;
        }
        else
        {
            g_psMonitorDataList->pLast->pNext = pmMonitorDataList;
        }
        g_psMonitorDataList->pLast = pmMonitorDataList;

        if( pthread_create(&pmMonitorDataList->sMonitorThread, nullptr, DataMonitor::monitorPollTask, static_cast<void*>(pmMonitorDataList->psMonitorList)) < 0 )
        {
            return nullptr;
        }
    }
    else
    {
        pmMonitorDataList = g_psMonitorDataList->pLast;
    }
    pMonitor->m_pvVal      = pvVal;
    pMonitor->m_iMaxVal    = iMaxVal;
    pMonitor->m_iMinVal    = iMinVal;
    pMonitor->m_eDataType  = eDataType;
    pMonitor->m_usDataId   = g_usMonitorID;

    if(eDataType == Uint8t)
    {
        ucValue = *static_cast<uint8_t*>(pvVal);
        pMonitor->m_uiDataVal = uint32_t(ucValue);
    }
    else if(eDataType == Uint16t)
    {
        usValue = *static_cast<uint16_t*>(pvVal);
        pMonitor->m_uiDataVal = uint32_t(usValue);
    }
    else if(eDataType == Int8t)
    {
        cValue = *static_cast<int8_t*>(pvVal);
        pMonitor->m_uiDataVal = uint32_t(cValue);
    }
    else if(eDataType == Int16t)
    {
        sValue = *static_cast<int16_t*>(pvVal);
        pMonitor->m_uiDataVal = uint32_t(sValue);
    }
    pmMonitorList->pMonitor = pMonitor;
    pmMonitorList->pNext = nullptr;

    if(pmMonitorDataList->psMonitorList == nullptr)
    {
        pmMonitorDataList->psMonitorList->pLast = pmMonitorList;
    }
    else
    {
        pmMonitorDataList->psMonitorList->pLast->pNext = pmMonitorList;
    }
    pmMonitorDataList->psMonitorList->pLast = pmMonitorList;

    g_usMonitorID++;
    return pMonitor;
}
