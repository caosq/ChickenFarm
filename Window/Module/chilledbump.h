#ifndef CHILLEDBUMP_H
#define CHILLEDBUMP_H

#include <QWidget>
#include <QMap>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"
#include "statebutton.h"
#include "modebutton.h"
#include "analogvalbutton.h"

namespace Ui {
class ChilledBump;
}

class ChilledBump : public Device
{
    Q_OBJECT

public:

    uint16_t  m_usFreqSet = 0;         //频率设置
    uint16_t  m_usFreq = 0;            //频率反馈
    uint16_t  m_usStateMask = 0;     //状态标志

    bool      m_xSwitchCmd = 0;        //启停命令
    bool      m_xRemote = 0;           //远程/本地
    bool      m_xRunningFlag = 0;      //运行标志
    bool      m_xErrorFlag = 0;        //设备故障
    bool      m_xControlFlag = 0;      //控制故障
    bool      m_xErrClean = 0;         //故障清除

    static uint8_t  m_usChilledBumpCount;   //水泵数量

public:
    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

    StateButton     *m_pSwitchCmdBtn;        //启停命令
    AnalogValButton *m_pFreqSetBtn;          //频率设置

    DataLabel       *m_pFreqLabel;           //频率反馈
    DataLabel       *m_pRemoteLabel;         //远程/本地
    DataLabel       *m_pRunningFlagLabel;    //运行标志
    DataLabel       *m_pErrorFlagLabel;      //设备故障
    DataLabel       *m_pControlFlagLabel;    //控制故障

private:
    void initLabel();
    void initButton();

private slots:
    void stateChangedSlot(void*);

public:
    explicit ChilledBump(QWidget *parent = nullptr);
    ~ChilledBump();

private:
    Ui::ChilledBump *ui;
};

#endif // CHILLEDBUMP_H
