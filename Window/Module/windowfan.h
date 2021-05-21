#ifndef WINDOWFAN_H
#define WINDOWFAN_H

#include <QWidget>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"
#include "statebutton.h"
#include "modebutton.h"
#include "analogvalbutton.h"

namespace Ui {
class WindowFan;
}

class WindowFan : public Device
{
    Q_OBJECT

public:

    uint16_t   m_usAngSet = 0;         //开度设置
    uint16_t   m_usAng = 0;            //开度反馈
    bool       m_xSwitchCmd = 0; //开关控制

    static uint8_t  m_usWindowFanCount;   //小窗电机数量

public:
    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

    StateButton     *m_pSwitchCmdBtn;        //开关控制
    AnalogValButton *m_pAngSetBtn;          //开度设置

    DataLabel       *m_pAngLabel;           //开度反馈

private:
    void initLabel();
    void initButton();

public:
    explicit WindowFan(QWidget *parent = nullptr);
    ~WindowFan();

private:
    Ui::WindowFan *ui;
};

#endif // WINDOWFAN_H
