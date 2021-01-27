#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QTimerEvent>
//#include "permissionControl/permissionset.h"
//#include "universalSet/buzzercontrol.h"
#include "textcontrol.h"

class Button : public QPushButton
{
    Q_OBJECT
public:
    //按键基础类
    //实现一下通用功能设置
    //如字号，颜色，按键触发控制，蜂鸣，操作权限等等
    explicit Button(QWidget *parent = 0);

    //是否按下
    bool isDown();

    //获取操作权限
   // permissionSet::permissionLevel permissionLevel(){return _level;}

    //设置操作权限
    //void setPermission(permissionSet::permissionLevel level){_level = level;}

protected:
    void timerEvent(QTimerEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    virtual bool canPress(){return true;}

private slots:
    void privatePressSlot();

private:
    bool pressDown;
    //permissionSet::permissionLevel _level;

};

#endif // BUTTON_H
