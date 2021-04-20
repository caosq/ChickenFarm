#ifndef STATEBUTTON_H
#define STATEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPainter>
#include "button.h"
#include "datamonitor.h"

class StateButton : public Button
{
    Q_OBJECT
public:

    enum ButtonState{
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
    }StateStyle;

    StateButton(QWidget *parent = nullptr);

    //祖父按键
    static StateButton *forefather();

    void setDeafultState(ButtonState state);

    //获取默认状态值
    ButtonState getDefaultState();

    //获取当前状态值
    ButtonState getCurrentState();

    //设置数据的绑定
    bool setMonitorData(void* pvVal = nullptr, Monitor::DataType emDataType = Monitor::Uint16t);

    //设置数据与状态值的绑定关系
    bool setValueMap(ButtonState state, int val);

    //设置状态值显示的文字
    void setStateText(ButtonState state, QString str);

    //设置状态值显示的背景图片
    void setPixmap(ButtonState state,QPixmap *pixmap);

    //设置文字。state决定状态，text是对应状态要显示的文字
    void setText(ButtonState state,const QString &text);

    //设置状态值激活状态下的文字颜色(使能状态下的文字颜色)
    void setStateActiveTextColor(ButtonState state,QColor color);
    //设置状态值非激活状态下的文字颜色
    void setStateInactiveTextColor(ButtonState state, QColor color);

    //设置文字大小
    void setTextSize(int size);

    //设置点击，触发点击信号
    void setClicked();

    //设置父类字号大小
    void setFatherFontSize(int size);

    //设置父类图片
    //state决定按下或抬起，filePath：图片位置
    //father这两个函数作用于全部abutton实体，但必须是在father初始化之后
    void setFatherPixmap(ButtonState state, QString filePath);

     int32_t getCurrentValue();

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void fontSlot();

public slots:
    void setValue(int32_t val);
    void setValue(Monitor* pMonitor);
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
    QMap<ButtonState, int>  m_StateValMap;
    QVector<StateStyle>     m_StateStyleVector;

    int32_t                 m_CurrentValue;
    ButtonState             m_CurrentState;
    ButtonState             m_DefaultState;
protected:

    ButtonState m_eDefaultState;
    ButtonState m_eCurrentState;
};




#endif // STATEBUTTON_H
