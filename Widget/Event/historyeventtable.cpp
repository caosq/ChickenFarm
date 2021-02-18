#include "historyeventtable.h"

#include "eventdatabase.h"
//#include "msgControl/msgmanage.h"

HistoryEventTable::HistoryEventTable(int column, QString saveDir, QWidget *parent):
    EventTable(column,parent)
{
   setItemExpandShow(column,true);

    onColor = Qt::red;
//    offColor = Qt::green;
    offColor = QColor("#0bd80b");
    fileNum = 0;
    userDir = saveDir;
    eventFile  = new QFile();
    rootDir = EventDataBase::rootDir();

    setReadDir(userDir);
}

void HistoryEventTable::nextDay()
{
    if( fileNum >= historyList.size() - 1 )
    {
//        msgManage::instance()->showUniversalMsg(43);   // msg[42] = tr("查看信息已到达首页，请往下页翻看！");
        return;
    }
    else
    {
        fileNum++;
    }
    readFile();
    verticalScrollBar()->setValue(verticalScrollBar()->minimum());
}

void HistoryEventTable::prevDay()
{
    if( fileNum <= 0 )
    {
//        msgManage::instance()->showUniversalMsg(42);
        return;
    }
    else
    {
        fileNum--;
    }
    readFile();
    verticalScrollBar()->setValue(verticalScrollBar()->minimum());
}

void HistoryEventTable::flushShow()
{
    if( !checkFileList() )
        return;

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    readFile();

    verticalScrollBar()->setValue(verticalScrollBar()->minimum());

    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void HistoryEventTable::setReadDir(QString str)
{
    if(!str.isEmpty() && !str.endsWith("/"))
        str.append("/");
    if( str.startsWith("/"))
        str.remove(0,1);
    userDir = str;
    checkDir(userDir);
}

bool HistoryEventTable::checkDir(QString name)
{
    name.prepend(rootDir);
    QDir dir(name);

    if(!dir.exists())
    {
        return dir.mkpath(name);
    }
    return true;
}


bool HistoryEventTable::checkFileList()
{
    checkDir(userDir);

    QString pathName = userDir;
    pathName.prepend(rootDir);
    QDir path(pathName);

    historyList.clear();
    historyList = path.entryList(QStringList()<<QString("*").append(EventDataBase::fileFormat()));

    fileNum = historyList.size() - 1;

    if( historyList.size() == 0 ){
        clearTabel();
        return false;
    }else{
        return true;
    }
}

bool HistoryEventTable::prepareFile()
{
    bool ret = false;

    if( fileNum < 0 || fileNum >= historyList.size() )
        return ret;

    QString name = historyList[fileNum];

    if( eventFile->isOpen() ){
        eventFile->close();
        eventFile->setFileName("");
    }

    if( !eventFile->fileName().contains(name)){
        if( eventFile->isOpen() )
            eventFile->close();

        name.prepend(userDir);
        name.prepend(rootDir);
        eventFile->setFileName(name);

        ret = eventFile->open(QIODevice::ReadOnly);
        if(!ret){
            qDebug("open event file faile");
        }
    }
    return ret;
}

bool HistoryEventTable::readFile()
{
    bool ret = false;
    ret = prepareFile();

    if( ret ){
        clearTabel();
        eventFile->seek(0);

        QTextStream curveData(eventFile);
        QStringList lines;
        QString line;

        curveData.setCodec(EventDataBase::dataCodec());
        line = curveData.readLine();

        int state;
        int i = 0;
        QString splitStr = EventDataBase::splitText();
        while(!curveData.atEnd()){
            i++;
            lines = curveData.readLine().split(splitStr);
            state = lines[0].toInt();
            lines.replace(0,QString::number(i));
            if( state == 0 ){
                insertRow(lines,offColor);
            }else{
                insertRow(lines,onColor);
            }
        }
    }
    return ret;
}


void HistoryEventTable::showEvent(QShowEvent *e)
{
    EventTable::showEvent(e);
    flushShow();
}
