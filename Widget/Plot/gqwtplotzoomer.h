#ifndef GQWTPLOTZOOMER_H
#define GQWTPLOTZOOMER_H

#include <qwt_plot_zoomer.h>

class gQwtPlotZoomer : public QwtPlotZoomer
{
    Q_OBJECT
public:
    //曲线放大类
    explicit gQwtPlotZoomer( QWidget *parent = 0, bool doReplot = true );
    explicit gQwtPlotZoomer( int xAxis, int yAxis,
                            QWidget *parent, bool doReplot = true );

signals:
    
public slots:

    
};

#endif // GQWTPLOTZOOMER_H
