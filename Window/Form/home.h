#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include "datalabel.h"
#include "textlabel.h"
#include "modebutton.h"
#include "analogvalbutton.h"

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

private:
    QVector<QWidget*>   m_Widgets;
    bool m_xSysModeChangedAlarm = false;  //自动切手动报警

    DataLabel     *m_pTempInLabel;    //室内温度
    DataLabel     *m_pHumiInLabel;    //室内湿度

    DataLabel     *m_pTempOutLabel;   //室外温度
    DataLabel     *m_pHumiOutLabel;   //室外湿度

    DataLabel     *m_pCO2Label;       //CO2浓度
    DataLabel     *m_pFreAirVolumeLabel;//新风量

    AnalogValButton  *m_pGrowDaysBtn;  //生长周期天数
    ModeButton    *m_pSysModeCmdBtn;   //系统工作模式设定
    DataLabel     *m_pSysStateLabel;   //系统状态

    DataLabel   *m_pModularStateLabel_1;     //模块1状态
    DataLabel   *m_pModularStateLabel_2;     //模块2状态
    DataLabel   *m_pModularStateLabel_3;     //模块3状态
    DataLabel   *m_pModularStateLabel_4;     //模块4状态

    DataLabel   *m_pModularAirModeLabel_1;  //1#组合柜模式
    DataLabel   *m_pModularAirModeLabel_2;  //2#组合柜模式

private:
    void initLabel();
    void initButton();

private slots:
    void systemDataChangedSlot();

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

private:
    Ui::Home *ui;
};

#endif // HOME_H
