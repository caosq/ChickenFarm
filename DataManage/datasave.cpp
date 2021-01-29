#include "datasave.h"

#include "globalMark.h"

#define SAVE_KEY    "[General]"
#define SYNC_FILE   "/home/root/.SyncFile"

#define CLEAR_TIME 12*60*60*1000

int dataSave::saveFd = -1;

//数据存储类。提供单个，批量的数据设置功能
//可立即保存数据，也可以延迟保存
//ps：数据保存相对耗时，连续的执行文件读写操作会导致界面卡顿
dataSave::dataSave(const QString &fileName, Format format, QObject *parent) :
    QSettings(fileName,format,parent)
{

#ifdef PLATFORM_ARM
    openFlushFile(SYNC_FILE);
#endif
    //initCheckSaveFile(fileName);

    _clearTimer = new QTimer(this);

    connect(_clearTimer,SIGNAL(timeout()),this,SLOT(timeOutSlot()));
    _clearTimer->start(CLEAR_TIME);
}

void dataSave::openFlushFile(const char *syncFile)
{
    if( -1 == saveFd )
    {
        saveFd = open(syncFile,O_RDWR | O_CREAT);
        if( -1 == saveFd ){
            qDebug("sync file errno");
        }else{
            system("echo 0 > /home/root/.SyncFile");
        }
    }
}

void dataSave::setValue(const QString &key, const QVariant &value)
{
    QSettings::setValue(key,  value);
    syncFile();
}

void dataSave::setValues(const QString &key, QVector<int32_t> value)
{
    bool ok = false;
    int addr = key.toInt(&ok);
    if( ok ){
        for(int i = 0; i < value.size(); i++ ){
            QSettings::setValue(QString::number(addr),value.at(i));
            addr++;
        }
        syncFile();
    }
}

void dataSave::setValues(QMap<int, int32_t> value)
{
    QList<int> addrs = value.keys();
    QList<int32_t> vals = value.values();

    for(int i = 0; i < vals.size(); i++ ){
        QSettings::setValue(QString::number(addrs.at(i)),vals.at(i));
    }
    syncFile();
}

void dataSave::setValueTardy(const QString &key, const QVariant &value)
{
    QSettings::setValue(key,value);
}

void dataSave::setValuesTardy(const QString &key, QVector<int32_t> value)
{
    bool ok = false;
    int addr = key.toInt(&ok);
    if( ok ){
        for(int i = 0; i < value.size(); i++ ){
            QSettings::setValue(QString::number(addr),value.at(i));
            addr++;
        }
    }
}

void dataSave::syncFile()
{
    sync();
    flushVal();
}

void dataSave::flushVal()
{
    if( saveFd != -1 ){
        write(saveFd,"",1);
        fsync(saveFd);
    }
}

void dataSave::initCheckSaveFile(QString file)
{

    QFile tempFile;
    tempFile.setFileName(file);
    if( !tempFile.exists() )
    {
        QString line = SAVE_KEY;
        line += "\n";

        tempFile.open(QIODevice::ReadWrite);
        tempFile.write(line.toLocal8Bit());
        tempFile.flush();
        fsync(tempFile.handle());
        tempFile.close();
    }

}

void dataSave::timeOutSlot()
{
    if( -1 == saveFd ){
        qDebug("sync file errno");
    }else{
        system("echo 0 > /home/root/.SyncFile");
    }
}
