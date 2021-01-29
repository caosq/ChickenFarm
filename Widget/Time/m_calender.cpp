#include "m_calender.h"

#include <QPainter>
#include <QDebug>
#include <stdio.h>

//#include "universalSet/buzzercontrol.h"
#include "akeyboard.h"

#define CALENDER_X  380
#define CALENDER_Y  280

#define WINDOW_Y    CALENDER_Y + 40

#define LEFT_ARROW_ICO  ":/images/baseFile/calender_left_arrow.png"
#define RIGHT_ARROW_ICO  ":/images/baseFile/calender_right_arrow.png"

#define MAX_YEAR 2037
#define MIN_YEAR 2015

//日历模块。提供年，月，日等信息显示
m_calender::m_calender(QWidget *parent) :
    QWidget(parent)
{
    setupUi();

    showToday();
    m_connects();
}

void m_calender::m_connects()
{
    //日期选择变更
    connect(my_calender,SIGNAL(selectionChanged()),SLOT(CurrentDateChangSlot()));
    //当前页变更，传递的参数是年份，月份
    connect(my_calender,SIGNAL(currentPageChanged(int,int)),SLOT(PageChangSlot(int,int)));

    //下一个月
    connect(right_button,SIGNAL(clicked()),SLOT(nextMonthSlot()));
    //上一个月
    connect(left_button,SIGNAL(clicked()),SLOT(previousMonthSlot()));
    //点击可通过键盘设置年份
    connect(yearBtn,SIGNAL(clicked()),SLOT(yearBtnSlot()));
    //点击可通过键盘设置月份
    connect(monthBtn,SIGNAL(clicked()),SLOT(monthBtnSlot()));

}

//每当日历显示时，显示当天日期
void m_calender::showEvent(QShowEvent *e)
{
    showToday();
}


void m_calender::setupUi()
{
    setStyleSheet("background-color:#efeeeb;font-size:18px;");
    resize(CALENDER_X,WINDOW_Y);

    QLocale chinese(QLocale::Chinese,QLocale::China);

    //新建日历
    my_calender = new QCalendarWidget(this);
    my_calender->setGeometry(0,33,CALENDER_X,CALENDER_Y);
    my_calender->setNavigationBarVisible(false);
    my_calender->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    my_calender->setLocale(chinese);

    //设置日历日期上下限
    MaximumDate = QDate(MAX_YEAR,12,31);
    MinimumDate = QDate(MIN_YEAR,1,1);
    my_calender->setMaximumDate(MaximumDate);
    my_calender->setMinimumDate(MinimumDate);

    yearBtn = new QPushButton(this);
    yearBtn->setFocusPolicy(Qt::NoFocus);
    yearBtn->setFlat(true);
    yearBtn->setGeometry(130,2,60,27);
    yearBtn->setStyleSheet("background-color:transparent;font-size:22px;color:#dfd8c8;");

    monthBtn = new QPushButton(this);
    monthBtn->setFocusPolicy(Qt::NoFocus);
    monthBtn->setFlat(true);
    monthBtn->setGeometry(200,2,60,27);
    monthBtn->setStyleSheet("background-color:transparent;font-size:22px;color:#dfd8c8;");

    right_icon = new QIcon(RIGHT_ARROW_ICO);
    left_icon = new QIcon(LEFT_ARROW_ICO);

    QSize rightSize(35,24);

    //下一月按键
    right_button = new QPushButton(this);
    right_button->setStyleSheet("background-color:transparent;");
    right_button->setIcon(*right_icon);
    right_button->setIconSize(rightSize);
    right_button->setFlat(true);
    right_button->setFocusPolicy(Qt::NoFocus);
    right_button->setGeometry(CALENDER_X - 36,4,35,25);

    //上一月按键
    left_button = new QPushButton(this);
    left_button->setStyleSheet("background-color:transparent;");
    left_button->setIcon(*left_icon);
    left_button->setIconSize(rightSize);
    left_button->setFlat(true);
    left_button->setFocusPolicy(Qt::NoFocus);
    left_button->setGeometry(0,4,35,25);

}

//刷新顶部年份，月份显示。积累当前年，月
void m_calender::updateLabelShow()
{   
    current_month = my_calender->monthShown();
    current_year = my_calender->yearShown();

    year_str.sprintf("%.4d",current_year);
    month_str.sprintf("%.2d",current_month);

    yearBtn->setText(year_str);
    monthBtn->setText(month_str);
}

void m_calender::setMaxDate(QDate date)
{
    my_calender->setMaximumDate(date);
}

//oldCurrentDate在日历显示时赋值，current_date在每次改变日期时赋值
bool m_calender::dateChange()
{
    if( current_date != oldCurrentDate )
        return true;
    else
        return false;
}


void m_calender::setLanguage(QLocale locale)
{
    my_calender->setLocale(locale);
}

void m_calender::setminDate(QDate date)
{
    my_calender->setMinimumDate(date);
}

QDate m_calender::getDate()
{
    return current_date;
}

void m_calender::showToday()
{
    current_date = QDate::currentDate();

    my_calender->showToday();
    my_calender->setSelectedDate(current_date);

    updateLabelShow();

    oldCurrentDate = current_date;
}

//实现年份跳转没，默认跳转后日期必定为1号
void m_calender::yearBtnSlot()
{
//    buzzerControl::instance()->wantToBB();

    int tempYear = aKeyBoard::getValue(current_year,MAX_YEAR,MIN_YEAR,0);

    if( tempYear >= 2015 && tempYear <=2037 ){
        current_year = tempYear;
    }else{
        return;
    }

    my_calender->setCurrentPage(current_year,current_month);

    updateLabelShow();

    QDate   tempDate;
    //每月的天数不一致，所以限定为1号
    tempDate.setDate(current_year,current_month,1);
    my_calender->setSelectedDate(tempDate);
}

//实现月份跳转
void m_calender::monthBtnSlot()
{
//    buzzerControl::instance()->wantToBB();

    int tempMonth = aKeyBoard::getValue(current_month,12,1,0);

    if( tempMonth >= 1 && tempMonth <=12 ){
        current_month = tempMonth;
    }else{
        return;
    }

    my_calender->setCurrentPage(current_year,current_month);

    updateLabelShow();

    QDate   tempDate;
    tempDate.setDate(current_year,current_month,1);
    my_calender->setSelectedDate(tempDate);

}

//跳转到下一月
void m_calender::nextMonthSlot()
{
//    buzzerControl::instance()->wantToBB();
    my_calender->showNextMonth();

    QDate   tempDate;
    tempDate.setDate(my_calender->yearShown(),my_calender->monthShown(),1);
    my_calender->setSelectedDate(tempDate);
}

//跳转到上一月
void m_calender::previousMonthSlot()
{
//    buzzerControl::instance()->wantToBB();
    my_calender->showPreviousMonth();
    QDate   tempDate;
    tempDate.setDate(my_calender->yearShown(),my_calender->monthShown(),1);
    my_calender->setSelectedDate(tempDate);
}

//刷新当前日历年月显示
void m_calender::PageChangSlot(int year, int month)
{
    updateLabelShow();
}

//更新当期日历
void m_calender::CurrentDateChangSlot()
{
    current_date = my_calender->selectedDate();
    my_calender->setSelectedDate(current_date);

}
