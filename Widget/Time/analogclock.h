#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H

#include <QWidget>
#include <QTime>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "akeyboard.h"

class clockBase : public QWidget
{
    Q_OBJECT
public:
    explicit clockBase(QWidget *parent = 0,int w = 200,int h = 200);

private:
    QPen    *basePen;

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

};

class analogclock : public QWidget
{
    Q_OBJECT
public:
    explicit analogclock(QWidget *parent = 0,int w = 200,int h = 200);

    //获取时钟定时器
    static QTimer *sysTimer();
    //模拟时钟静态实例
    static analogclock *instance(QWidget *parent = 0,int w = 200,int h = 200);
    //获取当前时间
    QTime getTime();

    //获取当前运行状态标志
    bool runningState();
    //获取时间更改标志
    bool timeChanged();
    //刷新时钟显示
    void flushClock(bool yes = true);

private:
    void InitMember();
    void m_connect();

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

signals:
    
public slots:
    //时，分，秒设置
    void hourBtSlot();
    void minBtSlot();
    void secBtSlot();

private:
    static analogclock *my_clock;
    static QTimer  *systemdTimer;
    QTime   clocktime;


    int t_hour;
    int t_min;
    int t_sec;

    clockBase   *my_base;

    QPoint hourHand[4];
    QPoint minuteHand[4];
    QPoint secondHand[4];

    QColor  *hourColor;
    QColor  *minuteColor;
    QColor  *secondColor;

    QColor  *ellipseColor;

    bool    Running;
    bool    changeState;

    QPushButton *m_hour;
    QPushButton *m_min;
    QPushButton *m_sec;
    
};

#endif // ANALOGCLOCK_H
