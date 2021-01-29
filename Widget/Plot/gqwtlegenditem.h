#ifndef GQWTLEGENDITEM_H
#define GQWTLEGENDITEM_H

#include "gqwtlegendbase.h"

class gQwtLegendItem : public gQwtLegendBase
{
    Q_OBJECT
public:
    //绘图标签类
    explicit gQwtLegendItem(QWidget *parent = 0);
    
    //设置推荐大小
    static void setSizeHint(QSize size);
    //推荐的控件大小
    static QSize legendSizeHint();

protected:
    void paintEvent(QPaintEvent *e);

private:
    static QSize   _sizeHint;

signals:
    
public slots:
    
};

#endif // GQWTLEGENDITEM_H
