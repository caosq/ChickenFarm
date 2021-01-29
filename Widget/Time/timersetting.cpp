#include "timersetting.h"

timerSetting::timerSetting(quint8 id,QWidget *parent) :
    QWidget(parent),timeId(id)
{
    initState = false;
    initTimer();
}

void timerSetting::initNow()
{
    setupUi();
    initState = true;
}

void timerSetting::setGeometry(int x, int y, int w, int h)
{
    move(x,y);
}

void timerSetting::setupUi()
{
    resize(838,452);
    tTable = new timerTable(this);
    tTable->move(0, 0);
    tTable->show();

    wtable = new weekTable(this);
    wtable->move(113, 370);
    wtable->show();

    wtable->setWeekItemData(timer->getTimerWeekModeList());
    tTable->setItemData(timer->getTimerItemTimeList());

    connect(tTable,SIGNAL(timerValChange(int,quint16)),timer,SLOT(setTimerItemTime(int,quint16)));
    connect(tTable,SIGNAL(selectChange(int,quint8)),timer,SLOT(setTimerItemSelectState(int,quint8)));
    connect(wtable,SIGNAL(weekModeChange(int,int)),timer,SLOT(setTimerWeekMode(int,int)));

}

//初始化定时器
void timerSetting::initTimer()
{
    //virtualTimer实现时间录入，时间超时检查，发送超时信号
    //配置定时属性(开，关或任意的8位数值)，并带有数据记忆功能
    //timeid作为存储的识别代号，需保证timeId唯一
    //36指定virtualTimer内部创建36个时间记录对象
    timer = new virtualTimer(timeId,36,this);
    timer->setTimerItemState(0,6,virtualTimer::timerOn);
    timer->setTimerItemState(12,18,virtualTimer::timerOn);
    timer->setTimerItemState(24,30,virtualTimer::timerOn);

    timer->setTimerItemState(6,12,virtualTimer::timerOff);
    timer->setTimerItemState(18,24,virtualTimer::timerOff);
    timer->setTimerItemState(30,36,virtualTimer::timerOff);

    timer->addTimerModeItem(1,0,12);
    timer->addTimerModeItem(2,12,24);
    timer->addTimerModeItem(3,24,36);

    connect(timer,SIGNAL(timeOut(quint8)),this,SIGNAL(timeOut(quint8)));
}

void timerSetting::showEvent(QShowEvent *e)
{
    if( !initState ){
        initNow();
    }
    QWidget::showEvent(e);
}
