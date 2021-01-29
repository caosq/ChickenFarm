#ifndef DATASAVE_H
#define DATASAVE_H

#include <QObject>
#include <QFile>
#include <QSettings>
#include <fcntl.h>
#include <unistd.h>
#include <QVector>
#include <QTimer>

class dataSave : public QSettings
{
    Q_OBJECT
public:
    explicit dataSave(const QString &fileName, Format format, QObject *parent = 0);

    //设置单个数据，会立即保存
    void setValue(const QString &key, const QVariant &value);
    //批量设置数据，会立即保存
    void setValues(const QString &key,QVector<int32_t> value);
    //批量设置数据，会立即保存
    void setValues(QMap<int,int32_t> value);

    //设置单个数据，不会立即保存，数据将按照系统策略执行保存
    void setValueTardy(const QString &key, const QVariant &value);
    //批量设置数据，不会立即保存，数据将按照系统策略执行保存
    void setValuesTardy(const QString &key,QVector<int32_t> value);

    //同步数据到文件系统
    void syncFile();

private:
    static int saveFd;
    QTimer  *_clearTimer;

private:
    static void flushVal();
    static void openFlushFile(const char *syncFile);
    void initCheckSaveFile(QString file);

signals:

private slots:
    void timeOutSlot();

    
};

#endif // DATASAVE_H
