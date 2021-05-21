#ifndef BYPASSVALVE_H
#define BYPASSVALVE_H

#include <QWidget>
#include "datalabel.h"
#include "textlabel.h"
#include "statebutton.h"
#include "modebutton.h"
#include "analogvalbutton.h"


namespace Ui {
class BypassValve;
}

class BypassValve : public Device
{
    Q_OBJECT

public:
    uint16_t  m_usAngSet = 0;     //开度设置
    uint16_t  m_usAng = 0;        //开度反馈
    bool      m_xControlMode = 0; //控制模式
    bool      m_xErrorFlag = 0;     //故障

public:
    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

    ModeButton      *m_pControlModeCmdBtn;  //控制模式
    AnalogValButton *m_pAngSetBtn;          //开度设置
    DataLabel       *m_pAngLabel;           //开度反馈
    DataLabel       *m_pErrLabel;           //故障

private:
    void initLabel();
    void initButton();

public:
    explicit BypassValve(QWidget *parent = nullptr);
    ~BypassValve();

private slots:
    void stateChangedSlot(void*);
    void paramSetBtnValChanged(void*);

private:
    Ui::BypassValve *ui;
};

#endif // BYPASSVALVE_H
