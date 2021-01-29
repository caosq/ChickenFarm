#ifndef QREALTIMECURVE_H
#define QREALTIMECURVE_H

#include "plotwidget.h"
#include <QFile>
#include <QTimer>
#include <QDir>
#include <QTextCodec>

class qrealTimeCurve : public plotWidget
{
    Q_OBJECT
public:
    typedef struct{
      double *dataAddr;
      QString name;
    }curveStruct;

    friend class qHistoryCurve;
    //实时数据采集控件
    explicit qrealTimeCurve(QWidget *parent = 0);

    ~qrealTimeCurve();

    //设置采样时间周期
    void setSamplingTime(unsigned int sec = 10);
    //开始采样
    void startSampling();

    //添加曲线，index指定唯一标号，addr指定数值采样地址(一般采用double型)
    //curveName指定记录的曲线名字(不会显示出来)，color指定曲线颜色
    void setCurve(int index,void *addr,
                  QString curveName,QColor color = Qt::red);

    //添加曲线，index指定唯一标号，addr指定数值采样地址(一般采用double型)
    //curveName指定记录的曲线名字(不会显示出来)，color指定曲线颜色
    //legendItem是要绑定的数值标签(右上角的标签)
    void setCurve(int index,void *addr,QString curveName,
                  gQwtLegendItem *legendItem = 0,QColor color = Qt::red);

    //设置保存的目录
    bool setSaveDirName(QString name);
    //设置是否保存
    void setEnabledSave(bool save = true){_saveFile = save;}
    //测试是否保存
    bool enableSave(){return _saveFile;}
    //设置保存数据的时机(采集多少个数据点后再保存)
    void setSaveSamplingNum(unsigned int num);
    void enableTouchMark(bool ok = false);
    static void saveAllData();
private slots:
    void upDateCurveSlot();
    void touchDelaySlot();

public slots:
    void reloadPlot();
    static void rePlot();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    void initMenber();
    void clearAddr();
    bool checkDir(QString dir);
    void makeNote(int count);
    void writeCurveDate();
    void updateCurrentDate();
    bool isNewDay();
    bool checkFile();
    void closeFile();
    bool makeFile();
    void moveName();
    void checkHead();
    void writeDate();
    void checkMaxStorage();
    int removeFile(QString str);
    void updatePlot();
    void enroolCoreMap(qrealTimeCurve *obj);
    void removeCoreMap(qrealTimeCurve *obj);

    static QString rootDir();
    static QString textCodec();
    static QString textSplitChar();
    static QString fileFormat();
    static QString timeLegendForamt();
    static QString topLegendFormat();
    static QString fileNameSplitChar();
    static QString dateMarkSplitChar();
    static int defSampleTime();

private:

    int _maxCurveNum;

    QStringList _fileNameList;

    QTextCodec *_textCodec;
    QTimer *_sampleTime;
    unsigned int _s_Time;

    double _hideDefaultVal;
    QVector<curveStruct> _curveParameter;

    QVector<QPolygonF> _points;


    QString  _dirPath;
    QString  _fileName;
    QFile    *_curveFile;

    int _samplingNum;
    int _samplingCount;

    QVector<QString> _tempPoints;

    bool _saveFile;
    bool _touchMark;
    QTimer *_touchDelay;

    static QMap<qrealTimeCurve *,int> _coreMap;
    

};

#endif // QREALTIMECURVE_H
