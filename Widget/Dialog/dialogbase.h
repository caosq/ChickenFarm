#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include <QDialog>
#include <QLocale>
//#include "universalSet/screensaver.h"

class dialogBase : public QDialog
{
    Q_OBJECT
public:
    explicit dialogBase(QWidget *parent = 0);
    
    //设置背景颜色
    void setBackGroundColor(QColor color);
    //设置边框和抬头颜色
    void setFramelessWindowColor(QColor color);
    //使能拖动false设置为不能拖动，ture为可以拖动
    void disableMove(bool state);
    //设置屏保时窗口是否隐藏，false屏保时隐藏，true不隐藏
    void setAlwaysStay(bool state);

    //设置语言
    void setLanguage(QLocale::Language lang);

    //设置默认的背景颜色，在调用这个函数后所有弹框默认背景颜色将修改生效。
    static void setDefBackGroundColor(QColor color){_backgroundColor = color;}
    //设置默认边框颜色，在调用这个函数后所有弹框默认便可颜色将修改生效。
    static void setDefFramelessWindowColor(QColor color){_frameLessColor = color;}

protected:
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);
    
private:
    virtual void retranslateUi(QLocale::Language lang);
    virtual void retranslateUi(){}
    virtual void setEnglish(){}
    virtual void setChinese(){}

private slots:
    virtual void langChangeSlot();

private:
    bool moveState;
    bool pressed;
    QPoint mousePosition;

    static QColor _frameLessColor;
    static QColor _backgroundColor;
    QColor frameLessColor;
};

#endif // DIALOGBASE_H
