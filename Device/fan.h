#ifndef FAN_H
#define FAN_H
#include "device.h"

class Fan : public Device
{
public:
    explicit Fan();

    //eFreqType  eFanFreqType;   //频率类型

    uint16_t   m_MinFreq;      //频率下限 = 实际值*10
    uint16_t   m_MaxFreq;      //频率上限 = 实际值*10
    uint8_t    m_Freq_AO;      //频率输出AO通道
    uint8_t    m_Freq_AI;      //频率输入AI通道
    uint8_t    m_Switch_DO;    //启停DO通道
    uint8_t    m_RunState_DI;  //运行状态DI通道
    uint8_t    m_Err_DI;       //故障DI通道
};

#endif // FAN_H
