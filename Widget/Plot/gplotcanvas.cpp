#include "gplotcanvas.h"

#define BGC_CMD "background-color:rgba(%1,%2,%3,%4);"

//#define BGC_CMD "background-color:rgba(1,2,3,4);"

gPlotCanvas::gPlotCanvas(QwtPlot *parent) :
    QwtPlotCanvas(parent)
{
    clearStyleSheet();
}

void gPlotCanvas::setBackGroundColor(QColor color)
{
    QString temp = BGC_CMD;
    QString cmd = temp.arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    setStyleSheet(cmd.toLocal8Bit().constData());
}

void gPlotCanvas::clearStyleSheet()
{
    setStyleSheet("");
}


