#include "datamonitor.h"
#include "stdint.h"
#include "stddef.h"
#include <unistd.h>

sMonitorMapList* DataMonitor::g_psMonitorMapList = nullptr;
DataMonitor*     DataMonitor::g_pDataMonitor = nullptr;
uint16_t         DataMonitor::g_usMonitorID = 0;

Monitor::Monitor(void* pvVal, DataType eDataType, uint16_t usDataId, int32_t iMaxVal, int32_t iMinVal)
{
    this->m_pvVal      = pvVal;
    this->m_iMaxVal    = iMaxVal;
    this->m_iMinVal    = iMinVal;
    this->m_DataType  =  eDataType;
    this->m_usDataId   = usDataId;
}

void Monitor::setValRange(int32_t iMaxVal, int32_t iMinVal)
{
    this->m_iMaxVal = iMaxVal;
    this->m_iMinVal = iMinVal;
}

void Monitor::setValType(DataType emDataType)
{
    this->m_DataType = emDataType;
}

void Monitor::setValue(uint32_t uiVal)
{
    uint8_t  ucValue = 0;
    uint16_t usValue = 0;

    int8_t   cValue  = 0;
    int16_t  sValue  = 0;

    if(m_pvVal != nullptr && uiVal <= uint32_t(m_iMaxVal) && uiVal >= uint32_t(m_iMinVal))
    {
        if(m_DataType == Uint8t)
        {
            ucValue = uiVal;
            m_uiDataVal = uint32_t(ucValue);
            *static_cast<uint8_t*>(m_pvVal) = ucValue;
        }
        else if(m_DataType == Uint16t)
        {
            usValue = uiVal;
            m_uiDataVal = uint32_t(usValue);
            *static_cast<uint16_t*>(m_pvVal) = usValue;
        }
        else if(m_DataType == Int8t)
        {
            cValue = uiVal;
            m_uiDataVal = uint32_t(cValue);
            *static_cast<int8_t*>(m_pvVal) = cValue;
        }
        else if(m_DataType == Int16t)
        {
            sValue = uiVal;
            m_uiDataVal = uint32_t(sValue);
            *static_cast<int16_t*>(m_pvVal) = sValue;
        }
    }
}

uint8_t Monitor::getValType()
{
    return m_DataType;
}


uint32_t Monitor::getCurVal()
{
    return m_uiDataVal;
}

void* Monitor::getCurValAddr()
{
    return m_pvVal;
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

    QMap<uint16_t, Monitor*>::iterator it;
    QMap<uint16_t, Monitor*>* pMonitorMap = static_cast< QMap<uint16_t, Monitor*> *>(pvArg);

    Monitor*      pMonitor = nullptr;

    while(1)
    {
        for(it=pMonitorMap->begin(); it!=pMonitorMap->end(); ++it)
        {
            pMonitor = it.value();
            if(pMonitor->m_DataType == Monitor::Uint8t)
            {
                ucValue = *static_cast<uint8_t*>(pMonitor->m_pvVal);
                uiDataVal = uint32_t(ucValue);
            }
            else if(pMonitor->m_DataType == Monitor::Uint16t)
            {
                usValue = *static_cast<uint16_t*>(pMonitor->m_pvVal);
                uiDataVal = uint32_t(usValue);
            }
            else if(pMonitor->m_DataType == Monitor::Int8t)
            {
                cValue = *static_cast<int8_t*>(pMonitor->m_pvVal);
                uiDataVal = uint32_t(cValue);
            }
            else if(pMonitor->m_DataType == Monitor::Int16t)
            {
                sValue = *static_cast<int16_t*>(pMonitor->m_pvVal);
                uiDataVal = uint32_t(sValue);
            }
            if(pMonitor->m_uiDataVal != uiDataVal)
            {
                pMonitor->m_uiDataVal = uiDataVal;
                emit pMonitor->valChanged(pMonitor);
            }
        }
    }
}

Monitor* DataMonitor::monitorRegist(void* pvVal, Monitor::DataType eDataType, int32_t iMaxVal, int32_t iMinVal)
{
    uint8_t  ucValue;
    uint16_t usValue;

    int8_t   cValue;
    int16_t  sValue;

    Monitor*         pMonitor = nullptr;
    sMonitorMapList* pmMonitorMapList = nullptr;

    if(pvVal == nullptr)
    {
        return nullptr;
    }
    if(g_usMonitorID/MONITOR_DATA_MAX_NUM == 0)
    {
        g_usMonitorID = 0;
        pmMonitorMapList = new sMonitorMapList;

        if(g_psMonitorMapList == nullptr)
        {
            g_psMonitorMapList = pmMonitorMapList;
        }
        else
        {
            g_psMonitorMapList->pLast->pNext = pmMonitorMapList;
        }
        g_psMonitorMapList->pLast = pmMonitorMapList;

        if( pthread_create(&pmMonitorMapList->sMonitorThread, nullptr, DataMonitor::monitorPollTask, static_cast<void*>(&pmMonitorMapList->m_MonitorMap)) < 0 )
        {
            return nullptr;
        }
    }
    else
    {
        pmMonitorMapList = g_psMonitorMapList->pLast;
    }

    if(pmMonitorMapList->m_MonitorMap.contains(pvVal))
    {
        return pmMonitorMapList->m_MonitorMap.value(pvVal);
    }
    else
    {
        pMonitor = new Monitor(pvVal, eDataType, g_usMonitorID, iMaxVal, iMinVal);
        pmMonitorMapList->m_MonitorMap.insert(pvVal,pMonitor);
    }
    if(pMonitor == nullptr)
    {
        return nullptr;
    }
    if(eDataType == Monitor::Uint8t)
    {
        ucValue = *static_cast<uint8_t*>(pvVal);
        pMonitor->m_uiDataVal = uint32_t(ucValue);
    }
    else if(eDataType == Monitor::Uint16t)
    {
        usValue = *static_cast<uint16_t*>(pvVal);
        pMonitor->m_uiDataVal = uint32_t(usValue);
    }
    else if(eDataType == Monitor::Int8t)
    {
        cValue = *static_cast<int8_t*>(pvVal);
        pMonitor->m_uiDataVal = uint32_t(cValue);
    }
    else if(eDataType == Monitor::Int16t)
    {
        sValue = *static_cast<int16_t*>(pvVal);
        pMonitor->m_uiDataVal = uint32_t(sValue);
    }
    g_usMonitorID++;
    return pMonitor;
}
