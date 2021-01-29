#ifndef QHISTORYCURVE_H
#define QHISTORYCURVE_H

#include <QDir>
#include <QFile>
#include "plotwidget.h"
//#include "universalModule/messageModule/messagebox.h"

class qHistoryCurve : public plotWidget
{
    Q_OBJECT
public:
    //历史曲线显示控件
    explicit qHistoryCurve(QWidget *parent = 0);
    
    ~qHistoryCurve();
    //设置读取的文件目录
    bool setHistoryDir(QString name);
    //设置采样时间，这里的采用时间必须和实际采集数据时的一样
    void setSamplingTime(unsigned int sec);
    //清楚所有历史数据记录
    static void clearAll();

private:
    void initMember();
    void updateFileList();

    void showDate();
    void showEmpty();
    void updateCurrentDate();
    bool readFile(QString fileName);
    void loadSamples(int min);
    void analyseScale();
    void enroolCoreMap(qHistoryCurve *obj);
    void removeCoreMap(qHistoryCurve *obj);

    void setChinese();
    void setEnglish();

private slots:
    virtual void scrollBarValSlot(int val);
    void updateShow();
    void showAll();
    //设置语言
    void setLanguage();

public slots:
    void nextDay();
    void previousDay();

protected:
    void showEvent(QShowEvent *e);
    
private:
    QTextCodec *_textCodec;

    QDir  *_dirPath;
    QFile *_curveFile;
    QStringList _fileList;

    QString _textSplitChar;
    QString _timeLegendFormat;
    QString _fileFormat;
    QString _topLeftLegendFormat;
    QStringList _historyPoint;
    QString _fileNameSplitChar;
    QString _dateMarkSplitChar;

    QTime _zeroTime;

    int _cFileNum;
    int _sampleTime;

    QVector<QPolygonF> _tempPoint;
    static QMap<qHistoryCurve *,int> _coreMap;
//    messageBox  *_msg;
};

#endif // QHISTORYCURVE_H
