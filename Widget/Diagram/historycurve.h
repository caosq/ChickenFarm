#ifndef HISTORYCURVE_H
#define HISTORYCURVE_H

#include <QFile>
#include <QStringList>
#include <QEvent>
#include <QTextStream>
#include <QStringList>
#include <QDateTime>
#include <QtAlgorithms>
#include <QDir>
#include <QTextCodec>

#include "qwtcorewidget.h"
#include "datasave.h"

class HistoryCurve : public QwtCoreWidget
{
    Q_OBJECT
public:
    explicit HistoryCurve(QWidget *parent = 0,QString filePath = "");

    void setHistoryPathName(QString name);
    void nextDay();
    void prevDay();
    int getMaxCurveNum();
    void setHistoryItemName(int index, QString str);
    void showFunction();

private:
    void InitMember();
    void updateFileList();

    void showDate();
    bool getDate(QString name);
    void checkAllFile();

public slots:
    void updateCurveShow(double min,double max);

protected:
    bool eventFilter(QObject * obj, QEvent * event);

private:
    QString directoryPath;
    QString configFilePath;
    QStringList copyFile;

    QTextCodec *_textCodec;
    QDir  *dirPath;
    QFile *configFile;
    QFile *dateFile;

    double X_ScaleMin;
    double X_ScaleMax;
    double X_ScaleStep;
    double X_ScaleMinBase;
    double X_ScaleMaxBase;

    double current_scale_val;
    double timeSecs;

    double X_ScaleJumpStep;

    double Y_ScaleMin;
    double Y_ScaleMax;
    double Y_ScaleStep;

    int curveNum;
    int curve_dot;
    int sampleTime;

    QString curve_unit;
    QVector<QString> historyCurveName;
    QStringList allFile;

    bool updateState;
    int currentFileNum;
    QStringList historyPoint;
    QVector<QPolygonF> tempPoint;

signals:
    
};

#endif // HISTORYCURVE_H
