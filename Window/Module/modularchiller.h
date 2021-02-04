#ifndef MODULARCHILLER_H
#define MODULARCHILLER_H

#include <QWidget>
#include "modular.h"
#include "analogvalbutton.h"
#include "datalabel.h"
#include "statebutton.h"
#include "modebutton.h"

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
        RUN_MODE_COOL     = 1,      //供冷
        RUN_MODE_HEAT     = 2,      //供热
        RUN_MODE_DEFROST  = 3,      //化霜
    }RunningMode;


    uint16_t      m_usUnitID;          //机型ID
    uint16_t      m_usProtocolVer;     //协议版本

    SwitchCmd     m_eSwitchCmd;        //启停命令
    RunningMode   m_eRunningModeCmd;   //机组运行工作模式设定

    int16_t       m_sRetWaterTemp;     //平均回水温度
    bool          m_xCommErr;          //通讯故障
    bool          m_xErrClean;         //故障清除

    static uint8_t  m_usModularChillerCount;  //机组数量

private:

    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

    StateButton   *m_pSwitchCmdBtn;        //启停命令
    ModeButton    *m_pRunningModeCmdBtn;   //机组运行工作模式设定

    DataLabel     *m_pRetWaterTempLabel;   //平均回水温度
    DataLabel     *m_pCommErrLabel;        //通讯故障
    //DataLabel     *m_pErrCleanLabel;       //故障清除

private:
    void initLabel();
    void initButton();


public:
    explicit ModularChiller(QWidget *parent = nullptr);
    ~ModularChiller();


private:
    Ui::ModularChiller *ui;

    QVector<Modular*>  m_Modulars;

};

#endif // MODULARCHILLER_H
