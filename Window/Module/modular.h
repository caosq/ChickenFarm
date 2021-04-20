#ifndef MODULAR_H
#define MODULAR_H

#include <QWidget>
#include <QMap>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"

#define MODULAR_NUM_IN_CHILLER  4

namespace Ui {
class Modular;
}

class Modular : public Device
{
    Q_OBJECT

public:
    typedef enum   /*模块状态*/
    {
        STATE_CLOSED        = 0,      //关机
        STATE_COOL          = 1,      //制冷
        STATE_HEAT          = 2,      //制热
        STATE_DEFROST       = 3,      //手动化霜
        STATE_ANTI_FREEZE   = 4,      //冬天防冻
    }ModularState;

    ModularState m_eModularState = STATE_CLOSED;   //模块状态

    Modular  *m_pBrotherModular;        //同一机组中相邻模块
    int16_t  m_sInputWaterTemp = 0;     //进水温度
    int16_t  m_sOutputWaterTemp = 0;    //出水温度

    bool   m_xRunningFlag = 0;    //运行标志
    bool   m_xErrorFlag = 0;      //总故障标志
    bool   m_xAlarmFlag = 0;      //总报警标志
    bool   m_xWaterValve = 0;     //水流开关
    bool   m_xCompRunnnig_1 = 0;  //压缩机1
    bool   m_xCompRunnnig_2 = 0;  //压缩机2

    static uint8_t  m_usModularCount;  //模块数量

public:
    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

    DataLabel *m_pModularStateLabel;     //模块状态
    DataLabel *m_pInputWaterTempLabel;   //进水温度
    DataLabel *m_pOutputWaterTempLabel;  //出水温度
    DataLabel *m_pRunningFlagLabel;      //运行标志
    DataLabel *m_pErrorFlagLabel;        //总故障标志
    DataLabel *m_pAlarmFlagLabel;        //总报警标志
    DataLabel *m_pWaterValveLabel;       //水流开关
    DataLabel *m_pCompRunnnig_1;         //压缩机1
    DataLabel *m_pCompRunnnig_2;         //压缩机2

private:
    void initLabel();
    void initButton();

public:
    explicit Modular(QWidget *parent = nullptr, uint16_t usDeviceIndex = MODULAR_NUM_IN_CHILLER);
    ~Modular();

private slots:
    void stateChangedSlot(void*);

private:
    Ui::Modular *ui;


};

#endif // MODULAR_H
