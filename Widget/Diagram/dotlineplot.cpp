#include "dotlineplot.h"
#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <QDebug>

#define USUAL_VAL   1000000

DotLinePlot::DotLinePlot(QWidget *parent) :
    QwtPlot(parent)
{
    userScaleDraw = NULL;
    normalScaleDraw = NULL;

    InitMember();
    setAutoReplot( true );

    resize(880,280);

}

void DotLinePlot::setYScale(double min, double max, double step)
{
    if( max >= USUAL_VAL )
    {
        if( testYScaleAndStep(min,max,step) ){
            userScaleDraw = new YScaleDraw(0);
            setAxisScaleDraw(QwtPlot::yLeft,userScaleDraw);
        }
    }
    else
    {
        if( testYScaleAndStep(min,max,step) ){
            normalScaleDraw = new QwtScaleDraw;
            setAxisScaleDraw(QwtPlot::yLeft,normalScaleDraw);
        }
    }

    setAxisScale(QwtPlot::yLeft,min,max,step);

    QwtScaleDraw *scaleDraw = axisScaleDraw( QwtPlot::yLeft );
    scaleDraw->enableComponent( QwtScaleDraw::Backbone, false );
    scaleDraw->enableComponent( QwtScaleDraw::Ticks, false );

}

void DotLinePlot::setUnit(QString str)
{
    unitText->setText(str);
    unitLabel->setText(*unitText);
}

void DotLinePlot::setPlotTitle(QString str,int size,QColor color)
{
    QwtText temp;
    QFont font;
    font.setPointSize(size);
    font.setBold(false);

    temp.setFont(font);
    temp.setColor(color);
    temp.setText(str);
    setTitle(temp);
}

void DotLinePlot::InitMember()
{
    setAutoFillBackground( true );

    setStyleSheet("font-size:15px;color:white;");

    setPalette( Qt::transparent );


    canvas = new QwtPlotCanvas();

    QPalette pal( QColor(0,87,174) );
    pal.setColor(QPalette::Foreground,QColor(151,151,151));

    canvas->setPalette(pal);
    canvas->setBorderRadius(5);
    canvas->setFrameStyle(QFrame::Box | QFrame::Plain );
    setCanvas( canvas );

    grid = new QwtPlotGrid;
    grid->enableX(false);
    grid->enableY(true);
    grid->setMajorPen(QColor("#acd5fd"),1,Qt::DashLine);
    grid->attach(this);

    my_legend = new DotLineLegend();
    my_legend->setMaxColumns(1);
    my_legend->setAlignment(Qt::AlignRight);
    my_legend->setBackgroundMode( QwtPlotLegendItem::ItemBackground );

    my_legend->setBorderRadius(4);
    my_legend->setMargin(0);
    my_legend->setSpacing(4);
    my_legend->setItemMargin(2);
    my_legend->attach(this);


    QFont font;
    font.setPointSize(15);
    font.setBold(false);

    unitText = new QwtText();
    unitText->setFont(font);
    unitText->setColor(Qt::red);
    unitText->setRenderFlags( Qt::AlignLeft | Qt::AlignTop );

    unitLabel = new QwtPlotTextLabel();
    unitLabel->setText(*unitText);
    unitLabel->attach(this);

    setAxisScale(QwtPlot::xBottom,1,12,1.0);
    setAxisScale(QwtPlot::yLeft,0,400000,80000);

    QwtScaleDraw *scaleDraw1 = axisScaleDraw( QwtPlot::xBottom );
    scaleDraw1->enableComponent( QwtScaleDraw::Backbone, false );
    scaleDraw1->enableComponent( QwtScaleDraw::Ticks, false );

    QwtScaleDraw *scaleDraw2 = axisScaleDraw( QwtPlot::yLeft );
    scaleDraw2->enableComponent( QwtScaleDraw::Backbone, false );
    scaleDraw2->enableComponent( QwtScaleDraw::Ticks, false );

    plotLayout()->setAlignCanvasToScale( QwtPlot::yLeft, true );
    plotLayout()->setAlignCanvasToScale( QwtPlot::xBottom, true );

    plotLayout()->setCanvasMargin( 0 );
    updateCanvasMargins();

    const char *colors[] =
    {
        "Yellow",
        "#85bee8",
        "Fuchsia",
        "PaleGreen",
        "Cornsilk",
        "red",
        "Peru",
        "Maroon",
        "PaleTurquoise",
        "black"
    };

    const int numColors = sizeof( colors ) / sizeof( colors[0] );

    colorList.resize(numColors);
    for( int i = 0; i < numColors; i++)
    {
        colorList[i] = QColor(colors[ i % numColors ]);
    }

}

bool DotLinePlot::testYScaleAndStep(double min, double max, double step)
{
    if( min != axisInterval(QwtPlot::yLeft).minValue() ||
            max != axisInterval(QwtPlot::yLeft).maxValue() ||
                step != axisStepSize(QwtPlot::yLeft) ) {
        return true;
    }
    else{
        return false;
    }

}

void DotLinePlot::setCurveTitle(int index, QString title)
{
    if( index >= curve.size() )
        return;
    curve[index]->setTitle(title);
}

void DotLinePlot::setCurveSamples(int index, QPolygonF points)
{
    if( index >= curve.size() )
        return;
    curve[index]->setSamples(points);
}


int DotLinePlot::AddCurve()
{
    curve += new QwtPlotCurve();

    int index = curve.size() - 1;
    QColor color = colorList[index % colorList.size()];
    curve[index]->setPen(color,2);
    curve[index]->setRenderHint(QwtPlotItem::RenderAntialiased,true);

    QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Rect,QBrush(color),
                                      QPen(color,2),QSize(5,5));
    curve[index]->setSymbol(symbol);
    curve[index]->attach(this);

    return index;
}

bool DotLinePlot::removeCurve(int index)
{
    if( index >= curve.size() )
        return false;
    curve[index]->hide();
    curve[index]->detach();
    return true;
}

DotLineLegend::DotLineLegend():
    QwtPlotLegendItem()
{
    setRenderHint(QwtPlotItem::RenderAntialiased);
    QColor  color(Qt::white);

    setTextPen( color );
    setBorderPen( color );
    QColor c( Qt::gray );
    c.setAlpha( 200 );

    setBackgroundBrush( c );
}
