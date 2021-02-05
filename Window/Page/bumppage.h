#ifndef BUMPPAGE_H
#define BUMPPAGE_H

#include <QWidget>
#include <QMap>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"
#include "statebutton.h"
#include "modebutton.h"
#include "analogvalbutton.h"

namespace Ui {
class BumpPage;
}

class BumpPage : public QWidget
{
    Q_OBJECT

private:
    QVector<TextLabel*> m_Labels_1;
    QVector<QWidget*>   m_Widgets_1;

    QVector<TextLabel*> m_Labels_2;
    QVector<QWidget*>   m_Widgets_2;

    StateButton     *m_pSwitchCmdBtn;        //启停命令
    AnalogValButton *m_pFreqSetBtn;          //频率设置

    StateButton    *m_pErrorCleanCmdBtn;   //故障清除

    DataLabel      *m_pPowerLabel;         //实时功率
    DataLabel      *m_pTotalEnergyLabel;   //累计耗电量

private:
    void initLabel();
    void initButton();

public:
    explicit BumpPage(QWidget *parent = nullptr);
    ~BumpPage();

private:
    Ui::BumpPage *ui;
};

#endif // BUMPPAGE_H
