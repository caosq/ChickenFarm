#include "realtimecurve.h"

#include <QtAlgorithms>
#include <unistd.h>
#include <stdlib.h>

#define DEF_TEXT_CODEC "gb2312"

#define MAX_SAMPLING_NUM    120
#define DEF_SAMPLING_NUM    60
#define MAX_SAVE_DAY    15
#define MAX_SAME_NUM    2

RealTimeCurve::RealTimeCurve(QWidget *parent) :
    QwtCoreWidget(parent)
{
    InitMember();
    InitAddr();
    connect(sampleTime,SIGNAL(timeout()),SLOT(upDateCurveSlot()));
}

void RealTimeCurve::InitMember()
{
    maxCurveNum = getCoreAstrictNum();

    absoluteDirPath = "";
    relativelyPath = "";
    sampling_num = DEF_SAMPLING_NUM;
    sampling_count = 0;

    _textCodec = QTextCodec::codecForName(DEF_TEXT_CODEC);

    //hideDefaultVal隐藏时默认显示数据
    hideDefaultVal = 0;
    save_file = false;

    curveFile = new QFile;
    historyList = new QFile;

    fileNameList.clear();


    data_addr.resize(maxCurveNum);
    save_name.resize(maxCurveNum);
    points.resize(maxCurveNum);

    temp_points.resize(DEF_SAMPLING_NUM);
    temp_points.fill("");

    sampleTime = new QTimer();
    sampleTime->setInterval(10 * 1000);

}

RealTimeCurve::~RealTimeCurve()
{

}

void RealTimeCurve::InitAddr()
{
    int i = 0;
    for(i = 0; i < maxCurveNum; i++){
        data_addr[i] = 0;
        save_name[i] = QString::number(i);
    }
}

void RealTimeCurve::enableSave(bool yes)
{
    save_file = yes;
}

bool RealTimeCurve::checkDir(QString dir)
{
    if( dir == "" )
        return false;
    relativelyPath = dir;

    QString temp = getBaseSavePath();

    temp += dir;

    QDir path(temp);

    if(!path.exists()){
        return path.mkpath(temp);
    }

    return true;
}

bool RealTimeCurve::checkFile()
{
    int i = 0;

    QString prevName;
    QString currentName;
    QString today;
    QString relativelyName;

    relativelyName = QDateTime::currentDateTime().date().toString("yy_MM_dd%1%2.csv");

    QString temp = absoluteDirPath;
    temp += relativelyName;

    fileName = temp.arg("").arg("");
    today = fileName;
    relativelyName = relativelyName.arg("").arg("");

    /*如果i = 0并且for循环退出，会直接使用到prevName，所以必须初始*/
    prevName = temp.arg("_").arg(QString::number(0));

    if( curveFile->isOpen() )
        curveFile->close();
    curveFile->setFileName(fileName);
    if( curveFile->exists() )
    {
        if( curveFile->isOpen() )
            curveFile->close();
        /*for循环结束后会得到可用的文件名称*/
        for( i = 0; i < MAX_SAME_NUM; i++ )
        {
            fileName.clear();
            fileName = temp.arg("_").arg(QString::number(i));

            curveFile->setFileName(fileName);
            if( curveFile->exists() )
            {
                if( curveFile->isOpen() )
                    curveFile->close();
            }
            else
                break;
        }
        /*如果 i = MAX_SAME_NUM 说明可用副本文件名以用完，删除最旧的副本文件,让出一个可用名*/
        if( i >= MAX_SAME_NUM )
        {
            curveFile->remove();
            i = MAX_SAME_NUM - 1;
        }
        /*循环修改副本文件名字，列:16_01_02_1.csv->16_01_02_2.csv*/
        while(i)
        {
            /*currentName 为可用名称,也是即将成为更加旧的文件名*/
            currentName = temp.arg("_").arg(QString::number(i));
            i--;
            /*prevName为现有的旧文件*/
            prevName = temp.arg("_").arg(QString::number(i));

            curveFile->setFileName(prevName);
            curveFile->rename(currentName);
        }

        /*经过上面的操作这里会空出一个副本名称xx_xx_xx_0.csv xx为年月日*/
        curveFile->setFileName(today);
        /*把当前日期历史文件重命名为xx_xx_xx_0.csv*/
        curveFile->rename(prevName);
    }
    /*至此，所有副本文件的名称以经变旧，最旧的副本文件被删除，当天历史文件变成副本0文件*/
    /*文件xx_xx_xx.csv始终为最新文件*/
    updateFileList(relativelyName);
    if( openFile(today) )
    {
        return writeHead();
    }
    return false;
}

void RealTimeCurve::updateFileList(QString str)
{
    int i = 0;
    bool isFind = false;
    int max = fileNameList.size();

    for(i = 0; i < max; i++)
    {
        if( fileNameList.at(i) == str )
        {
            isFind = true;
            break;
        }
    }

    if( isFind )
    {
        if( i == max - 1 )
            return;
        QString temp = fileNameList.at(i);
        fileNameList.removeAt(i);
        fileNameList<<temp;
        updateConfigFile(str,i,false);
    }
    else
    {
        if( i < MAX_SAVE_DAY)
        {
            fileNameList<<str;
            updateConfigFile(str,-1,false);
        }
        else
        {
            fileNameList.removeAt(0);
            fileNameList<<str;
            updateConfigFile(str,-1,true);
        }
    }
}

void RealTimeCurve::updateConfigFile(QString str,int target,bool removeOld)
{
    QString listName;
    listName = absoluteDirPath;
    listName += ConfigFileName();
    int max_key = 0;

    QList<int> intKey;
    QString oldName;
    QString oldKey;

    intKey = getSettingsKey(listName);

    dataSave settings(listName,QSettings::IniFormat);

    if( intKey.size() != 0 )
    {
        max_key = intKey.at(intKey.size()-1) + 1;

        oldKey = QString::number(intKey.at(0));
        oldName = settings.value(oldKey).toString();
    }

    if( removeOld )
    {
        removeFile(oldName);
        settings.remove(oldKey);
    }
    else if( -1 != target )
    {
        settings.remove(QString::number(intKey.at(target)));
    }
    settings.setValue(QString::number(max_key),str);
}

bool RealTimeCurve::removeFile(QString str)
{
    QByteArray removeMember;
    QString cmd;
    QString basecmd = "rm ";
    basecmd.append(absoluteDirPath);
    basecmd.append("%1");

    cmd = basecmd.arg(str.replace(".csv","*"));
    removeMember = cmd.toUtf8();

    return system(removeMember.constData());
}

void RealTimeCurve::checkHistoryList()
{
    /*********************************
    *  configFileName历史记录配置文件名
    *  configFile是否存在配置文件标志
    *  historyFile是否存在历史文件标志
    *  filePath历史文件保存路径
    ********************************/
    QString configFileName;
    bool configFile = false;
    bool historyFile = false;
    QDir filePath;

    if( absoluteDirPath != "" )
    {
        //检测是否存在历史文件
        configFileName = absoluteDirPath;
        configFileName += ConfigFileName();
        historyList->setFileName(configFileName);

        configFile = historyList->exists();
    }

    /********************************
    *  fileList历史文件列表
    *  nameFilter文件名过滤规则
    *  resultList去除副本名称后的列表
    *******************************/
    QStringList fileList;
    QStringList nameFilter;
    QStringList resultList;

    nameFilter<<"*.csv";

    if( absoluteDirPath != "" )
    {
        //resultList得到去除副本名的文件列表
        filePath.setPath(absoluteDirPath);
        filePath.setNameFilters(nameFilter);

        fileList = filePath.entryList(QDir::NoFilter,QDir::NoSort);

        if( fileList.size() )
        {
            historyFile = true;
            for(int i = 0; i < fileList.size(); i++)
            {
                if(fileList[i].count("_") == 3)
                    continue;
                else
                    resultList<<fileList[i];
            }
        }
    }
    //历史记录和配置文件同时存在
    if( historyFile == true && configFile == true )
    {
        QList<int> intKey;
        QString name;
        QString keyNum;

        intKey = getSettingsKey(configFileName);
        dataSave settings(configFileName,QSettings::IniFormat);

        //检索配置文件中的文件名，如果配置文件中的文件不存在则删除该记录
        //insertOneNode把文件名更新到链表
        fileNameList.clear();
        for(int i = 0; i < intKey.size(); i++)
        {
            keyNum = QString::number(intKey.at(i));
            name = settings.value(keyNum).toString();
            if( resultList.contains(name) )
            {
                fileNameList<<name;
                resultList.removeOne(name);
            }
            else
            {
                settings.remove(keyNum);
            }
        }
        settings.syncFile();

        //删除多余文件
        for(int i = 0; i < resultList.size(); i++)
        {
            removeFile(resultList[i]);
        }
        return;
    }

    //historyFile不存在则直接返回
    if( historyFile == false && configFile == true )
    {
        if( historyList->isOpen() )
            historyList->close();
        historyList->remove();
        return;
    }

    //历史文件存在，配置文件不存在，则创建配置文件，并按文件名的时间从小到大排列
    //超出最大存储天数时删除最旧的历史记录文件
    if( historyFile == true && configFile == false )
    {
        int num = resultList.size() - MAX_SAVE_DAY;
        //历史文件数是否超出最大限制
        if( num > 0 )
        {
            //删除超出部分，超出部分为时间最小的部分
            for( int i = 0; i < num; i++ )
            {
                removeFile(resultList[i]);
                resultList.removeAt(i);
            }
        }

        dataSave settings(configFileName,QSettings::IniFormat);
        //创建配置文件,insertOneNode把文件名更新到链表
        fileNameList.clear();
        for( int i = 0; i < resultList.size(); i++ )
        {
            settings.setValueTardy(QString::number(i),resultList[i]);
            fileNameList<<resultList[i];
            //insertOneNode(resultList[i],i);
        }
        settings.syncFile();

        return;
    }
}

bool RealTimeCurve::openFile(QString file)
{
    if( file != curveFile->fileName() )
    {
        curveFile->close();
        curveFile->setFileName(file);
    }

    if( curveFile->isOpen() && curveFile->exists() )
        return true;
    else
    {
        curveFile->close();
        return curveFile->open(QIODevice::ReadWrite);
    }

}

bool RealTimeCurve::writeHead()
{
    //x轴,最大，最小，步长
    //y轴,最大，最小，步长
    //other,数据精度，单位，跳转步长,转换后X轴最小值,采样时间
    double argMin,argMax,argStep;
    QString arg1,arg2,arg3,arg4,arg5;

    QString x_ScaleAttribute = "%1,%2,%3\n";
    QString y_ScaleAttribute = "%1,%2,%3\n";
    QString otherAttribute = "%1,%2,%3,%4,%5\n";

    getXAxisScale(&argMin,&argMax,&argStep);
    arg1 = QString::number(argMin);
    arg2 = QString::number(argMax);
    arg3 = QString::number(argStep);
    x_ScaleAttribute = x_ScaleAttribute.arg(arg1).arg(arg2).arg(arg3);

    getYAxisScale(&argMin,&argMax,&argStep);
    arg1 = QString::number(argMin);
    arg2 = QString::number(argMax);
    arg3 = QString::number(argStep);
    y_ScaleAttribute = y_ScaleAttribute.arg(arg1).arg(arg2).arg(arg3);

    arg1 = QString::number(curveDot());
    arg2 = curveUnit();
    arg3 = QString::number(JumpStep());
    arg4 = QString::number(xScaleConverssionMin());
    arg5 = QString::number(S_Time);
    otherAttribute = otherAttribute.arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5);

    QString head;
    head = x_ScaleAttribute;
    head += y_ScaleAttribute;
    head += otherAttribute;
    head += "number,DateTime";

    for(int i = 0; i < maxCurveNum; i++)
    {
        if(data_addr[i])
        {
            head += ",";
            head += save_name[i];
        }
    }
    head += "\n";

    QByteArray strArray = _textCodec->fromUnicode(head);
    curveFile->write(strArray);
    curveFile->flush();
    fsync(curveFile->handle());
    return true;
}

void RealTimeCurve::writeCurveDate(bool writeState)
{
    if( !save_file || !writeState )
    {
        temp_points.fill("");
        sampling_count = 0;
        fileNameList.clear();
        if(curveFile->isOpen())
            curveFile->close();
        return;
    }

    //检测保存路径
    if( !checkDir(relativelyPath) )
        return;

    //检测是否存在
    if( !curveFile->exists() )
    {
        openFile(curveFile->fileName());
    }

    //检测是否成功打开
    if(!curveFile->isOpen())
        return;

    if(curveFile->size() == 0)
        writeHead();

    int i = 0;
    QByteArray dataArray;
    for( i = 0; i < sampling_count; i++){
        dataArray = _textCodec->fromUnicode(temp_points[i]);
        curveFile->write(dataArray);
    }

    //clear up temp_points
    temp_points.fill("");
    sampling_count = 0;

    curveFile->flush();
    fsync(curveFile->handle());

}

bool RealTimeCurve::makeNewFile()
{
    if( save_file )
    {
        if( curveFile->isOpen() )
        {
            curveFile->flush();
            fsync(curveFile->handle());
            curveFile->close();
        }
        return checkFile();
    }
    return false;
}

QList<int> RealTimeCurve::getSettingsKey(QString name)
{
    QStringList sourceList;
    QList<int>  intList;

    dataSave settings(name,QSettings::IniFormat);

    sourceList = settings.allKeys();
    for(int i = 0; i < sourceList.size();i++ )
    {
        intList<<sourceList.at(i).toInt();
    }

    qSort(intList.begin(),intList.end());

    return intList;
}

bool RealTimeCurve::setSaveDirName(QString dir_name)
{
    if( checkDir(dir_name) )
    {
        absoluteDirPath.clear();
        absoluteDirPath = getBaseSavePath();
        absoluteDirPath += dir_name;

        if( !absoluteDirPath.endsWith("/") )
            absoluteDirPath += "/";

        return true;
    }
    else
        return false;
}

void RealTimeCurve::setSamplingTime(unsigned int sec)
{
    S_Time = sec;
    sampleTime->setInterval(S_Time * 1000);
}

void RealTimeCurve::setDebugSamplingTime(unsigned int msec, unsigned int samplginTime)
{
    S_Time = samplginTime;
    sampleTime->setInterval(msec);
}

//第一次启动时先检测历史文件列表，并更新文件名，开启循环
void RealTimeCurve::startSampling()
{
    checkHistoryList();
    if( checkFile() )
        enableSave(true);

    sampleTime->start();
}

void RealTimeCurve::restartSampling()
{
    checkHistoryList();
    //应该判断下是否新建文件成功的！但testFile不能调用两次，先这样
    enableSave(true);
    sampleTime->start();
}

void RealTimeCurve::setHideDefaultVal(double val)
{
    hideDefaultVal = val;
}

void RealTimeCurve::stopSampling()
{
    sampleTime->stop();
}

void RealTimeCurve::setSaveSamplingNum(unsigned int num)
{
    if( num > MAX_SAMPLING_NUM )
        num = MAX_SAMPLING_NUM;

    sampling_num = num;

    if(!temp_points.isEmpty())
        temp_points.clear();
    temp_points.resize(num);
    temp_points.fill("");
}

void RealTimeCurve::updateCurrentTimeMark(QDateTime c_date)
{
    setCurrentTimeMark(c_date);
}

void RealTimeCurve::setCurve(int index, void *addr,QString curveSaveName)
{
    if( index >= maxCurveNum )
        return;

    AddCurve(index);
    data_addr[index] = (double *)addr;

    if( curveSaveName != "" )
        save_name[index] = curveSaveName;
}

void RealTimeCurve::natureUpdate()
{
    LoadTimeScale(currentDateTime());
    natureReloadXAxisScale();

    for(int i = 0; i < maxCurveNum; i++)
    {
        if(data_addr[i])
        {
            points[i].clear();
        }
    }

    makeNewFile();
}

void RealTimeCurve::coercedUpdate()
{
    LoadTimeScale(currentDateTime());
    coercedReloadXAxisScale();

    for(int i = 0; i < maxCurveNum; i++)
    {
        if(data_addr[i])
        {
            points[i].clear();
            //强制更新时定时器并没有到点，所以不会刷新曲线，这里手动刷一下
            setSamples(i,points[i]);
        }
    }
    makeNewFile();
}

//coercedUpdateSlot(bool saveCopy)
//的状态量是为了控制writeCurveDate(saveCopy);
//要不要执行一次写入的操作

//因为默认会产生副本，所以当清除了曲线
//记录文件后，如果这时强制刷新当前曲线的话
//默认是会把当前数据写入文件的。
//这样在后面检测时发现有同名的文件，就会
//产生16/04/12/0类似这样的副本
void RealTimeCurve::coercedUpdateSlot(bool saveCopy)
{
    stopSampling();

    writeCurveDate(saveCopy);

    clearCurrentTimeMark();

    coerceUpdateCDT(QDateTime::currentDateTime());

    coercedUpdate();

//    coercedUpdate中调用了makeNewFile,
//    startSampling中的testFile也会调用makeNewFile
//    这样会导致当天数据直接变成副本1
    //startSampling();

    restartSampling();
}

void RealTimeCurve::saveData()
{
    writeCurveDate();
}

void RealTimeCurve::upDateCurveSlot()
{
    /*used currentTimeStr() after is code;UpdateTimeScale will change currentTimeStr*/
    UpdateTimeScale(S_Time);/*first,we make addition*/

    if( IsNewDay() )
    {
        writeCurveDate();
        natureUpdate();
    }

    temp_points[sampling_count] = QString::number(CurrentScaleVal());
    temp_points[sampling_count] += ",";
    temp_points[sampling_count] += currentTimeStr();

    double tempVal = 0;
    for(int i = 0; i < maxCurveNum; i++)
    {
        if(data_addr[i])
        {
            if( curveIsVisible(i) )
                tempVal = *(data_addr[i]);
            else
                tempVal = hideDefaultVal;
            points[i]<<QPointF(CurrentScaleVal(),tempVal);
            temp_points[sampling_count] += ",";
            temp_points[sampling_count] += QString::number(tempVal);

            setSamples(i,points[i]);
        }
    }

    temp_points[sampling_count] += "\n";

    sampling_count++;

    if( sampling_count >= sampling_num )
    {
        writeCurveDate();
    }
}

CurveThread::CurveThread()
{
    baseTime = new QTimer();
    baseTime->setInterval(10 * 1000);

    //connect(baseTime,SIGNAL(timeout()),SLOT(start()));
}

void CurveThread::setTime(unsigned int time)
{
    baseTime->setInterval( time * 1000 );
}

void CurveThread::ReloadTime(unsigned int time)
{
    baseTime->stop();
    baseTime->setInterval(time * 1000);
    baseTime->start();
}

void CurveThread::StartRunning()
{
    baseTime->start();
}

void CurveThread::StopRunning()
{
    baseTime->stop();
}

void CurveThread::run()
{

}
