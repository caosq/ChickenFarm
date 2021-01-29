#include "axiscurve.h"

#define MAX_CURVE 10
#define POINT_SIZE 4
#define DEF_DOT 2
#define DEF_DOMAIN 20000

#define MAX_COUNT 30
#define DEF_ST_SEC 10
#include <QDebug>

axisCurve::axisCurve(QWidget *parent) :
    QwtPlot(parent)
{
    plotLayout()->setAlignCanvasToScales( true );

    canvas()->setStyleSheet("background-color:rgba(0,0,0,0);");


    _scaleXDraw = new g_qwtScaleDraw;
    _scaleYDraw = new g_qwtScaleDraw;

    setAxisScaleDraw(QwtPlot::xBottom,_scaleXDraw);
    setAxisScaleDraw(QwtPlot::yLeft,_scaleYDraw);


    _grid = new QwtPlotGrid;
    _grid->enableX( true );//设置网格线
    _grid->enableY( true );
    _grid->setMajorPen( Qt::darkGray, 1, Qt::DashLine );
    _grid->attach( this );

    setDomain(DEF_DOMAIN,DEF_DOT);

    _maxPointNum = MAX_COUNT;
    _points.resize(MAX_CURVE);

    _timer = new QTimer;
    _timer->setInterval(DEF_ST_SEC * 1000);

    setAutoReplot(true);

    connect(_timer,SIGNAL(timeout()),this,SLOT(timeOutSlot()));
}

void axisCurve::setMarkNum(int count)
{
    _maxPointNum = count;
    if( _maxPointNum < 0 ){
        _maxPointNum = 0;
    }else if( _maxPointNum > MAX_COUNT){
        _maxPointNum = MAX_COUNT;
    }

    clearPoints();
}


void axisCurve::timeOutSlot()
{
    emit prepareData();

    updateSamples();
}

void axisCurve::setSamples(unsigned int index,
                           const QVector<QPointF> points)
{
    if( _curve.contains(index) ){
        axisCurveData data = _curve.value(index);
        data.curve->setSamples(points);
    }
}

void axisCurve::clearPoints()
{
    QList<int>  keys = _curve.keys();
    QVector<QPointF> points;
    axisCurveData data;

    for(int i = 0; i < keys.size(); i++ ){
        data = _curve.value(keys.at(i));
        data.curve->setSamples(points);
        _points[keys.at(i)].clear();
    }
}

void axisCurve::updateSamples()
{
    QList<int>  keys = _curve.keys();

    axisCurveData data;
    int index = 0;

    for(int i = 0; i < keys.size(); i++ ){
        index  = keys.at(i);
        data = _curve.value(index);

        if( _points.size() > index ){
            if( _points[index].size() >= _maxPointNum ){
                _points[index].clear();
            }
            _points[index]<<QPointF(*(data.valx),*(data.valy));
            data.curve->setSamples(_points[index]);

        }
    }
}

void axisCurve::startSampling(bool start)
{
    if( start ){
        _timer->start();
    }else{
        _timer->stop();
    }
}

void axisCurve::setSamplingTime(int sec)
{
     _timer->setInterval(sec * 1000);
}

bool axisCurve::addCurve(int index, QColor color, double *val1, double *val2)
{
    if( _curve.size() >= MAX_CURVE || index >= MAX_CURVE)
        return false;

    if( _curve.contains(index) ){
        return false;
    }

    QwtPlotCurve *tempCurve = new QwtPlotCurve(QString::number(index));
    tempCurve->setPen(color,4);
    tempCurve->setStyle(QwtPlotCurve::Dots);
    tempCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);
    tempCurve->attach(this);
    axisCurveData data = {val1,val2,tempCurve};
    _curve.insert(index,data);

    return true;
}

void axisCurve::removeCurve(int index)
{
    if( _curve.contains(index) ){
        axisCurveData data = _curve.value(index);

        data.curve->detach();
        delete data.curve;
        _curve.remove(index);
    }
}

void axisCurve::setPointSize(int index, int size)
{
    if( _curve.contains(index) ){
        axisCurveData data = _curve.value(index);

         data.curve->setPen( data.curve->pen().color(),size);
    }
}

void axisCurve::setDomain(double val, int dot)
{
    _scaleXDraw->setScaleRaised(dot);
    _scaleYDraw->setScaleRaised(dot);

    setAxisScale(QwtPlot::yLeft,-val,val,val / 4);
    setAxisScale(QwtPlot::xBottom,-val,val,val / 4);

}

