#ifndef BUTTERFLYVALVE_H
#define BUTTERFLYVALVE_H

#include <QWidget>
#include <QMap>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"
#include "statebutton.h"
#include "modebutton.h"
#include "analogvalbutton.h"

namespace Ui {
class ButterflyValve;
}

class ButterflyValve : public Device
{
    Q_OBJECT
public:

    typedef enum
    {
        CMD_CLOSE = 0,
        CMD_OPEN  = 1,
    }SwitchCmd;

    SwitchCmd     m_eSwitchCmd;        //启停命令
    bool          m_xRemote;           //远程/本地
    bool          m_xOpened;           //开到位
    bool          m_xClosed;           //关到位
    bool          m_xErrorFlag;        //设备故障
    bool          m_xErrClean;         //故障清除

    static uint8_t  m_usButterflyValveCount;   //水泵数量

private:
    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

    StateButton     *m_pSwitchCmdBtn;        //启停命令
    DataLabel       *m_pRemoteLabel;         //远程/本地
    DataLabel       *m_xOpenedLabel;         //开到位
    DataLabel       *m_xClosedLabel;         //关到位
    DataLabel       *m_pErrorFlagLabel;      //设备故障

private:
    void initLabel();
    void initButton();

public:
    explicit ButterflyValve(QWidget *parent = nullptr);
    ~ButterflyValve();

private:
    Ui::ButterflyValve *ui;
};

#endif // BUTTERFLYVALVE_H
