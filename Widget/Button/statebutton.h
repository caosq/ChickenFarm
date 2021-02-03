#ifndef STATEBUTTON_H
#define STATEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPainter>
#include "button.h"
#include "datamonitor.h"

enum eButtonState{
    State0 = 0,
    State1 = 1,
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
    StateButton(QWidget *parent = nullptr);

    //祖父按键
    static StateButton *forefather();

    void setDeafultState(eButtonState state);

    //获取默认状态值
    eButtonState getDefaultState();

    //获取当前状态值
    eButtonState getCurrentState();

    //设置数据的绑定
    bool setMonitorData(void* pvVal = nullptr, Monitor::DataType emDataType = Monitor::Uint16t);

    //设置数据与状态值的绑定关系
    bool setValueMap(eButtonState state, int val);

    //设置状态值显示的文字
    void setStateText(eButtonState state, QString str);

    //设置状态值显示的背景图片
    void setPixmap(eButtonState state,QPixmap *pixmap);

    //设置文字。state决定状态，text是对应状态要显示的文字
    void setText(eButtonState state,const QString &text);

    //设置状态值激活状态下的文字颜色(使能状态下的文字颜色)
    void setStateActiveTextColor(eButtonState state,QColor color);
    //设置状态值非激活状态下的文字颜色
    void setStateInactiveTextColor(eButtonState state, QColor color);

    //设置文字大小
    void setTextSize(int size);

    //设置点击，触发点击信号
    void setClicked();

    //设置父类字号大小
    void setFatherFontSize(int size);

    //设置父类图片
    //state决定按下或抬起，filePath：图片位置
    //father这两个函数作用于全部abutton实体，但必须是在father初始化之后
    void setFatherPixmap(eButtonState state, QString filePath);

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void fontSlot();


public slots:
    void setValue(unsigned int val);
    void clickedSlot();
signals:
    void fontChange();

protected:
    static StateButton *father;
    static QFont       fatherFont;
    static QPixmap     *pixmap0;
    static QPixmap     *pixmap1;

    bool defValState;
    bool enableValMarker;
    bool save;
    bool automaticAttack;

    Monitor::DataType   m_eDataType;

private:
    Monitor*    m_pMonitor;
    QMap<eButtonState, int>  m_StateValMap;
    QVector<sStateStyle>     m_StateStyleVector;

    eButtonState             m_CurrentState;
    eButtonState             m_DefaultState;
protected:

    eButtonState m_eDefaultState;
    eButtonState m_eCurrentState;
};




#endif // STATEBUTTON_H
