#ifndef EVENTDATABASE_H
#define EVENTDATABASE_H

#include <QObject>
#include <QFile>
#include <QStringList>
#include <QDir>
#include <QMap>
#include <QTextCodec>
#include <QColor>

class EventDataBase : public QObject
{
    Q_OBJECT
public:
    explicit EventDataBase();

   //事件记录数据记录模块
    explicit EventDataBase(QString strDirPath = "",QObject *parent = nullptr);

    ~EventDataBase();

    //获取一个静态实例
    //static EventDataBase *core();

    //获取所有事件记录的存储目录
    static QList<QString> currentDirList();

    //获取数据存储的根目录
    static QString rootDir();

    //获取文件格式
    static QString fileFormat(){return _fileFormat;}

    //获取文件分割标志
    static QString splitText();

    //获取文件保存编码
    static QTextCodec *dataCodec(){return m_pTextCodec;}

    //设置文件保存格式(仅仅更改文件后缀名)
    static void setFileFormat(QString format){_fileFormat = format;}

    //设置文件保存的编码格式
    static void setDataCodec(QString codecName){
        m_pTextCodec = QTextCodec::codecForName(codecName.toLocal8Bit());}

    //获取文件保存的抬头信息
    QStringList titleFormat(){return m_strTitleFormat;}

    //获取最大的文件存储数据量
    int fileMaxNumber(){return m_ucMaxNumber;}

    //设置最大的文件存储数量
    void setMaxNumber(quint8 num){m_ucMaxNumber = num;}
    //设置数据存储的抬头信息
    void setTitleFormat(QStringList format);

    //写入事件记录
    void writeData(QStringList list);
    //删除数据存储目录
    void removeDir();
    //刷新数据写入
    void flushFile();

private:
    bool checkFile(QString name);
    void checkMaxFileCount();
    void checkHead();
    bool checkDir(QString name);

    bool addToCoreDirMap(QString name = "", EventDataBase *obj = nullptr);
    void removeCoreDirMapItem(QString name = "");
    void setSaveDir(QString str);
    void setFormat(QString format);

public slots:
    static void removeAllDir();
    static void removeDir(QString name);

signals:
       void dataCleared();

private:
    QFile*   m_pDataBaseFile;
    QString  m_strDataBaseDir;

    QStringList m_strTitleFormat;
    quint8      m_ucMaxNumber;


    //static EventDataBase *_core;
    static QTextCodec *m_pTextCodec;
    static QString _fileFormat;

    static QMap<QString, EventDataBase*> _map;
};

#endif // EVENTDATABASE_H
