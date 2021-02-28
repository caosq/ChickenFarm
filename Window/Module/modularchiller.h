#ifndef MODULARCHILLER_H
#define MODULARCHILLER_H

#include <QWidget>
#include "modular.h"
#include "analogvalbutton.h"
#include "datalabel.h"
#include "statebutton.h"
#include "modebutton.h"
#include "meter.h"

namespace Ui {
class ModularChiller;
}

class ModularChiller : public Device
{
    Q_OBJECT

public:
    typedef enum
    {
        CMD_OPEN  = 0x00AA,
        CMD_CLOSE = 0x0055,
    }SwitchCmd;

    typedef enum   /*运行模式*/
    {
        RUN_MODE_COOL     = 1,      //制冷
        RUN_MODE_HEAT     = 2,      //制热
        RUN_MODE_DEFROST  = 3,      //手动化霜
    }RunningMode;

    uint16_t      m_usUnitID = 0x2017;    //机型ID
    uint16_t      m_usProtocolVer = 10;   //协议版本

    SwitchCmd     m_eSwitchCmd = CMD_CLOSE;           //启停命令
    RunningMode   m_eRunningModeCmd = RUN_MODE_COOL;  //机组运行工作模式设定

    int16_t   m_sRetWaterTemp = 0;             //平均回水温度
    uint16_t  m_usChillerCoolInTemp = 120;    //机组制冷进水温度设定值
    uint16_t  m_usChillerCoolOutTemp = 70;    //机组制冷出水温度设定值
    uint16_t  m_usChillerHeatInTemp = 450;    //机组制热进水温度设定值
    uint16_t  m_usChillerHeatOutTemp = 500;   //机组制热出水温度设定值

    bool          m_xCommErr = 0;             //通讯故障
    bool          m_xErrClean = 0;            //故障清除

    QVector<Modular*>  m_Modulars;

    uint16_t  m_usModularNum = 0;
    static uint8_t  m_usModularChillerCount;  //机组数量

public:
    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

    StateButton   *m_pSwitchCmdBtn;        //启停命令
    ModeButton    *m_pRunningModeCmdBtn;   //机组运行工作模式设定

    AnalogValButton *m_pChillerCoolInTempBtn;      //机组制冷进水温度设定值
    AnalogValButton *m_pChillerCoolOutTempBtn;     //机组制冷出水温度设定值
    AnalogValButton *m_pChillerHeatInTempBtn;      //机组制热进水温度设定值
    AnalogValButton *m_pChillerHeatOutTempBtn;     //机组制热出水温度设定值

    DataLabel     *m_pRetWaterTempLabel;   //平均回水温度
    DataLabel     *m_pCommErrLabel;        //通讯故障
    //DataLabel     *m_pErrCleanLabel;     //故障清除

private:
    void initLabel();
    void initButton();


public:
    explicit ModularChiller(QWidget *parent = nullptr);
    ~ModularChiller();

private slots:
    void sysModeCmdChangedSlot();

    void on_pushButton_clicked();

private:
    Ui::ModularChiller *ui;
};

#endif // MODULARCHILLER_H
