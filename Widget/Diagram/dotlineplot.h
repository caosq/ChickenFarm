#ifndef DOTLINEPLOT_H
#define DOTLINEPLOT_H

#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_textlabel.h>
#include <qwt_plot_legenditem.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_symbol.h>
#include <qwt_scale_div.h>

class YScaleDraw:public QwtScaleDraw
{
public:
    YScaleDraw(const double base)
        : baseVal(base)
    {
    }

    virtual QwtText label(double v) const
    {
        int temp = v / 10000;

        QFont font("Bitstream Charter", 15);
        font.setBold(true);

        QString str = QString::number(temp);

        if( temp != 0 )
            str += "万";

        QwtText text = str;
        text.setFont(font);
        return text;
    }

private:
    double baseVal;
};


class DotLineLegend: public QwtPlotLegendItem
{
public:
    DotLineLegend();
};

class DotLinePlot : public QwtPlot
{
    Q_OBJECT
public:
    explicit DotLinePlot(QWidget *parent = 0);

    void setYScale(double min = 0,double max = 1000,double step = 100);
    void setUnit(QString str);
    void setPlotTitle(QString str = "",int size = 20,QColor color = QColor("#dfd8c8"));
    void setCurveTitle(int index,QString title);
    void setCurveSamples(int index,QPolygonF points);


    int AddCurve();
    bool removeCurve(int index);

private:
    void InitMember();
    bool testYScaleAndStep(double min = 0,double max = 1000,double step = 100);

private:
    QVector<QwtPlotCurve    *> curve;

    QVector<QColor> colorList;

    QwtPlotCanvas *canvas;
    QwtPlotGrid *grid;
    DotLineLegend  *my_legend;

    QwtText *unitText;
    QwtPlotTextLabel    *unitLabel;

    YScaleDraw  *userScaleDraw;
    QwtScaleDraw    *normalScaleDraw;

signals:
    
public slots:

    
};

#endif // DOTLINEPLOT_H
