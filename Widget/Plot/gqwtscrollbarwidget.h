#ifndef GQWTSCROLLBARWIDGET_H
#define GQWTSCROLLBARWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include "gqwtscrollbar.h"
#include "qwtscrollbarbutton.h"

class gqwtScrollBarWidget : public QWidget
{
    Q_OBJECT
public:
    //绘图滚动条控件，集成上一页，下一页按键
    explicit gqwtScrollBarWidget(QWidget *parent = 0);

    //设置空间位置大小
    void setGeometry(int x, int y, int w, int h);
    //重置滚动条当前值，最大，最小值为0
    void reloadScrollBar();
    //设置最大滚动值
    void setMaximumVal(int max);
    //获取滚动条最大值
    int maximum();
    //跳转到最大值
    void goMax();
    //跳转到最小值
    void goMin();
    //获取当前值
    int currentValue();

private:
    void autoLength(int length);

private:
    gqwtScrollBar   *_scrollBar;
    qwtScrollBarButton  *_rBt;
    qwtScrollBarButton  *_lBt;

signals:
    //滚动数值有更改即发送信号
    void valueChanged(int);
    
private slots:

};

#endif // GQWTSCROLLBARWIDGET_H
