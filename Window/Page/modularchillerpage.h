#ifndef MODULARCHILLERPAGE_H
#define MODULARCHILLERPAGE_H

#include <QWidget>
#include <QWidget>
#include "textlabel.h"
#include "device.h"
#include "statebutton.h"
#include "modebutton.h"
#include "datalabel.h"
#include "analogvalbutton.h"
#include "modularchiller.h"

namespace Ui {
class ModularChillerPage;
}

class ModularChillerPage : public QWidget
{
    Q_OBJECT

public:    QVector<ModularChiller*> m_ModularChillers;

private:

    QVector<TextLabel*> m_Labels_1;
    QVector<QWidget*>   m_Widgets_1;

    QVector<TextLabel*> m_Labels_2;
    QVector<QWidget*>   m_Widgets_2;

    StateButton   *m_pSwitchCmdBtn;         //启停命令
    ModeButton    *m_pRunningModeCmdBtn;    //机组运行工作模式设定

    DataLabel      *m_pPowerLabel;           //实时功率
    DataLabel      *m_pTotalEnergyLabel;     //累计耗电量

private:
    void initDevice();
    void initLabel();
    void initButton();


public:
    explicit ModularChillerPage(QWidget *parent = nullptr);
    ~ModularChillerPage();

private:
    Ui::ModularChillerPage *ui;
};

#endif // MODULARCHILLERPAGE_H