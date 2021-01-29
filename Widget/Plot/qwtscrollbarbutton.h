#ifndef QWTSCROLLBARBUTTON_H
#define QWTSCROLLBARBUTTON_H

#include <QPushButton>
#include <QTimerEvent>
#include <QPainter>

class qwtScrollBarButton : public QPushButton
{
    Q_OBJECT
public:
    explicit qwtScrollBarButton(QWidget *parent = 0);
    
    //获取推荐大小值
    QSize sizeHint();
    //按键按下
    bool isDown();
    //设置背景图
    bool setPixmap(QString path);

protected:
    void timerEvent(QTimerEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    bool _pressDown;
    QPixmap * _pix;

signals:
    
public slots:
    
};

#endif // QWTSCROLLBARBUTTON_H
