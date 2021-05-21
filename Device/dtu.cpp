#include "dtu.h"
#include "system.h"
#include "mbmap_m.h"

#define PROTOCOL_TYPE_ID   0x5601+10
#define COMM_ERR_DELAY_S   30
#define OFFLINE_CONTAIN_S  60

DTU::DTU(QObject *parent) :
    QObject(parent)
{
    m_sMBDTUDev247.ucDevAddr = 247;
    m_sMBDTUDev247.usProtocolID = 0;
    m_sMBDTUDev247.xDevOnTimeout = 0;
    m_sMBDTUDev247.xSynchronized = 0;
    m_sMBDTUDev247.ucOfflineTimes = 0;
    m_sMBDTUDev247.xStateTestRequest = 0;
    m_sMBDTUDev247.xDataReady = 0;
    m_sMBDTUDev247.xOnLine = 0;
    m_sMBDTUDev247.psDevDataInfo = nullptr;
    m_sMBDTUDev247.psDevCurData = nullptr;
    m_sMBDTUDev247.pLast = nullptr;
    m_sMBDTUDev247.pNext = nullptr;

    m_sMBDTUDev200.ucDevAddr = 200;
    m_sMBDTUDev200.usProtocolID = 0;
    m_sMBDTUDev200.xDevOnTimeout = 0;
    m_sMBDTUDev200.xSynchronized = 0;
    m_sMBDTUDev200.ucOfflineTimes = 0;
    m_sMBDTUDev200.xStateTestRequest = 0;
    m_sMBDTUDev200.xDataReady = 0;
    m_sMBDTUDev200.xOnLine = 0;
    m_sMBDTUDev200.psDevDataInfo = nullptr;
    m_sMBDTUDev200.psDevCurData = nullptr;
    m_sMBDTUDev200.pLast = nullptr;
    m_sMBDTUDev200.pNext = nullptr;
}

void DTU::registDevCommData()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    ModularChiller *pModularChiller = nullptr;
    ModularAir     *pModularAir = nullptr;

    ChilledBump    *pChilledBump = nullptr;
    AxialFan       *pAxialFan    = nullptr;
    ButterflyValve *pValve       = nullptr;

MASTER_DEV_INIT(m_sDevCommData)

MASTER_BEGIN_DATA_BUF(m_psRegHoldBuf, m_sDevCommData.sMBRegHoldTable, NULL)

    MASTER_REG_HOLD_DATA(0,  uint16,  0,  65535,  0x5601, WO, 1, m_usUnitID);
    MASTER_REG_HOLD_DATA(1,  uint16,  0,  65535,  11,     WO, 1, m_usProtocolVer);
    MASTER_REG_HOLD_DATA(2,  uint16,  0,  65535,  10,     WO, 1, m_usSoftWareVer);
    MASTER_REG_HOLD_DATA(4,  uint16,  0,  65535,  0X55,   WO, 1, m_usProductPro);

    MASTER_REG_HOLD_DATA(5,   uint16,  0,  65535,  0,  WO, 1, m_usProductID_1);
    MASTER_REG_HOLD_DATA(6,   uint16,  0,  65535,  0,  WO, 1, m_usProductID_2);
    MASTER_REG_HOLD_DATA(7,   uint16,  0,  65535,  0,  WO, 1, m_usProductID_3);
    MASTER_REG_HOLD_DATA(8,   uint16,  0,  65535,  0,  WO, 1, m_usProductID_4);
    MASTER_REG_HOLD_DATA(9,   uint16,  0,  65535,  0,  WO, 1, m_usProductID_5);
    MASTER_REG_HOLD_DATA(10,  uint16,  0,  65535,  0,  WO, 1, m_usProductID_6);
    MASTER_REG_HOLD_DATA(11,  uint16,  0,  65535,  0,  WO, 1, m_usProductID_7);
    MASTER_REG_HOLD_DATA(12,  uint16,  0,  65535,  0,  WO, 1, m_usProductID_8);

    MASTER_REG_HOLD_DATA(13,  uint16,  0,  65535,  0,  WO, 1, m_usProductCode_1);
    MASTER_REG_HOLD_DATA(14,  uint16,  0,  65535,  0,  WO, 1, m_usProductCode_2);
    MASTER_REG_HOLD_DATA(15,  uint16,  0,  65535,  0,  WO, 1, m_usProductCode_3);

    MASTER_REG_HOLD_DATA(17,  uint16,  0,  65535,  0,  WO, 1, pSystem->m_eSystemState);
    MASTER_REG_HOLD_DATA(18,  uint16,  0,  65535,  0,  WO, 1, pSystem->m_eSystemModeCmd);

    MASTER_REG_HOLD_DATA(28,  uint16,  100,  500,  240,  WO, 1, pSystem->m_usCHWPressureDiff);
    MASTER_REG_HOLD_DATA(29,  uint16,  50,   200,   70,  WO, 1, pSystem->m_usCHWOutletCoolTemp);

    MASTER_REG_HOLD_DATA(38,  uint16,    0,  300,   10,  WO, 1, pSystem->m_usCHWBumpCloseDelay);
    MASTER_REG_HOLD_DATA(39,  uint16,    0,  300,   10,  WO, 1, pSystem->m_usChilledDevsOpenDelay);

    MASTER_REG_HOLD_DATA(44,  uint16,    0,  300,   10,  WO, 1, pSystem->m_usButterflyValveCloseDelay);
    MASTER_REG_HOLD_DATA(48,  uint16,    0,  300,   10,  WO, 1, pSystem->m_usCHWBypassAdjustTime);
    MASTER_REG_HOLD_DATA(49,  uint16,    0,  500,  300,  WO, 1, pSystem->m_usCHWBumpAdjustPeriod);

    MASTER_REG_HOLD_DATA(53,  uint16,    0,  300,   10,  WO, 1, pSystem->m_usChilledDevsCloseDelay);

    MASTER_REG_HOLD_DATA(63,  uint16,    0,  500,  500,  WO, 1, pSystem->m_usCHWBumpMaxFreq);
    MASTER_REG_HOLD_DATA(64,  uint16,    0,  500,  350,  WO, 1, pSystem->m_usCHWBumpMinFreq);
    MASTER_REG_HOLD_DATA(67,  uint16,    0,  500,  500,  WO, 1, pSystem->m_usExAirFanMaxFreq);
    MASTER_REG_HOLD_DATA(68,  uint16,    0,  500,  350,  WO, 1, pSystem->m_usExAirFanMinFreq);

    MASTER_REG_HOLD_DATA(69,  uint16,    0,   30,   10,  WO, 1, pSystem->m_usChillerPlusPeriod);
    MASTER_REG_HOLD_DATA(72,  uint16,    0,   30,   10,  WO, 1, pSystem->m_usChillerSubPeriod);

    MASTER_REG_HOLD_DATA(80,  uint16,  0,  500,    10,  WO, 1, pSystem->m_usCHWBumpAdjustFreq_1);
    MASTER_REG_HOLD_DATA(81,  uint16,  0,  500,     5,  WO, 1, pSystem->m_usCHWBumpAdjustFreq_2);
    MASTER_REG_HOLD_DATA(82,  uint16,  0,  500,    10,  WO, 1, pSystem->m_usCHWBumpAdjustFreq_3);
    MASTER_REG_HOLD_DATA(83,  uint16,  0,  500,     5,  WO, 1, pSystem->m_usCHWBumpAdjustFreq_4);
    MASTER_REG_HOLD_DATA(84,  uint16,  0,  500,   350,  WO, 1, pSystem->m_usExAirFanPlusFreq_1);
    MASTER_REG_HOLD_DATA(85,  uint16,  0,  500,   380,  WO, 1, pSystem->m_usExAirFanPlusFreq_2);
    MASTER_REG_HOLD_DATA(86,  uint16,  0,  500,   350,  WO, 1, pSystem->m_usExAirFanSubFreq_1);
    MASTER_REG_HOLD_DATA(87,  uint16,  0,  500,   380,  WO, 1, pSystem->m_usExAirFanSubFreq_2);
    MASTER_REG_HOLD_DATA(88,  uint16,  50,  100,  100,  WO, 1, pSystem->m_usCHWBypassMaxAng);
    MASTER_REG_HOLD_DATA(89,  uint16,   0,   20,    0,  WO, 1, pSystem->m_usCHWBypassMinAng);

    MASTER_REG_HOLD_DATA(90,  uint16,   1,   20,    5,  WO, 1, pSystem->m_usCHWBypassRange);
    MASTER_REG_HOLD_DATA(91,  uint16,   2,   15,    5,  WO, 1, pSystem->m_usCHWPressureDeviat);
    MASTER_REG_HOLD_DATA(96,  uint16,  25,   60,   50,  WO, 1, pSystem->m_usCHWTempDiff);

    MASTER_REG_HOLD_DATA(123,  uint16, 1970, 9999, 1970,  WO, 1, pSystem->m_usSysYear);
    MASTER_REG_HOLD_DATA(124,  uint16,    1,   12,    1,  WO, 1, pSystem->m_usSysMon);
    MASTER_REG_HOLD_DATA(125,  uint16,    1,   31,    1,  WO, 1, pSystem->m_usSysDay);
    MASTER_REG_HOLD_DATA(127,  uint16,    0,   23,    0,  WO, 1, pSystem->m_usSysHour);
    MASTER_REG_HOLD_DATA(128,  uint16,    0,   59,    0,  WO, 1, pSystem->m_usSysMin);
    //MASTER_REG_HOLD_DATA(405,  uint16,    0,   59,    0,  WO, 1, pSystem->m_usSysSec);

    MASTER_REG_HOLD_DATA(188,  int16,  -200,   1000,   0,  WO, 1, pSystem->m_pCHWTempSensors[0]->m_sTemp);
    MASTER_REG_HOLD_DATA(189,  int16,  -200,   1000,   0,  WO, 1, pSystem->m_pCHWTempSensors[1]->m_sTemp);
    MASTER_REG_HOLD_DATA(190,  uint16,    0,  16000,   0,  WO, 1, pSystem->m_pCHWPressureSensors[0]->m_usPressure);
    MASTER_REG_HOLD_DATA(191,  uint16,    0,  16000,   0,  WO, 1, pSystem->m_pCHWPressureSensors[1]->m_usPressure);
    MASTER_REG_HOLD_DATA(192,  int16,  -400,    700,   0,  WO, 1, pSystem->m_sAmbientIn_T);
    MASTER_REG_HOLD_DATA(194,  uint16,    0,   1000,   0,  WO, 1, pSystem->m_usAmbientIn_H);

    MASTER_REG_HOLD_DATA(196,  int16,  -400,    700,   0,  WO, 1, pSystem->m_sAmbientOut_T);
    MASTER_REG_HOLD_DATA(197,  uint16,    0,   1000,   0,  WO, 1, pSystem->m_usAmbientOut_H);
    MASTER_REG_HOLD_DATA(202,  uint16,    0,  65535,   0,  WO, 1, pSystem->m_usTotalFreAir_H);
    MASTER_REG_HOLD_DATA(203,  uint16,    0,  65535,   0,  WO, 1, pSystem->m_usTotalFreAir_L);

    pModularChiller = pSystem->m_pModularChillers[0];

    MASTER_REG_HOLD_DATA(228,  uint16,  100,  250, 120,  WO, 1, pModularChiller->m_usChillerCoolInTemp);
    MASTER_REG_HOLD_DATA(229,  uint16,   50,  200,  70,  WO, 1, pModularChiller->m_usChillerCoolOutTemp);
    MASTER_REG_HOLD_DATA(230,  uint16,  300,  450, 400,  WO, 1, pModularChiller->m_usChillerHeatInTemp);
    MASTER_REG_HOLD_DATA(231,  uint16,  350,  500, 450,  WO, 1, pModularChiller->m_usChillerHeatOutTemp);
    MASTER_REG_HOLD_DATA(234,  uint16,   85,  170,  85,  WO, 1,  pModularChiller->m_eSwitchCmd);
    MASTER_REG_HOLD_DATA(235,  uint16,    1,    3,   1,  WO, 1,  pModularChiller->m_eRunningModeCmd);
    MASTER_REG_HOLD_DATA(236,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[0]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(237,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[0]->m_sOutputWaterTemp);
    MASTER_REG_HOLD_DATA(238,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[1]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(239,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[1]->m_sOutputWaterTemp);

    MASTER_REG_HOLD_DATA(244,  uint16,  0,  4, 0,  WO, 1, pModularChiller->m_Modulars[0]->m_eModularState);
    MASTER_REG_HOLD_DATA(245,  uint16,  0,  4, 0,  WO, 1, pModularChiller->m_Modulars[1]->m_eModularState);
    MASTER_REG_HOLD_DATA(246,  uint16,  0,  4, 0,  WO, 1, pModularChiller->m_Modulars[2]->m_eModularState);

    pModularChiller = pSystem->m_pModularChillers[1];

    MASTER_REG_HOLD_DATA(264,  uint16,  100,  250, 120,  WO, 1, pModularChiller->m_usChillerCoolInTemp);
    MASTER_REG_HOLD_DATA(265,  uint16,   50,  200,  70,  WO, 1, pModularChiller->m_usChillerCoolOutTemp);
    MASTER_REG_HOLD_DATA(266,  uint16,  300,  450, 400,  WO, 1, pModularChiller->m_usChillerHeatInTemp);
    MASTER_REG_HOLD_DATA(267,  uint16,  350,  500, 450,  WO, 1, pModularChiller->m_usChillerHeatOutTemp);

    MASTER_REG_HOLD_DATA(270, uint16,   85,  170,  85, WO, 1,  pModularChiller->m_eSwitchCmd);
    MASTER_REG_HOLD_DATA(271, uint16,    1,    3,  1,  WO, 1,  pModularChiller->m_eRunningModeCmd);

    MASTER_REG_HOLD_DATA(272,  int16,  -300,  700, uint16_t(-300),  WO, 1, pSystem->m_pModularChillers[0]->m_Modulars[2]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(273,  int16,  -300,  700, uint16_t(-300),  WO, 1, pSystem->m_pModularChillers[0]->m_Modulars[2]->m_sOutputWaterTemp);

    MASTER_REG_HOLD_DATA(280,  uint16,  0,  4, 0,  WO, 1, pModularChiller->m_Modulars[0]->m_eModularState);
    MASTER_REG_HOLD_DATA(281,  uint16,  0,  4, 0,  WO, 1, pModularChiller->m_Modulars[1]->m_eModularState);
    MASTER_REG_HOLD_DATA(282,  uint16,  0,  4, 0,  WO, 1, pModularChiller->m_Modulars[2]->m_eModularState);
    MASTER_REG_HOLD_DATA(283,  uint16,  0,  4, 0,  WO, 1, pModularChiller->m_Modulars[3]->m_eModularState);

    MASTER_REG_HOLD_DATA(307,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[0]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(308,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[0]->m_sOutputWaterTemp);
    MASTER_REG_HOLD_DATA(309,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[1]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(310,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[1]->m_sOutputWaterTemp);
    MASTER_REG_HOLD_DATA(342,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[2]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(343,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[2]->m_sOutputWaterTemp);
    MASTER_REG_HOLD_DATA(344,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[3]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(345,  int16,  -300,  700, uint16_t(-300),  WO, 1, pModularChiller->m_Modulars[3]->m_sOutputWaterTemp);

    for(uint8_t i = 0; i < pSystem->m_pChilledBumps.count(); i++)
    {
        pChilledBump = pSystem->m_pChilledBumps[i];
        MASTER_REG_HOLD_DATA(374+i*4,  uint16,  0,  500,   0,  WO, 1, pChilledBump->m_usFreqSet);
        MASTER_REG_HOLD_DATA(375+i*4,  uint16,  0,  500,   0,  WO, 1, pChilledBump->m_usFreq);
        MASTER_REG_HOLD_DATA(377+i*4,  uint16,  0,  65535,   0,  WO, 1, pChilledBump->m_usStateMask);
    }
    MASTER_REG_HOLD_DATA(393,  uint16, 0, 100, 0,  WO, 1, pSystem->m_pWindowFans[0]->m_usAngSet);
    MASTER_REG_HOLD_DATA(394,  uint16, 0, 100, 0,  WO, 1, pSystem->m_pWindowFans[1]->m_usAngSet);
    MASTER_REG_HOLD_DATA(395,  uint16, 0, 100, 0,  WO, 1, pSystem->m_pWindowFans[0]->m_usAng);
    MASTER_REG_HOLD_DATA(396,  uint16, 0, 100, 0,  WO, 1, pSystem->m_pWindowFans[1]->m_usAng);

    for(uint8_t i = 0; i < pSystem->m_pAxialFans.count(); i++)
    {
        pAxialFan = pSystem->m_pAxialFans[i];
        MASTER_REG_HOLD_DATA(434+i*4,  uint16,  0,  500,   0,  WO, 1, pAxialFan->m_usFreqSet);
        MASTER_REG_HOLD_DATA(435+i*4,  uint16,  0,  500,   0,  WO, 1, pAxialFan->m_usFreq);
        MASTER_REG_HOLD_DATA(437+i*4,  uint16,  0,  65535, 0,  WO, 1, pAxialFan->m_usStateMask);
    }
    for(uint8_t i = 0; i < pSystem->m_pButterflyValves.count(); i++)
    {
        pValve = pSystem->m_pButterflyValves[i];
        MASTER_REG_HOLD_DATA(514+i,  uint16,  0,  65535,   0,  WO, 1, pValve->m_usStateMask);
    }

    MASTER_REG_HOLD_DATA(558,  uint16,  0,    100,  0,  WO, 1, pSystem->m_pBypassValve->m_usAngSet);
    MASTER_REG_HOLD_DATA(561,  uint16,  0,    100,  0,  WO, 1, pSystem->m_pBypassValve->m_usAng);
    MASTER_REG_HOLD_DATA(566,  uint16,  0,  65535,  0,  WO, 1, m_usStateMask_2);
    MASTER_REG_HOLD_DATA(583,  uint16,  0,  65535,  0,  WO, 1, m_usStateMask_1);
    MASTER_REG_HOLD_DATA(584,  uint16,  0,  65535,  0,  WO, 1, m_usStateMask_3);

    MASTER_REG_HOLD_DATA(593,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pChillerMeter->m_usPower);
    MASTER_REG_HOLD_DATA(595,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[0]->m_sMeter.m_usPower);
    MASTER_REG_HOLD_DATA(596,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[1]->m_sMeter.m_usPower);
    MASTER_REG_HOLD_DATA(597,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pBumpMeter->m_usPower);


    MASTER_REG_HOLD_DATA(625,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pChillerMeter->m_usTotalEnergy_H);
    MASTER_REG_HOLD_DATA(626,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pChillerMeter->m_usTotalEnergy_L);

    MASTER_REG_HOLD_DATA(629,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[0]->m_sMeter.m_usTotalEnergy_H);
    MASTER_REG_HOLD_DATA(630,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[0]->m_sMeter.m_usTotalEnergy_L);
    MASTER_REG_HOLD_DATA(631,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[1]->m_sMeter.m_usTotalEnergy_H);
    MASTER_REG_HOLD_DATA(632,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[1]->m_sMeter.m_usTotalEnergy_L);

    MASTER_REG_HOLD_DATA(633,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pBumpMeter->m_usTotalEnergy_H);
    MASTER_REG_HOLD_DATA(634,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pBumpMeter->m_usTotalEnergy_L);

    MASTER_REG_HOLD_DATA(702,   int16,    -2,    462,      0,  WO, 1, pSystem->m_sChickenGrowDay);
    MASTER_REG_HOLD_DATA(703,  uint16,  10000,  30000,  20000,  WO, 1, pSystem->m_usCO2PPMSet);
    MASTER_REG_HOLD_DATA(704,  uint16,   160,    350,    240,  WO, 1, pSystem->m_usTempSet);
    MASTER_REG_HOLD_DATA(705,  uint16,     0,   1000,    500,  WO, 1, pSystem->m_usHumiSet);
    MASTER_REG_HOLD_DATA(706,  uint16,     0,    500,     10,  WO, 1, pSystem->m_usExAirFanAdjustFreq);

    MASTER_REG_HOLD_DATA(708,  uint16,  0,   450,  250,  WO, 1, pSystem->m_usEnergyTemp);
    MASTER_REG_HOLD_DATA(709,  uint16,  0,   550,  450,  WO, 1, pSystem->m_usSupAirMaxTemp);
    MASTER_REG_HOLD_DATA(710,  uint16,  0,   100,    5,  WO, 1, pSystem->m_usTempDeviat);
    MASTER_REG_HOLD_DATA(711,  uint16,  0,  1000,  350,  WO, 1, pSystem->m_usCoolWaterDamperMinAng_1);
    MASTER_REG_HOLD_DATA(712,  uint16,  0,  1000,   50,  WO, 1, pSystem->m_usCoolWaterDamperMinAng_2);
    MASTER_REG_HOLD_DATA(713,  uint16,  0,  1000,    0,  WO, 1, pSystem->m_usHeatWaterDamperMinAng);
    MASTER_REG_HOLD_DATA(714,  uint16,  0,  1000,  100,  WO, 1, pSystem->m_usExAirDamperMinAng);

    MASTER_REG_HOLD_DATA(715,  uint16,  0,  10000,  5000,  WO, 1, pSystem->m_usCO2AdjustDeviat_Up);
    MASTER_REG_HOLD_DATA(716,  uint16,  0,  10000,  5000,  WO, 1, pSystem->m_usCO2AdjustDeviat_Down);
    MASTER_REG_HOLD_DATA(717,  uint16,  0,  35000,  2000,  WO, 1, pSystem->m_usCO2ExAirDeviat_1);
    MASTER_REG_HOLD_DATA(718,  uint16,  0,  35000,  2000,  WO, 1, pSystem->m_usCO2ExAirDeviat_2);
    MASTER_REG_HOLD_DATA(719,  uint16,  0,  35000, 27000,  WO, 1, pSystem->m_usCO2AdjustThr);
    MASTER_REG_HOLD_DATA(720,  uint16,  0,  35000, 30000,  WO, 1, pSystem->m_usCO2PPMAlarm);
    MASTER_REG_HOLD_DATA(721,  uint16,  0,  7200,   1200,  WO, 1, pSystem->m_usExAirFanFreqRunPeriod);
    MASTER_REG_HOLD_DATA(722,  uint16,  0,  1440,     60,  WO, 1, pSystem->m_usExAirFanIntervalMin);
    MASTER_REG_HOLD_DATA(723,  uint16,  0,  7200,    300,  WO, 1, pSystem->m_usExAirFanRunTimeMin);
    MASTER_REG_HOLD_DATA(724,  uint16,  0,  7200,     30,  WO, 1, pSystem->m_usExAirFanFreqAdjustTime);
    MASTER_REG_HOLD_DATA(725,  uint16,  0,    50,     10,  WO, 1, pSystem->m_usExAirFanTempDeviat);
    MASTER_REG_HOLD_DATA(726,  uint16,  0,   200,     90,  WO, 1, pSystem->m_usExAirFanRatio);

    MASTER_REG_HOLD_DATA(727,  uint16,  0,   30,  10,  WO, 1, pSystem->m_usChillerPlusRemain);
    MASTER_REG_HOLD_DATA(728,  uint16,  0,   30,  10,  WO, 1, pSystem->m_usChillerSubRemain);

    MASTER_REG_HOLD_DATA(729,  uint16,  0,   60,  10,  WO, 1, pSystem->m_usModeChangePeriod_1);
    MASTER_REG_HOLD_DATA(730,  uint16,  0,   60,  10,  WO, 1, pSystem->m_usModeChangePeriod_2);
    MASTER_REG_HOLD_DATA(731,  uint16,  0,   60,  10,  WO, 1, pSystem->m_usModeChangePeriod_3);
    MASTER_REG_HOLD_DATA(732,  uint16,  0,   60,  10,  WO, 1, pSystem->m_usModeChangePeriod_4);
    MASTER_REG_HOLD_DATA(733,  uint16,  0,   60,  10,  WO, 1, pSystem->m_usModeChangePeriod_5);
    MASTER_REG_HOLD_DATA(734,  uint16,  0,   60,  10,  WO, 1, pSystem->m_usModeChangePeriod_6);

    MASTER_REG_HOLD_DATA(735,   uint16,  0,   100,  10,  WO, 1, pSystem->m_usModeAdjustTemp_1);
    MASTER_REG_HOLD_DATA(736,   uint16,  0,   100,  10,  WO, 1, pSystem->m_usModeAdjustTemp_2);
    MASTER_REG_HOLD_DATA(737,   uint16,  0,   100,  10,  WO, 1, pSystem->m_usModeAdjustTemp_3);
    MASTER_REG_HOLD_DATA(738,   uint16,  0,   100,  10,  WO, 1, pSystem->m_usModeAdjustTemp_4);
    MASTER_REG_HOLD_DATA(739,  uint16,   0,   100,  10,  WO, 1, pSystem->m_usModeAdjustTemp_5);
    MASTER_REG_HOLD_DATA(740,  uint16,   0,   100,  10,  WO, 1, pSystem->m_usModeAdjustTemp_6);
    MASTER_REG_HOLD_DATA(741,  uint16,  350,  500, 500,  WO, 1, pSystem->m_usCHWOutletHeatTemp);


    MASTER_REG_HOLD_DATA(742,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[0]->m_usStateMask_1);
    MASTER_REG_HOLD_DATA(743,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[0]->m_usStateMask_2);
    MASTER_REG_HOLD_DATA(744,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[1]->m_usStateMask_2);
    MASTER_REG_HOLD_DATA(748,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularAirs[1]->m_usStateMask_1);


    MASTER_REG_HOLD_DATA(917,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularChillers[0]->m_usStateMask_1);
    MASTER_REG_HOLD_DATA(918,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularChillers[0]->m_usStateMask_2);
    MASTER_REG_HOLD_DATA(922,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularChillers[1]->m_usStateMask_1);
    MASTER_REG_HOLD_DATA(923,  uint16,  0,  65535, 0,  WO, 1, pSystem->m_pModularChillers[1]->m_usStateMask_2);

    MASTER_REG_HOLD_DATA(971,  uint16,  0,  7200,   180,  WO, 1, pSystem->m_usExAirFanPlusTime);
    MASTER_REG_HOLD_DATA(972,  uint16,  0,  7200,   180,  WO, 1, pSystem->m_usExAirFanSubTime);

    MASTER_REG_HOLD_DATA(1050,  uint16,  100,  250, 120,  WO, 1, pSystem->m_usChillerCoolInTemp);
    MASTER_REG_HOLD_DATA(1051,  uint16,   50,  200,  70,  WO, 1, pSystem->m_usChillerCoolOutTemp);
    MASTER_REG_HOLD_DATA(1052,  uint16,  300,  450, 450,  WO, 1, pSystem->m_usChillerHeatInTemp);
    MASTER_REG_HOLD_DATA(1053,  uint16,  350,  500, 500,  WO, 1, pSystem->m_usChillerHeatOutTemp);

    MASTER_REG_HOLD_DATA(1054,  uint16,    0,  50,   25,  WO, 1, pSystem->m_usCHCoolPlusSupTempDeviat);
    MASTER_REG_HOLD_DATA(1055,  uint16,    0,  50,   21,  WO, 1, pSystem->m_usCHCoolPlusTempDiff);
    MASTER_REG_HOLD_DATA(1056,  uint16,    0,  50,   25,  WO, 1, pSystem->m_usCHHeatPlusSupTempDeviat);
    MASTER_REG_HOLD_DATA(1057,  uint16,    0,  50,   21,  WO, 1, pSystem->m_usCHHeatPlusTempDiff);

    MASTER_REG_HOLD_DATA(1058,  uint16,    0,  50,   20,  WO, 1, pSystem->m_usCHCoolSubSupTempDeviat);
    MASTER_REG_HOLD_DATA(1059,  uint16,    0,  50,   21,  WO, 1, pSystem->m_usCHCoolSubTempDiff);
    MASTER_REG_HOLD_DATA(1060,  uint16,    0,  50,   20,  WO, 1, pSystem->m_usCHHeatSubSupTempDeviat);
    MASTER_REG_HOLD_DATA(1061,  uint16,    0,  50,   21,  WO, 1, pSystem->m_usCHHeatSubTempDiff);

    MASTER_REG_HOLD_DATA(1062,  uint16,    0, 100,   50,  WO, 1, pSystem->m_usCHWBCoolPlusFreqTempDeviat);
    MASTER_REG_HOLD_DATA(1063,  uint16,    0,  50,   10,  WO, 1, pSystem->m_usCHWBCoolPlusFreqTempDiff_1);
    MASTER_REG_HOLD_DATA(1064,  uint16,    0,  50,    5,  WO, 1, pSystem->m_usCHWBCoolPlusFreqTempDiff_2);
    MASTER_REG_HOLD_DATA(1065,  uint16,    0,  100,  50,  WO, 1, pSystem->m_usCHWBHeatPlusFreqTempDeviat);
    MASTER_REG_HOLD_DATA(1066,  uint16,    0,  50,   10,  WO, 1, pSystem->m_usCHWBHeatPlusFreqTempDiff_1);
    MASTER_REG_HOLD_DATA(1067,  uint16,    0,  50,    5,  WO, 1, pSystem->m_usCHWBHeatPlusFreqTempDiff_2);

    MASTER_REG_HOLD_DATA(1068,  uint16,    0,     50,   10,  WO, 1, pSystem->m_usCHWBCoolSubFreqTempDeviat);
    MASTER_REG_HOLD_DATA(1069,  uint16,    0,     50,   10,  WO, 1, pSystem->m_usCHWBCoolSubFreqTempDiff_1);
    MASTER_REG_HOLD_DATA(1070,  uint16,    0,     50,    5,  WO, 1, pSystem->m_usCHWBCoolSubFreqTempDiff_2);
    MASTER_REG_HOLD_DATA(1071,  uint16,    0,     50,   10,  WO, 1, pSystem->m_usCHWBHeatSubFreqTempDeviat);
    MASTER_REG_HOLD_DATA(1072,  uint16,    0,     50,   10,  WO, 1, pSystem->m_usCHWBHeatSubFreqTempDiff_1);
    MASTER_REG_HOLD_DATA(1073,  uint16,    0,     50,    5,  WO, 1, pSystem->m_usCHWBHeatSubFreqTempDiff_2);
    MASTER_REG_HOLD_DATA(1074,  uint16,    0,  50000,    0,  WO, 1, pSystem->m_usCO2PPM);

    for(uint8_t i = 0; i < pSystem->m_pModularAirs.count(); i++)
    {
        pModularAir = pSystem->m_pModularAirs[i];

        MASTER_REG_HOLD_DATA(1075+i*7,  uint16, 85,    170,  85,  WO, 1, pModularAir->m_eSwitchCmd);
        MASTER_REG_HOLD_DATA(1076+i*7,  uint16,  0,      4,   0,  WO, 1, pModularAir->m_eRunningModeCmd);
        MASTER_REG_HOLD_DATA(1077+i*7,  uint16,  0,      1,   0,  WO, 1, pModularAir->m_eControlMode);
        MASTER_REG_HOLD_DATA(1078+i*7,  uint16,  0,      3,   0,  WO, 1, pModularAir->m_eModularState);
        MASTER_REG_HOLD_DATA(1079+i*7,  uint16,  0,      3,   0,  WO, 1, pModularAir->m_eRunningMode);
        MASTER_REG_HOLD_DATA(1080+i*7,  uint16,  0,  65535,   0,  WO, 1, pModularAir->m_usExitAirVolume);
        MASTER_REG_HOLD_DATA(1081+i*7,  uint16,  0,  65535,   0,  WO, 1, pModularAir->m_usFreAirVolume);

        MASTER_REG_HOLD_DATA(1100+i*24,  uint16,   160,     350,   240,  WO, 1, pModularAir->m_usTempSet);
        MASTER_REG_HOLD_DATA(1101+i*24,  uint16,     0,    1000,   600,  WO, 1, pModularAir->m_usHumiSet);
        MASTER_REG_HOLD_DATA(1102+i*24,  uint16, 10000,   30000, 20000,  WO, 1, pModularAir->m_usCO2Set);

        MASTER_REG_HOLD_DATA(1103+i*24,  uint16,  0,   1000,   0,  WO, 1, pModularAir->m_usExitAirDamperAng);
        MASTER_REG_HOLD_DATA(1104+i*24,  uint16,  0,   1000,   0,  WO, 1, pModularAir->m_usFreAirDamperAng);
        MASTER_REG_HOLD_DATA(1105+i*24,  uint16,  0,   1000,   0,  WO, 1, pModularAir->m_usDivideDamperAng);
        MASTER_REG_HOLD_DATA(1106+i*24,  uint16,  0,   1000,   0,  WO, 1, pModularAir->m_usRetAirDamperAng);
        MASTER_REG_HOLD_DATA(1107+i*24,  uint16,  0,   1000,   0,  WO, 1, pModularAir->m_usCoilerDamperAng);

        MASTER_REG_HOLD_DATA(1108+i*24,  uint16,  0,    500,   0,  WO, 1, pModularAir->m_usSupAirFanFreg);
        MASTER_REG_HOLD_DATA(1109+i*24,  uint16,  0,    500,   0,  WO, 1, pModularAir->m_usExitAirFanFreg);

        MASTER_REG_HOLD_DATA(1110+i*24,  int16,  -400,  700,   0,  WO, 1, pModularAir->m_sSupAirTemp);
        MASTER_REG_HOLD_DATA(1111+i*24,  uint16,  0,   1000,   0,  WO, 1, pModularAir->m_usSupAirHumi);

        MASTER_REG_HOLD_DATA(1112+i*24,  uint16,    0, 50000, 0,  WO, 1, pModularAir->m_pCO2Sensors[0]->m_usCO2ppm);
        MASTER_REG_HOLD_DATA(1113+i*24,  uint16,    0, 50000, 0,  WO, 1, pModularAir->m_pCO2Sensors[1]->m_usCO2ppm);
        MASTER_REG_HOLD_DATA(1114+i*24,   int16, -400,   700, 0,  WO, 1, pModularAir->m_pTempHumiOutSensor->m_sTemp);
        MASTER_REG_HOLD_DATA(1115+i*24,  uint16,    0,  1000, 0,  WO, 1, pModularAir->m_pTempHumiOutSensor->m_usHumi);

        MASTER_REG_HOLD_DATA(1116+i*24,  int16, -400, 700, 0,  WO, 1, pModularAir->m_pTempHumiInSensors[0]->m_sTemp);
        MASTER_REG_HOLD_DATA(1117+i*24,  int16, -400, 700, 0,  WO, 1, pModularAir->m_pTempHumiInSensors[1]->m_sTemp);
        MASTER_REG_HOLD_DATA(1118+i*24,  int16, -400, 700, 0,  WO, 1, pModularAir->m_pTempHumiInSensors[2]->m_sTemp);
        MASTER_REG_HOLD_DATA(1119+i*24,  int16, -400, 700, 0,  WO, 1, pModularAir->m_pTempHumiInSensors[3]->m_sTemp);

        MASTER_REG_HOLD_DATA(1120+i*24,  uint16,  0,  1000, 0,  WO, 1, pModularAir->m_pTempHumiInSensors[0]->m_usHumi);
        MASTER_REG_HOLD_DATA(1121+i*24,  uint16,  0,  1000, 0,  WO, 1, pModularAir->m_pTempHumiInSensors[1]->m_usHumi);
        MASTER_REG_HOLD_DATA(1122+i*24,  uint16,  0,  1000, 0,  WO, 1, pModularAir->m_pTempHumiInSensors[2]->m_usHumi);
        MASTER_REG_HOLD_DATA(1123+i*24,  uint16,  0,  1000, 0,  WO, 1, pModularAir->m_pTempHumiInSensors[3]->m_usHumi);
    }

MASTER_END_DATA_BUF(0, 1171)

    m_sDevCommData.pNext = nullptr;
    m_sDevCommData.usProtocolID = PROTOCOL_TYPE_ID;
    m_sMBDTUDev200.psDevDataInfo = &m_sDevCommData;
    m_sMBDTUDev200.psDevCurData  = &m_sDevCommData;
}

void DTU::initComm()
{
    registDevCommData();
    connect(System::getInstance(), SIGNAL(systemTimeChanged()), this, SLOT(systemTimeChangedSlot()));
}

void DTU::systemTimeChangedSlot()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    ModularChiller *pModularChiller = nullptr;
    ModularAir     *pModularAir = nullptr;

    for(uint8_t i = 0; i < pSystem->m_pModularAirs.count(); i++)
    {
        pModularAir = pSystem->m_pModularAirs[i];
        pModularAir->m_usStateMask_2 = pModularAir->m_pCO2Sensors[0]->m_xError | (pModularAir->m_pCO2Sensors[1]->m_xError << 1) |
                    (pModularAir->m_pTempHumiOutSensor->m_xTempErr << 2) | (pModularAir->m_pTempHumiOutSensor->m_xHumiErr << 3) |
                    (pModularAir->m_pTempHumiInSensors[0]->m_xTempErr << 4) | (pModularAir->m_pTempHumiInSensors[0]->m_xHumiErr << 5) |
                    (pModularAir->m_pTempHumiInSensors[1]->m_xTempErr << 6) | (pModularAir->m_pTempHumiInSensors[1]->m_xHumiErr << 7) |
                    (pModularAir->m_pTempHumiInSensors[2]->m_xTempErr << 8) | (pModularAir->m_pTempHumiInSensors[2]->m_xHumiErr << 9) |
                    (pModularAir->m_pTempHumiInSensors[3]->m_xTempErr << 10) | (pModularAir->m_pTempHumiInSensors[3]->m_xHumiErr << 11);

    }
    pModularChiller = pSystem->m_pModularChillers[0];
    pModularChiller->m_usStateMask_1 = pModularChiller->m_xCommErr | (pModularChiller->m_Modulars[0]->m_xErrorFlag << 1) |
            (pModularChiller->m_Modulars[0]->m_xAlarmFlag << 2) | (pModularChiller->m_Modulars[0]->m_xRunningFlag << 3) |
            (pModularChiller->m_Modulars[0]->m_xCompRunnnig_1 << 4) | (pModularChiller->m_Modulars[0]->m_xCompRunnnig_2 << 5) |
            (pModularChiller->m_Modulars[0]->m_xWaterValve << 6) | (pModularChiller->m_Modulars[1]->m_xErrorFlag << 7) |

            (pModularChiller->m_Modulars[1]->m_xAlarmFlag << 8) | (pModularChiller->m_Modulars[1]->m_xRunningFlag << 9) |
            (pModularChiller->m_Modulars[1]->m_xCompRunnnig_1 << 10) | (pModularChiller->m_Modulars[1]->m_xCompRunnnig_2 << 11) |
            (pModularChiller->m_Modulars[1]->m_xWaterValve << 12) | (pModularChiller->m_Modulars[2]->m_xErrorFlag << 13) |
            (pModularChiller->m_Modulars[2]->m_xAlarmFlag << 14) | (pModularChiller->m_Modulars[2]->m_xRunningFlag << 15);

    pModularChiller->m_usStateMask_2 = pModularChiller->m_Modulars[2]->m_xCompRunnnig_1 | (pModularChiller->m_Modulars[2]->m_xCompRunnnig_2 << 1) |
                                       (pModularChiller->m_Modulars[2]->m_xWaterValve << 2);

    pModularChiller = pSystem->m_pModularChillers[1];
    pModularChiller->m_usStateMask_1 = pModularChiller->m_xCommErr | (pModularChiller->m_Modulars[0]->m_xErrorFlag << 1) |
            (pModularChiller->m_Modulars[0]->m_xAlarmFlag << 2) | (pModularChiller->m_Modulars[0]->m_xRunningFlag << 3) |
            (pModularChiller->m_Modulars[0]->m_xCompRunnnig_1 << 4) | (pModularChiller->m_Modulars[0]->m_xCompRunnnig_2 << 5) |
            (pModularChiller->m_Modulars[0]->m_xWaterValve << 6) | (pModularChiller->m_Modulars[1]->m_xErrorFlag << 7) |

            (pModularChiller->m_Modulars[1]->m_xAlarmFlag << 8) | (pModularChiller->m_Modulars[1]->m_xRunningFlag << 9) |
            (pModularChiller->m_Modulars[1]->m_xCompRunnnig_1 << 10) | (pModularChiller->m_Modulars[1]->m_xCompRunnnig_2 << 11) |
            (pModularChiller->m_Modulars[1]->m_xWaterValve << 12) | (pModularChiller->m_Modulars[2]->m_xErrorFlag << 13) |
            (pModularChiller->m_Modulars[2]->m_xAlarmFlag << 14) | (pModularChiller->m_Modulars[2]->m_xRunningFlag << 15);

    pModularChiller->m_usStateMask_2 = pModularChiller->m_Modulars[2]->m_xCompRunnnig_1 | (pModularChiller->m_Modulars[2]->m_xCompRunnnig_2 << 1) |
            (pModularChiller->m_Modulars[2]->m_xWaterValve << 2) | (pModularChiller->m_Modulars[3]->m_xErrorFlag << 3) |
            (pModularChiller->m_Modulars[3]->m_xAlarmFlag << 4) | (pModularChiller->m_Modulars[3]->m_xRunningFlag << 5) |
            (pModularChiller->m_Modulars[3]->m_xCompRunnnig_1 << 6) | (pModularChiller->m_Modulars[3]->m_xCompRunnnig_2 << 7) |
            (pModularChiller->m_Modulars[3]->m_xWaterValve << 8);

    m_usStateMask_1 = pSystem->m_pWindowFans[0]->m_xSwitchCmd | (pSystem->m_pWindowFans[1]->m_xSwitchCmd << 1);

    m_usStateMask_2 = pSystem->m_pBypassValve->m_xControlMode | (pSystem->m_pBypassValve->m_xErrorFlag << 2);

    m_usStateMask_2 = pSystem->m_pChillerMeter->m_xCommErr | (pSystem->m_pModularAirs[0]->m_sMeter.m_xCommErr << 1) |
            (pSystem->m_pModularAirs[1]->m_sMeter.m_xCommErr << 2) | (pSystem->m_pBumpMeter->m_xCommErr << 4);

}
