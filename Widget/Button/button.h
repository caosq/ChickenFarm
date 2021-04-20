#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QTimerEvent>
#include <QTimer>
#include "datamonitor.h"
//#include "messagebox.h"

//#include "permissionControl/permissionset.h"
//#include "universalSet/buzzercontrol.h"
#include "textcontrol.h"

#define MAX_CHECK_COUNT 10

/*class  BtnCheckData : public QObject
{
    Q_OBJECT
public:
    explicit BtnCheckData(void* pCheckValAddr, int32_t  iCheckVal, Monitor::DataType eCheckDataType,
                                       const QString  &strChecktext, QObject *parent = nullptr);
public:

    void*    m_pCheckValAddr;
    int32_t  m_iCheckVal;
    Monitor::DataType m_eCheckDataType;
    QString  m_strChecktext;
};*/

class Button : public QPushButton
{
    Q_OBJECT
public:

    typedef struct BtnCheckData
        {
            void*    pCheckValAddr;
            int32_t  iCheckVal;
            Monitor::DataType eCheckDataType;
            QString  strChecktext;
        }BtnCheckData;


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

    //延时自动回弹模式
     void setDelayMode(int32_t iDelayTimeMs, int32_t iInitValue);

     //取消延时自动回弹按钮
     void disableDelayMode();

     //设置按键确认
     void setCheckMode(QVector<BtnCheckData> *pBtnCheckDatas);

     //设置按键确认
     void setCheckMode(void* pCheckValAddr, int32_t iCheckVal, const QString &text, Monitor::DataType emDataType = Monitor::Uint16t);

     void setCheckMode(int32_t iItemCount, ...);

     //取消按键确认
     void disableCheckMode();

     //设置确认模式
     void setConfirmMode(const QString &text);

     //取消确认模式
     void disableConfirmMode();

     static void setDebugMode(bool xConfirmMode);

public:
     QVector<BtnCheckData*> m_BtnCheckDatas;

     QTimer   m_DelayTimer;

     int32_t  m_iDelayTimeMs;
     int32_t  m_iInitValue;

     int32_t  m_iDefaultVal;
     int32_t  m_iCurrentVal;

     QString  m_sConfirmtext;

     bool  m_xDelayMode = false;
     bool  m_xCheckMode = false;
     bool  m_xConfirmMode = false;

     static bool g_xDebugMode;

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
    void valChanged(void*);

public slots:
    void privatePressSlot();
    void delayTimeOutSlot();
    void clickedSlot();

private:

    bool pressDown;


    //permissionSet::permissionLevel _level;

};

#endif // BUTTON_H
