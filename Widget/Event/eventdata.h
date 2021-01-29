#ifndef EVENTDATA_H
#define EVENTDATA_H

#include <QObject>
#include <QFile>
#include <QStringList>
#include <QDir>
#include <QMap>
#include <QTextCodec>
#include <QColor>
#include "datamonitor.h"


class EventData : public QObject
{
public:
    explicit EventData(QString strFileName = "", QObject *parent = nullptr);

    bool checkDir(QString strDir);

    //写入数据
    void writeData(QStringList list);

    //删除数据
    void removeDir();

    //刷新数据写入
    void flushFile();

private:
    QFile*   m_pFile;
    QString  m_strFileName;



    static QTextCodec *m_pTextCodec;



};

#endif // EVENTDATA_H
