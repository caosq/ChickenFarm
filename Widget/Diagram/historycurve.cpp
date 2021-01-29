#include "historycurve.h"
/*******************************************************
*setHistoryPathName设置历史记录目录
*installEventFilter插入事件过滤，监测事件，这里show是会执行动作
********************************************************/

#define DEF_TEXT_CODEC "gb2312"

HistoryCurve::HistoryCurve(QWidget *parent,QString filePath) :
    QwtCoreWidget(parent)
{
    _textCodec = QTextCodec::codecForName(DEF_TEXT_CODEC);

    setHistoryPathName(filePath);
    connect(this,SIGNAL(xScrolChange(double,double)),SLOT(updateCurveShow(double,double)));
    installEventFilter(this);
}

void HistoryCurve::updateCurveShow(double min, double max)
{
    if( !updateState )
        return;
    int start = (min - timeSecs) / sampleTime;
    int end = (max - timeSecs) / sampleTime;

    start -= X_ScaleStep / sampleTime;
    if( start < 0 )
    {
        start = 0;
    }
    end += X_ScaleStep / sampleTime;
    if( end > historyPoint.size() )
    {
        end = historyPoint.size();
    }

    tempPoint.clear();
    tempPoint.resize(curveNum);
    QString line;
    QStringList lines;
    int pointNum;
    for( int i = start ; i < end; i++ )
    {
        line = historyPoint[i];
        lines = line.split(",");
        pointNum = lines[0].toInt();
        for(int j = 0; j < curveNum; j++)
        {
            tempPoint[j]<<QPointF(pointNum,lines[j+2].toFloat());
        }
    }
    getPlotHandle()->clearAllCurveData();

    for(int i = 0; i < curveNum; i++)
    {
        setSamples(i,tempPoint[i]);
    }
}

/*****************************************
*历史路径名不为空则初始化历史曲线类成员
******************************************/
void HistoryCurve::setHistoryPathName(QString name)
{
    if( name == "" )
    {
        directoryPath = "";
        return;
    }
    else
    {
        directoryPath = name;
        InitMember();
    }
}

/*****************************************
*最新的文件对应的序号为0，如果不是0就自减
*清除历史显示
******************************************/
void HistoryCurve::nextDay()
{
    if( currentFileNum <= 0 )
        return;
    else
    {
        while( currentFileNum > 0 )
        {
            currentFileNum--;
            QString tempName = allFile[currentFileNum];
            QFile tempFile;

            tempName = tempName.insert(0,directoryPath);
            tempFile.setFileName(tempName);
            if( tempFile.exists() )
            {
                //清除当前时间显示
                clearHistoryTimeMark();
                showDate();
                break;
            }
            else
                allFile.removeAt(currentFileNum);
        }
    }
}

/*****************************************
*最旧的文件对应的序号为列表大小减1
*(假设大小是4，实际成员是0，1，2，3)
*清除历史显示
******************************************/
void HistoryCurve::prevDay()
{
    if( currentFileNum >= allFile.size() - 1 )
        return;
    else
    {
        while( currentFileNum < allFile.size() - 1 )
        {
            currentFileNum++;
            QString tempName = allFile[currentFileNum];
            QFile tempFile;

            tempName = tempName.insert(0,directoryPath);
            tempFile.setFileName(tempName);
            if( tempFile.exists() )
            {
                //清除当前时间显示
                clearHistoryTimeMark();
                showDate();
                break;
            }
            else
                allFile.removeAt(currentFileNum);
        }
    }
}

int HistoryCurve::getMaxCurveNum()
{
    return curveNum;
}

void HistoryCurve::setHistoryItemName(int index, QString str)
{
    AddCurve(index);
    //forcibleMakeItemList(index);
    setItemName(index,str);
}

void HistoryCurve::showFunction()
{
    updateFileList();
    clearHistoryTimeMark();
    showDate();
}

/****************************************************************
*1.getBaseSavePath获取核心组件中历史文件保存的起始路径
*(如/home/Curve,固定了所有历史文件存在于/home/Curve目录下)
*2.ConfigFileName配置文件名称(配置文件保存现有历史记录以及文件的存在顺序)
*3.X_ScaleMin,X_ScaleMax.x轴最小值，x轴最大值,数据读取时会改变
*4.X_ScaleMinBase,X_ScaleMaxBase,设置x轴范围,X_ScaleStep设置x轴步长
*5.Y_ScaleMin,Y_ScaleMax,设置Y轴范围,Y_ScaleStep设置Y轴步长
*6.X_ScaleJumpStep设置x轴跳转步长(点快进和后退的跳转值)
*7.timeSecs起始时间秒数(也是x轴实际起始点或起始时间)
*8.currentFileNum当前文件号(上一天，下一天用到)
*9.curveNum曲线条数;
*10.curve_dot精度
*11.curve_unit单位;
****************************************************************/
void HistoryCurve::InitMember()
{
    QString basePath = getBaseSavePath();
    basePath += directoryPath;
    directoryPath = basePath;

    if( !directoryPath.endsWith("/") )
        directoryPath += "/";
    configFilePath = directoryPath;
    configFilePath += ConfigFileName();

    X_ScaleMin = 0;
    X_ScaleMax = 20 * 60;
    X_ScaleMinBase = 0;
    X_ScaleMaxBase = 0;
    X_ScaleStep = 2 * 60;
    X_ScaleJumpStep = 10 * 60;
    timeSecs = 0;

    Y_ScaleMin = -10;
    Y_ScaleMax = 10;
    Y_ScaleStep = 1;

    currentFileNum = 0;
    curveNum = 0;
    curve_dot = 0;
    curve_unit = "";
    sampleTime = 0;
    updateState = false;

    allFile.clear();
    dirPath = new QDir;

    configFile = new QFile;
    configFile->setFileName(configFilePath);

    dateFile = new QFile;
}

/*******************************************************
*读取配置文件，根据配置文件的新旧顺序插入historyCurveName
*并创建所有文件列表(checkAllFile)
********************************************************/
void HistoryCurve::updateFileList()
{
    if(directoryPath == "")
        return;
    if( !configFile->exists() || configFile->size() == 0 )
        return;
    dataSave settings(configFilePath,QSettings::IniFormat);

    QStringList key;
    QList<int> intKey;
    key = settings.allKeys();

    for(int i = 0; i < key.size(); i++ )
    {
        intKey<<key.at(i).toInt();
    }
    qSort(intKey.begin(),intKey.end());

    historyCurveName.resize(key.size());
    historyCurveName.fill("");

    for(int i = 0; i < intKey.size(); i++)
    {
//        historyCurveName[i] = settings.value(QString::number(intKey.at(i))).toString();
    }

    checkAllFile();
    currentFileNum = 0;
}

/*******************************************************
*根据historyCurveName的文件新旧顺序逐个检测其副本，
*并且插入到allFile中
********************************************************/
void HistoryCurve::checkAllFile()
{
    QStringList nameFilter;
    QString fileName;
    int num = 0;

    dirPath->setPath(directoryPath);

    num = historyCurveName.size() - 1;
    allFile.clear();
    for(int i = num; i >= 0; i-- )
    {
        fileName = historyCurveName[i];
        fileName = fileName.replace(".csv","*");

        nameFilter.clear();
        nameFilter<<fileName;

        dirPath->setNameFilters(nameFilter);
        allFile += dirPath->entryList(QDir::NoFilter,QDir::NoSort);
    }
}

/*******************************************************
*显示数据
********************************************************/
void HistoryCurve::showDate()
{
    if(directoryPath == "")
        return;
    QString currentDateName;
    bool dateReady = false;

    if( allFile.size() == 0 )
        return;
    currentDateName = allFile[currentFileNum];
    dateReady = getDate(currentDateName);
    //设置左上角日期
    currentDateName = currentDateName.replace(QString(".csv"),QString(""));
    currentDateName.replace("_","/");
    setCurrentTimeMark(currentDateName);

    if( dateReady )
    {
        QDate my_date;
        QTime my_time(0,0,0,0);
        QDateTime my_DateTime;

        //用来设置x轴时间轴
        currentDateName = currentDateName.left(8);
        currentDateName.insert(0,"20");
        my_date = QDate::fromString(currentDateName, "yyyy_MM_dd");
        //数据起始时间
        my_time = my_time.addSecs(timeSecs);

        my_DateTime.setDate(my_date);
        my_DateTime.setTime(my_time);

        setXAxisScale(X_ScaleMinBase,X_ScaleMaxBase,X_ScaleStep);
        setYAxisScale(Y_ScaleMin,Y_ScaleMax,Y_ScaleStep);

        setCurveAttribute(curve_dot,curve_unit);
        LoadTimeScale(my_DateTime);
        setJumpStep(X_ScaleJumpStep);

        for(int i = 0; i < curveNum; i++ )
        {
            AddCurve(i);
            //setSamples(i,historyPoint[i]);
        }
        //gotoMax必须在setXAxisScale后面使用，否则会由于qwtcorewidget中的成员变量未初始化而导致内存错误
        scrollBarHandle()->gotoMax();
    }
}

/************************************************
*1 数据头格式
*2 x轴最小值，x轴最大值，x步长
*3 y轴最小值，y轴最大值，y步长
*4 精度,单位，跳转步长，实际起始时间,采样时间
*5 数据点序号，录入时间，曲线一数据，曲线二数据...
*6 实际数据
************************************************/
bool HistoryCurve::getDate(QString name)
{
    if( dateFile->isOpen() )
        dateFile->close();
    name = name.insert(0,directoryPath);
    dateFile->setFileName(name);

    if( !dateFile->exists() || dateFile->size() == 0 )
        return false;

    if( !dateFile->isOpen() )
        dateFile->open(QIODevice::ReadOnly);

    QTextStream curveDate(dateFile);
    QString line;
    QStringList lines;

    curveDate.setCodec(_textCodec);

    line = curveDate.readLine();
    lines = line.split(",");

    //x轴最小值
    X_ScaleMinBase = lines[0].toInt();
    //x轴最大值
    X_ScaleMaxBase = lines[1].toInt();
    //x步长
    X_ScaleStep = lines[2].toInt();

    if( X_ScaleMaxBase == 0 || X_ScaleStep == 0 )
        return false;

    lines.clear();
    line.clear();

    line = curveDate.readLine();
    lines = line.split(",");

    //y轴最小值
    Y_ScaleMin = lines[0].toInt();
    //y轴最大值
    Y_ScaleMax = lines[1].toInt();
    //y步长
    Y_ScaleStep = lines[2].toInt();
    if( Y_ScaleMax == 0 || Y_ScaleStep == 0 )
        return false;
    lines.clear();
    line.clear();

    line = curveDate.readLine();
    lines = line.split(",");

    //精度
    curve_dot = lines[0].toUInt();
    //单位
    curve_unit = lines[1];
    //跳转步长
    X_ScaleJumpStep = lines[2].toUInt();
    //实际起始时间
    timeSecs = lines[3].toInt();
    //采样时间
    sampleTime = lines[4].toInt();

    if( X_ScaleJumpStep == 0 || sampleTime == 0)
        return false;
    //x轴起始时间
    X_ScaleMin = timeSecs;

    //加上x轴范围值，刚好就是一页数据长度
    X_ScaleMax = X_ScaleMin + ( X_ScaleMaxBase - X_ScaleMinBase );

    line = curveDate.readLine();
    lines = line.split(",");

    //曲线数量，为什么减2看上面第5点格式
    curveNum = lines.size() - 2;

    lines.clear();
    line.clear();

    historyPoint.clear();
    //还没有数据时直接返回
    //曲线刷新标志，这里是false后面的刷新步骤直接跳过
    updateState = false;
    if( curveDate.atEnd() )
    {
        scrollBarHandle()->ReloadRange();
        updateState = true;
        return true;
    }
    //录入实际数据，更新滚动条数值
    while(!curveDate.atEnd())
    {
        line = curveDate.readLine();
        historyPoint<<line;
    }

    int pointNum = 0;
    int maxBarVal = 0;
    int tempMax = X_ScaleMax;
    int tempStep = X_ScaleJumpStep;

    //lines是最后一条数据
    lines = line.split(",");
    //数据点序号(以每天0点0分0秒开始计数的秒数)
    pointNum = lines[0].toUInt();

    //最后一个点的序号整除步长，得到滚动条最大值
    //(这里滚动的步长强制设置为1，所以有多少屏数据，最大值就是多少)
    //如果存在余数则加1，多出的那一些数据即使不足一屏也需要
    //多显示一页

    maxBarVal = ( pointNum - tempMax ) / tempStep;
    if( ( pointNum - tempMax ) % tempStep != 0 && ( pointNum - tempMax ) > 0 )
    {
        maxBarVal++;
    }

    //这里maxBarVal存在负数的可能，所以当是负数时就等于0
    if(maxBarVal < 0)
        maxBarVal = 0;

    //更新当前滚动条x轴范围值
    X_ScaleMin += (maxBarVal * tempStep);
    X_ScaleMax += (maxBarVal * tempStep);
    //初始化滚动条数值
    updateState = false;
    scrollBarHandle()->ReloadRange();
    scrollBarHandle()->setBarMaxmum(maxBarVal);
    updateState = true;
    return true;
}

/**************************************
*检测显示事件，每次显示时更新文件列表并显示数据
*清除历史显示
***************************************/
bool HistoryCurve::eventFilter(QObject *obj, QEvent *event)
{
    if( obj == this )
    {
        if( event->type() == QEvent::Show )
        {
            showFunction();
//            updateFileList();
//            clearHistoryTimeMark();
//            showDate();
        }
    }
    return false;
}

