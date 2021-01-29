#ifndef GTOOLBUTTON_H
#define GTOOLBUTTON_H

#include <QToolButton>
#include <QPainter>

class gtoolButton : public QToolButton
{
    Q_OBJECT
public:
    enum STATE{
        press = 0,
        release = 1
    };
    //曲线界面缩放按键类
    explicit gtoolButton(QWidget *parent = 0);
    
    //设置背景图
    void setPixmap(STATE state, QString filePath);
    //获取推荐大小值
    QSize sizeHint();

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void stateChange(bool _state);

private:
    QPixmap *pixOn;
    QPixmap *pixOff;
    bool state;
    
public slots:
    
};

#endif // GTOOLBUTTON_H
