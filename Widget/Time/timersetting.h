#ifndef TIMERSETTING_H
#define TIMERSETTING_H

#include <QWidget>

#include "universalModule/timerOnOff/timertable.h"
#include "universalModule/timerOnOff/weektable.h"
#include "universalModule/timerOnOff/virtualtimer.h"

class timerSetting : public QWidget
{
    Q_OBJECT
public:
    //定时模块，提供定时功能。定时时间到将触发相关信号通知用户
    explicit timerSetting(quint8 id = 0,QWidget *parent = 0);
    
    //初始化界面显示
    void initNow();
    //设置控件显示位置和大小
    void setGeometry(int x, int y, int w, int h);
private:
    void setupUi();
    void initTimer();

protected:
    void showEvent(QShowEvent *e);

private:
    timerTable  *tTable;
    weekTable   *wtable;
    virtualTimer *timer;
    quint8 timeId;
    bool initState;

signals:
    void timeOut(quint8);

public slots:
    
};

#endif // TIMERSETTING_H
