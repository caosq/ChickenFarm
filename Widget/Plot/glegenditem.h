#ifndef GLEGENDITEM_H
#define GLEGENDITEM_H

#include <qwt_plot_legenditem.h>

class gLegendItem : public QwtPlotLegendItem
{
public:
    enum legendItemStyle{
        style0,
        style1
    };

    //图例条目
    explicit gLegendItem();
    
    //获取控件最小大小
    virtual QSize minimumSize(const QwtLegendData &data) const;

    //设置图例风格
    void setStyle(legendItemStyle style);

private:
    virtual void drawIcon(QPainter *p,QColor baseColor,QRectF rectf) const;

protected:
    virtual void drawLegendData(QPainter *painter,
                                const QwtPlotItem * plotItem, const QwtLegendData & data,
                                const QRectF & rect) const;

signals:
    
public slots:

private:
    legendItemStyle _style;
    
};

#endif // GLEGENDITEM_H
