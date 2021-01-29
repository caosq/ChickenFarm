#ifndef REALTIMECURVE_H
#define REALTIMECURVE_H

#include "qwtcorewidget.h"

#include <QDateTime>
#include <QTimer>
#include <QMap>

#include <QThread>
#include <QDir>
#include <QStringList>
#include <QTextCodec>

#include "datasave.h"

class CurveThread : public QThread
{
public:
    CurveThread();

    void setTime(unsigned int time = 10);

    void ReloadTime(unsigned int time = 10);
    void StartRunning();
    void StopRunning();

protected:
    void run();

private:
    QTimer  *baseTime;


signals:

public slots:

};


class RealTimeCurve : public QwtCoreWidget
{
    Q_OBJECT
public:
    explicit RealTimeCurve(QWidget *parent = 0);

    void setSamplingTime(unsigned int sec = 10);
    void startSampling();
    void setHideDefaultVal(double val);
    void setSaveSamplingNum(unsigned int num = 60);

    bool setSaveDirName(QString dir_name = "");

    void setCurve(int index = 0,void *addr = 0,QString curveSaveName = "");

    void setDebugSamplingTime(unsigned int msec = 100,unsigned int samplginTime = 10);
    void coercedUpdateSlot(bool saveCopy = true);
    void saveData();
    void enableSave(bool yes = true);

    ~RealTimeCurve();
private:
    void InitAddr();
    void InitMember();
    void natureUpdate();
    void coercedUpdate();
    void updateCurrentTimeMark(QDateTime c_date);

    void stopSampling();
    void restartSampling();

    bool checkDir(QString dir = "");
    bool checkFile();
    void checkHistoryList();

    bool writeHead();
    bool openFile(QString file);
    void writeCurveDate(bool writeState = true);
    bool makeNewFile();

    QList<int> getSettingsKey(QString name);
    bool removeFile(QString str);
    void updateFileList(QString str);
    void updateConfigFile(QString str,int target,bool removeOld);

private:
    int maxCurveNum;

    QStringList fileNameList;

    QTextCodec *_textCodec;
    QTimer *sampleTime;
    unsigned int S_Time;

    double hideDefaultVal;
    QVector<double *>data_addr;

    QVector<QPolygonF> points;
    QVector<QString> save_name;

    QString  absoluteDirPath;
    QString  relativelyPath;
    QString  fileName;
    QFile    *curveFile;
    QFile    *historyList;

    int sampling_num;
    int sampling_count;

    QVector<QString>    temp_points;

    bool save_file;

signals:
    
public slots:
    void upDateCurveSlot();
};

#endif // REALTIMECURVE_H
