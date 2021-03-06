#include "qrealtimecurve.h"

#include <unistd.h>
#include <syslog.h>
#include <error.h>
#include "timesetting.h"

#define ROOT_DIR "/home/Curve/"

#define DEF_TEXT_CODEC "gb2312"
#define DEF_SPLIT_C ","
#define DEF_FORMAT  ".csv"

#define DEF_FILE_NAME_SPLIT "_"
#define DEF_DATE_FORMAT_SPLIT "/"
#define DEF_BASE_FILENAME_FORMAT "yy_MM_dd"
#define DEF_EXPAND_FILENAME_FORMAT "yy_MM_dd%1%2"
#define DEF_FILE_TIME_LEGNED_FORMAT "hh:mm:ss"
#define DEF_TOP_LEFT_LEGEND_FORMAT "yy/MM/dd"

#define MAX_SAMPLING_NUM    120
#define DEF_SAMPLING_NUM    120
#define MAX_SAVE_DAY    30
#define MAX_SAME_NUM    9

#define DEF_SAMPLING_SEC 10
#define DEF_DAY_OF_SEC  86400   //60 * 60 * 24
#define FAST_TEST_MULTIPLE 50
#define TOUCH_DELAY_TIME 200

QMap<qrealTimeCurve *,int> qrealTimeCurve::_coreMap;

qrealTimeCurve::qrealTimeCurve(QWidget *parent) :
    plotWidget(parent)
{
    initMenber();
    clearAddr();
    updateCurrentDate();

    connect(_sampleTime,SIGNAL(timeout()),SLOT(upDateCurveSlot()));
    connect(_touchDelay,SIGNAL(timeout()),this,SLOT(touchDelaySlot()));

    connect(timeSetting::instance(),SIGNAL(systemTimeChange()),this,SLOT(reloadPlot()));
    enroolCoreMap(this);

}

qrealTimeCurve::~qrealTimeCurve()
{
    removeCoreMap(this);
}

void qrealTimeCurve::setSamplingTime(unsigned int sec)
{
    _s_Time = sec;
#ifdef FAST_TEST
    _sampleTime->setInterval(_s_Time * (1000 / FAST_TEST_MULTIPLE));
#else
    _sampleTime->setInterval(_s_Time * 1000);
#endif
}

bool qrealTimeCurve::setSaveDirName(QString name)
{
    bool ret = false;
    if( name != ""){
        QString temp = ROOT_DIR;
        temp += name;

        if( checkDir(temp) ){
            _dirPath.clear();
            _dirPath = temp;

            if( !_dirPath.endsWith("/") )
                _dirPath += "/";
            ret = true;
        }else{
            ret = false;
        }
    }else{

    }
    setEnabledSave(ret);
    return ret;
}

void qrealTimeCurve::startSampling()
{
    _sampleTime->start();
}

void qrealTimeCurve::setSaveSamplingNum(unsigned int num)
{
    if( num > MAX_SAMPLING_NUM )
        num = MAX_SAMPLING_NUM;

    _samplingNum = num;

    if(!_tempPoints.isEmpty())
        _tempPoints.clear();
    _tempPoints.resize(num);
    _tempPoints.fill("");
}

void qrealTimeCurve::enableTouchMark(bool ok)
{
    _touchMark = ok;
    if( ok ){
        disablePlotPicker(ok);
    }
}

void qrealTimeCurve::saveAllData()
{
    QList<qrealTimeCurve *> list = _coreMap.keys();
    for(int i = 0; i < list.size(); i++){
        list[i]->writeCurveDate();
    }
}

void qrealTimeCurve::rePlot()
{
    QList<qrealTimeCurve *> list = _coreMap.keys();
    for(int i = 0; i < list.size(); i++){
        list[i]->reloadPlot();
    }
}

void qrealTimeCurve::reloadPlot()
{
    _sampleTime->stop();
    updatePlot();
    _sampleTime->start();
}

void qrealTimeCurve::setCurve(int index, void *addr, QString curveName,
                              QColor color)
{

    if( index >= _maxCurveNum ){return;}
    if( addCurve(index, color) )
    {
        _curveParameter[index].dataAddr = (double *)addr;
        if( curveName != "" )
        {
            _curveParameter[index].name = curveName;
        }
    }
}

void qrealTimeCurve::setCurve(int index, void *addr, QString curveName,
                              gQwtLegendItem *legendItem, QColor color)
{
    if( index >= _maxCurveNum )
        return;

    if( addCurve(index,legendItem,color) ){
        _curveParameter[index].dataAddr = (double *)addr;

        if( curveName != "" ){
            _curveParameter[index].name = curveName;
        }
    }
}

QString qrealTimeCurve::rootDir()
{
    return ROOT_DIR;
}

void qrealTimeCurve::initMenber()
{
    _maxCurveNum = g_qwtPlot::maxCurveCapability();

    _dirPath = "";
    _samplingNum = DEF_SAMPLING_NUM;
    _samplingCount = 0;

    _textCodec = QTextCodec::codecForName(DEF_TEXT_CODEC);

    //hideDefaultVal隐藏时默认显示数据
    _hideDefaultVal = 0;
    _saveFile = false;

    _touchMark = false;
    _touchDelay = new QTimer;
    _touchDelay->setInterval(TOUCH_DELAY_TIME);

    _curveFile = new QFile;
    _curveFile->setFileName("");

    _fileNameList.clear();

    _curveParameter.resize(_maxCurveNum);
    _points.resize(_maxCurveNum);

    _tempPoints.resize(DEF_SAMPLING_NUM);
    _tempPoints.fill("");

    _s_Time = DEF_SAMPLING_SEC;

    _sampleTime = new QTimer();
    _sampleTime->setInterval(_s_Time * 1000);
}

void qrealTimeCurve::clearAddr()
{
    for(int i = 0; i < _maxCurveNum; i++ ){
        _curveParameter[i].dataAddr = 0;
        _curveParameter[i].name = QString::number(i);
    }
}

bool qrealTimeCurve::checkDir(QString dir)
{
    if( dir == "" )
        return false;

    QDir path(dir);

    if(!path.exists()){
        return path.mkpath(dir);
    }

    return true;
}

void qrealTimeCurve::makeNote(int count)
{
#ifdef FAST_TEST
    QTime time;
    time.setHMS(0,0,0);
    _tempPoints[count] = time.addSecs(currentXScaleVal()).toString(DEF_FILE_TIME_LEGNED_FORMAT);
#else
    _tempPoints[count] = QTime::currentTime().toString(DEF_FILE_TIME_LEGNED_FORMAT);
#endif

    double tempVal = 0;
    for(int i = 0; i < _maxCurveNum; i++){
        if( _curveParameter[i].dataAddr ){
            if( curveVisible(i) ){
                tempVal = *(_curveParameter[i].dataAddr);
            }else{
                tempVal = _hideDefaultVal;
            }

            _points[i]<<QPointF(currentXScaleVal(),tempVal);
            _tempPoints[count] += DEF_SPLIT_C;
            _tempPoints[count] += QString::number(tempVal);

            setSamples(i,_points[i]);
        }
    }

    _tempPoints[count] += "\n";
}

void qrealTimeCurve::checkHead()
{
    if(  _curveFile->size() == 0 ){
        QString head = "time";
        for(int i = 0; i < _curveParameter.size(); i++ ){
            if( _curveParameter[i].dataAddr ){
                head += DEF_SPLIT_C;
                head += _curveParameter[i].name;
            }
        }

        head += "\n";
        QByteArray strArray = _textCodec->fromUnicode(head);
        _curveFile->write(strArray);
        _curveFile->flush();
        fsync(_curveFile->handle());
    }
}

void qrealTimeCurve::writeDate()
{
    checkHead();

    int i = 0;
    QByteArray dataArray;
    for( i = 0; i < _samplingCount; i++){
        dataArray = _textCodec->fromUnicode(_tempPoints[i]);
        _curveFile->write(dataArray);
    }

    _curveFile->flush();
    fsync(_curveFile->handle());
}

void qrealTimeCurve::writeCurveDate()
{
    if( !enableSave() ){
        return;
    }

    if( !checkDir(_dirPath) ){
        return;
    }

    if( checkFile() ){
        writeDate();
    }

    _tempPoints.fill("");
    _samplingCount = 0;
}

void qrealTimeCurve::updateCurrentDate()
{
    setTopLeftLegendShow(QDate::currentDate().toString(DEF_TOP_LEFT_LEGEND_FORMAT));
}

bool qrealTimeCurve::isNewDay()
{
    if( currentXScaleVal() >= DEF_DAY_OF_SEC ){
        return true;
    }else{
        return false;
    }
}

bool qrealTimeCurve::checkFile()
{
    bool ret = false;
    if( !_curveFile->isOpen() || !_curveFile->exists()){
        ret = makeFile();
        if( !ret ){
            qDebug("make curve file faile!\n");
            syslog(LOG_ERR,_curveFile->errorString().toLocal8Bit().constData());
            qDebug()<<_curveFile->errorString();
        }
    }else{
        ret = true;
    }
    return ret;
}

void qrealTimeCurve::closeFile()
{
    _curveFile->close();
    _curveFile->setFileName("");
}

bool qrealTimeCurve::makeFile()
{
    QString relativelyName = QDate::currentDate().toString(DEF_BASE_FILENAME_FORMAT);
    relativelyName += DEF_FORMAT;

    _curveFile->setFileName(relativelyName.prepend(_dirPath));
    if( _curveFile->exists()){
        if( _curveFile->isOpen() ){
            _curveFile->close();
        }
        moveName();
    }else{
        checkMaxStorage();
    }
    _curveFile->setFileName(relativelyName);
    return _curveFile->open(QIODevice::ReadWrite);
}

void qrealTimeCurve::moveName()
{
    QFile tempFile;

    QString relativelyName = QDate::currentDate().toString(DEF_EXPAND_FILENAME_FORMAT);
    relativelyName += DEF_FORMAT;

    QString fileName = relativelyName.prepend(_dirPath);

    QString today = fileName.arg("").arg("");
    QString currentName;
    QString prevName;

    int i = 0;

    prevName = fileName.arg("_").arg(0);

    for( i = 0; i < MAX_SAME_NUM; i++ ){
        tempFile.setFileName(fileName.arg("_").arg(i));
        if( tempFile.exists() ){
            if( tempFile.isOpen() )
                tempFile.close();
        }else{
            break;
        }
    }

    if( i >= MAX_SAME_NUM ){
        tempFile.remove();
        i = MAX_SAME_NUM - 1;
    }

    while(i){
        currentName = fileName.arg("_").arg(i);
        i--;
        prevName = fileName.arg("_").arg(i);

        tempFile.setFileName(prevName);
        tempFile.rename(currentName);
    }

    tempFile.setFileName(today);

    tempFile.rename(prevName);
}

void qrealTimeCurve::checkMaxStorage()
{
    QDir filePath;

    QStringList fileList;
    QStringList resultList;

    if( _dirPath != "" ){
        filePath.setPath(_dirPath);
        fileList = filePath.entryList(QStringList()<<QString("*").append(DEF_FORMAT));

        for(int i = 0; i < fileList.size(); i++){
            if(fileList[i].count("_") == 3)
                continue;
            else
                resultList<<fileList[i];
        }
    }

    for( int i = 0; i < resultList.size() - MAX_SAVE_DAY; i++ ){
        if( -1 == removeFile(resultList.at(i)) ){
            qDebug("remove curve file faile.%d\n",error);
        }
    }

}

int qrealTimeCurve::removeFile(QString str)
{
    QByteArray removeMember;
    QString cmd;
    QString basecmd = "rm ";
    basecmd.append(_dirPath);
    basecmd.append("%1");

    cmd = basecmd.arg(str.replace(DEF_FORMAT,"*"));
    removeMember = cmd.toUtf8();
    qDebug()<<removeMember;
    return system(removeMember.constData());
}

void qrealTimeCurve::updatePlot()
{
    writeCurveDate();

    reloadXScale();

    updateCurrentDate();

    for(int i = 0; i < _maxCurveNum; i++){
        if( _curveParameter[i].dataAddr){
            _points[i].clear();
            setSamples(i,_points[i]);
        }
    }

    closeFile();
}

void qrealTimeCurve::enroolCoreMap(qrealTimeCurve *obj)
{
    _coreMap.insert(obj,_coreMap.size());
}

void qrealTimeCurve::removeCoreMap(qrealTimeCurve *obj)
{
    _coreMap.remove(obj);
}

QString qrealTimeCurve::textCodec()
{
    return DEF_TEXT_CODEC;
}

QString qrealTimeCurve::textSplitChar()
{
    return DEF_SPLIT_C;
}

QString qrealTimeCurve::fileFormat()
{
    return DEF_FORMAT;
}

QString qrealTimeCurve::timeLegendForamt()
{
    return DEF_FILE_TIME_LEGNED_FORMAT;
}

QString qrealTimeCurve::topLegendFormat()
{
    return DEF_TOP_LEFT_LEGEND_FORMAT;
}

QString qrealTimeCurve::fileNameSplitChar()
{
    return DEF_FILE_NAME_SPLIT;
}

QString qrealTimeCurve::dateMarkSplitChar()
{
    return DEF_DATE_FORMAT_SPLIT;
}

int qrealTimeCurve::defSampleTime()
{
    return DEF_SAMPLING_SEC;
}

void qrealTimeCurve::upDateCurveSlot()
{
    updateTimeScale(_s_Time);

    if( isNewDay() ){
        updatePlot();
    }

    makeNote(_samplingCount);

    _samplingCount++;

    if( _samplingCount >= _samplingNum ){
        writeCurveDate();
    }

    updateCurrentDate();
}

void qrealTimeCurve::touchDelaySlot()
{
    _touchDelay->stop();
    disablePlotPicker(false);
}

void qrealTimeCurve::showEvent(QShowEvent *e)
{
    writeCurveDate();
    plotWidget::showEvent(e);

    if( _touchMark ){
        if( _touchDelay ){
            _touchDelay->start();
        }
    }
}

void qrealTimeCurve::hideEvent(QHideEvent *e)
{
    if( _touchMark ){
        disablePlotPicker(true);
    }
}



