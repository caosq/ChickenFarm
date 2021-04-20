#include "datamonitor.h"
#include "stdint.h"
#include "stddef.h"
#include <unistd.h>
#include "system.h"

#define SYNC_FILE   "/home/root/.DataSync"
#define MONITOR_DATA_MAX_NUM   100     //最大可监控点位数，根据实际情况调整

Monitor::Monitor(void* pvVal, DataType eDataType, uint32_t uiDataId, int32_t iMaxVal, int32_t iMinVal, bool xSyncMode)
{
    this->m_pvVal     = pvVal;
    this->m_iMaxVal   = iMaxVal;
    this->m_iMinVal   = iMinVal;
    this->m_DataType  = eDataType;
    this->m_uiDataId  = uiDataId;
    this->m_xSyncMode = xSyncMode;
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

void Monitor::setValue(int32_t iVal)
{
    bool     xValue = 0;
    uint8_t  ucValue = 0;
    uint16_t usValue = 0;
    uint32_t uiValue  = 0;

    int8_t   cValue  = 0;
    int16_t  sValue  = 0;
    int32_t  iValue  = 0;

    if(m_pvVal != nullptr && iVal <= m_iMaxVal && iVal >= m_iMinVal)
    {
        if(m_DataType == Uint8t)
        {
            ucValue = uint8_t(iVal);

            if(m_iDataVal == int32_t(ucValue)){return;}
            m_iDataVal = int32_t(ucValue);
            *static_cast<uint8_t*>(m_pvVal) = ucValue;
        }
        else if(m_DataType == Uint16t)
        {
            usValue = uint16_t(iVal);

            if(m_iDataVal == int32_t(usValue)){return;}
            m_iDataVal = int32_t(usValue);
            *static_cast<uint16_t*>(m_pvVal) = usValue;
        }
        else if(m_DataType == Int8t)
        {
            cValue = int8_t(iVal);

            if(m_iDataVal == int32_t(cValue)){return;}
            m_iDataVal = int32_t(cValue);
            *static_cast<int8_t*>(m_pvVal) = cValue;
        }
        else if(m_DataType == Int16t)
        {
            sValue = int16_t(iVal);

            if(m_iDataVal == int32_t(sValue)){return;}
            m_iDataVal = int32_t(sValue);
            *static_cast<int16_t*>(m_pvVal) = sValue;
        }
        else if(m_DataType == Int32t)
        {
            iValue = int32_t(iVal);

            if(m_iDataVal == int32_t(iValue)){return;}
            m_iDataVal = int32_t(iValue);
            *static_cast<int32_t*>(m_pvVal) = iValue;
        }
        else if(m_DataType == Uint32t)
        {
            uiValue = uint32_t(iVal);

            if(m_iDataVal == int32_t(uiValue)){return;}
            m_iDataVal = int32_t(uiValue);
            *static_cast<uint32_t*>(m_pvVal) = uiValue;
        }
        else if(m_DataType == Boolean)
        {
            xValue = bool(iVal);

            if(m_iDataVal == int32_t(xValue)){return;}
            m_iDataVal = int32_t(xValue);
            *static_cast<bool*>(m_pvVal) = xValue;
        }
        if(m_xSyncMode)
        {
            DataMonitor::readFile()->setValue(QString::number(m_uiDataId), m_iDataVal);
        }
        emit valChanged(this);
    }
}

uint8_t Monitor::getValType()
{
    return m_DataType;
}


int32_t Monitor::getCurVal()
{
    return m_iDataVal;
}

void* Monitor::getValueAddr()
{
    return m_pvVal;
}

uint32_t Monitor::getDataId()
{
    return m_uiDataId;
}

QMap<void*, Monitor*>  DataMonitor::g_Monitors;
sMonitorMapList*       DataMonitor::g_psMonitorMapList = nullptr;
//DataMonitor*           DataMonitor::g_pDataMonitor = nullptr;
DataSave *             DataMonitor::g_pSyncFile = nullptr;
uint16_t               DataMonitor::g_usMonitorID = 0;

DataMonitor::DataMonitor(QObject *parent) :
     QObject(parent)
{
}

DataSave* DataMonitor::readFile()
{
    if(g_pSyncFile == nullptr)
    {
        g_pSyncFile = new DataSave(SYNC_FILE, QSettings::IniFormat);
    }
    return g_pSyncFile;
}

void* DataMonitor::monitorPollTask(void *pvArg)
{
    bool     xValue;
    uint8_t  ucValue = 0;
    uint16_t usValue = 0;

    int8_t   cValue  = 0;
    int16_t  sValue  = 0;
    uint32_t uiValue = 0;
    int32_t  iValue = 0;

    int32_t iDataVal = 0;

    QMap<uint16_t, Monitor*>::iterator it;
    QMap<uint16_t, Monitor*>* pMonitorMap = static_cast< QMap<uint16_t, Monitor*> *>(pvArg);

    Monitor* pMonitor = nullptr;
    while(pMonitorMap != nullptr)
    {
        usleep(1000);
        for(it=pMonitorMap->begin(); it!=pMonitorMap->end(); ++it)
        {
            pMonitor = it.value();
            if(pMonitor == nullptr){continue;}
            if(pMonitor->m_DataType == Monitor::Uint8t)
            {
                ucValue = *static_cast<uint8_t*>(pMonitor->m_pvVal);
                iDataVal = int32_t(ucValue);
            }
            else if(pMonitor->m_DataType == Monitor::Uint16t)
            {
                usValue = *static_cast<uint16_t*>(pMonitor->m_pvVal);
                iDataVal = int32_t(usValue);
            }
            else if(pMonitor->m_DataType == Monitor::Uint32t)
            {
                uiValue = *static_cast<uint32_t*>(pMonitor->m_pvVal);
                iDataVal = int32_t(uiValue);
            }
            else if(pMonitor->m_DataType == Monitor::Int8t)
            {
                cValue = *static_cast<int8_t*>(pMonitor->m_pvVal);
                iDataVal = int32_t(cValue);
            }
            else if(pMonitor->m_DataType == Monitor::Int16t)
            {
                sValue = *static_cast<int16_t*>(pMonitor->m_pvVal);
                iDataVal = int32_t(sValue);
            }
            else if(pMonitor->m_DataType == Monitor::Int32t)
            {
                iValue = *static_cast<int32_t*>(pMonitor->m_pvVal);
                iDataVal = int32_t(iValue);
            }
            else if(pMonitor->m_DataType == Monitor::Boolean)
            {
                xValue = *static_cast<bool*>(pMonitor->m_pvVal);
                iDataVal = int32_t(xValue);
            }

            if(pMonitor->m_iDataVal != iDataVal)
            {
                if(pMonitor->m_xSyncMode)
                {
                    DataMonitor::readFile()->setValue(QString::number(pMonitor->getDataId()), iDataVal);
                }
                pMonitor->m_iDataVal = iDataVal;
                emit pMonitor->valChanged(pMonitor);

                qDebug("pMonitor->valChanged %d ID %d pMonitorMap %d \n", iDataVal, pMonitor->getDataId(), pMonitorMap);
            }
        }
    }
}

Monitor* DataMonitor::monitorRegist(void* pvVal, Monitor::DataType eDataType, int32_t iMaxVal, int32_t iMinVal, bool xSyncMode)
{
    bool     xValue = 0;
    uint8_t  ucValue = 0;
    uint16_t usValue = 0;

    int8_t   cValue = 0;
    int16_t  sValue = 0;

    int32_t  iValue = 0;
    uint32_t uiValue = 0;

    Monitor          *pMonitor = nullptr;
    DataSave         *pSyncFile = nullptr;
    sMonitorMapList* pmMonitorMapList = nullptr;

    if(pvVal == nullptr){return nullptr;}
    if(g_Monitors.contains(pvVal))
    {
        return g_Monitors.value(pvVal);
    }
    if(g_usMonitorID == 0)
    {
        pmMonitorMapList = new sMonitorMapList;
        g_psMonitorMapList = pmMonitorMapList;
        g_psMonitorMapList->pLast = pmMonitorMapList;

        if( pthread_create(&pmMonitorMapList->sMonitorThread, nullptr, DataMonitor::monitorPollTask,
                           static_cast<void*>(&pmMonitorMapList->m_MonitorMap)) < 0 )
        {
            return nullptr;
        }
    }
    else if(g_usMonitorID % MONITOR_DATA_MAX_NUM != 0)
    {
        pmMonitorMapList = g_psMonitorMapList->pLast;
        if(pmMonitorMapList->m_MonitorMap.contains(pvVal))
        {
            return pmMonitorMapList->m_MonitorMap.value(pvVal);
        }
    }
    else if(g_usMonitorID % MONITOR_DATA_MAX_NUM == 0)
    {
        pmMonitorMapList = new sMonitorMapList;
        g_psMonitorMapList->pLast->pNext = pmMonitorMapList;
        g_psMonitorMapList->pLast = pmMonitorMapList;

        if( pthread_create(&pmMonitorMapList->sMonitorThread, nullptr, DataMonitor::monitorPollTask,
                           static_cast<void*>(&pmMonitorMapList->m_MonitorMap)) < 0 )
        {
            return nullptr;
        }
    }
    pMonitor = new Monitor(pvVal, eDataType, g_usMonitorID, iMaxVal, iMinVal, xSyncMode);

    if(pMonitor == nullptr){return nullptr;}
    if(xSyncMode){
        pSyncFile = DataMonitor::readFile();
    }
    if(eDataType == Monitor::Uint8t)
    {
        if(xSyncMode)
        {
            if(pSyncFile->contains(QString::number(g_usMonitorID)))
            {
                ucValue = pSyncFile->value(QString::number(g_usMonitorID)).toInt();
                *static_cast<uint8_t*>(pvVal) = uint8_t(ucValue);
            }
            else
            {
                ucValue =  *static_cast<uint8_t*>(pvVal);
                pSyncFile->setValue(QString::number(g_usMonitorID), ucValue);
            }
        }
        else
        {
            ucValue = *static_cast<uint8_t*>(pvVal);
        }
        pMonitor->m_iDataVal = int32_t(ucValue);

    }
    else if(eDataType == Monitor::Uint16t)
    {
        if( xSyncMode)
        {
            if(pSyncFile->contains(QString::number(g_usMonitorID)))
            {
                usValue = pSyncFile->value(QString::number(g_usMonitorID)).toInt();
                *static_cast<int32_t*>(pvVal) = int32_t(usValue);
            }
            else
            {
                usValue =  *static_cast<uint16_t*>(pvVal);
                pSyncFile->setValue(QString::number(g_usMonitorID), usValue);
            }
        }
        else
        {
            usValue = *static_cast<uint16_t*>(pvVal);
        }
        pMonitor->m_iDataVal = int32_t(usValue);

    }
    else if(eDataType == Monitor::Int8t)
    {
        if(xSyncMode)
        {
            if(pSyncFile->contains(QString::number(g_usMonitorID)))
            {
                cValue = DataMonitor::readFile()->value(QString::number(g_usMonitorID)).toInt();
                *static_cast<int8_t*>(pvVal) = int8_t(cValue);
            }
            else
            {
                cValue =  *static_cast<int8_t*>(pvVal);
                pSyncFile->setValue(QString::number(g_usMonitorID), cValue);
            }
        }
        else
        {
            cValue = *static_cast<int8_t*>(pvVal);
        }
        pMonitor->m_iDataVal = int32_t(cValue);
    }
    else if(eDataType == Monitor::Int16t)
    {
        if(xSyncMode)
        {
            if(pSyncFile->contains(QString::number(g_usMonitorID)))
            {
                sValue = DataMonitor::readFile()->value(QString::number(g_usMonitorID)).toInt();
                *static_cast<int16_t*>(pvVal) = int16_t(sValue);
            }
            else
            {
                sValue =  *static_cast<int16_t*>(pvVal);
                pSyncFile->setValue(QString::number(g_usMonitorID), sValue);
            }
        }
        else
        {
            sValue = *static_cast<int16_t*>(pvVal);
        }
        pMonitor->m_iDataVal = int32_t(sValue);
    }
    else if(eDataType == Monitor::Uint32t)
    {
        if(xSyncMode)
        {
            if(pSyncFile->contains(QString::number(g_usMonitorID)))
            {
                uiValue = DataMonitor::readFile()->value(QString::number(g_usMonitorID)).toInt();
                *static_cast<uint32_t*>(pvVal) = uint32_t(sValue);
            }
            else
            {
                uiValue =  *static_cast<uint32_t*>(pvVal);
                pSyncFile->setValue(QString::number(g_usMonitorID), uiValue);
            }
        }
        else
        {
            uiValue = *static_cast<uint32_t*>(pvVal);
        }
        pMonitor->m_iDataVal = int32_t(uiValue);
    }
    else if(eDataType == Monitor::Int32t)
    {
        if( xSyncMode && pSyncFile->contains(QString::number(g_usMonitorID)) )
        {
            if(pSyncFile->contains(QString::number(g_usMonitorID)))
            {
                iValue = DataMonitor::readFile()->value(QString::number(g_usMonitorID)).toInt();
                *static_cast<int32_t*>(pvVal) = int32_t(sValue);
            }
            else
            {
                iValue =  *static_cast<int32_t*>(pvVal);
                pSyncFile->setValue(QString::number(g_usMonitorID), iValue);
            }
        }
        else
        {
            iValue = *static_cast<int32_t*>(pvVal);
        }
        pMonitor->m_iDataVal = iValue;
    }
    else if(eDataType == Monitor::Boolean)
    {
        if( xSyncMode && pSyncFile->contains(QString::number(g_usMonitorID)) )
        {
            if(pSyncFile->contains(QString::number(g_usMonitorID)))
            {
                xValue = DataMonitor::readFile()->value(QString::number(g_usMonitorID)).toBool();
                *static_cast<bool*>(pvVal) = bool(sValue);
            }
            else
            {
                xValue =  *static_cast<bool*>(pvVal);
                pSyncFile->setValue(QString::number(g_usMonitorID), xValue);
            }
        }
        else
        {
            xValue = *static_cast<bool*>(pvVal);
        }
        pMonitor->m_iDataVal = int32_t(xValue);
    }
    g_usMonitorID++;
    g_Monitors.insert(pvVal, pMonitor);
    pmMonitorMapList->m_MonitorMap.insert(pvVal,pMonitor);
    return pMonitor;
}
