#include "eventdatabase.h"

#include <QDate>
#include <unistd.h>
#include <syslog.h>

#define DEF_MAX_FILE_NUM 180

#define DEF_TEXT_CODEC "gb2312"
#define EventDataBase_DEF_FORMAT ".csv"
#define ROOT_EVENT_DIR   "/home/Event/"

#define FIX_TITLE_FORMAT ""
#define DEF_SPLIT_T ","
#define DEF_ENDIND "\n"

QMap<QString, EventDataBase*> EventDataBase::_map;

QString EventDataBase::_fileFormat = EventDataBase_DEF_FORMAT;
QTextCodec *EventDataBase::m_pTextCodec = QTextCodec::codecForName(DEF_TEXT_CODEC);

EventDataBase::EventDataBase()
{

}

EventDataBase::EventDataBase(QString strDataBaseDir, QObject *parent) :
    QObject(parent)
{
    m_pDataBaseFile = new QFile;
    m_strDataBaseDir = strDataBaseDir;
    m_ucMaxNumber = DEF_MAX_FILE_NUM;

    m_strTitleFormat = QString(FIX_TITLE_FORMAT).split(DEF_SPLIT_T);

    setSaveDir(m_strDataBaseDir);

   // addToCoreDirMap(m_strDataBaseDir,this);
}

EventDataBase::~EventDataBase()
{
    removeCoreDirMapItem(m_strDataBaseDir);
}

void EventDataBase::removeDir(QString name)
{
    EventDataBase *obj = _map.value(name,0);
    if(obj)
    {
        obj->removeDir();
    }
}

QString EventDataBase::rootDir()
{
    return ROOT_EVENT_DIR;
}

QString EventDataBase::splitText()
{
    return DEF_SPLIT_T;
}

void EventDataBase::setTitleFormat(QStringList format)
{
    m_strTitleFormat.clear();
    m_strTitleFormat = format;
}

QList<QString> EventDataBase::currentDirList()
{
    return _map.keys();
}

void EventDataBase::removeAllDir()
{
    QList<EventDataBase *> list = _map.values();
    for(int i = 0; i < list.size(); i++ ){
        list[i]->removeDir();
    }
}

void EventDataBase::removeDir()
{
    if( checkDir(m_strDataBaseDir) )
    {
        QString cmd = "rm ";
        cmd.append(ROOT_EVENT_DIR);
        cmd.append(m_strDataBaseDir);
        cmd.append(" -r");
        system(cmd.toUtf8().constData());
    }
    checkDir(m_strDataBaseDir);

    if(m_pDataBaseFile->isOpen())
    {
        m_pDataBaseFile->close();
    }
    m_pDataBaseFile->setFileName("");
    emit dataCleared();
}

void EventDataBase::setSaveDir(QString str)
{
    if(!str.isEmpty() && !str.endsWith("/"))
    {
        str.append("/");
    }
    if(str.startsWith("/"))
    {
        str.remove(0,1);
    }
    m_strDataBaseDir = str;
    checkDir(m_strDataBaseDir);
}

void EventDataBase::setFormat(QString format)
{
    _fileFormat = format;
}

bool EventDataBase::checkDir(QString name)
{
    name.prepend(ROOT_EVENT_DIR);
    QDir dir(name);

    if(!dir.exists())
    {
        return dir.mkpath(name);
    }
    return true;
}

void EventDataBase::removeCoreDirMapItem(QString name)
{
    _map.remove(name);
}

void EventDataBase::writeData(QStringList list)
{
    if( list.size() == 0 )
    {
        return;
    }
    QString data = QDate::currentDate().toString("yyyy-MM-dd");

    if( !checkFile(data) )
        return;

    checkHead();

    QString line = "";
    int lastIndex = list.size() - 1;
    for(int i = 0; i < lastIndex; i++ )
    {
        line += list.at(i);
        line += DEF_SPLIT_T;
    }
    line += list.at(lastIndex);
    line += DEF_ENDIND;

    QByteArray strArray = m_pTextCodec->fromUnicode(line);
    m_pDataBaseFile->write(strArray);
}

void EventDataBase::flushFile()
{
    if( m_pDataBaseFile->isOpen() )
    {
        m_pDataBaseFile->flush();
        fsync(m_pDataBaseFile->handle());
    }
}

void EventDataBase::checkHead()
{
    if(0 == m_pDataBaseFile->size())
    {
        QString title = "";
        if( m_strTitleFormat.size() <= 0 )
        {
            return;
        }
        int lastIndex = m_strTitleFormat.size() - 1;
        for(int i = 0; i < lastIndex ; i++ )
        {
            title += m_strTitleFormat.at(i);
            title += DEF_SPLIT_T;
        }
        title += m_strTitleFormat.at(lastIndex);
        title += DEF_ENDIND;

        QByteArray strArray = m_pTextCodec->fromUnicode(title);
        m_pDataBaseFile->write(strArray);
    }
}

bool EventDataBase::checkFile(QString name)
{
    bool ret = true;
    if(!m_pDataBaseFile->fileName().contains(name))
    {
        QString path = ROOT_EVENT_DIR;
        path+=m_strDataBaseDir;

        if(m_pDataBaseFile->isOpen())
            m_pDataBaseFile->close();

        checkMaxFileCount();
        path += name;
        path += _fileFormat;

        m_pDataBaseFile->setFileName(path);

        ret = m_pDataBaseFile->open(QIODevice::Append);
        if(!ret)
        {
            syslog(LOG_ERR,m_pDataBaseFile->errorString().toLocal8Bit().constData());
        }
    }
    return ret;
}


void EventDataBase::checkMaxFileCount()
{
    QString dir = ROOT_EVENT_DIR;
    dir.append(m_strDataBaseDir);

    if( !dir.endsWith("/") )
        dir.append("/");
    QDir path(dir);

    QStringList list;
    list = path.entryList(QStringList()<<QString("*").append(_fileFormat));

    int remove = list.size() - m_ucMaxNumber;
    if(remove > 0)
    {
        QFile removeFile;
        QString removeName = dir;
        removeName+="%1";
        for(int i = 0; i < remove; i++)
        {
            removeFile.setFileName(removeName.arg(list[i]));
            if( removeFile.exists() )
            {
                if(removeFile.isOpen())
                {
                    removeFile.close();
                }
                removeFile.remove();
            }
        }
    }
}


