#include "timesetting.h"
#include <QDebug>
#include <time.h>

timeSetting  *timeSetting::sysTime = nullptr;

//时间，日期设置界面
//可直接调用instance显示界面
timeSetting::timeSetting(int width, int height,QWidget *parent) :
    dialogBase(parent)
{
    resize(width,height);

    setupUi();

//    setLanguage(translate::instance()->currentLanguage());

    connect(todayBtn,SIGNAL(clicked()),SLOT(todaySlot()));
    connect(ok,SIGNAL(clicked()),SLOT(okBtnSlot()));
    connect(cancel,SIGNAL(clicked()),SLOT(hide()));
}

timeSetting *timeSetting::instance(int width, int height, QWidget *parent)
{
    if( sysTime == NULL ){
        sysTime = new timeSetting(width,height,parent);
    }
    return sysTime;
}

void timeSetting::setupUi()
{
    my_calendar = new m_calender(this);
    my_calendar->setGeometry(20,45,380,320);

    my_clock = new analogclock(this);
    my_clock->setGeometry(440,120,200,300);

    todayBtn = new ubuttonBK(this);
    todayBtn->setGeometry(20,370,100,40);

    ok = new ubuttonBK(this);
    ok->setGeometry(420,420,100,40);

    cancel = new ubuttonBK(this);
    cancel->setGeometry(550,420,100,40);
}

void timeSetting::setTime()
{
    DateVal = my_calendar->getDate();
    TimeVal = my_clock->getTime();

    tm_ptr.tm_year = DateVal.year() - 1900;
    tm_ptr.tm_mon = DateVal.month() - 1;
    tm_ptr.tm_mday = DateVal.day();

    tm_ptr.tm_hour = TimeVal.hour();
    tm_ptr.tm_min = TimeVal.minute();
    tm_ptr.tm_sec = TimeVal.second();

    time = mktime(&tm_ptr);

    stime(&time);
    system("hwclock -w");

    hide();
}

void timeSetting::retranslateUi(QLocale::Language lang)
{
    switch(lang)
    {
        case QLocale::Chinese:{
        setChinese();
        my_calendar->setLanguage(lang);
    }
        break;
    case QLocale::English:{
        setEnglish();
        my_calendar->setLanguage(lang);
    }
        break;
    default:
        break;
    }
}

void timeSetting::setEnglish()
{
    ok->setText("Set date");
    todayBtn->setText("Today");
    cancel->setText("Cancel");
}

void timeSetting::setChinese()
{
    ok->setText("确定");
    todayBtn->setText("回到今天");
    cancel->setText("取消");
}

void timeSetting::okBtnSlot()
{
    //日期没有更改，时间也没有更改
    if( !my_calendar->dateChange() && !my_clock->timeChanged() ){
        hide();
        return;
    }

/*    if( buzzerControl::instance()->isRunning() ){
        while( !buzzerControl::instance()->isFinished() ){
            usleep(50 * 1000);
        }
    }
*/
    setTime();

    emit systemTimeChange();
}

void timeSetting::todaySlot()
{
    my_calendar->showToday();
}
