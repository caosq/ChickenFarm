#include "qwtcorewidget.h"

#include <qwt_plot_layout.h>
#include <QMouseEvent>
#include <QEvent>

#define WIDGET_WIDTH    850
#define WIDGET_HIGHT    550
#define SCALE_ORIGIN    0
#define SCALE_MAX       10
#define SCALE_STEP      1
#define BT_LENG         33
#define BAR_HEIGHT      26

#define MAX_CURVE 10
#define CURVE_DIR_PATH "/home/Curve/"
#define HISTORY_INFO   ".history.ini"

QIcon *ScrollBarWidget::previouIcon = NULL;
QIcon *ScrollBarWidget::nextIcon = NULL;


QwtCoreWidget::QwtCoreWidget(QWidget *parent) :
    QWidget(parent)
{
    parentPointer = parent;

    my_curve = new QwtCurveCore(this);

    nameItemList.resize(MAX_CURVE);
    nameItemList.fill(NULL);

    time_scale = NULL;

    updateXScale = true;

    zeroTime.setHMS(0,0,0);

    my_scrollBar = new ScrollBarWidget(this);
    my_scrollBar->setGeometry(0,WIDGET_HIGHT + 5,WIDGET_WIDTH,BAR_HEIGHT);
    setScrollBarAttribute();

    my_tracker = new CurveTracker(my_curve->CanvasHandle());
    my_tracker->setStateMachine( new QwtPickerTrackerMachine() );
    my_tracker->setRubberBandPen( QPen( "MediumOrchid" ) );

    my_tracker->setXText(my_curve->historyText());
    my_tracker->setXTextLabel(my_curve->historyLabelHandle());

    installEventFilter(this);

    connect(my_scrollBar,SIGNAL(valChanged(int)),SLOT(BarSlot(int)));

}

void QwtCoreWidget::setScrollBarAttribute()
{
    my_scrollBar->ReloadRange();

    my_scrollBar->setBarVal(0);
    my_scrollBar->setBarStep(1);
    my_scrollBar->setBarPageStep(1);
}


QwtCurveCore *QwtCoreWidget::getPlotHandle()
{
    return my_curve;
}

double QwtCoreWidget::CurrentScaleVal()
{
    return current_scale_val;
}

double QwtCoreWidget::xScaleConverssionMin()
{
    return X_ScaleConverssionMin;
}

bool QwtCoreWidget::eventFilter(QObject *obj, QEvent *event)
{
    if( obj == this )
    {
        if( event->type() == QEvent::Show )
        {
            updateXScale = true;
            my_scrollBar->gotoMax();
        }

        if( event->type() == QEvent::Hide )
        {
            clearHistoryTimeMark();
        }
    }
    return false;
}

void QwtCoreWidget::updateXScaleByBar(double min, double max)
{
    my_curve->setAxisScale(QwtPlot::xBottom,min,max,X_ScaleStep);
    Q_EMIT xScrolChange(min,max);
}

void QwtCoreWidget::BarSlot(int val)
{
    if( current_scale_val >= (X_ScaleConverssionMax + (val * X_ScaleJumpStep)) )
    {
        updateXScale = false;
    }
    else
        updateXScale = true;
    updateXScaleByBar( X_ScaleConverssionMin + (val * X_ScaleJumpStep),X_ScaleConverssionMax + (val * X_ScaleJumpStep));
}

void QwtCoreWidget::setXAxisScale(double min, double max, double step)
{
    my_curve->setAxisScale(QwtPlot::xBottom,min,max,step);
    X_ScaleMin = min;
    X_ScaleMax = max;
    X_ScaleStep = step;

    X_ScaleConverssionMin = X_ScaleMin;
    X_ScaleConverssionMax = X_ScaleMax;

    X_ScaleMinBase = X_ScaleMin;
    X_ScaleMaxBase = X_ScaleMax;

    X_ScaleJumpStep = X_ScaleMax / 2;

    /*do not change current_scale_val in this path*/
    /*current_scale_val must be modification in conversionToZeroTime()*/

    //current_scale_val = X_ScaleMin;
}

void QwtCoreWidget::setYAxisScale(double min, double max, double step)
{
    my_curve->setAxisScale(QwtPlot::yLeft,min,max,step);
    my_curve->updateLayout();
    Y_ScaleMin = min;
    Y_ScaleMax = max;
    Y_ScaleStep = step;
}

void QwtCoreWidget::getXAxisScale(double *min, double *max, double *step)
{
    *min = X_ScaleMinBase;
    *max = X_ScaleMaxBase;
    *step = X_ScaleStep;
}

void QwtCoreWidget::getYAxisScale(double *min, double *max, double *step)
{
    *min = Y_ScaleMin;
    *max = Y_ScaleMax;
    *step = Y_ScaleStep;
}

void QwtCoreWidget::natureReloadXAxisScale()
{
    setXAxisScale(X_ScaleMinBase,X_ScaleMaxBase,X_ScaleStep);

    setScrollBarAttribute();
    clearHistoryTimeMark();
}

void QwtCoreWidget::coercedReloadXAxisScale()
{
    /*it will be change "X_ScaleMinBase X_ScaleMaxBase",do not used in this scope*/
    //setXAxisScale(X_ScaleMin,X_ScaleMax,X_ScaleStep);

    setScrollBarAttribute();
    clearHistoryTimeMark();
}

void QwtCoreWidget::setBarSignalStep(int step)
{
    my_scrollBar->setBarStep(step);
}

void QwtCoreWidget::setBarPageStep(int step)
{
    my_scrollBar->setBarPageStep(step);
}

void QwtCoreWidget::clearCurrentTimeMark()
{
    my_curve->setCurrentTime("");
}

void QwtCoreWidget::clearHistoryTimeMark()
{
    my_curve->historyText()->setText(QString::null);
    my_curve->historyLabelHandle()->setText(*(my_curve->historyText()));
    //清除右上角的数据
    my_curve->clearItemData();
}

QDateTime QwtCoreWidget::currentDateTime()
{
    return currentDate;
}

QString QwtCoreWidget::currentTimeStr()
{
    return currentTime;
}

void QwtCoreWidget::coerceUpdateCDT(QDateTime temp)
{
    currentDate = temp;
}

void QwtCoreWidget::conversionToZeroTime(QDateTime d_time)
{
    baseDate = d_time;
    currentDate = d_time;

    X_ScaleMin = zeroTime.secsTo(baseDate.time());
    X_ScaleMax = X_ScaleMin + X_ScaleMaxBase;

    current_scale_val = X_ScaleMin;

    int residual;

    if(true)
    {
        residual = ((int)X_ScaleMin % (int)X_ScaleStep);

        X_ScaleMin = X_ScaleMin - residual;
        X_ScaleMax = X_ScaleMax - residual;

        X_ScaleConverssionMin = X_ScaleMin;
        X_ScaleConverssionMax = X_ScaleMax;
    }

    baseDate.setTime(zeroTime);

    my_curve->setAxisScale(QwtPlot::xBottom,X_ScaleMin,X_ScaleMax,X_ScaleStep);
}

void QwtCoreWidget::LoadTimeScale(QDateTime start_time)
{
    //baseTime = start_time.time();
    if( NULL == time_scale )
    {
        time_scale = new TimeScaleDraw(zeroTime);
    }
    else
    {
        delete time_scale;
        time_scale = new TimeScaleDraw(zeroTime);
    }

    my_curve->setAxisScaleDraw(QwtPlot::xBottom,time_scale);

    my_curve->updateLayout();
    /*the follow 2 line code must be after "my_curve->setAxisScaleDraw"*/
    setTrackerDate(start_time,true);
    conversionToZeroTime(start_time);
}

bool QwtCoreWidget::IsNewDay()
{
    if( baseDate.date().day() == currentDate.date().day() )
        return false;
    else
        return true;
}

void QwtCoreWidget::setJumpStep(int JumpStep)
{
    int num;
    if( JumpStep < X_ScaleStep )
        X_ScaleJumpStep = X_ScaleStep;
    else
    {
        num = (JumpStep / X_ScaleStep);
        X_ScaleJumpStep = num * X_ScaleStep;
    }
}

void QwtCoreWidget::UpdateTimeScale(unsigned int SampleStep)
{
    if( current_scale_val >= X_ScaleMax )
    {
        X_ScaleMin += X_ScaleJumpStep;
        X_ScaleMax += X_ScaleJumpStep;

        my_scrollBar->MaxmumMountUp();

        if( updateXScale )
        {
            my_curve->setAxisScale(QwtPlot::xBottom,X_ScaleMin,X_ScaleMax,X_ScaleStep);
            my_scrollBar->gotoMax();
        }
    }

    current_scale_val += SampleStep;
    currentDate = baseDate.addSecs(current_scale_val);

    setCurrentTimeMark(currentDate);//must be update after "currentDate = baseDate.addSecs(current_scale_val)"
}


bool QwtCoreWidget::AddCurve(unsigned int index)
{
    bool temp;
    temp = my_curve->AddCurve(index);

    if( temp )
    {
        nameItemList[index] = new QwtNameItem(parentPointer,index);
        nameItemList[index]->setItemColor(my_curve->getCurveColor(index));
    }
    return temp;
}

bool QwtCoreWidget::hideCurve(unsigned int index)
{
    if( my_curve->CurveHandle(index) == NULL )
        return false;
    my_curve->CurveHandle(index)->hide();
    my_curve->CurveHandle(index)->detach();
    itemNameShowOrHide(index,false);
    return true;
}

bool QwtCoreWidget::showCurve(unsigned int index)
{
    if( my_curve->CurveHandle(index) == NULL )
        return false;
    my_curve->CurveHandle(index)->show();
    my_curve->CurveHandle(index)->attach(my_curve);
    itemNameShowOrHide(index,true);
    return true;
}

bool QwtCoreWidget::curveIsVisible(unsigned int index)
{
    if( my_curve->CurveHandle(index) == NULL )
        return false;
    return my_curve->CurveHandle(index)->isVisible();
}

void QwtCoreWidget::setItemGeometry(int index, int x, int y, int w, int h)
{
    if( nameItemList[index] )
    {
        nameItemList[index]->m_setGeometry(x,y,w,h);
    }
}

bool QwtCoreWidget::setItemName(int index, QString str)
{
    if( nameItemList[index] )
    {
        nameItemList[index]->setItemName(str);
        return true;
    }
    return false;
}

void QwtCoreWidget::itemNameShowOrHide(int index,bool state)
{
    if( nameItemList[index] )
    {
        if( state )
            nameItemList[index]->show();
        else
            nameItemList[index]->hide();
    }
}

unsigned int QwtCoreWidget::getCoreAstrictNum()
{
    return MAX_CURVE;
}

QString QwtCoreWidget::getBaseSavePath()
{
    return CURVE_DIR_PATH;
}

QString QwtCoreWidget::ConfigFileName()
{
    return HISTORY_INFO;
}

int QwtCoreWidget::curveDot()
{
    return my_curve->CurveDot();
}

QString QwtCoreWidget::curveUnit()
{
    return my_curve->CurveUnit();
}

int QwtCoreWidget::JumpStep()
{
    return X_ScaleJumpStep;
}

ScrollBarWidget *QwtCoreWidget::scrollBarHandle()
{
    return my_scrollBar;
}

void QwtCoreWidget::forcibleMakeItemList(int index)
{
    if( nameItemList[index] == NULL )
    {
        nameItemList[index] = new QwtNameItem(parentPointer,index);
        nameItemList[index]->setItemColor(my_curve->forcibleGetCurveColor(index));
    }
}

void QwtCoreWidget::setCurrentTimeMark(QDateTime c_date)
{
    QString str;

    str = c_date.date().toString("yy/MM/dd  ");

    currentTime = c_date.time().toString("hh:mm:ss");

    str += currentTime;

    my_curve->setCurrentTime(str);
}

void QwtCoreWidget::setCurrentTimeMark(QString str)
{
    currentTime = "";
    my_curve->setCurrentTime(str);
}

void QwtCoreWidget::setHistoryTimeMark(QString str)
{
    my_curve->setHistoryTime(str);
}

void QwtCoreWidget::setSamples(unsigned int index,const QVector<QPointF> points)
{
    my_curve->CurveHandle(index)->setSamples(points);
}

void QwtCoreWidget::setCurveAttribute(unsigned int dot, QString unit)
{
    my_curve->setDateAttribute(dot,unit);

    my_tracker->setCurveAttribute(dot,unit);
}

void QwtCoreWidget::m_setGeometry(int x, int y, int w, int h)
{
    my_curve->setGeometry(0,0,w,h - BAR_HEIGHT - 5);
    my_scrollBar->setGeometry(0,h - BAR_HEIGHT,w,BAR_HEIGHT);
    my_scrollBar->upDateBarGeometry();

    setGeometry(x,y,w,h);
}

void QwtCoreWidget::setTrackerDate(QDateTime date,bool soigne)
{
    my_tracker->setCurveDate(date,soigne);
}

QwtCurveCore::QwtCurveCore(QWidget *parent) :
    QwtPlot(parent)
{
    InitMember();

    /*画布适配坐标*/
    plotLayout()->setAlignCanvasToScales( true );

    //-----------设置x，y坐标和范围--------------//
    setAxisScale(QwtPlot::yLeft,SCALE_ORIGIN,SCALE_MAX,SCALE_STEP);
    setAxisScale(QwtPlot::xBottom,SCALE_ORIGIN,SCALE_MAX,SCALE_STEP);

    resize(WIDGET_WIDTH,WIDGET_HIGHT);

    //replot();

    setAutoReplot( true );//设置自动重画，相当于更新

}

void QwtCurveCore::InitMember()
{
    setPalette(Qt::darkGray);

    setStyleSheet("font-size:15px;color:white;");

    //---------设置画布---------//
    canvas=new QwtPlotCanvas();

//    QPalette pal = palette();
//    pal.setBrush(QPalette::Window,QBrush(QColor(0,87,174)));
//    canvas->setPalette(pal);
    canvas->setBorderRadius(5);
    setCanvas( canvas );

    setCanvasBackground(QBrush(QColor(0,87,174)));

    //----------------设置栅格线-------------------//
    grid = new QwtPlotGrid;
    grid->enableX( true );//设置网格线
    grid->enableY( true );
    grid->setMajorPen( Qt::gray, 0, Qt::DotLine );
    grid->attach( this );

    /* date and time text*/
    QFont font("Bitstream Charter", 15);
    font.setBold(true);

    history_time = new QwtText();
    history_time->setFont(font);
    history_time->setColor(Qt::red);
    history_time->setRenderFlags( Qt::AlignTop );

    current_time = new QwtText();
    current_time->setFont(font);
    current_time->setColor(Qt::red);
    current_time->setRenderFlags( Qt::AlignLeft | Qt::AlignTop );

    historyLabel = new QwtPlotTextLabel();
    historyLabel->setText(*history_time);
    historyLabel->attach(this);

    currentLabel = new QwtPlotTextLabel();
    currentLabel->setText(*current_time);
    currentLabel->attach(this);

    my_legend = new LegendItem();
    my_legend->setMaxColumns(1);
    my_legend->setAlignment(Qt::AlignRight);
    my_legend->setBackgroundMode( QwtPlotLegendItem::ItemBackground );

    my_legend->setBorderRadius(4);
    my_legend->setMargin(0);
    my_legend->setSpacing(4);
    my_legend->setItemMargin(2);
    my_legend->attach(this);

    int i = 0;
    curve_array.resize(MAX_CURVE);
    colorList.resize(MAX_CURVE);

    const char *colors[] =
    {
        "LightSalmon",
        "Yellow",
        "Fuchsia",
        "PaleGreen",
        "Cornsilk",
        "red",
        "Peru",
        "Maroon",
        "PaleTurquoise",
        "black"
    };
    const int numColors = sizeof( colors ) / sizeof( colors[0] );
    for( i = 0; i < MAX_CURVE; i++)
    {
        curve_array[i] = NULL;
        colorList[i] = QColor(colors[ i % numColors ]);
    }

    curve_dot = 0;
    curve_unit = "";

}

void QwtCurveCore::SetLegendFormatDate(unsigned int index)
{
    if(curve_array[index])
    {
        QString temp = "";

        temp = QString::number(0,'.',curve_dot);
        temp += curve_unit;

        curve_array[index]->setTitle(temp);
    }
}

/*******************
*"LightSalmon"
*"Yellow"
*"Fuchsia"
*"PaleGreen"
*"Cornsilk"
*"HotPink"
*"Peru"
*"Maroon"
*"PaleTurquoise"
*"SteelBlue"
********************/
bool QwtCurveCore::AddCurve(unsigned int index)
{
    if( index >= MAX_CURVE )
        return false;
    if( curve_array[index] )
    {
        return false;
    }

    curve_array[index] = new QwtPlotCurve(QString::number(index));
    curve_array[index]->setPen(colorList[index],2);//设置曲线颜色 粗细
    curve_array[index]->setRenderHint(QwtPlotItem::RenderAntialiased,true);//线条光滑化
    curve_array[index]->attach(this);
    SetLegendFormatDate(index);

    return true;
}

QwtPlotCurve *QwtCurveCore::CurveHandle(unsigned int index)
{
    if( index >= MAX_CURVE )
        return NULL;

    return curve_array[index];
}

void QwtCurveCore::setCurrentTime(QString str)
{
    current_time->setText(str);
    currentLabel->setText(*current_time);
}

void QwtCurveCore::setHistoryTime(QString str)
{
    history_time->setText(str);
    historyLabel->setText(*history_time);
}

void QwtCurveCore::setDateAttribute(unsigned int dot, QString unit)
{
    curve_dot = dot;
    curve_unit = unit;

    if( curve_dot > 2 )
        curve_dot = 2;

    for(int i = 0;i < MAX_CURVE; i++)
    {
         SetLegendFormatDate(i);
    }
}

QwtPlotTextLabel *QwtCurveCore::historyLabelHandle()
{
    return historyLabel;
}

QwtText *QwtCurveCore::historyText()
{
    return history_time;
}

QColor QwtCurveCore::getCurveColor(int index)
{
    QColor temp(Qt::black);

    if( curve_array[index] )
        return curve_array[index]->pen().color();
    return temp;
}

QColor QwtCurveCore::forcibleGetCurveColor(int index)
{
    return colorList[index];
}

int QwtCurveCore::CurveDot()
{
    return curve_dot;
}

QString QwtCurveCore::CurveUnit()
{
    return curve_unit;
}

void QwtCurveCore::clearItemData()
{
    QString temp;
    temp = QString::number(0,'.',curve_dot);
    temp += curve_unit;
    for(int i = 0; i < MAX_CURVE; i++)
    {
        if(curve_array[i] != NULL)
            curve_array[i]->setTitle(temp);
    }
}

void QwtCurveCore::clearAllCurveData()
{
    for(int i = 0; i < MAX_CURVE; i++)
    {
        if(curve_array[i] != NULL){
            curve_array[i]->setSamples(QVector<QPointF>(0,QPointF(0,0)));
        }
    }
}

QwtPlotCanvas *QwtCurveCore::CanvasHandle()
{
    return canvas;
}


QwtPlotGrid *QwtCurveCore::GridHandle()
{
    return grid;
}


QwtScrollBar::QwtScrollBar(QWidget *parent) :
    QScrollBar(parent)
{

    setStyleSheet("QScrollBar:horizontal"
                      "{"
                      "border-image:url(:/images/baseFile/sc_slot_background.png);"
                      "margin:0px 33px 0px 33px;"
                      "}"
                      "QScrollBar::handle:horizontal"
                      "{"
                      "background-color:#acd5fd;"
                      "border-radius:3px;"
                      "min-width:60;"
                      "}"
                      "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal"
                      "{"
                      "background:none;"
                      "}"
                      "QScrollBar::add-line:horizontal"
                      "{"
                      "border-image:url(:/images/baseFile/sc_bar_right.png);"
                      "height:26px;"
                      "width:32px;"
                      "subcontrol-position:right;"
                      "subcontrol-origin: margin;"
                      "}"
                      "QScrollBar::sub-line:horizontal"
                      "{"
                      "border-image:url(:/images/baseFile/sc_bar_left.png);"
                      "height:26px;"
                      "width:33px;"
                      "subcontrol-position:left;"
                      "subcontrol-origin: margin;"
                      "}"

                      );

    barMove = false;
    timeState = false;

    installEventFilter(this);
}

bool QwtScrollBar::sliderIsMove()
{
    return barMove;
}

/************************************************
*点击滚动条的槽(这里被设置为透明)，会触发滑块的
*移动，但是释放是滚动条没有更新。这里监听timer事件
*在释放时手动发出valueChanged信号
************************************************/

bool QwtScrollBar::eventFilter(QObject *obj, QEvent *event)
{
    if( this == obj )
    {
        if( event->type() == QEvent::MouseMove )
            barMove = true;
        if( event->type() == QEvent::Timer )
            timeState = true;
        if( event->type() == QEvent::MouseButtonRelease )
        {
            barMove = false;
            if( timeState )
            {
                timeState = false;
                Q_EMIT valueChanged(this->value());
            }
        }
    }
    return false;
}

ScrollBarWidget::ScrollBarWidget(QWidget *parent,int width, int height):
    QWidget(parent)
{
    my_bar = new QwtScrollBar(this);
    my_bar->setOrientation(Qt::Horizontal);
    //my_bar->setGeometry(BT_LENG + 5,0,width - (BT_LENG + 5) * 2,height);
    my_bar->setGeometry(BT_LENG,0,width - BT_LENG * 2,height);

    QSize bt_size(BT_LENG,BAR_HEIGHT);

    getIcon();

    previouPage = new QPushButton(this);
    previouPage->setStyleSheet("border:0px;");
    previouPage->setIconSize(bt_size);
    previouPage->setIcon(*previouIcon);
    previouPage->setFocusPolicy(Qt::NoFocus);
    previouPage->setGeometry(0,0,BT_LENG,BAR_HEIGHT);

    nextPage = new QPushButton(this);
    nextPage->setStyleSheet("border:0px;");
    nextPage->setIconSize(bt_size);
    nextPage->setIcon(*nextIcon);
    nextPage->setFocusPolicy(Qt::NoFocus);
    nextPage->setGeometry(width - BT_LENG,0,BT_LENG,BAR_HEIGHT);

    max_val = 0;

    connect(my_bar,SIGNAL(valueChanged(int)),SLOT(barSLot(int)));
    connect(my_bar,SIGNAL(sliderReleased()),SLOT(sliderReleaseSLot()));
    connect(my_bar,SIGNAL(sliderPressed()),SLOT(sliderPressSlot()));
    connect(previouPage,SIGNAL(pressed()),SLOT(previouPageSLot()));
    connect(nextPage,SIGNAL(pressed()),SLOT(nextPageSlot()));
}

void ScrollBarWidget::upDateBarGeometry()
{
    //my_bar->setGeometry(BT_LENG + 5,0,this->width() - (BT_LENG + 5) * 2,BAR_HEIGHT);
    my_bar->setGeometry(BT_LENG,0,this->width() - BT_LENG * 2,BAR_HEIGHT);

    nextPage->setGeometry(this->width() - BT_LENG,0,BT_LENG,BAR_HEIGHT);
}

QwtScrollBar *ScrollBarWidget::BarHandle()
{
    return my_bar;
}

void ScrollBarWidget::setBarMaxmum(int max)
{
    max_val = max;
    my_bar->setMaximum(max);
}

void ScrollBarWidget::MaxmumMountUp()
{
    max_val++;
    my_bar->setMaximum(max_val);
}

void ScrollBarWidget::ReloadRange()
{
    my_bar->setMinimum(0);
    my_bar->setMaximum(0);
    max_val = 0;
}

void ScrollBarWidget::gotoMax()
{
    if( my_bar->sliderIsMove() )
    {
        return;
    }
    my_bar->setValue(my_bar->maximum());

    //如果最大值是0(只有一屏的数据时)，当前值等于最大值不会触发valueChanged
    //导致历史记录只有一屏时不刷新显示，这里强制刷一下
    if( my_bar->value() == my_bar->maximum() )
        Q_EMIT valChanged(my_bar->maximum());
}

//连续移动时不发送更新信号，提高效率
void ScrollBarWidget::barSLot(int val)
{
    if( my_bar->sliderIsMove() )
    {
        release_val = val;
        return;
    }

    Q_EMIT valChanged(val);
}

void ScrollBarWidget::sliderReleaseSLot()
{
    if( press_val == my_bar->value() )
        return;

    Q_EMIT valChanged(release_val);
}

void ScrollBarWidget::sliderPressSlot()
{
    press_val = my_bar->value();
}

void ScrollBarWidget::nextPageSlot()
{
    my_bar->setValue(my_bar->maximum());
}

void ScrollBarWidget::previouPageSLot()
{
    my_bar->setValue(my_bar->minimum());
}


void ScrollBarWidget::setBarMinimum(int min)
{
    my_bar->setMinimum(min);
}


void ScrollBarWidget::setBarVal(int val)
{
    my_bar->setValue(val);
}

void ScrollBarWidget::setBarPageStep(int step)
{
    my_bar->setPageStep(step);
}

void ScrollBarWidget::setBarStep(int step)
{
    my_bar->setSingleStep(step);
}

LegendItem::LegendItem():
    QwtPlotLegendItem()
{
    setRenderHint(QwtPlotItem::RenderAntialiased);
    QColor  color(Qt::white);

    setTextPen( color );
    QColor c( Qt::gray );
    c.setAlpha( 200 );

    setBorderPen( c );
    setBackgroundBrush( c );
}

QwtNameItem::QwtNameItem(QWidget *parent,int num):
    QWidget(parent)
{
    colorBlock = new QLabel(this);
    colorBlock->setGeometry(0,2,16,16);

    curveName = new QLabel(this);
    curveName->setStyleSheet("color:white;font-size:16px;");
    curveName->setGeometry(18,0,62,20);

    index = num;

    minWidth = 80;
    minHeight = 20;

    this->setMinimumSize(80,20);
    this->hide();
}

void QwtNameItem::setItemColor(QColor color)
{
    QString color_str = "border-radius:3px;background-color:";

    color_str += color.name();

    colorBlock->setStyleSheet(color_str);
}

void QwtNameItem::setItemName(QString str)
{
    curveName->setText(str);
}

void QwtNameItem::m_setGeometry(int x, int y, int w, int h)
{
    this->setGeometry(x,y,w,h);

    colorBlock->setGeometry(0,(this->height() - 16) / 2,16,16);
    curveName->setGeometry(18,0,w - 18,h);

    this->show();
}

