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
                               QColor colorOccurred, QColor colorCompleted, int32_t iMaxVal, int32_t iMinVal)
{
    sEventItem tempEvent = {strContext, colorOccurred, colorCompleted};

    Monitor* pMonitor = DataMonitor::getInstance()->monitorRegist(pvVal, emDataType, iMaxVal, iMinVal);

    if(pMonitor)
    {
        connect(pMonitor, SIGNAL(valChanged(Monitor)), this, SLOT(valChangedSlot(Monitor)));
    }
    if(m_eventItemMap.contains(pvVal))
    {
        return;
    }
    else
    {
        m_eventItemMap.insert(pvVal, tempEvent);
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

bool EventMonitor::setMonitorItemColor(void* pvVal, QColor onColor,QColor offColor)
{
    if(m_eventItemMap.contains(pvVal))
    {
        m_eventItemMap[pvVal].colorOccurred = onColor;
        m_eventItemMap[pvVal].colorCompleted = offColor;

        return true;
    }
    else
    {
        return false;
    }
}

void EventMonitor::autoWrite(sEventItem mItem, Monitor* pMonitor)
{
    QStringList list;
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


void EventMonitor::emitEvent(sEventItem mItem, Monitor* pMonitor)
{
    if(mItem.strContext == "" )
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
}

void EventMonitor::valChangedSlot(Monitor* pMonitor)
{
    if( !m_eventItemMap.contains(pMonitor->getCurValAddr()) )
    {
        return;
    }
    else
    {
        sEventItem mItem = m_eventItemMap[pMonitor->getCurValAddr()];

        if(autoWork)
        {
            autoWrite(mItem, pMonitor);
        }
        else
        {
            emitEvent(mItem, pMonitor);
        }

    }
}
