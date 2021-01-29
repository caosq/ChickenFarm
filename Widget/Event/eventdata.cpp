#include "eventdata.h"
#include "datamonitor.h"

#include <QDate>

#define DEF_TEXT_CODEC "gb2312"
#define EVENTDATA_DEF_FORMAT ".csv"
#define ROOT_EVENT_DIR   "/home/Event/"

#define FIX_TITLE_FORMAT ""
#define DEF_SPLIT_T ","
#define DEF_ENDIND "\n"


QTextCodec* EventData::m_pTextCodec = QTextCodec::codecForName(DEF_TEXT_CODEC);

EventData::EventData(QString strFileName, QObject *parent) :
    QObject(parent)
{
     QString strPath = ROOT_EVENT_DIR;

     checkDir(strPath);

     m_pFile = new QFile(strPath + strFileName);
     m_strFileName = strFileName;
}

void EventData::writeData(QStringList list)
{
    if(list.size() == 0)
    {
        return;
    }
    QString line = "";
    int lastIndex = list.size() - 1;
    for(int i = 0; i < lastIndex; i++)
    {
        line += list.at(i);
        line += DEF_SPLIT_T;
    }
    line += list.at(lastIndex);
    line += DEF_ENDIND;

    QByteArray strArray = m_pTextCodec->fromUnicode(line);
    m_pFile->write(strArray);
}

bool EventData::checkDir(QString strDir)
{
    QDir dir(strDir);

    if(!dir.exists())
    {
        return dir.mkpath(strDir);
    }
    return true;
}

