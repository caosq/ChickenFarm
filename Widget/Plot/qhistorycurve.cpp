#include "qhistorycurve.h"

#include "qrealtimecurve.h"
//#include "languageTranslate/translate.h"
//#include "msgControl/msgmanage.h"

#include <QDebug>

QMap<qHistoryCurve *,int> qHistoryCurve::_coreMap;

qHistoryCurve::qHistoryCurve(QWidget *parent) :
    plotWidget(parent)
{
    initMember();

    updateCurrentDate();

    enroolCoreMap(this);

    setLanguage();

    connect(this,SIGNAL(reduced()),SLOT(showAll()));
//    connect(translate::instance(),SIGNAL(translatorIsReady()),this,SLOT(setLanguage()));
}

qHistoryCurve::~qHistoryCurve()
{
    removeCoreMap(this);
}


void qHistoryCurve::setSamplingTime(unsigned int sec)
{
    _sampleTime = sec;
}

void qHistoryCurve::nextDay()
{
    if( _cFileNum >= qMax(_fileList.size() - 1,0) ){
     //   msgManage::instance()->showUniversalMsg(43);
        return;
    }else{
        _cFileNum++;
        showDate();
    }
}

void qHistoryCurve::previousDay()
{
    if( _cFileNum <= 0 ){
      //  msgManage::instance()->showUniversalMsg(42);
        return;
    }else{
        _cFileNum--;
        showDate();
    }
}

void qHistoryCurve::clearAll()
{
    QString cmd = "rm ";
    cmd.append(qrealTimeCurve::rootDir());
    cmd.append("* -r");

    system(cmd.toLocal8Bit().constData());

    QList<qHistoryCurve *> list = _coreMap.keys();
    for(int i = 0; i < list.size(); i++ ){
        list[i]->updateShow();
    }
}

bool qHistoryCurve::setHistoryDir(QString name)
{
    bool ret = false;
    if( name != "" ){
        QString path = qrealTimeCurve::rootDir();
        path += name;
        _dirPath->setPath(path);
        ret = _dirPath->exists();
    }else{
        ret = false;
    }
    return ret;
}

void qHistoryCurve::showEvent(QShowEvent *e)
{
    updateShow();
    plotWidget::showEvent(e);
}

void qHistoryCurve::updateShow()
{
    updateFileList();
    showDate();
}

void qHistoryCurve::showAll()
{
    int start = 0;

    int end = _historyPoint.size();

    int curveNum = curveNumber();
    _tempPoint.clear();
    _tempPoint.resize(curveNum);

    QString line;
    QStringList lines;
    int pointNum;

    for( int i = start; i < end; i++ ){
        line = _historyPoint[i];
        lines = line.split(_textSplitChar);
        pointNum = _zeroTime.secsTo(QTime::fromString(lines.at(0),_timeLegendFormat));
        for(int j = 0; j < qMin(lines.size() - 1,curveNum); j++){
            _tempPoint[j]<<QPointF(pointNum,lines[j+1].toFloat());
        }
    }

    for(int i = 0; i < curveNum; i++){
        setSamples(i,_tempPoint[i]);
    }

}

void qHistoryCurve::loadSamples(int min)
{
    int start = (min - baseXScaleMinimum()) / _sampleTime;

    start -= xScaleJumpStep() / _sampleTime;
    if( start < 0 ){
        start = 0;
    }

    int end = start + (baseXScaleMaximum() - baseXScaleMinimum());

    if( end > _historyPoint.size() ){
        end = _historyPoint.size();
    }

    int curveNum = curveNumber();
    _tempPoint.clear();
    _tempPoint.resize(curveNum);

    QString line;
    QStringList lines;
    int pointNum;

    for( int i = start; i < end; i++ ){
        line = _historyPoint[i];
        lines = line.split(_textSplitChar);
        pointNum = _zeroTime.secsTo(QTime::fromString(lines.at(0),_timeLegendFormat));
        for(int j = 0; j < qMin(lines.size() - 1,curveNum); j++){
            _tempPoint[j]<<QPointF(pointNum,lines[j+1].toFloat());
        }
    }

    for(int i = 0; i < curveNum; i++){
        setSamples(i,_tempPoint[i]);
    }

}

void qHistoryCurve::analyseScale()
{
    if( !_historyPoint.isEmpty() ){
        QString first = _historyPoint.at(0);
        QStringList firstDetail = first.split(_textSplitChar);

        QString last = _historyPoint.at(_historyPoint.size() - 1);
        QStringList lastDetail = last.split(_textSplitChar);

        if( !firstDetail.isEmpty() && !lastDetail.isEmpty()){
            QTime firstTime = QTime::fromString(firstDetail.at(0),_timeLegendFormat);

            if( !firstTime.isNull() ){
                reloadXScale(firstTime);

                QTime lastTime = QTime::fromString(lastDetail.at(0),_timeLegendFormat);
                int lastNum = _zeroTime.secsTo(lastTime);

                int tempVal = lastNum - baseXScaleMaximum();
                int maxBarVal = tempVal / xScaleJumpStep();

                if( tempVal % xScaleJumpStep() != 0 &&
                        tempVal > 0){
                    maxBarVal++;
                }
                maxBarVal = qMax(0,maxBarVal);

                if( maxBarVal == 0 ){
                    int tempMin = baseXScaleMinimum();
                    loadSamples(tempMin);
                }

                setScrollBarMaximum(maxBarVal);
            }
        }
    }
    else
    {
        QString src_date = topLeftLegendStr();
        showEmpty();
        setTopLeftLegendShow(src_date);
    }
}

void qHistoryCurve::enroolCoreMap(qHistoryCurve *obj)
{
    _coreMap.insert(obj,_coreMap.size());
}

void qHistoryCurve::removeCoreMap(qHistoryCurve *obj)
{
    _coreMap.remove(obj);
}

void qHistoryCurve::setLanguage()
{
  /*  QLocale::Language lang = translate::instance()->currentLanguage();
    switch(lang)
    {
        case QLocale::Chinese:{
        setChinese();
    }
        break;
    case QLocale::English:{
        setEnglish();
    }
        break;
    default:
        break;
    }
*/
}

void qHistoryCurve::setChinese()
{
//    _msg->setInformativeText("没有更多数据可供显示！");
 //   _msg->setButtonText(messageBox::Yes,"关闭");
}

void qHistoryCurve::setEnglish()
{
//    _msg->setInformativeText("Not any more data！");
//    _msg->setButtonText(messageBox::Yes,"Close");
}

void qHistoryCurve::scrollBarValSlot(int val)
{
    plotWidget::scrollBarValSlot(val);

    int tempMin = baseXScaleMinimum() + ( val * xScaleJumpStep() );

    loadSamples(tempMin);
}

void qHistoryCurve::updateFileList()
{
    _fileList.clear();

    if( _dirPath->exists() ){
        _fileList = _dirPath->entryList(QStringList()<<QString("*").append(_fileFormat));
        QStringList list1;
        QStringList list2;
        list2.clear();
        list1.clear();

        for(int i = 0; i < _fileList.size(); i++ ){
            //不是副本的时候，“_”只有两个
            if( 2 == _fileList[i].count(_fileNameSplitChar) ){
                list1<<list2;
                list2.clear();
            }
            list2.prepend(_fileList[i]);
        }
        if( !list2.isEmpty() ){
            list1<<list2;
            list2.clear();
        }
        _fileList = list1;
    }

    _cFileNum = qMax(0,_fileList.size() - 1);
}

void qHistoryCurve::showDate()
{
    if( _fileList.isEmpty() ){
        showEmpty();
    }else{
        if( readFile(_fileList.at(_cFileNum)) ){
            analyseScale();
        }else{
            showEmpty();
            qDebug("2\n");
        }
    }
}

void qHistoryCurve::showEmpty()
{
    reloadXScale();
    updateCurrentDate();
    qwtPlot()->clearCurveVal();
    qwtPlot()->clearLegendItemVal();
}

void qHistoryCurve::updateCurrentDate()
{
    setTopLeftLegendShow(QDate::currentDate().toString(_topLeftLegendFormat));
}

bool qHistoryCurve::readFile(QString fileName)
{
    bool ret = false;

    if( fileName != "" ){
        if( _curveFile->isOpen() ){
            _curveFile->close();
        }
        QString filePath = _dirPath->path();
        if( !filePath.endsWith("/") ){
            filePath.append("/");
        }

        _curveFile->setFileName(filePath.append(fileName));
        _historyPoint.clear();

        if( _curveFile->open(QIODevice::ReadOnly)){
            QTextStream curveDate(_curveFile);
            QString line;

            curveDate.setCodec(_textCodec);
            line = curveDate.readLine();

            while(!curveDate.atEnd()){
                _historyPoint<<curveDate.readLine();
            }

            QString dateLabelText = fileName.remove(_fileFormat);
            setTopLeftLegendShow(dateLabelText.replace(_fileNameSplitChar,_dateMarkSplitChar));

            ret = true;
        }else{
            _curveFile->setFileName("");
            ret = false;
        }

    }else{
        ret = false;
    }

    return ret;
}

void qHistoryCurve::initMember()
{
    _dirPath = new QDir;
    _dirPath->setPath("");

    _fileList.clear();

    _curveFile = new QFile("");

    _textCodec = QTextCodec::codecForName(qrealTimeCurve::textCodec().toLocal8Bit());

    _textSplitChar = qrealTimeCurve::textSplitChar();

    _fileFormat = qrealTimeCurve::fileFormat();

    _topLeftLegendFormat = qrealTimeCurve::topLegendFormat();

    _timeLegendFormat = qrealTimeCurve::timeLegendForamt();

    _fileNameSplitChar = qrealTimeCurve::fileNameSplitChar();

    _dateMarkSplitChar = qrealTimeCurve::dateMarkSplitChar();

    _cFileNum = 0;

    _zeroTime.setHMS(0,0,0,0);

    _sampleTime = qrealTimeCurve::defSampleTime();

//    _msg = new messageBox;
}
