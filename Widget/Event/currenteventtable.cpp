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

    connect(m_pEventMonitor, SIGNAL(eventComing(sEventItem, Monitor*)), this, SLOT(addEvent(sEventItem, Monitor*)));

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
                               QColor colorOccurred, QColor colorCompleted, int32_t iMaxVal, int32_t iMinVal)
{
    m_pEventMonitor->registMonitorItem(pvVal, emDataType, strContext,  colorOccurred, colorCompleted, iMaxVal, iMinVal);
}

void CurrentEventTable::removeMonitorItem(void* pvVal)
{
    m_pEventMonitor->removeMonitorItem(pvVal);
}

bool CurrentEventTable::setMonitorItemColor(void* pvVal, QColor onColor,QColor offColor)
{
    return m_pEventMonitor->setMonitorItemColor(pvVal, onColor, offColor);
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

    eventNum++;
}

void CurrentEventTable::addEvent(sEventItem mItem, Monitor* pMonitor)
{
    if(pMonitor->getCurVal() == 0)
    {
        furbishEvent(pMonitor->getCurValAddr(), mItem.strContext, mItem.colorCompleted);
    }
    else
    {
        insertEvent(pMonitor->getCurValAddr(), mItem.strContext, mItem.colorOccurred);
    }
}

void CurrentEventTable::clearTableSlot()
{
    eventNum = 0;
    minNum = 1;
    clearTabel();
}

void CurrentEventTable::insertEvent(void* pvVal, QString str, QColor color)
{
    checkMaxVisibleItem(pvVal);

    QStringList list;
    QStringList ItemData;

    QString time = QDate::currentDate().toString("yyyy-MM-dd");
    time.append(" ");
    time.append(QTime::currentTime().toString("hh:mm:ss"));

    ItemData.append(QString::number(eventNum));
    ItemData.append(time);
    ItemData.append("");
    ItemData.append(str);

    list.append("1");
    list.append(ItemData[1]);
    list.append(ItemData[3]);

    insertRow(ItemData,color);

    m_pEventMonitor->writeData(list);
}

void CurrentEventTable::furbishEvent(void* pvVal, QString str, QColor color)
{
    QStringList list;

    QString time = QDate::currentDate().toString("yyyy-MM-dd");
    time.append(" ");
    time.append(QTime::currentTime().toString("hh:mm:ss"));

    list.append("0");
    list.append(time);
    list.append(str);

    m_pEventMonitor->writeData(list);

    if( !eventMap.contains(pvVal) )
    {
        return;
    }

    int row = eventNum - eventMap.value(pvVal) - 1;

    setRowColor(row,color);
    item(row, 2)->setText(time);

    addrMap.remove(eventMap.value(pvVal));
    eventMap.remove(pvVal);
}



