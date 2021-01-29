#include "gplotpicker.h"

#include "g_qwtplot.h"
#define DEF_COLOR "#73baf2"

#include <QDebug>

struct compareX
{
    inline bool operator()( const double x, const QPointF &pos ) const
    {
        return ( x < pos.x() );
    }
};

gPlotPicker::gPlotPicker(QwtPlot *pPlot) :
    QwtPlotPicker(pPlot ? pPlot->canvas() : 0)
{
    //设置采集模式
    setTrackerMode( QwtPlotPicker::ActiveOnly );
    setRubberBand( VLineRubberBand );

    setStateMachine( new QwtPickerTrackerMachine() );

    setResizeMode( KeepSize );

    setRubberBandPen( QPen(QColor(DEF_COLOR)) );

    _plot = static_cast<g_qwtPlot *>(pPlot);
    _e = 0;

}

g_qwtPlot *gPlotPicker::fatherPlot() const
{
    return _plot;
}

void gPlotPicker::clearDisplay()
{
    reset();
}

QwtText gPlotPicker::trackerTextF(const QPointF &pos) const
{
    return setScrollVal(pos);
}


QwtText gPlotPicker::setScrollVal(const QPointF &pos) const
{
    QwtPlotItemList curves =
        plot()->itemList( QwtPlotItem::Rtti_PlotCurve );

    bool lineNull = false;
    for ( int i = 0; i < curves.size(); i++ )
    {
        lineNull = curveInfoAt(static_cast<QwtPlotCurve *>( curves[i] ), pos );
    }

    QwtText text("");
    text.setColor(Qt::red);
    QFont font;
    font.setPixelSize(16);
    text.setFont(font);

    if( !lineNull ){
        QTime time;
        text.setText(time.addSecs(pos.x()).toString("hh:mm:ss"));
    }else{

    }

    fatherPlot()->updateUserLegend();

    return text;
}

bool gPlotPicker::curveInfoAt(
    QwtPlotCurve *curve, const QPointF &pos ) const
{
    QString temp = "";

    const QLineF line = curveLineAt( curve, pos.x() );

    if ( line.isNull() ){
        temp = QString::number(0,'.',fatherPlot()->curveDot());
        temp += fatherPlot()->curveUnit();
        curve->setTitle(temp);
        return true;
    }

    QPointF temp_point;
    temp_point = line.pointAt(( pos.x() - line.p1().x() ) / line.dx() );

    temp = QString::number(temp_point.y() / qPow(10,fatherPlot()->curveDot()),
                           '.',fatherPlot()->curveDot());
    temp += fatherPlot()->curveUnit();
    curve->setTitle(temp);

    return false;
}

QLineF gPlotPicker::curveLineAt(
    const QwtPlotCurve *curve, double x ) const
{
    QLineF line;
    if ( curve->dataSize() >= 2 ){
        int index = qwtUpperSampleIndex<QPointF>(
                    *curve->data(), x, compareX() );

        if ( index == -1 &&
             x == curve->sample( curve->dataSize() - 1 ).x() ){
            index = curve->dataSize() - 1;
        }

        if ( index > 0 ){
            line.setP1( curve->sample( index - 1 ) );
            line.setP2( curve->sample( index ) );
        }
    }

    return line;
}

void gPlotPicker::widgetLeaveEvent(QEvent *e)
{
    // QwtPicker::widgetLeaveEvent(e);
    _e = e;
}

void gPlotPicker::widgetEnterEvent(QEvent *e)
{
    if( _e )
        QwtPicker::widgetLeaveEvent(_e);
    QwtPicker::widgetEnterEvent(e);
}

