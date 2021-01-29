#ifndef TIMESETTING_H
#define TIMESETTING_H

#include "dialogbase.h"
#include "ubuttonbk.h"
#include "m_calender.h"
#include "analogclock.h"

class timeSetting : public dialogBase
{
    Q_OBJECT
public:
    explicit timeSetting(int width = 680, int height = 480,QWidget *parent = NULL);
    static timeSetting *instance(int width = 680, int height = 480,
                                 QWidget *parent = NULL);

private:
    void setupUi();
    void setTime();
    virtual void retranslateUi(QLocale::Language lang);
    virtual void setEnglish();
    virtual void setChinese();

private:
    //QPushButton *todayBtn;
    static timeSetting  *sysTime;
    ubuttonBK   *todayBtn;
    ubuttonBK   *cancel;
    ubuttonBK   *ok;

    m_calender *my_calendar;
    analogclock *my_clock;

    QDate   DateVal;
    QTime   TimeVal;

    time_t  time;
    struct tm tm_ptr;


signals:
    void systemTimeChange();

public slots:
    //显示今日日期
    void todaySlot();
    //执行时间设置
    void okBtnSlot();


};

#endif // TIMESETTING_H
