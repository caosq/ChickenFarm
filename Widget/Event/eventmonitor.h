#ifndef EVENTMONITOR_H
#define EVENTMONITOR_H

#include <QObject>
#include <QFile>
#include <QStringList>
#include <QDir>
#include <QMap>
#include <QTextCodec>
#include <QColor>
#include "datamonitor.h"
#include "eventdatabase.h"

typedef struct
{
    QString strContext;
    QColor  colorOccurred;
    QColor  colorCompleted;
}sEventItem;

class EventMonitor : public QObject
{
    Q_OBJECT
public:
      enum clearAction{
          noAction,
          scanAgain
      };
    //事件触发器
    //用于绑定要检测的数据地址，同时产生事件信号
    explicit EventMonitor(QString strDirPath = "", QObject *parent = nullptr);

    //绑定要监视的数据地址以及相应要显示的文字
    void registMonitorItem(void* pvVal, Monitor::DataType emDataType, QString strContext,
                        QColor colorOccurred = QColor(Qt::red), QColor colorCompleted = QColor("#0bd80b"),
                        int32_t iMaxVal = 65535, int32_t iMinVal = -65535);
    //移除监视地址
    void removeMonitorItem(void* pvVal);

    //设置自动工作
    //自动模式下，根据预先绑定的数据地址以及相应的文字显示自动写入文件
    //非自动模式下当数据发生变化时只会产生信号通知
    void enableAutoWork(bool state);

    //是否自动工作
    bool isAutoWork(){return autoWork;}

    //设置执行清除操作后要执行的动作
    //noAction什么也不做
    //scanAgain再扫描一次当前所有事件记录
    void setClearAction(clearAction action);

    //设置某个绑定的数据地址产生事件时文字的颜色
    //onColor决定状态1颜色，offColor决定状态0颜色
    bool setMonitorItemColor(void* pvVal, QColor onColor,QColor offColor);

     void writeData(QStringList list);

private:
    void checkMonitorList(QString id);

    void autoWrite(sEventItem mItem, Monitor* pMonitor);

    void emitEvent(sEventItem mItem, Monitor* pMonitor);

    void completeScan();

private slots:
   // void valSlot(QString id, int addr, int nb);
 //   void valSlot(QString id, QList<int> list);
  //  void autoScan();
   void valChangedSlot(Monitor* pMonitor);

signals:
    //清除事件记录时会触发该信号
    void clearTable();

    //当有任何事件发生会触发该信号
    void eventComing(sEventItem mItem, Monitor* pMonitor);

    void eventStringEmpty(sEventItem mItem, Monitor* pMonitor);

private:
    uint32_t m_DataIndex = 0;
    EventDataBase* m_pDataBase;

    clearAction _clearAction;
    bool autoWork;
    QMap< void*, sEventItem> m_eventItemMap;
};

#endif // EVENTMONITOR_H
