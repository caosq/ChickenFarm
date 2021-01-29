#include "gqwtplotzoomer.h"

#define DEF_STACK_DEPTH 3

gQwtPlotZoomer::gQwtPlotZoomer(QWidget * parent, bool doReplot):
    QwtPlotZoomer(parent,doReplot)
{
    setTrackerMode(QwtPicker::AlwaysOff);
    setRubberBandPen( QColor( Qt::black ) );
    setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    setMaxStackDepth(DEF_STACK_DEPTH);

}

gQwtPlotZoomer::gQwtPlotZoomer(int xAxis, int yAxis,
                               QWidget *parent, bool doReplot):
    QwtPlotZoomer(xAxis,yAxis,parent,doReplot)
{
    setTrackerMode(QwtPicker::AlwaysOff);
    setRubberBandPen( QColor( Qt::black ) );
    setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    setMaxStackDepth(DEF_STACK_DEPTH);
}
