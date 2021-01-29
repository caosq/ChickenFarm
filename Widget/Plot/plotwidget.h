#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>

#include <QTime>

#include "g_qwtplot.h"
#include "gqwtscrollbarwidget.h"
#include "gplotpicker.h"

#include "gqwtlegenditem.h"
#include "gqwtplotzoomer.h"
#include "gtoolbutton.h"

class plotWidget : public QWidget
{
    Q_OBJECT
public:
    friend class qHistoryCurve;
    friend class qrealTimeCurve;

    //曲线绘制控件
    explicit plotWidget(QWidget *parent = 0);

    //设置x轴范围
    void setXAxisScale(double min, double max, double step = 0);
    //设置y轴范围
    void setYAxisScale(double min, double max, double step = 0);

    //设置图例间的间隔
    void setLegendItemSpace(int space);
    //设置图例距离绘图区顶部间隔
    void setLegendItemTopSpace(int space);
    //设置图例距离绘图区右侧的间隔
    void setLegendItemRightSpace(int space);

    //设置位置和大小
    void setGeometry(int x, int y, int w, int h);
    //设置曲线精度（dot）属性，单位 unit
    void setCurveAttribute(int dot,QString unit);
    //设置曲线数值采样点,index指定曲线，points装载样本点
    void setSamples(unsigned int index,const QVector<QPointF> points);

    //获取绘图对象
    g_qwtPlot   *qwtPlot(){return gPlot;}
    //更新时间轴sampleSetp指定采样间隔
    void updateTimeScale(unsigned int sampleStep);

    //查看当前曲线是否可视
    bool curveVisible(int index);
    //获取当前x轴的数值
    int currentXScaleVal();

    //添加曲线，index指定曲线标号(标号要唯一)，color指定颜色
    bool addCurve(int index = 0,QColor color = Qt::red);
    //添加曲线，index指定曲线标号(标号要唯一)，color指定颜色
    //legendItem绑定要关联的数值显示标签，关联后点击曲线能显示数值
    bool addCurve(int index = 0,gQwtLegendItem *legendItem = 0,
                  QColor color = Qt::red);
    //设置曲线是否可视
    void showCurve(int index = 0,bool on = true);

    //重置x轴坐标，使用内置基准时间
    void reloadXScale();
    //重置x轴坐标，使用外置基准时间
    void reloadXScale(QTime time);
    //重置滚动条
    void reloadScrollBar();
    //获取x轴最大值
    int baseXScaleMaximum();
    //获取x轴最小值
    int baseXScaleMinimum();
    //获取x轴跳转步长
    int xScaleJumpStep();

    //设置滚动条最大值
    void setScrollBarMaximum(int max);
    //获取绘制的曲线数量
    int curveNumber();
    //设置x轴坐标数值
    //min最小值，max最大值，step步长，time参考基准时间
    void setXAxisScale(double min, double max,
                       double step,QTime time);
    void disablePlotPicker(bool ok = true);
    bool plotPickerIsEnable();
    QString topLeftLegendStr();
private:
    void setLegendGeometry();
    void setTopLeftLegendGeometry();
    void initValue();
    void initTimeScale();
    void scrollBarGoMax();

protected:
    void showEvent(QShowEvent *e);

signals:
    void reduced();

public slots:
    void clearZoomSlot();

private slots:
    void setTopLeftLegendShow(QString str);
    virtual void scrollBarValSlot(int val);
    void enableZoomMode( bool );
    void reduceSlot();

    void updateYScale(QRectF rect);

private:
    enum zoomType{
        clear = 0,
        zoom = 1,
        reduce = 2,
        zoomTypeSize = 3
    };

    g_qwtPlot   *gPlot;
    gqwtScrollBarWidget   *scBar;
    gPlotPicker *_plotPicker;
    QMap<int,gQwtLegendItem *> _item;

    QTime _zeroTime;

    QLabel  *_topLeftShow;
    gQwtPlotZoomer *_zoomer;
    int _reduceDepth;

    QVector<gtoolButton *> _zoomItem;

    int _itemSpace;
    int _itemTopSpace;
    int _itemRightSpace;

    double _xScaleVal;
    double _xScaleStepVal;

    double _baseXScaleMin;
    double _baseXScaleMax;

    double _currentXScaleMin;
    double _currentXScaleMax;

    double _xScaleJumpStep;

    double _currentXScaleVal;


};
#endif // PLOTWIDGET_H
