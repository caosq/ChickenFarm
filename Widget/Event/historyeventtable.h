#ifndef HISTORYEVENTTABLE_H
#define HISTORYEVENTTABLE_H

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "eventtable.h"

class HistoryEventTable : public EventTable
{
    Q_OBJECT
public:
    //历史事件记录类
    explicit HistoryEventTable(int column = 0,QString saveDir = "",QWidget *parent = nullptr);
    
    //设置状态1显示的颜色
    void setStateOnColor(QColor color){onColor = color;}
    //设置状态0显示的颜色
    void setStateOffColor(QColor color){offColor = color;}

    //上一天数据
    void prevDay();
    //下一天数据
    void nextDay();

private:
    void setReadDir(QString str);
    bool checkDir(QString name);
    void flushShow();
    bool checkFileList();
    bool prepareFile();
    bool readFile();

protected:
    void showEvent(QShowEvent *e);

private:
    QColor onColor;
    QColor offColor;

    QFile   *eventFile;
    QStringList historyList;
    QString userDir;
    QString rootDir;
    int fileNum;
    
};

#endif // HISTORYEVENTTABLE_H
