#include "eventmonitor.h"
#include "eventdatabase.h"
#include <QStringList>
#include <QDate>

EventMonitor::EventMonitor(QString strDirPath, QObject *parent) :
    QObject(parent)
{
    autoWork = false;
    _clearAction = scanAgain;

    EventDataBase::setFileFormat((QStringList()<<"State"<<"Time"<<"Description").join(","));

    m_pDataBase = new EventDataBase(strDirPath);

}

void EventMonitor::registMonitorItem(void* pvVal, Monitor::DataType emDataType, QString strContext,
                                     int32_t iOccurredVal, QColor colorOccurred, QColor colorCompleted)
{
    sEventItem tempEvent = {strContext, colorOccurred, colorCompleted, iOccurredVal};

    if(!m_eventItemMap.contains(pvVal))
    {
        Monitor* pMonitor = DataMonitor::getInstance()->monitorRegist(pvVal, emDataType);
        if(pMonitor)
        {
            connect(pMonitor, SIGNAL(valChanged(Monitor*)), this, SLOT(valChangedSlot(Monitor*)));
        }
    }
    if(m_eventItemMap.contains(pvVal))
    {
        if(m_eventItemMap[pvVal].contains(iOccurredVal))
        {
            return;
        }
        else
        {
            m_eventItemMap[pvVal].insert(iOccurredVal, tempEvent);
        }
    }
    else
    {
        QMap<int32_t, sEventItem> tempMap;
        tempMap.insert(iOccurredVal, tempEvent);
        m_eventItemMap.insert(pvVal, tempMap);
    }
}

void EventMonitor::removeMonitorItem(void* pvVal)
{
    if( m_eventItemMap.contains(pvVal) )
    {
        m_eventItemMap.remove(pvVal);
    }
}

void EventMonitor::setClearAction(EventMonitor::clearAction action)
{
    _clearAction = action;
}

void EventMonitor::enableAutoWork(bool state)
{
    autoWork = state;
}

bool EventMonitor::setMonitorItemColor(void* pvVal, int32_t iOccurredVal, QColor onColor, QColor offColor)
{
    if(m_eventItemMap.contains(pvVal))
    {
        if(m_eventItemMap[pvVal].contains(iOccurredVal))
        {
            m_eventItemMap[pvVal][iOccurredVal].colorOccurred = onColor;
            m_eventItemMap[pvVal][iOccurredVal].colorCompleted = offColor;
            return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}

void EventMonitor::autoWrite(QMap<int32_t, sEventItem> mEventMap, Monitor* pMonitor)
{
    QStringList list;

    sEventItem mItem = mEventMap[pMonitor->getCurVal()];

    list.append(QString::number(pMonitor->getCurVal()));
    list.append(QDate::currentDate().toString("yyyy-MM-dd"));
    list.append(QTime::currentTime().toString("hh:mm:ss"));
    list.append(mItem.strContext);

    m_pDataBase->writeData(list);
}

void EventMonitor::writeData(QStringList list)
{
     m_pDataBase->writeData(list);
}

void EventMonitor::emitEvent(QMap<int32_t, sEventItem> mEventMap, Monitor* pMonitor)
{
/*    if(mItem.strContext == "" )
    {
        if(pMonitor->getCurVal() != 0)
        {
            emit eventStringEmpty(mItem, pMonitor);
        }
    }
    else
    {
        if(pMonitor->getCurVal() != 0 )
        {
            emit eventComing(mItem, pMonitor);
        }
    }
*/
}

void EventMonitor::valChangedSlot(Monitor* pMonitor)
{
    if( !m_eventItemMap.contains(pMonitor->getValueAddr()) )
    {
        return;
    }
    else
    {
        QMap<int32_t, sEventItem> mEventMap = m_eventItemMap[pMonitor->getValueAddr()];
        if(autoWork)
        {
            autoWrite(mEventMap, pMonitor);
        }
        else
        {
            //emitEvent(mEventMap, pMonitor);
            emit eventComing(mEventMap, pMonitor);
        }

    }
}
