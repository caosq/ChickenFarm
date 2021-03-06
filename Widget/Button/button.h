#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QTimerEvent>
#include <QTimer>
#include "datamonitor.h"

//#include "permissionControl/permissionset.h"
//#include "universalSet/buzzercontrol.h"
#include "textcontrol.h"

class messageBox;
class Button : public QPushButton
{
    Q_OBJECT
public:
    //按键基础类
    //实现一下通用功能设置
    //如字号，颜色，按键触发控制，蜂鸣，操作权限等等
    explicit Button(QWidget *parent = nullptr);

    //是否按下
    bool isDown();

    //获取操作权限
   // permissionSet::permissionLevel permissionLevel(){return _level;}

    //设置操作权限
    //void setPermission(permissionSet::permissionLevel level){_level = level;}

     void setDelayMode(int32_t iDelayTimeMs, int32_t iInitValue);

     void disableDelayMode();

     void setCheckMode(void* pCheckValAddr, int32_t iCheckVal, const QString &text, Monitor::DataType emDataType = Monitor::Uint16t);

public:
    QTimer   m_DelayTimer;
    Monitor::DataType m_CheckDataType;
    messageBox *confirmationBox;

    int32_t  m_iDelayTimeMs;
    int32_t  m_iInitValue;

    int32_t  m_iCheckVal;
    void*    m_pCheckValAddr;

    bool     m_xDelayMode = false;
    bool     m_xCheckMode = false;

protected:
    void timerEvent(QTimerEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    void initMessage();

    virtual bool canPress(){return true;}

signals:
    void delayTimeOut(int32_t);
    void buttonClicked();

public slots:
    void privatePressSlot();
    void delayTimeOutSlot();
    void clickedSlot();

private:
    bool pressDown;
    //permissionSet::permissionLevel _level;

};

#endif // BUTTON_H
