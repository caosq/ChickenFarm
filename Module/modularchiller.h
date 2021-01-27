#ifndef MODULARCHILLER_H
#define MODULARCHILLER_H

#include <QWidget>
#include "modular.h"

namespace Ui {
class ModularChiller;
}

class ModularChiller : public QWidget
{
    Q_OBJECT

public:
    explicit ModularChiller(QWidget *parent = nullptr);
    ~ModularChiller();

    uint16_t      m_UnitID;                 //机型ID
    uint16_t      m_ProtocolVer;            //协议版本

    //eFuncMode     m_FuncMode;               //功能设定
    //eRunningMode  m_RunningMode;            //运行模式设定
    //eSwitchCmd    m_SwitchCmd;              //启停命令
    //eSwitchCmd    m_SwitchState;            //启停状态(实际下发命令)

    int16_t       m_sRetAirTemp;               //回风温度
    int16_t       m_SupAir_T;               //送风温度

    uint16_t      m_RetAir_Vol;             //回风风量
    uint16_t      m_SupAir_Vol;             //送风风量
    uint16_t      m_usFreAir_Vol;             //新风风
    uint16_t      m_RetAirDamper_Ang;       //回风阀当前开度
    uint16_t      m_FreAirDamper_Ang;       //新风阀当前开
    uint16_t      m_CoolTempSet;            //制冷温度值设定
    uint16_t      m_HeatTempSet;            //制热温度值设定
    uint16_t      m_FreAirSet_Vol;          //目标新风风量设定

    int16_t       m_AmbientIn_T;             //室内环境温度(群控检测)
    int16_t       m_AmbientInSelf_T;         //室内环境温度
    uint16_t      m_AmbientIn_H;             //室内环境湿度(群控检测)
    uint16_t      m_AmbientInSelf_H;         //室内环境湿度
    int16_t       m_AmbientOutSelf_T;        //室外环境温度
    uint16_t      m_AmbientOutSelf_H;        //室外环境湿度
    uint16_t      m_CO2PPM;                  //CO2平均浓度(群控检测)
    uint16_t      m_CO2PPMSelf;              //CO2平均浓度
    uint16_t      m_CO2AdjustThr_V;          //CO2浓度调节阈值
    uint16_t      m_CO2AdjustDeviat;         //CO2浓度调节

    bool          m_xCommErr;                 //通讯故障
    bool          m_StopErrFlag;             //停整机故障标志
    bool          m_ErrClean;                //故障清除
    bool          m_CO2SenErr;               //CO2传感器故障(群控下发)
    bool          m_TempSenInErr;            //室内温度传感器故障(群控下发)
    bool          m_HumiSenInErr;            //室内湿度传感器故障(群控下发)
    bool          m_TempSenInSelfErr;        //室内温度传感器故障(机组)
    bool          m_HumiSenInSelfErr;        //室内湿度传感器故障(机组)
    bool          m_RetAirSenErr;            //回风风速传感器故障
    bool          m_FreAirSenErr;            //新风风速传感器故障
    bool          m_CO2SenSelfErr_1;         //CO2传感器故障1(机组)
    bool          m_CO2SenSelfErr_2;         //CO2传感器故障2(机组)
    bool          m_SupAirDamperState;       //送风风阀状态
    bool          m_WetMode;                 //湿膜加湿
    bool          m_CompRunning;             //压缩机运行
    uint16_t      m_HumidityMax;             //设定湿度max
    uint16_t      m_HumidityMin;             //设定湿度min


    Modular* m_psModular1;
    Modular* m_psModular2;

private:
    Ui::ModularChiller *ui;
};

#endif // MODULARCHILLER_H
