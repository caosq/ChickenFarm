#ifndef STATEBUTTON_H
#define STATEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include "button.h"
#include "datamonitor.h"

enum eButtonState{
    StateOff = 0,
    StateOn = 1,
    Error = 2
};

typedef struct{
    QPixmap *pImage;
    QString  strText;
    QColor   activeColor;
    QColor   inactiveColor;
    int      iValue;
}sStateStyle;


class StateButton : public Button
{
    Q_OBJECT
public:
    StateButton();

    //获取默认状态值
    eButtonState getDefaultState();

    //获取当前状态值
    eButtonState getCurrentState();

    //设置数据的绑定
    bool setMonitorData(void* pvVal = nullptr, eDataType emDataType = Uint16t);

    //设置数据与状态值的绑定关系
    bool setValueMap(eButtonState state, int val);

    //设置状态值显示的文字
    void setStateText(eButtonState state, QString str);

    //设置状态值显示的背景图片
    void setPixmap(eButtonState state,QPixmap *pixmap);

    //设置文字。state决定状态，text是对应状态要显示的文字
    void setText(eButtonState state,const QString text);

    //设置状态值激活状态下的文字颜色(使能状态下的文字颜色)
    void setStateActiveTextColor(eButtonState state,QColor color);
    //设置状态值非激活状态下的文字颜色
    void setStateInactiveTextColor(eButtonState state, QColor color);

    //设置文字大小
    void setTextSize(int size);

    //设置点击，触发点击信号
    void setClicked();

private:
    Monitor*    m_pMonitor;
    QMap<eButtonState, int>  m_StateValMap;
    QVector<sStateStyle>      m_StateStyleVector;

    eButtonState             m_CurrentState;

protected:

    eButtonState m_eDefaultState;
    eButtonState m_eCurrentState;

public slots:
    void setValue(unsigned int val);
    void clickedSlot();
};




#endif // STATEBUTTON_H
