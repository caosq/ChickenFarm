#ifndef _BAR_CHART_WIDGET_H_

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_textlabel.h>
#include <qwt_plot_legenditem.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_canvas.h>

#include <qwt_column_symbol.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTime>


class QwtPlotMultiBarChart;

class NewScaleDraw:public QwtScaleDraw
{
public:
    NewScaleDraw()
    {
    }

    virtual QwtText label(double v) const
    {
        QString str = QString::number(v);
        QFont font("Bitstream Charter", 15);
        font.setBold(true);

        QwtText text;
        text.setText(QString::null);
        text.setFont(font);
        return text;
    }

private:
    QTime baseTime;
};

class BarChartLegend: public QwtPlotLegendItem
{
public:
    BarChartLegend();
};

class BarChart: public QwtPlot
{
public:
    BarChart( QWidget * = NULL );

    void setUnitName(QString str = "");
    void setYScale(double min,double max,double step);
    bool setBarVal(int month,int barNum,double val);
    bool setBarVal(int month,QVector<double> val);
    void setBarTitle(QStringList str);
    void setBarSamples(QVector< QVector<double> > array);

private:
    void InitMember();
    void InitText();
    void populate();

    void setOrientation(int orientation);
    void setMode(int mode);
    void exportChart();

    QVector< QVector<double> > series;

    QVector<QwtColumnSymbol *> symbol;

    QwtPlotMultiBarChart *d_barChartItem;
    QwtPlotGrid *grid;
    BarChartLegend  *my_legend;

    QwtPlotCanvas   *canvas;
    QwtText *unitText;
    QwtPlotTextLabel    *unitLabel;
};

class chartLabel: public QLabel
{
public:
    chartLabel(QWidget * parent = NULL,QString str = "",QColor color = Qt::black);

    void setTextSize(int size);
    void setTextColor(QColor color);

};

class BarChartWidget: public QWidget
{
public:

    BarChartWidget(QWidget * parent = NULL);

    void setChartTitle(QString str = "",int size = 20,QColor color = QColor("#dfd8c8"));
    void setChartUnit(QString unit);
    void setChartDate(int month,int whichBar,double val);
    void setChartDate(int month, QVector<double> val);
    void setChartYScale(double min,double max,double step);
    void setBarSamples(QVector< QVector<double> > array);
    void setSamplesData(QVector< QVector<double> > array);

    void setChartBarName(QStringList str);
    void setLabelTextColor(int row, QColor color);
    void setLabelTextSize(int row, int size);
    void setLabelBoxSpacing(int left,int top,int right,int bottom);
    void hideBottomData(bool state = false);

private:
    void InitLabel();

private:
    BarChart    *myChart;

    QVector<chartLabel *> pGroup;
    QVector<chartLabel *> cGroup;

    QVBoxLayout *vBox;
    QHBoxLayout *hBox1;
    QHBoxLayout *hBox2;
    QGridLayout *gridBox;
};

#endif
