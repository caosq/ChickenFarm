#ifndef AXISCURVE_H
#define AXISCURVE_H

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_curve.h>
#include <QTimer>

#include "g_qwtscaledraw.h"

class axisCurve : public QwtPlot
{
    Q_OBJECT
public:
    typedef struct{
        double *valx;
        double *valy;
        QwtPlotCurve *curve;
    }axisCurveData;

    //散点图(轴心图)
    //注：不能保存数据
    explicit axisCurve(QWidget *parent = 0);
    
    //设置坐标范围
    //ex:val=20000,dot=2,则x,y轴范围-200 ～ 200，精度保留2位小数
    void setDomain(double val,int dot);

    //添加曲线(实际只会画点，不会连线)
    //index：曲线id,color 曲线颜色， val1 : x轴数据地址，val2 : y轴数据地址
    bool addCurve(int index, QColor color,double *val1,double *val2);

    //移除曲线
    void removeCurve(int index);

    //设置描点的大小
    void setPointSize(int index,int size);
    //设置样本点
    void setSamples(unsigned int index, const QVector<QPointF> points);

    //清除所有描点
    void clearPoints();
    //刷新描点
    void updateSamples();
    //开始采集数据点
    void startSampling(bool start = true);
    //设置采集时间
    void setSamplingTime(int sec);
    //设置最大描点数
    void setMarkNum(int count);

private slots:
    void timeOutSlot();

signals:
    void prepareData();

private:
    QwtPlotGrid *_grid;
    g_qwtScaleDraw *_scaleXDraw;
    g_qwtScaleDraw *_scaleYDraw;

    QMap<int,axisCurveData> _curve;
    QVector<QPolygonF> _points;
    int _maxPointNum;
    QTimer  *_timer;
    
};

#endif // AXISCURVE_H
