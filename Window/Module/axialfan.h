#ifndef AXIALFAN_H
#define AXIALFAN_H

#include <QWidget>
#include <QMap>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"
#include "statebutton.h"
#include "modebutton.h"
#include "analogvalbutton.h"

namespace Ui {
class AxialFan;
}

class AxialFan : public Device
{
    Q_OBJECT
public:

    typedef enum
    {
        CMD_CLOSE = 0,
        CMD_OPEN  = 1,
    }SwitchCmd;

    SwitchCmd     m_eSwitchCmd;        //启停命令

    uint8_t       m_usFreqSet;         //频率设置
    uint8_t       m_usFreq;            //频率反馈

    bool          m_xRemote;           //远程/本地
    bool          m_xRunningFlag;      //运行标志
    bool          m_xErrorFlag;        //设备故障
    bool          m_xControlFlag;      //控制故障
    bool          m_xErrClean;         //故障清除

    static uint8_t  m_usAxialFanCount;   //水泵数量

private:
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

public:
    explicit AxialFan(QWidget *parent = nullptr);
    ~AxialFan();

private:
    Ui::AxialFan *ui;
};

#endif // AXIALFAN_H