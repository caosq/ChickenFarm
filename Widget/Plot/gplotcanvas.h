#ifndef GPLOTCANVAS_H
#define GPLOTCANVAS_H

#include <qwt_plot_canvas.h>
#include <QPainter>

class gPlotCanvas : public QwtPlotCanvas
{
    Q_OBJECT
public:
    //画布控件
    explicit gPlotCanvas(QwtPlot  *parent = 0);
    
    //设置画布背景颜色
    void setBackGroundColor(QColor color);
    //清除画布风格，恢复初始默认风格
    void clearStyleSheet();

signals:
    
public slots:
    
};

#endif // GPLOTCANVAS_H
