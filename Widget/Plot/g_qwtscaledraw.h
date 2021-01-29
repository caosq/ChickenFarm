#ifndef G_QWTSCALEDRAW_H
#define G_QWTSCALEDRAW_H

#include <qwt_scale_draw.h>
#include <QPainter>
#include <QTime>

class g_qwtScaleDraw : public QwtScaleDraw
{
public:
    enum scaleDrawStyle{
        style0,
        style1
    };

    enum scaleValStyle{
        valueStyle,
        timeStyle
    };

    //绘制坐标刻度
    explicit g_qwtScaleDraw();
    
    //根据数值v返回刻度字符串
    virtual QwtText label(double v) const;
    //设置刻度绘画风格
    //style0是传统风格(白色的刻度)
    //style1是蓝色渐变刻度风格
    void setScaleDrawStyle(scaleDrawStyle style);
    //设置坐标轴数值显示风格
    //valueStyle显示实际数值
    //timeStyle会把数据转换成时间显示
    void setScaleValStyle(scaleValStyle style);
    //设置坐标轴文字字号
    void setScaleLabelTextSize(int size);
    //设置坐标轴文字颜色
    void setScaleLabelTextColor(QColor color);
    //设置坐标轴文字字族
    void setScaleLabelTextFamily(QString family);
    //设置坐标轴显示的精度
    void setScaleRaised(int dot);
    //获取坐标轴的宽度
    int widgetWidth();
private:
    void init();

protected:
    virtual void drawBackbone(QPainter *p) const;
    virtual void drawTick(QPainter *p, double val, double len) const;

private:
    QTime _baseTime;
    QFont _font;
    QColor _color;
    scaleDrawStyle _style;
    scaleValStyle _val_style;
    int _dot;

signals:
    
public slots:
    
};

#endif // G_QWTSCALEDRAW_H
