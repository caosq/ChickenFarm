#include "barchartwidget.h"
#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_multi_barchart.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QDebug>

#define MONTH_NUM 12
#define BAR_NUM 2
#define MIN_WIDTH 880
#define MIN_HEIGHT 380
#define LABEL_MAX_HEIGHT 20
#define LABEL_MIN_WIDTH 65

QColor colorGroup[BAR_NUM] = {Qt::yellow,QColor("#85bee8")};

BarChart::BarChart( QWidget *parent ):
    QwtPlot( parent )
{

    InitMember();

    populate();

    setOrientation(0);

    InitText();

    setAutoReplot( true );

    setMinimumSize(400,200);
    setMaximumSize(880,400);
}

void BarChart::InitMember()
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

    d_barChartItem = new QwtPlotMultiBarChart( "Bar Chart " );
    d_barChartItem->setLayoutPolicy( QwtPlotMultiBarChart::AutoAdjustSamples );
    d_barChartItem->setSpacing( 20 );
    d_barChartItem->setMargin( 3 );
    d_barChartItem->attach( this );

    grid = new QwtPlotGrid;
    grid->enableX(false);
    grid->enableY(true);
    grid->setMajorPen(QColor("#acd5fd"),1,Qt::DashLine);
    grid->attach(this);

    my_legend = new BarChartLegend();
    my_legend->setMaxColumns(1);
    my_legend->setAlignment(Qt::AlignRight);
    my_legend->setBackgroundMode( QwtPlotLegendItem::ItemBackground );

    my_legend->setBorderRadius(4);
    my_legend->setMargin(0);
    my_legend->setSpacing(4);
    my_legend->setItemMargin(2);
    my_legend->attach(this);

    QFont font("Bitstream Charter", 15);
    font.setBold(true);

    unitText = new QwtText();
    unitText->setFont(font);
    unitText->setColor(Qt::red);
    unitText->setRenderFlags( Qt::AlignLeft | Qt::AlignTop );

    unitLabel = new QwtPlotTextLabel();
    unitLabel->setText(*unitText);
    unitLabel->attach(this);

}

void BarChart::InitText()
{
    setTitle( tr("") );

    unitText->setText("");
    unitLabel->setText(*unitText);

    QList<QwtText> titles;

    for ( int i = 0; i < BAR_NUM; i++ )
    {
        QString title("Bar %1");
        titles += title.arg( i );
    }

    d_barChartItem->setBarTitles( titles );
    d_barChartItem->setLegendIconSize( QSize(8,8) );
}

void BarChart::populate()
{
    const int numSamples = MONTH_NUM + 1;

    symbol.resize(BAR_NUM);
    for ( int i = 0; i < BAR_NUM; i++ )
    {
        symbol[i] = new QwtColumnSymbol( QwtColumnSymbol::Box );
        symbol[i]->setFrameStyle( QwtColumnSymbol::NoFrame );
        symbol[i]->setPalette( QPalette( colorGroup[i] ) );

        d_barChartItem->setSymbol( i, symbol[i] );
    }
    for ( int i = 0; i < numSamples; i++ )
    {
        QVector<double> values;
        for ( int j = 0; j < BAR_NUM; j++ )
            values += 0;

        series += values;
    }

    d_barChartItem->setSamples( series );
}

void BarChart::setMode( int mode )
{
    if ( mode == 0 )
    {
        d_barChartItem->setStyle( QwtPlotMultiBarChart::Grouped );
    }
    else
    {
        d_barChartItem->setStyle( QwtPlotMultiBarChart::Stacked );
    }
}

void BarChart::setOrientation( int orientation )
{
    QwtPlot::Axis axis1, axis2;

    if ( orientation == 0 )
    {
        axis1 = QwtPlot::xBottom;
        axis2 = QwtPlot::yLeft;

        d_barChartItem->setOrientation( Qt::Vertical );
    }
    else
    {
        axis1 = QwtPlot::yLeft;
        axis2 = QwtPlot::xBottom;

        d_barChartItem->setOrientation( Qt::Horizontal );
    }

    setAxisScale( axis1, 1, 12, 1 );
    setAxisScale( axis2, 0, 60000, 15000 );

    QwtScaleDraw *scaleDraw1 = axisScaleDraw( axis1 );
    scaleDraw1->enableComponent( QwtScaleDraw::Backbone, false );
    scaleDraw1->enableComponent( QwtScaleDraw::Ticks, false );

    QwtScaleDraw *scaleDraw2 = axisScaleDraw( axis2 );
    scaleDraw2->enableComponent( QwtScaleDraw::Backbone, false );
    scaleDraw2->enableComponent( QwtScaleDraw::Ticks, false );

    plotLayout()->setAlignCanvasToScale( axis1, true );
    plotLayout()->setAlignCanvasToScale( axis2, false );

    plotLayout()->setCanvasMargin( 0 );
    updateCanvasMargins();

    replot();
}

void BarChart::exportChart()
{
    QwtPlotRenderer renderer;
    renderer.exportTo( this, "barchart.pdf" );
}

void BarChart::setUnitName(QString str)
{
    unitText->setText(str);
    unitLabel->setText(*unitText);
}

void BarChart::setYScale(double min, double max, double step)
{
    setAxisScale(QwtPlot::yLeft,min,max,step);
}

bool BarChart::setBarVal(int month, int barNum, double val)
{
    if( month < 0 || month > MONTH_NUM )
        return false;
    if( barNum < 0 || barNum >= BAR_NUM )
        return false;

    series[month][barNum] = val;
    d_barChartItem->setSamples(series);
    return true;
}

bool BarChart::setBarVal(int month, QVector<double> val)
{
    if( month < 1 || month > MONTH_NUM )
        return false;

    for(int i = 0; i < BAR_NUM; i++)
    {
        if( i >= val.size() )
            break;
        series[month][i] = val.value(i);
    }

    d_barChartItem->setSamples(series);
    return true;
}

void BarChart::setBarTitle(QStringList str)
{
    QList<QwtText> titles;
    for( int i = 0; i < BAR_NUM; i++ )
    {
        if( i == str.size() )
            break;
        titles += str[i];
    }

    if( 8 == d_barChartItem->legendIconSize().width() )
    {
        d_barChartItem->setLegendIconSize(QSize(10,10));
        d_barChartItem->setBarTitles(titles);
    }

    d_barChartItem->setLegendIconSize(QSize(8,8));
    d_barChartItem->setBarTitles(titles);
}

void BarChart::setBarSamples(QVector< QVector<double> > array)
{
    d_barChartItem->setSamples(array);
}

BarChartLegend::BarChartLegend():
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

BarChartWidget::BarChartWidget(QWidget *parent):
    QWidget(parent)
{
    myChart = new BarChart(this);

    InitLabel();

    vBox = new QVBoxLayout;
    vBox->addWidget(myChart);
    vBox->addLayout(hBox1);
    vBox->addLayout(hBox2);
    vBox->setSpacing(2);

    this->setLayout(vBox);

    resize(880,360);
}

void BarChartWidget::setChartTitle(QString str,int size,QColor color)
{
    QwtText temp;
    QFont font;
    font.setPointSize(size);
    font.setBold(false);

    temp.setFont(font);
    temp.setColor(color);
    temp.setText(str);

    myChart->setTitle(temp);
}

void BarChartWidget::setChartUnit(QString unit)
{
    myChart->setUnitName(unit);
}

//pGroup、和pGroup有效范围是0-11
//这里月份是1-12
//
void BarChartWidget::setChartDate(int month, int whichBar, double val)
{
    if( month < 1 || month > MONTH_NUM )
        return;
    //myChart->setBarVal(month,whichBar,val);
    if( 0 == whichBar )
        pGroup[month - 1]->setText(QString::number(val));
    else if( 1 == whichBar )
        cGroup[month - 1]->setText(QString::number(val));
}

//pGroup、和pGroup有效范围是0-11
//这里月份是1-12
void BarChartWidget::setChartDate(int month, QVector<double> val)
{
    if( month < 1 || month > MONTH_NUM )
        return;
    if( val.size() < BAR_NUM )
        return;
    //myChart->setBarVal(month,val);
    pGroup[month - 1]->setText(QString::number(val[0]));
    cGroup[month - 1]->setText(QString::number(val[1]));
}

void BarChartWidget::setChartYScale(double min, double max, double step)
{
    myChart->setYScale(min,max,step);
    //解决不在当前柱状图时y轴显示异常问题
    myChart->updateLayout();
}

void BarChartWidget::setBarSamples(QVector< QVector<double> > array)
{
    myChart->setBarSamples(array);
}

void BarChartWidget::setSamplesData(QVector< QVector<double> > array)
{
    if( array.size() == 0 )
        return;
    if( array.size() > MONTH_NUM )
    {
        return;
    }
    myChart->setBarSamples(array);
    //myChart->setBarVal(month,val);
    for( int i = 0; i < array.size(); i++ )
    {
        if( array.at(i).size() < BAR_NUM )
            continue;
        for( int j = 0; j < BAR_NUM; j++ )
        {
            pGroup[i]->setText(QString::number(array.at(i).at(j++)));
            cGroup[i]->setText(QString::number(array.at(i).at(j)));
        }
    }
}

void BarChartWidget::setChartBarName(QStringList str)
{
    myChart->setBarTitle(str);
}

void BarChartWidget::setLabelBoxSpacing(int left,int top,int right,int bottom)
{

    hBox1->setContentsMargins(left,top,right,bottom);
    hBox2->setContentsMargins(left,top,right,bottom);
}

void BarChartWidget::hideBottomData(bool state)
{
    if( state )
    {
        vBox->removeItem(hBox1);
        vBox->removeItem(hBox2);
        for(int i = 0; i < MONTH_NUM; i++ )
        {
            cGroup[i]->hide();
            pGroup[i]->hide();
        }
    }
    else
    {
        vBox->addLayout(hBox1);
        vBox->addLayout(hBox2);
        for(int i = 0; i < MONTH_NUM; i++ )
        {
            cGroup[i]->show();
            pGroup[i]->show();
        }
    }
}

void BarChartWidget::setLabelTextSize(int row,int size)
{
    if( 0 == row )
    {
        for(int i = 0; i < MONTH_NUM; i++ )
        {
            pGroup[i]->setTextSize(size);
        }
    }
    else if( 1 == row )
    {
        for(int i = 0; i < MONTH_NUM; i++ )
        {
            cGroup[i]->setTextSize(size);
        }
    }
}

void BarChartWidget::setLabelTextColor(int row,QColor color)
{
    if( 0 == row )
    {
        for(int i = 0; i < MONTH_NUM; i++ )
        {
            pGroup[i]->setTextColor(color);
        }
    }
    else if( 1 == row )
    {
        for(int i = 0; i < MONTH_NUM; i++ )
        {
            cGroup[i]->setTextColor(color);
        }
    }
}

void BarChartWidget::InitLabel()
{
    hBox1 = new QHBoxLayout;
    hBox2 = new QHBoxLayout;

    hBox1->setContentsMargins(60,0,0,0);
    hBox2->setContentsMargins(60,0,0,0);

    pGroup.resize(MONTH_NUM);
    cGroup.resize(MONTH_NUM);

    for(int i = 0; i < MONTH_NUM; i++)
    {
        pGroup[i] = new chartLabel(this,"0",colorGroup[0]);
        cGroup[i] = new chartLabel(this,"0",colorGroup[1]);

        hBox1->addWidget(pGroup[i]);
        hBox2->addWidget(cGroup[i]);
    }
}


chartLabel::chartLabel(QWidget *parent, QString str,QColor color) :
    QLabel(parent)
{
    setAlignment(Qt::AlignCenter);

    setMinimumWidth(LABEL_MIN_WIDTH - 5);
    setMaximumWidth(LABEL_MIN_WIDTH);

    setTextSize(15);
    setTextColor(color);
    setText(str);
}

void chartLabel::setTextSize(int size)
{
    QFont font("Bitstream Charter", size);
    setFont(font);
}

void chartLabel::setTextColor(QColor color)
{
    QPalette palette;
    palette.setBrush(QPalette::WindowText,QBrush(color));
    setPalette(palette);
}


