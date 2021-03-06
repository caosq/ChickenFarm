#include "currenteventtable.h"
#include "eventdatabase.h"

#define MAX_ITEM_NUM 100

CurrentEventTable::CurrentEventTable(int column, QString saveDir, QWidget *parent) :
    EventTable(column,parent)
{
    setItemExpandShow(column,true);
    eventNum = 0;
    minNum = 1;
    maxVisibleNum = MAX_ITEM_NUM;

    m_pEventMonitor = new EventMonitor(saveDir, this);

    connect(m_pEventMonitor, SIGNAL(eventComing(QMap<int32_t, EventMonitor::sEventItem>, Monitor*)), this, SLOT(eventComingSlot(QMap<int32_t, EventMonitor::sEventItem>, Monitor* )));

   // connect(_data,SIGNAL(clearTable()),this,SLOT(clearTableSlot()));
}

void CurrentEventTable::setSaveFileTitle(QStringList title)
{
    EventDataBase::setFileFormat(title.join(","));
}

void CurrentEventTable::setClearAction(EventMonitor::clearAction action)
{
    m_pEventMonitor->setClearAction(action);
}

void CurrentEventTable::registMonitorItem(void* pvVal, Monitor::DataType emDataType, QString strContext,
                                          int32_t iOccurredVal, QColor colorOccurred, QColor colorCompleted)
{
    m_pEventMonitor->registMonitorItem(pvVal, emDataType, strContext, iOccurredVal, colorOccurred, colorCompleted);
}

void CurrentEventTable::removeMonitorItem(void* pvVal)
{
    m_pEventMonitor->removeMonitorItem(pvVal);
}

bool CurrentEventTable::setMonitorItemColor(void* pvVal, int32_t iOccurredVal, QColor onColor, QColor offColor)
{
    return m_pEventMonitor->setMonitorItemColor(pvVal, iOccurredVal, onColor, offColor);
}

void CurrentEventTable::setMaxVisibleRowNum(int num)
{
    if( num < 0 ){
        return;
    }

    maxVisibleNum = num;
}

void CurrentEventTable::checkMaxVisibleItem(void* pvVal)
{
    if( eventNum >= maxVisibleNum )
    {
        if( addrMap.contains(minNum - 1) )
        {
            eventMap.remove(addrMap.value(minNum - 1));
            addrMap.remove(minNum - 1);
        }
        removeRow(rowCount() - 1);
        minNum++;
    }
    eventMap.insert(pvVal,eventNum);
    addrMap.insert(eventNum,pvVal);

    //eventNum++;
}

void CurrentEventTable::eventComingSlot(QMap<int32_t, EventMonitor::sEventItem> mEventMap, Monitor* pMonitor)
{
    QMap<int32_t, EventMonitor::sEventItem>::iterator it;
    EventMonitor::sEventItem mItem;

    for(it=mEventMap.begin(); it!=mEventMap.end(); ++it)
    {
        mItem = it.value();
        if(pMonitor->getCurVal() == mItem.iOccurredVal)
        {
            insertEvent(mItem, pMonitor);
        }
        else
        {
            furbishEvent(it.key(), mItem, pMonitor);
        }
    }
   // qDebug("insertEvent getCurVal %d  iOccurredVal %d", pMonitor->getCurVal(), mItem.iOccurredVal);
}

void CurrentEventTable::clearTableSlot()
{
    eventNum = 0;
    minNum = 1;
    clearTabel();
}

void CurrentEventTable::insertEvent(EventMonitor::sEventItem mItem, Monitor* pMonitor)
{
    checkMaxVisibleItem(pMonitor->getValueAddr());

    QStringList list;
    QStringList ItemData;
    sEventItemData mEventItemData;
    QMap<int32_t, sEventItemData> mEventMap;

    QString time = QDate::currentDate().toString("yyyy-MM-dd");
    time.append(" ");
    time.append(QTime::currentTime().toString("hh:mm:ss"));

    eventNum++;

    ItemData.append(QString::number(eventNum));
    ItemData.append(time);
    ItemData.append("");
    ItemData.append(mItem.strContext);

    list.append("1");
    list.append(ItemData[1]);
    list.append(ItemData[3]);
    insertRow(ItemData, mItem.colorOccurred);

    mEventItemData = {eventNum, ItemData};
    mEventMap.insert(pMonitor->getCurVal(), mEventItemData);

    m_eventItemMap.insert(pMonitor->getValueAddr(), mEventMap);
    m_pEventMonitor->writeData(list);
}

void CurrentEventTable::furbishEvent(int32_t iMapKey, EventMonitor::sEventItem mItem, Monitor* pMonitor)
{
    /*if( !eventMap.contains(pvVal) )
    {
        return;
    }

    int row = eventNum - eventMap.value(pvVal) - 1;

    setRowColor(row,color);
    item(row, 2)->setText(time);

    addrMap.remove(eventMap.value(pvVal));
    eventMap.remove(pvVal);
    */

    QStringList list;
    QString time = QDate::currentDate().toString("yyyy-MM-dd");
    sEventItemData mEventItemData;
    void*  pvVal = pMonitor->getValueAddr();
    int    row = 0;

    if(m_eventItemMap.contains(pvVal))
    {
        if(m_eventItemMap[pvVal].contains(iMapKey))
        {
            time.append(" ");
            time.append(QTime::currentTime().toString("hh:mm:ss"));

            list.append("0");
            list.append(time);
            list.append(mItem.strContext);

            mEventItemData = m_eventItemMap[pvVal][iMapKey];
            row = eventNum - mEventItemData.iEventNum;

            setRowColor(row, mItem.colorCompleted);
            item(row, 2)->setText(time);
            m_eventItemMap[pvVal].remove(iMapKey);

            m_pEventMonitor->writeData(list);
        }
    }
}



