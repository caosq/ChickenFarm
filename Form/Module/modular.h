#ifndef MODULAR_H
#define MODULAR_H

#include <QWidget>
#include <QMap>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"

class Compressor : public QWidget
{
    Q_OBJECT

public:
    explicit Compressor(QWidget *parent = nullptr);

private:
     bool m_xRunningFlag;   //运行标志

private slots:
    void valChangedSlot(Monitor*);

signals:
    void stateChanged(bool);
};


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
        STATE_DEFROST       = 3,      //化霜
        STATE_ANTI_FREEZE   = 4,      //冬天防冻
    }ModularState;

    ModularState   m_eModularState;     //模块状态

    uint8_t  m_usCompNum;
    uint8_t  m_RunnningCompCount = 0;   //运行压缩机数量

    int16_t  m_sInputWaterTemp;         //进水温度
    int16_t  m_sOutputWaterTemp;        //出水温度

    bool          m_xRunningFlag;      //运行标志
    bool          m_xErrorFlag;        //总故障标志
    bool          m_xAlarmFlag;        //总报警标志
    bool          m_xWaterValve;       //水流开关

    static uint8_t  m_usModularCount;  //模块数量

private:
    QVector<TextLabel*> m_Labels;

    DataLabel   m_eModularStateLabel;     //模块状态
    DataLabel   m_RunnningCompCountLabel; //运行压缩机数量
    DataLabel   m_sInputWaterTempLabel;   //进水温度
    DataLabel   m_sOutputWaterTempLabel;  //出水温度
    DataLabel   m_xRunningFlagLabel;      //运行标志
    DataLabel   m_xErrorFlagLabel;        //总故障标志
    DataLabel   m_xAlarmFlagLabel;        //总报警标志
    DataLabel   m_xWaterValveLabel;       //水流开关

private:
    void initLabel();
    void initButton();

public:
    explicit Modular(QWidget *parent = nullptr, uint8_t usCompNum = 4);
    ~Modular();

private slots:
    void compStateChangedSlot(bool);

private:
    Ui::Modular *ui;

    QVector<Compressor*>  m_Comps;
};

#endif // MODULAR_H
