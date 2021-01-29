#ifndef M_CALENDER_H
#define M_CALENDER_H

#include <QWidget>

#include <QCalendarWidget>
#include <QLabel>
#include <QPushButton>

class m_calender : public QWidget
{
    Q_OBJECT
public:
    explicit m_calender(QWidget *parent = 0);

    //获取当前系统日期并设置日历显示
    void showToday();
    //获取当前日期
    QDate getDate();
    //限定日历显示最小日期
    void setminDate(QDate date);
    //设置日历显示最大日期
    void setMaxDate(QDate date);

    //判断日期是否修改
    bool dateChange();
    //设置语言（翻译）
    void setLanguage(QLocale locale);
private:
    void setupUi();
    void updateLabelShow();
    void m_connects();

protected:
    void showEvent(QShowEvent *e);

private:
    QCalendarWidget *my_calender;

    QDate   MaximumDate;
    QDate   MinimumDate;
    QDate   current_date;
    int current_month;
    int current_year;

    QString year_str;
    QString month_str;

    QPushButton *yearBtn;
    QPushButton *monthBtn;

    QIcon *right_icon;
    QIcon *left_icon;

    QPushButton *right_button;
    QPushButton *left_button;

    QDate   oldCurrentDate;

signals:
    
public slots:
    void PageChangSlot(int year,int month);
    void CurrentDateChangSlot();

    void previousMonthSlot();
    void nextMonthSlot();
    void yearBtnSlot();
    void monthBtnSlot();

    
};

#endif // M_CALENDER_H
