#include "plotwidget.h"

#define ZOOM_PIX_OFF ":/UI/baseFile/zoomOff.png"
#define ZOOM_PIX_ON ":/UI/baseFile/zoomOn.png"
#define ZOOM_CLEAR ":/UI/baseFile/zero.png"
#define ZOOM_REDUCE ":/UI/baseFile/reduce.png"

//DEF_ZOOM_MARGIN_RIGHT_SPACE
#define DEF_ZM_RS 90
//DEF_ZOOM_MARGIN_BOTTOM_SPACE
#define DEF_ZM_BS 140

#define DEF_WIDTH 920
#define DEF_HEIGHT 600

#define SCBAR_W_SPEC 40
#define SCBAR_H_SPEC 5

#define TOP_LEFT_GEOMETRY   20,10,150,25

#define DEF_ITEM_SPACE 5
#define DEF_ITEM_TOP_SPACE 10
#define DEF_ITEM_RIGHT_SPACE 35

#define DEF_XSCALE_MIN 0
#define DEF_XSCALE_MAX 1200
#define DEF_XSCALE_STEP 120

#define DEF_YSCALE_MIN 0
#define DEF_YSCALE_MAX 1000
#define DEF_YSCALE_STEP 100

#define REDUCE_XSCALE_MIN_VAL 0
#define REDUCE_XSCALE_MAX_VAL 86400
#define REDUCE_XSCALE_STEP 7200

plotWidget::plotWidget(QWidget *parent) :
    QWidget(parent)
{
    initValue();
    initTimeScale();
    setGeometry(0,0,DEF_WIDTH,DEF_HEIGHT);
    setTopLeftLegendGeometry();

    connect(scBar,SIGNAL(valueChanged(int)),this,SLOT(scrollBarValSlot(int)));
}

void plotWidget::updateTimeScale(unsigned int sampleStep)
{
    if( _currentXScaleVal >= _currentXScaleMax ){
        _currentXScaleMin += _xScaleJumpStep;
        _currentXScaleMax += _xScaleJumpStep;

        if( scBar->currentValue() >= scBar->maximum() ){
            gPlot->setAxisScale(QwtPlot::xBottom,_currentXScaleMin,
                         _currentXScaleMax,_xScaleStepVal);
        }

        scBar->setMaximumVal(scBar->maximum() + 1);

    }
    _currentXScaleVal += sampleStep;
}

bool plotWidget::curveVisible(int index)
{
    return gPlot->curveVisible(index);
}

int plotWidget::currentXScaleVal()
{
    return _currentXScaleVal;
}

void plotWidget::reloadXScale()
{
    setXAxisScale(0,_xScaleVal,_xScaleStepVal);
}

void plotWidget::reloadXScale(QTime time)
{
    setXAxisScale(0,_xScaleVal,_xScaleStepVal,time);
}

void plotWidget::reloadScrollBar()
{
    scBar->reloadScrollBar();
}

int plotWidget::baseXScaleMaximum()
{
    return _baseXScaleMax;
}

int plotWidget::baseXScaleMinimum()
{
    return _baseXScaleMin;
}

int plotWidget::xScaleJumpStep()
{
    return _xScaleJumpStep;
}

void plotWidget::setScrollBarMaximum(int max)
{
    scBar->setMaximumVal(max);
}

int plotWidget::curveNumber()
{
    return gPlot->curveNumber();
}

void plotWidget::setXAxisScale(double min, double max,
                               double step, QTime time)
{
    _xScaleVal = max - min;
    _xScaleStepVal = step;

    _xScaleJumpStep = _xScaleVal / 2;

    int baseTime = _zeroTime.secsTo(time);
    int residual = ((int)baseTime % (int)_xScaleStepVal);
    baseTime = baseTime - residual;

    _baseXScaleMin = baseTime;
    _baseXScaleMax = baseTime + _xScaleVal;

    _currentXScaleMin = _baseXScaleMin;
    _currentXScaleMax = _baseXScaleMax;

    _currentXScaleVal = _currentXScaleMin;

    gPlot->setAxisScale(QwtPlot::xBottom,_currentXScaleMin,
                        _currentXScaleMax,_xScaleStepVal);

    reloadScrollBar();
}

void plotWidget::disablePlotPicker(bool ok)
{
    if(_plotPicker){
        _plotPicker->setEnabled(!ok);
    }
}

bool plotWidget::plotPickerIsEnable()
{
    bool ok = false;
    if( _plotPicker ){
        ok = _plotPicker->isEnabled();
    }
    return ok;
}

QString plotWidget::topLeftLegendStr()
{
    return _topLeftShow->text();
}

void plotWidget::setXAxisScale(double min, double max, double step)
{
    _xScaleVal = max - min;
    _xScaleStepVal = step;

    _xScaleJumpStep = _xScaleVal / 2;

    QTime cTime = QTime::currentTime();

    int baseTime = _zeroTime.secsTo(cTime);
    int residual = ((int)baseTime % (int)_xScaleStepVal);

    _currentXScaleVal = baseTime;

    baseTime = baseTime - residual;

    _baseXScaleMin = baseTime;
    _baseXScaleMax = baseTime + _xScaleVal;

    _currentXScaleMin = _baseXScaleMin;
    _currentXScaleMax = _baseXScaleMax;

    gPlot->setAxisScale(QwtPlot::xBottom,_currentXScaleMin,
                        _currentXScaleMax,_xScaleStepVal);


    reloadScrollBar();
}

void plotWidget::setYAxisScale(double min, double max, double step)
{
    gPlot->setAxisScale(QwtPlot::yLeft,min,max,step);
}

void plotWidget::setLegendItemSpace(int space)
{
    _itemSpace = space;
}

void plotWidget::setLegendItemTopSpace(int space)
{
    _itemTopSpace = space;
}

void plotWidget::setLegendItemRightSpace(int space)
{
    _itemRightSpace = space;
}

bool plotWidget::addCurve(int index, QColor color)
{
    if( gPlot->addCurve(index,color) ){
        gQwtLegendItem *temp = new gQwtLegendItem(this);
        _item.insert(index,temp);
        gPlot->connectLegendItem(index,temp);
        return true;
    }else{
        return false;
    }
}

bool plotWidget::addCurve(int index, gQwtLegendItem *legendItem,
                          QColor color)
{
    if( gPlot->addCurve(index,color) ){
        if( legendItem ){
            _item.insert(index,legendItem);
            gPlot->connectLegendItem(index,legendItem);
        }
        return true;
    }else{
        return false;
    }
}

void plotWidget::showCurve(int index, bool on)
{
    gPlot->showCurve(index,on);
}

void plotWidget::setGeometry(int x, int y, int w, int h)
{
    gPlot->setGeometry(0,0,w,h - scBar->height() - SCBAR_H_SPEC);
    scBar->setGeometry(SCBAR_W_SPEC,h - scBar->height(),
                       w - SCBAR_W_SPEC * 2,scBar->height());
    QWidget::setGeometry(x,y,w,h);
}

void plotWidget::setCurveAttribute(int dot, QString unit)
{
    gPlot->setCurveAttribute(dot,unit);
}

void plotWidget::setSamples(unsigned int index,
                            const QVector<QPointF> points)
{
    gPlot->setSamples(index,points);
}

void plotWidget::setTopLeftLegendShow(QString str)
{
    _topLeftShow->setText(str);
}

void plotWidget::scrollBarValSlot(int val)
{
    int tempMin = _baseXScaleMin + ( val * _xScaleJumpStep );
    int tempMax = _baseXScaleMax + ( val * _xScaleJumpStep );

    gPlot->setAxisScale(QwtPlot::xBottom,tempMin,
                        tempMax,_xScaleStepVal);
}

void plotWidget::setLegendGeometry()
{
    QList<gQwtLegendItem *> list = _item.values();

    QSize size = gQwtLegendItem::legendSizeHint();

    int startX = this->width() - DEF_ITEM_RIGHT_SPACE - size.width();
    int startY = DEF_ITEM_TOP_SPACE;

    for(int i = 0; i < list.size(); i++){
        list[i]->setGeometry(startX,startY + i * (size.height() + DEF_ITEM_SPACE),
                                              size.width(),size.height());
    }

    for(int i = 0; i < _zoomItem.size(); i++){
        _zoomItem[i]->move(this->width() - DEF_ZM_RS,
                           this->height() - (DEF_ZM_BS + i * _zoomItem[i]->size().height()));
    }

    gPlot->clearLegendItemVal();
}

void plotWidget::setTopLeftLegendGeometry()
{
    QRect rect = QRect(TOP_LEFT_GEOMETRY);
    rect.setLeft(gPlot->yScaleWidth() + 20);
    _topLeftShow->setGeometry(rect);
}

void plotWidget::initValue()
{
    _itemSpace = DEF_ITEM_SPACE;
    _itemTopSpace = DEF_ITEM_TOP_SPACE;
    _itemRightSpace = DEF_ITEM_RIGHT_SPACE;

    _xScaleVal = DEF_XSCALE_MAX - DEF_XSCALE_MIN;
    _xScaleStepVal = DEF_XSCALE_STEP;

    _baseXScaleMin = DEF_XSCALE_MIN;
    _baseXScaleMax = DEF_XSCALE_MAX;

    _currentXScaleMin = DEF_XSCALE_MIN;
    _currentXScaleMax = DEF_XSCALE_MAX;
    _currentXScaleVal = DEF_XSCALE_MIN;

    _xScaleJumpStep = _xScaleVal / 2;

    gPlot = new g_qwtPlot(this);
    scBar = new gqwtScrollBarWidget(this);
    _plotPicker = new gPlotPicker(gPlot);

    _topLeftShow = new QLabel(this);
    _topLeftShow->setStyleSheet("color:red;font-size:15px;");

    //_zoomer = new gQwtPlotZoomer(QwtPlot::xTop, QwtPlot::yLeft,gPlot->canvas() );
    _zoomer = new gQwtPlotZoomer(gPlot->canvas());
    _zoomer->setRubberBandPen(_plotPicker->rubberBandPen().color());
    _zoomer->setEnabled(false);

    _reduceDepth = 0;

    QStringList pixList;
    pixList<<ZOOM_CLEAR<<ZOOM_PIX_ON<<ZOOM_REDUCE;

    _zoomItem.resize(zoomTypeSize);
    for(int i = 0; i < zoomTypeSize; i++){
        _zoomItem[i] = new gtoolButton(this);
        _zoomItem[i]->setPixmap(gtoolButton::press,pixList.at(i));
        _zoomItem[i]->setPixmap(gtoolButton::release,pixList.at(i));
        _zoomItem[i]->resize(_zoomItem[i]->sizeHint());
    }

    _zoomItem[zoom]->setPixmap(gtoolButton::release,ZOOM_PIX_OFF);

    connect( _zoomItem[zoom], SIGNAL( toggled(bool) ), SLOT( enableZoomMode( bool ) ) );
    connect( _zoomItem[clear], SIGNAL( released()), SLOT(clearZoomSlot()) );
    connect( _zoomItem[reduce],SIGNAL(released()),SLOT(reduceSlot()));
    connect(_zoomer,SIGNAL(zoomed(QRectF)),SLOT(updateYScale(QRectF)));
}

void plotWidget::enableZoomMode( bool on )
{
    _plotPicker->clearDisplay();
    _zoomer->setEnabled(on);
}

void plotWidget::clearZoomSlot()
{
    _plotPicker->clearDisplay();
    _zoomer->zoom(0);
    gPlot->updateScale();
     setTopLeftLegendGeometry();
}

void plotWidget::reduceSlot()
{
    _plotPicker->clearDisplay();
    clearZoomSlot();
    gPlot->setTempAxisScale(QwtPlot::xBottom,
                            REDUCE_XSCALE_MIN_VAL,
                            REDUCE_XSCALE_MAX_VAL,
                            REDUCE_XSCALE_STEP);

    setTopLeftLegendGeometry();
    emit reduced();
}

void plotWidget::updateYScale(QRectF rect)
{
    setTopLeftLegendGeometry();
  //  qDebug("updateYScale");
}

void plotWidget::initTimeScale()
{
    setXAxisScale(DEF_XSCALE_MIN,DEF_XSCALE_MAX,DEF_XSCALE_STEP);
}

void plotWidget::scrollBarGoMax()
{
    scBar->goMax();
}

void plotWidget::showEvent(QShowEvent *e)
{
    scrollBarGoMax();
    setLegendGeometry();
    setTopLeftLegendGeometry();
    _plotPicker->clearDisplay();
    QWidget::showEvent(e);
}


