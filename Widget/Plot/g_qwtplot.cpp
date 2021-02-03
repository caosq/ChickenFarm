#include "g_qwtplot.h"

#include "gplotcanvas.h"
#include "gplotgrid.h"
#include "g_qwtscaledraw.h"
#include "glegenditem.h"
#include "gqwtlegenditem.h"

#define SCALE_ORIGIN    0
#define SCALE_MAX       10
#define SCALE_STEP      1

#define WIDGET_WIDTH    920
#define WIDGET_HIGHT    650

#define MAX_CURVE   15

g_qwtPlot::g_qwtPlot(QWidget *parent) :
    QwtPlot(parent)
{
    _plotStyle = style1;
    _dot = 0;
    _unit = "";

    initPlot();

    setPlotStyle(_plotStyle);

    plotLayout()->setAlignCanvasToScales( true );


    setAxisScale(QwtPlot::yLeft,SCALE_ORIGIN,SCALE_MAX,SCALE_STEP);
    setAxisScale(QwtPlot::xBottom,SCALE_ORIGIN,SCALE_MAX,SCALE_STEP);

    resize(WIDGET_WIDTH,WIDGET_HIGHT);

    setAutoReplot( true );
}

bool g_qwtPlot::addCurve(int index, QColor color)
{
    if( _curve.size() >= MAX_CURVE )
        return false;

    if( _curve.contains(index) ){
        return false;
    }

    QwtPlotCurve *tempCurve = new QwtPlotCurve(QString::number(index));
    tempCurve->setPen(color,2);
    tempCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);
    tempCurve->attach(this);

    _curve.insert(index,tempCurve);

    setLegendItemVal(index,0);

    return true;
}

void g_qwtPlot::setPlotStyle(g_qwtPlot::qwtPlotStyle plotStyle)
{
    _plotStyle = plotStyle;

    switch( plotStyle )
    {
    case style0:
    {
        setStyle0();
    }
        break;
    case style1:
    {
        setStyle1();
    }
        break;
    default:
        break;
    }

    update();
}

void g_qwtPlot::setLegendItemVal(int index, int val)
{
    if( _curve.contains(index) ){
        QString temp = "";

        temp = QString::number(val,'.',_dot);
        temp += _unit;

        _curve.value(index)->setTitle(temp);
    }
}

void g_qwtPlot::setCurveAttribute(int dot, QString unit)
{
    _dot = dot;
    _unit = unit;

    setScaledValueRaised( _dot );

    QList<int> list = _curve.keys();

    for(int i = 0; i < list.size(); i++ ){
        setLegendItemVal(list.at(i),0);
    }

}

void g_qwtPlot::setScaledValueRaised(int dot)
{
    _YgScaleDraw->setScaleRaised(dot);
    QwtInterval interVal = axisInterval(QwtPlot::yLeft);

    setAxisScale(QwtPlot::yLeft,0,
                 10,axisStepSize(QwtPlot::yLeft));
    setAxisScale(QwtPlot::yLeft,interVal.minValue(),
                 interVal.maxValue(),axisStepSize(QwtPlot::yLeft));
}

void g_qwtPlot::setSamples(unsigned int index,
                           const QVector<QPointF> points)
{
    if( _curve.contains(index) ){
        QwtPlotCurve *tempCurve = _curve.value(index);

        tempCurve->setSamples(points);
    }
}

int g_qwtPlot::maxCurveCapability()
{
    return MAX_CURVE;
}

void g_qwtPlot::connectLegendItem(int index, gQwtLegendItem *item)
{
    _item.insert(index,item);
    setLegendItemIconColor(index,item);
}

void g_qwtPlot::disconnectLegendItem(int index)
{
    _item.remove(index);
}

int g_qwtPlot::yScaleWidth()
{
    return _YgScaleDraw->widgetWidth();
}

void g_qwtPlot::clearLegendItemVal()
{
    QList<gQwtLegendItem *> list = _item.values();

    QString temp = "";

    temp = QString::number(0,'.',_dot);
    temp += _unit;

    for(int i = 0 ;i < list.size(); i++){
        list.at(i)->setText(temp);
    }
}

void g_qwtPlot::clearCurveVal()
{
    QList<QwtPlotCurve *> tempCurve = _curve.values();
    QVector<QPointF> points;
    for(int i = 0; i < tempCurve.size(); i++){
        tempCurve[i]->setSamples(points);
    }
}

bool g_qwtPlot::curveVisible(int index)
{
    if( _curve.value(index) == 0 ){
        return false;
    }else{
        return _curve.value(index)->isVisible();
    }
}

void g_qwtPlot::showCurve(int index, bool on)
{
    if( _curve.contains(index && _item.contains(index))){
        _curve.value(index)->setVisible(on);
        _item.value(index)->setVisible(on);
    }
    replot();
}

int g_qwtPlot::curveNumber()
{
    return _curve.size();
}

void g_qwtPlot::updateScale()
{
   QList<int> axis =  _scalPar.keys();
   QList<scaleParameter> sp = _scalPar.values();
   for(int i = 0; i < axis.size(); i++){
       QwtPlot::setAxisScale(axis.at(i),sp[i]._min,sp[i]._max,sp[i]._step);
   }
}

void g_qwtPlot::setAxisScale(int axisId, double min,
                             double max, double step)
{
    scaleParameter sp = {min,max,step};
    _scalPar.insert(axisId,sp);
    QwtPlot::setAxisScale(axisId,min,max,step);
}

void g_qwtPlot::setTempAxisScale(int axisId, double min, double max, double step)
{
    QwtPlot::setAxisScale(axisId,min,max,step);
}

void g_qwtPlot::setLegendItemIconColor(int index,
                                       gQwtLegendItem *item)
{
    QwtPlotCurve *tempCurve = _curve.value(index,0);
    if( tempCurve ){
        item->setIconColor(tempCurve->pen().color());
    }
}

void g_qwtPlot::updateUserLegend()
{
    QList<int> list = _item.keys();
    QList<gQwtLegendItem *> tempItem = _item.values();

    QString text = "";
    text = QString::number(0,'.',curveDot());
    text += curveUnit();

    for(int i = 0; i < list.size(); i++){
        if( _curve.value(list.at(i),0)){
            tempItem[i]->setText(_curve.value(list.at(i))->title().text());
        }else{
            tempItem[i]->setText(text);
        }
    }
}


void g_qwtPlot::setStyle0()
{
    _canvas->clearStyleSheet();
    _canvas->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    _canvas->setBorderRadius(5);
    setCanvas( _canvas );
    setCanvasBackground(QBrush(QColor(0,87,174)));

    _grid->setMajorPen( Qt::gray, 0, Qt::DotLine );
    _grid->attach( this );

    _XgScaleDraw->setScaleDrawStyle(g_qwtScaleDraw::style0);
    _YgScaleDraw->setScaleDrawStyle(g_qwtScaleDraw::style0);

}

void g_qwtPlot::setStyle1()
{
    _canvas->setFrameStyle(QFrame::Plain | QFrame::Panel);
    _canvas->setBorderRadius(0);
    _canvas->setBackGroundColor(QColor(0,0,0,255));

    setCanvas( _canvas );

    _grid->setMajorPen( QColor(49,183,226,80), 1, Qt::SolidLine );
    _grid->attach( this );

    _XgScaleDraw->setScaleDrawStyle(g_qwtScaleDraw::style1);
    _YgScaleDraw->setScaleDrawStyle(g_qwtScaleDraw::style1);

}

void g_qwtPlot::initMainBodyStyle()
{
    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(this->backgroundRole(),QBrush(Qt::transparent));
    setPalette(palette);

    //setStyleSheet("font-size:15px;color:white;");
}

void g_qwtPlot::initCSG()
{
    _canvas =new gPlotCanvas;

    _grid = new gPlotGrid;
    _grid->enableX( true );//设置网格线
    _grid->enableY( true );
    _grid->attach( this );

    _XgScaleDraw = new g_qwtScaleDraw;
    _YgScaleDraw = new g_qwtScaleDraw;

    setAxisScaleDraw(QwtPlot::xBottom,_XgScaleDraw);
    setAxisScaleDraw(QwtPlot::yLeft,_YgScaleDraw);

}


void g_qwtPlot::initPlot()
{
    initMainBodyStyle();

    initCSG();
}

