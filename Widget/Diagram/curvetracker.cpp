#include "curvetracker.h"
#include <qwt_picker_machine.h>
#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>

struct compareX
{
    inline bool operator()( const double x, const QPointF &pos ) const
    {
        return ( x < pos.x() );
    }
};

CurveTracker::CurveTracker( QWidget *canvas):
    QwtPlotPicker( canvas )
{
    curve_dot = 0;
    curve_unit = "";
    TimeMode = false;

    setTrackerMode( QwtPlotPicker::ActiveOnly );
    setRubberBand( VLineRubberBand );

    setStateMachine( new QwtPickerDragPointMachine() );
    setResizeMode( Stretch );

}

void CurveTracker::setCurveAttribute(unsigned int dot, QString unit)
{
    curve_dot = dot;
    curve_unit = unit;
}

void CurveTracker::setCurveDate(QDateTime qdate,bool soigne)
{
    current_date = qdate;
    baseTime = current_date.time();
    yearMonDay = current_date.date().toString("yy/MM/dd  ");
    TimeMode = true;

    if( soigne )
    {
        baseTime.setHMS(0,0,0);
    }
}

void CurveTracker::setXText(QwtText *addr)
{
    x_val = addr;
}

void CurveTracker::setXTextLabel(QwtPlotTextLabel *addr)
{
    x_label = addr;
}

void CurveTracker::setTimeStep(unsigned int step)
{
    time_step = step;
}

QwtText CurveTracker::trackerTextF( const QPointF &pos ) const
{
    QwtText trackerText;
    x_date temp_date;
    QString timeStr;

    trackerText.setText( QString::null );

    QwtPlotItemList curves =
        plot()->itemList( QwtPlotItem::Rtti_PlotCurve );

    for ( int i = 0; i < curves.size(); i++ )
    {
        temp_date = curveInfoAt(static_cast<QwtPlotCurve *>( curves[i] ), pos );
    }

    if( temp_date.lineIsNull )
    {
        x_val->setText(QString::null);
        x_label->setText(*x_val);

        return trackerText;
    }

    if( TimeMode ){
        timeStr = yearMonDay;
        timeStr += baseTime.addSecs(temp_date.date).toString("hh:mm:ss");
        x_val->setText(timeStr);
    } else{
        x_val->setText(QString::number(temp_date.date));
    }
    x_label->setText(*x_val);
    return trackerText;

}

CurveTracker::x_date CurveTracker::curveInfoAt(
    QwtPlotCurve *curve, const QPointF &pos ) const
{
    QString temp = "";
    x_date date;
    const QLineF line = curveLineAt( curve, pos.x() );
    date.lineIsNull = line.isNull();

    if ( date.lineIsNull ){
        temp = QString::number(0,'.',curve_dot);
        temp += curve_unit;
        curve->setTitle(temp);
        return date;
    }

    QPointF temp_point;
    temp_point = line.pointAt(( pos.x() - line.p1().x() ) / line.dx() );

    date.date = temp_point.x();

    temp = QString::number(temp_point.y(),'.',curve_dot);
    temp += curve_unit;
    curve->setTitle(temp);

    return date;
}

QLineF CurveTracker::curveLineAt(
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


