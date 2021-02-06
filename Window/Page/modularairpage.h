#ifndef MODULARAIRPAGE_H
#define MODULARAIRPAGE_H

#include <QWidget>
#include "textlabel.h"
#include "device.h"
#include "statebutton.h"
#include "modebutton.h"
#include "datalabel.h"
#include "analogvalbutton.h"
#include "modularair.h"

namespace Ui {
class ModularAirPage;
}

class ModularAirPage : public QWidget
{
    Q_OBJECT

public:
    QVector<ModularAir*> m_ModularAirs;

private:

    QVector<TextLabel*> m_Labels_1;
    QVector<QWidget*>   m_Widgets_1;

    QVector<TextLabel*> m_Labels_2;
    QVector<QWidget*>   m_Widgets_2;

    StateButton   *m_pSwitchCmdBtn;         //启停命令
    ModeButton    *m_pRunningModeCmdBtn;    //机组运行工作模式设定

    AnalogValButton  *m_pTempSetBtn;     //目标温度设定
    AnalogValButton  *m_pHumiSetBtn;     //目标湿度设定
    AnalogValButton  *m_pCO2SetBtn;      //目标CO2设定

    StateButton    *m_pErrorCleanCmdBtn;   //故障清除

    DataLabel      *m_pPowerLabel;         //实时功率
    DataLabel      *m_pTotalEnergyLabel;   //累计耗电量


private:
    void initDevice();
    void initLabel();
    void initButton();

public:
    explicit ModularAirPage(QWidget *parent = nullptr);
    ~ModularAirPage();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ModularAirPage *ui;
};

#endif // MODULARAIRPAGE_H
