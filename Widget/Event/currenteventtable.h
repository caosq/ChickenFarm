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
    //当前事件记录
    explicit CurrentEventTable(int column = 0,QString saveDir = "",
                               QWidget *parent = nullptr);

    //绑定要监视的数据地址以及相应要显示的文字
    void registMonitorItem(void* pvVal, eDataType emDataType, QString strContext,
                        QColor colorOccurred = QColor(Qt::red), QColor colorCompleted = QColor("#0bd80b"),
                        int32_t iMaxVal = 65535, int32_t iMinVal = -65535);
    //移除监视地址
    void removeMonitorItem(void* pvVal);

    //设置某个绑定的数据地址产生事件时文字的颜色
    //onColor决定状态1颜色，offColor决定状态0颜色
    bool setMonitorItemColor(void* pvVal, QColor onColor,QColor offColor);

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
    void insertEvent(void* pvVal, QString str, QColor color);
    //恢复事件记录
    void furbishEvent(void* pvVal, QString str, QColor color);

private:
    void checkMaxVisibleItem(void* pvVal);

private slots:
    void addEvent(sEventItem mItem, Monitor* pMonitor);
    void clearTableSlot();

private:

    EventMonitor *m_pEventMonitor;
    int maxVisibleNum;
    int eventNum;
    int minNum;

    QMap<void*,int>   eventMap;
    QMap<int,void*>   addrMap;

    uint32_t m_ucRowIndex = 0;

};

#endif // CurrentEventTable_H