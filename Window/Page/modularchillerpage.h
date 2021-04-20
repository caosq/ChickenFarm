#ifndef MODULARCHILLERPAGE_H
#define MODULARCHILLERPAGE_H

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

public:
    Meter    m_sChillerMeter;   //电表
    QVector<ModularChiller*>   m_ModularChillers;

private:

    QVector<TextLabel*> m_Labels_1;
    QVector<QWidget*>   m_Widgets_1;

    QVector<TextLabel*> m_Labels_2;
    QVector<QWidget*>   m_Widgets_2;

    StateButton   *m_pSwitchCmdBtn;         //启停命令
    ModeButton    *m_pRunningModeCmdBtn;    //机组运行工作模式设定

    AnalogValButton *m_pChillerCoolInTemp;      //机组制冷进水温度设定值
    AnalogValButton *m_pChillerCoolOutTemp;     //机组制冷出水温度设定值
    AnalogValButton *m_pChillerHeatInTemp;      //机组制热进水温度设定值
    AnalogValButton *m_pChillerHeatOutTemp;     //机组制热出水温度设定值

    DataLabel  *m_pPowerLabel;           //实时功率
    DataLabel  *m_pTotalEnergyLabel;     //累计耗电量
    DataLabel  *m_pCommErrLabel;         //通讯故障

    //uint8_t    m_usCurrentIndex = 0;

private:
    void initDevice();
    void initLabel();
    void initButton();


public:
    explicit ModularChillerPage(QWidget *parent = nullptr);
    ~ModularChillerPage();

private slots:
    void paramSetBtnValChanged(void*);
    void on_pushButton_clicked();

private:
    Ui::ModularChillerPage *ui;
};

#endif // MODULARCHILLERPAGE_H
