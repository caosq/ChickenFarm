#ifndef GQWTSCROLLBAR_H
#define GQWTSCROLLBAR_H

#include <QScrollBar>

class gqwtScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    //绘图界面滚动条类
    explicit gqwtScrollBar(QWidget *parent = 0);
    
    //获取推荐大小
    QSize sizeHint();

protected:
    void timerEvent(QTimerEvent *e);


signals:
    
public slots:
    void gotoMax();
    void gotoMin();
    
};

#endif // GQWTSCROLLBAR_H
