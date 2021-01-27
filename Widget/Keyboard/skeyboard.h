#ifndef SKEYBOARD_H
#define SKEYBOARD_H

#include <QDialog>
#include <QLineEdit>
#include <QLocale>
#include "keybutton.h"

//#include "screensaver.h"

class skeyboard : public QDialog
{
    Q_OBJECT
public:
    //键盘基础类，构建通用按键，提供通用功能实现
    explicit skeyboard(QWidget *parent = 0);

    //设置 返回主页或屏保时是否 显示，传false返回主页时将隐藏
    void setAlwaysStay(bool state);
    //语言切换
    void setLanguage(QLocale::Language lang);
protected:
    //按下事件
    void mousePressEvent(QMouseEvent * e);
    //释放事件
    void mouseReleaseEvent(QMouseEvent *e);
    //移动事件
    void mouseMoveEvent(QMouseEvent *e);
    //显示事件
    void showEvent(QShowEvent *e);

private:
    virtual void retranslateUi(QLocale::Language lang){}
    virtual void retranslateUi(){}
    virtual void setEnglish(){}
    virtual void setChinese(){}
    virtual void readyExit(){}

private slots:
    void hideSlot();
    virtual void langChangeSlot();

public:
    QVector<keyButton *> digitButton;
    QLineEdit   *lineEdit;

private:
    bool moveState;
    QPoint mousePosition;
    
private slots:
    virtual void digitClicked(){}
    
};

#endif // SKEYBOARD_H
