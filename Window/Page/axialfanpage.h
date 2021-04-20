#ifndef AXIALFANPAGE_H
#define AXIALFANPAGE_H

#include <QWidget>
#include "textlabel.h"
#include "statebutton.h"
#include "modebutton.h"
#include "datalabel.h"
#include "analogvalbutton.h"
#include "axialfan.h"
#include "windowfan.h"

namespace Ui {
class AxialFanPage;
}

class AxialFanPage : public QWidget
{
    Q_OBJECT
public:
    QVector<AxialFan*> m_AxialFans;
    QVector<WindowFan*> m_WindowFans;

private:

    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

    StateButton     *m_pSwitchCmdBtn;        //启停命令
    AnalogValButton *m_pFreqSetBtn;          //频率设置

    StateButton    *m_pErrorCleanCmdBtn;     //故障清除

private:
    void initDevice();
    void initLabel();
    void initButton();

public:
    explicit AxialFanPage(QWidget *parent = nullptr);
    ~AxialFanPage();

private slots:
    void paramSetBtnValChanged(void*);

private:
    Ui::AxialFanPage *ui;
};

#endif // AXIALFANPAGE_H
