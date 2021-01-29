#ifndef GPLOTPICKER_H
#define GPLOTPICKER_H

#include <QWidget>
#include <qwt_plot_picker.h>
#include <qwt_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_text.h>
#include <qwt_plot_textlabel.h>
#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include <qwt_text_label.h>

#include <QDateTime>
#include <QMouseEvent>


class g_qwtPlot;
class gPlotPicker : public QwtPlotPicker
{
    Q_OBJECT
public:
    //绘图数据采集控件
    //功能：将触碰区域曲线转换成数据显示
    explicit gPlotPicker(QwtPlot *pPlot = 0);
    
    //获取数据采集的父类
    g_qwtPlot * fatherPlot() const;
    void clearDisplay();
protected:
    virtual QwtText trackerTextF( const QPointF & pos) const;

private:
    QwtText setScrollVal(const QPointF &pos) const;
    bool curveInfoAt(QwtPlotCurve *curve, const QPointF &pos) const;
    QLineF curveLineAt(const QwtPlotCurve *curve, double x) const;

public:
    virtual void widgetLeaveEvent(QEvent *e);
    virtual void widgetEnterEvent(QEvent *e);

private:
    g_qwtPlot *_plot;
    QEvent *_e;


};

#endif // GPLOTPICKER_H
