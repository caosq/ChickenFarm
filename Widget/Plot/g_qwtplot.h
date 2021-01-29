#ifndef G_QWTPLOT_H
#define G_QWTPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>
#include <qwt_plot_textlabel.h>
#include <qwt_plot_legenditem.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_layout.h>

#include <qwt_legend.h>
#include <QDebug>
#include <QMap>

//#include <QScrollBar>
//#include <QPushButton>
//#include <QTime>
//#include <QLabel>
#include <QEvent>

//声明使用到的类
class gPlotCanvas;
class gPlotGrid;
class g_qwtScaleDraw;
class gLegendItem;
class gQwtLegendItem;

class g_qwtPlot : public QwtPlot
{
    Q_OBJECT
public:
    enum qwtPlotStyle{
        style0,
        style1
    };

    //绘图控件
    explicit g_qwtPlot(QWidget *parent = 0);
    
    //添加一条曲线并以index标记，color制定曲线颜色
    //index应保证唯一
    bool addCurve(int index = 0,QColor color = Qt::red);
    //设置绘图风格，style0为旧的蓝底背景不透明风格
    //style1为新半透明背景风格
    void setPlotStyle(qwtPlotStyle plotStyle);
    //获取当前绘图风格
    qwtPlotStyle plotStyle(){return _plotStyle;}
    //设置图例(右上角小图标)的数值显示
    void setLegendItemVal(int index,int val);
    //设置曲线数据显示的精度，单位
    //dot=0精度1,dot=1精度0.1
    void setCurveAttribute(int dot,QString unit);
    //设置y轴精度
    void setScaledValueRaised(int dot);
    //获取曲线精度
    int curveDot(){return _dot;}
    //获取单位
    QString curveUnit(){return _unit;}

    //设置曲线数据，index指定曲线
    //points为二维坐标数组
    void setSamples(unsigned int index,const QVector<QPointF> points);
    //获取最大允许曲线绘图上限
    static int maxCurveCapability();
    //关联曲线和图例
    void connectLegendItem(int index,gQwtLegendItem *item);
    //取消曲线和图例的关联
    void disconnectLegendItem(int index);

    //获取y轴的宽度
    int yScaleWidth();
    //清除图例数值显示并设置为0
    void clearLegendItemVal();
    //清除所有曲线数据
    void clearCurveVal();
    //测试曲线是否可视
    bool curveVisible(int index);
    //设置曲线是否显示
    void showCurve(int index,bool on);
    //获取当前曲线总数
    int curveNumber();

    //更新坐标轴显示
    void updateScale();
    //设置坐标轴参数
    //axisId指定x,y轴，min:坐标轴最小值，max:坐标轴最大值，step:步长
    //该设置会影响实际的数据存储
    void setAxisScale( int axisId, double min, double max, double step = 0 );
    //设置坐标轴参数
    //axisId指定x,y轴，min:坐标轴最小值，max:坐标轴最大值，step:步长
    //该设置临时有效，不会影响实际的数据存储
    void setTempAxisScale(int axisId, double min, double max, double step = 0);
    //更新图例
    virtual void updateUserLegend();

private:
    void initPlot();
    void initCSG();
    void initMainBodyStyle();
    void setStyle0();
    void setStyle1();
    void setLegendItemIconColor(int index,gQwtLegendItem *item);

private:
    typedef struct{
        double _min;
        double _max;
        double _step;
    }scaleParameter;

    qwtPlotStyle    _plotStyle;
    gPlotCanvas *_canvas;
    gPlotGrid *_grid;

    g_qwtScaleDraw  *_XgScaleDraw;
    g_qwtScaleDraw  *_YgScaleDraw;

    QMap<int,QwtPlotCurve *> _curve;
    QMap<int,gQwtLegendItem *> _item;
    QMap<int,scaleParameter> _scalPar;

    int _dot;
    QString _unit;

signals:
    
public slots:
    
};

#endif // G_QWTPLOT_H
