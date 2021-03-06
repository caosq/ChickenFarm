#ifndef CURRENTEVENTTABLE_H
#define CURRENTEVENTTABLE_H

#include "eventtable.h"
#include "eventdatabase.h"
#include "eventmonitor.h"

#include <QTime>
#include <QDate>
#include <QFile>

class CurrentEventTable : public EventTable
{
    Q_OBJECT

public:

    typedef struct
    {
        int32_t iEventNum;
        QStringList strItemData;
    }sEventItemData;

    //当前事件记录
    explicit CurrentEventTable(int column = 0,QString saveDir = "",
                               QWidget *parent = nullptr);

    //绑定要监视的数据地址以及相应要显示的文字
    void registMonitorItem(void* pvVal, Monitor::DataType emDataType, QString strContext, int32_t iOccurredVal,
                           QColor colorOccurred = QColor("#0bd80b"), QColor colorCompleted = Qt::gray);
    //移除监视地址
    void removeMonitorItem(void* pvVal);

    //设置某个绑定的数据地址产生事件时文字的颜色
    bool setMonitorItemColor(void* pvVal, int32_t iOccurredVal, QColor onColor,QColor offColor);

    //设置保存的文件中第一行的抬头信息
    void setSaveFileTitle(QStringList title);

    //设置当执行清除操作时执行的动作 clearAction的noAction不执行任何操作  clearAction的scanAgain再次扫描一次
    void setClearAction(EventMonitor::clearAction action);

    //设置监视的数据地址以及对应要显示的文字
    void setMonitorItem(QString id,int addr,QString str);

    //删除监视的数据地址
    void removeMonitorItem(QString id,int addr);

    //设置监视条目文字显示颜色
    //onColor对应状态1,off对应状态0
    bool setMonitorItemColor(QString id,int addr,QColor onColor,QColor offColor);

    //设置表格显示的最大条目数量
    void setMaxVisibleRowNum(int num);

    //获取数据触发器
    EventMonitor *dataHandle(){return m_pEventMonitor;}

    //插入事件记录
    void insertEvent(EventMonitor::sEventItem mItem, Monitor* pMonitor);
    //恢复事件记录
    void furbishEvent(int32_t iMapKey, EventMonitor::sEventItem mItem, Monitor* pMonitor);

private:
    void checkMaxVisibleItem(void* pvVal);

private slots:
    void eventComingSlot(QMap<int32_t, EventMonitor::sEventItem>, Monitor*);
    void clearTableSlot();

private:

    EventMonitor *m_pEventMonitor;
    int maxVisibleNum;
    int eventNum = 0;
    int minNum;

    QMap<void*,int>   eventMap;
    QMap<int,void*>   addrMap;

    QMap<void*, QMap<int32_t, sEventItemData>> m_eventItemMap;

    uint32_t m_ucRowIndex = 0;
};

#endif // CurrentEventTable_H
