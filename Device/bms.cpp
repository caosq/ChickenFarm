#include "bms.h"
#include "system.h"
#include "mbmap.h"

BMS::BMS(QObject *parent) :
    QObject(parent)
{
    m_xCommDataInited = false;
}

void BMS::registCommData()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    ModularChiller *pModularChiller = nullptr;
    ModularAir     *pModularAir = nullptr;
    Modular        *pModular = nullptr;

    ChilledBump    *pChilledBump = nullptr;
    AxialFan       *pAxialFan    = nullptr;
    ButterflyValve *pValve       = nullptr;

SLAVE_PBUF_INDEX_ALLOC

SLAVE_BEGIN_DATA_BUF(m_psRegHoldBuf, m_sBMSCommData.sMBRegHoldTable, m_usRegHoldIndex)

    SLAVE_REG_HOLD_DATA(0,  uint16,  0,  65535,  RO, 1, pSystem->m_usUnitID);
    SLAVE_REG_HOLD_DATA(1,  uint16,  1,  65535,  RO, 1, pSystem->m_usProtocolVer);
    SLAVE_REG_HOLD_DATA(2,  uint16,  0,      3,  RW, 1, pSystem->m_eSystemModeCmd);
    SLAVE_REG_HOLD_DATA(4,  uint16,  0,      7,  RO, 1, pSystem->m_eSystemState);

    SLAVE_REG_HOLD_DATA(11,   int16,    -2,    462, WO, 1, pSystem->m_sChickenGrowDay);
    SLAVE_REG_HOLD_DATA(12,  uint16, 10000,  30000, RW, 1, pSystem->m_usCO2PPMSet);
    SLAVE_REG_HOLD_DATA(13,  uint16,   160,    350, RW, 1, pSystem->m_usTempSet);
    SLAVE_REG_HOLD_DATA(14,  uint16,     0,   1000, RW, 1, pSystem->m_usHumiSet);

    SLAVE_REG_HOLD_DATA(22,  uint16,  0,   450,  RW, 1, pSystem->m_usEnergyTemp);
    SLAVE_REG_HOLD_DATA(23,  uint16,  0,   550,  RW, 1, pSystem->m_usSupAirMaxTemp);
    SLAVE_REG_HOLD_DATA(24,  uint16,  0,   100,  RW, 1, pSystem->m_usTempDeviat);

    SLAVE_REG_HOLD_DATA(27,  uint16,  0,  1000,  RW, 1, pSystem->m_usCoolWaterDamperMinAng_1);
    SLAVE_REG_HOLD_DATA(28,  uint16,  0,  1000,  RW, 1, pSystem->m_usCoolWaterDamperMinAng_2);
    SLAVE_REG_HOLD_DATA(29,  uint16,  0,  1000,  RW, 1, pSystem->m_usHeatWaterDamperMinAng);
    SLAVE_REG_HOLD_DATA(30,  uint16,  0,  1000,  RW, 1, pSystem->m_usExAirDamperMinAng);

    SLAVE_REG_HOLD_DATA(33,  uint16,  0,  10000,  RW, 1, pSystem->m_usCO2AdjustDeviat_Up);
    SLAVE_REG_HOLD_DATA(34,  uint16,  0,  10000,  RW, 1, pSystem->m_usCO2AdjustDeviat_Down);
    SLAVE_REG_HOLD_DATA(35,  uint16,  0,  35000,  RW, 1, pSystem->m_usCO2ExAirDeviat_1);
    SLAVE_REG_HOLD_DATA(36,  uint16,  0,  35000,  RW, 1, pSystem->m_usCO2ExAirDeviat_2);
    SLAVE_REG_HOLD_DATA(37,  uint16,  0,  35000,  RW, 1, pSystem->m_usCO2AdjustThr);
    SLAVE_REG_HOLD_DATA(38,  uint16,  0,  35000,  RW, 1, pSystem->m_usCO2PPMAlarm);

    SLAVE_REG_HOLD_DATA(45,  uint16,  0,  500,   RW, 1, pSystem->m_usExAirFanMinFreq);
    SLAVE_REG_HOLD_DATA(46,  uint16,  0,  500,   RW, 1, pSystem->m_usExAirFanMaxFreq);
    SLAVE_REG_HOLD_DATA(47,  uint16,  0,  500,   RW, 1, pSystem->m_usExAirFanAdjustFreq);
    SLAVE_REG_HOLD_DATA(48,  uint16,  0,  1,     RW, 1, pSystem->m_usExAirFanRatedVol_H);
    SLAVE_REG_HOLD_DATA(49,  uint16,  0,  65535, RW, 1, pSystem->m_usExAirFanRatedVol_L);
    SLAVE_REG_HOLD_DATA(50,  uint16,  0,  7200,  RW, 1, pSystem->m_usExAirFanFreqRunPeriod);
    SLAVE_REG_HOLD_DATA(51,  uint16,  0,  1440,  RW, 1, pSystem->m_usExAirFanIntervalMin);
    SLAVE_REG_HOLD_DATA(52,  uint16,  0,  7200,  RW, 1, pSystem->m_usExAirFanRunTimeMin);
    SLAVE_REG_HOLD_DATA(53,  uint16,  0,  7200,  RW, 1, pSystem->m_usExAirFanPlusTime);
    SLAVE_REG_HOLD_DATA(54,  uint16,  0,  7200,  RW, 1, pSystem->m_usExAirFanFreqAdjustTime);
    SLAVE_REG_HOLD_DATA(55,  uint16,  0,  7200,  RW, 1, pSystem->m_usExAirFanSubTime);
    SLAVE_REG_HOLD_DATA(56,  uint16,  0,   500,  RW, 1, pSystem->m_usExAirFanPlusFreq_1);
    SLAVE_REG_HOLD_DATA(57,  uint16,  0,   500,  RW, 1, pSystem->m_usExAirFanPlusFreq_2);
    SLAVE_REG_HOLD_DATA(58,  uint16,  0,   500,  RW, 1, pSystem->m_usExAirFanSubFreq_1);
    SLAVE_REG_HOLD_DATA(59,  uint16,  0,   500,  RW, 1, pSystem->m_usExAirFanSubFreq_2);
    SLAVE_REG_HOLD_DATA(60,  uint16,  0,    50,  RW, 1, pSystem->m_usExAirFanTempDeviat);
    SLAVE_REG_HOLD_DATA(61,  uint16,  0,   200,  RW, 1, pSystem->m_usExAirFanRatio);

    SLAVE_REG_HOLD_DATA(65,  uint16,  0,  500,  RW, 1, pSystem->m_usCHWBumpMinFreq);
    SLAVE_REG_HOLD_DATA(66,  uint16,  0,  500,  RW, 1, pSystem->m_usCHWBumpMaxFreq);
    SLAVE_REG_HOLD_DATA(67,  uint16,  0,  300,  RW, 1, pSystem->m_usChilledDevsOpenDelay);
    SLAVE_REG_HOLD_DATA(68,  uint16,  0,  300,  RW, 1, pSystem->m_usChilledDevsCloseDelay);
    SLAVE_REG_HOLD_DATA(69,  uint16,  0,  300,  RW, 1, pSystem->m_usCHWBumpCloseDelay);
    SLAVE_REG_HOLD_DATA(70,  uint16,  0,  300,  RW, 1, pSystem->m_usButterflyValveCloseDelay);
    SLAVE_REG_HOLD_DATA(71,  uint16,  0,  500,  RW, 1, pSystem->m_usCHWBumpAdjustPeriod);
    SLAVE_REG_HOLD_DATA(72,  uint16,  0,  500,  RW, 1, pSystem->m_usCHWBumpAdjustFreq_1);
    SLAVE_REG_HOLD_DATA(73,  uint16,  0,  500,  RW, 1, pSystem->m_usCHWBumpAdjustFreq_2);
    SLAVE_REG_HOLD_DATA(74,  uint16,  0,  500,  RW, 1, pSystem->m_usCHWBumpAdjustFreq_3);
    SLAVE_REG_HOLD_DATA(75,  uint16,  0,  500,  RW, 1, pSystem->m_usCHWBumpAdjustFreq_4);

    SLAVE_REG_HOLD_DATA(79,  uint16,  0,   30,  RW, 1, pSystem->m_usChillerPlusPeriod);
    SLAVE_REG_HOLD_DATA(80,  uint16,  0,   30,  RW, 1, pSystem->m_usChillerSubPeriod);
    SLAVE_REG_HOLD_DATA(81,  uint16,  0,   30,  RW, 1, pSystem->m_usChillerPlusRemain);
    SLAVE_REG_HOLD_DATA(82,  uint16,  0,   30,  RW, 1, pSystem->m_usChillerSubRemain);

    SLAVE_REG_HOLD_DATA(85,  uint16,  0,   60,  RW, 1, pSystem->m_usModeChangePeriod_1);
    SLAVE_REG_HOLD_DATA(86,  uint16,  0,   60,  RW, 1, pSystem->m_usModeChangePeriod_2);
    SLAVE_REG_HOLD_DATA(87,  uint16,  0,   60,  RW, 1, pSystem->m_usModeChangePeriod_3);
    SLAVE_REG_HOLD_DATA(88,  uint16,  0,   60,  RW, 1, pSystem->m_usModeChangePeriod_4);
    SLAVE_REG_HOLD_DATA(89,  uint16,  0,   60,  RW, 1, pSystem->m_usModeChangePeriod_5);
    SLAVE_REG_HOLD_DATA(90,  uint16,  0,   60,  RW, 1, pSystem->m_usModeChangePeriod_6);

    SLAVE_REG_HOLD_DATA(95,   uint16,  0,   100,  RW, 1, pSystem->m_usModeAdjustTemp_0);
    SLAVE_REG_HOLD_DATA(96,   uint16,  0,   100,  RW, 1, pSystem->m_usModeAdjustTemp_1);
    SLAVE_REG_HOLD_DATA(97,   uint16,  0,   100,  RW, 1, pSystem->m_usModeAdjustTemp_2);
    SLAVE_REG_HOLD_DATA(98,   uint16,  0,   100,  RW, 1, pSystem->m_usModeAdjustTemp_3);
    SLAVE_REG_HOLD_DATA(99,   uint16,  0,   100,  RW, 1, pSystem->m_usModeAdjustTemp_4);
    SLAVE_REG_HOLD_DATA(100,  uint16,  0,   100,  RW, 1, pSystem->m_usModeAdjustTemp_5);
    SLAVE_REG_HOLD_DATA(101,  uint16,  0,   100,  RW, 1, pSystem->m_usModeAdjustTemp_6);

    SLAVE_REG_HOLD_DATA(106,  uint16,   50,  200,  RW, 1, pSystem->m_usCHWOutletCoolTemp);
    SLAVE_REG_HOLD_DATA(107,  uint16,  350,  500,  RW, 1, pSystem->m_usCHWOutletHeatTemp);
    SLAVE_REG_HOLD_DATA(108,  uint16,   25,   60,  RW, 1, pSystem->m_usCHWTempDiff);
    SLAVE_REG_HOLD_DATA(109,  uint16,  100,  500,  RW, 1, pSystem->m_usCHWPressureDiff);
    SLAVE_REG_HOLD_DATA(110,  uint16,    2,   15,  RW, 1, pSystem->m_usCHWPressureDeviat);
    SLAVE_REG_HOLD_DATA(111,  uint16,   1,    20,  RW, 1, pSystem->m_usCHWBypassRange);
    SLAVE_REG_HOLD_DATA(112,  uint16,  50,   100,  RW, 1, pSystem->m_usCHWBypassMaxAng);
    SLAVE_REG_HOLD_DATA(113,  uint16,   0,    20,  RW, 1, pSystem->m_usCHWBypassMinAng);
    SLAVE_REG_HOLD_DATA(114,  uint16,   0,   300,  RW, 1, pSystem->m_usCHWBypassAdjustTime);
    SLAVE_REG_HOLD_DATA(115,  uint16,   0,   100,  RW, 1, pSystem->m_pBypassValve->m_usAngSet);
    SLAVE_REG_HOLD_DATA(116,  uint16,   0,   100,  RW, 1, pSystem->m_pBypassValve->m_usAng);

    SLAVE_REG_HOLD_DATA(120,  uint16,  100,  250, RW, 1, pSystem->m_usChillerCoolInTemp);
    SLAVE_REG_HOLD_DATA(121,  uint16,   50,  200, RW, 1, pSystem->m_usChillerCoolOutTemp);
    SLAVE_REG_HOLD_DATA(122,  uint16,  300,  450, RW, 1, pSystem->m_usChillerHeatInTemp);
    SLAVE_REG_HOLD_DATA(123,  uint16,  350,  500, RW, 1, pSystem->m_usChillerHeatOutTemp);

    SLAVE_REG_HOLD_DATA(128,  uint16,    0,  50,  RW, 1, pSystem->m_usCHCoolPlusSupTempDeviat);
    SLAVE_REG_HOLD_DATA(129,  uint16,    0,  50,  RW, 1, pSystem->m_usCHCoolPlusTempDiff);
    SLAVE_REG_HOLD_DATA(130,  uint16,    0,  50,  RW, 1, pSystem->m_usCHHeatPlusSupTempDeviat);
    SLAVE_REG_HOLD_DATA(131,  uint16,    0,  50,  RW, 1, pSystem->m_usCHHeatPlusTempDiff);

    SLAVE_REG_HOLD_DATA(132,  uint16,    0,  50,  RW, 1, pSystem->m_usCHCoolSubSupTempDeviat);
    SLAVE_REG_HOLD_DATA(133,  uint16,    0,  50,  RW, 1, pSystem->m_usCHCoolSubTempDiff);
    SLAVE_REG_HOLD_DATA(134,  uint16,    0,  50,  RW, 1, pSystem->m_usCHHeatSubSupTempDeviat);
    SLAVE_REG_HOLD_DATA(135,  uint16,    0,  50,  RW, 1, pSystem->m_usCHHeatSubTempDiff);

    SLAVE_REG_HOLD_DATA(140,  uint16,    0, 100,  RW, 1, pSystem->m_usCHWBCoolPlusFreqTempDeviat);
    SLAVE_REG_HOLD_DATA(141,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBCoolPlusFreqTempDiff_1);
    SLAVE_REG_HOLD_DATA(142,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBCoolPlusFreqTempDiff_2);
    SLAVE_REG_HOLD_DATA(143,  uint16,    0, 100,  RW, 1, pSystem->m_usCHWBHeatPlusFreqTempDeviat);
    SLAVE_REG_HOLD_DATA(144,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBHeatPlusFreqTempDiff_1);
    SLAVE_REG_HOLD_DATA(145,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBHeatPlusFreqTempDiff_2);

    SLAVE_REG_HOLD_DATA(146,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBCoolSubFreqTempDeviat);
    SLAVE_REG_HOLD_DATA(147,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBCoolSubFreqTempDiff_1);
    SLAVE_REG_HOLD_DATA(148,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBCoolSubFreqTempDiff_2);
    SLAVE_REG_HOLD_DATA(149,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBHeatSubFreqTempDeviat);
    SLAVE_REG_HOLD_DATA(150,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBHeatSubFreqTempDiff_1);
    SLAVE_REG_HOLD_DATA(151,  uint16,    0,  50,  RW, 1, pSystem->m_usCHWBHeatSubFreqTempDiff_2);

    SLAVE_REG_HOLD_DATA(157,  uint16,    0,  65535,  RW, 1, pSystem->m_usTotalFreAir_H);
    SLAVE_REG_HOLD_DATA(158,  uint16,    0,  65535,  RW, 1, pSystem->m_usTotalFreAir_L);
    SLAVE_REG_HOLD_DATA(159,  uint16,    0,  50000,  RW, 1, pSystem->m_usCO2PPM);
    SLAVE_REG_HOLD_DATA(160,  int16,  -400,    700,  RW, 1, pSystem->m_sAmbientIn_T);
    SLAVE_REG_HOLD_DATA(161,  int16,  -400,    700,  RW, 1, pSystem->m_sAmbientOut_T);
    SLAVE_REG_HOLD_DATA(162,  uint16,    0,   1000,  RW, 1, pSystem->m_usAmbientIn_H);
    SLAVE_REG_HOLD_DATA(163,  uint16,    0,   1000,  RW, 1, pSystem->m_usAmbientOut_H);

    SLAVE_REG_HOLD_DATA(167,  int16,  -200,   1000,  RW, 1, pSystem->m_pCHWTempSensors[0]->m_sTemp);
    SLAVE_REG_HOLD_DATA(168,  int16,  -200,   1000,  RW, 1, pSystem->m_pCHWTempSensors[1]->m_sTemp);
    SLAVE_REG_HOLD_DATA(169,  uint16,    0,  16000,  RW, 1, pSystem->m_pCHWPressureSensors[0]->m_usPressure);
    SLAVE_REG_HOLD_DATA(170,  uint16,    0,  16000,  RW, 1, pSystem->m_pCHWPressureSensors[1]->m_usPressure);

    for(uint8_t i = 0; i < pSystem->m_pModularAirs.count(); i++)
    {
        pModularAir = pSystem->m_pModularAirs[i];

        SLAVE_REG_HOLD_DATA(183+i*59,  uint16, 85,  170, RW, 1, pModularAir->m_eSwitchCmd);
        SLAVE_REG_HOLD_DATA(184+i*59,  uint16,  0,    4, RW, 1, pModularAir->m_eRunningModeCmd);
        SLAVE_REG_HOLD_DATA(185+i*59,  uint16,  0,    1, RO, 1, pModularAir->m_eControlMode);
        SLAVE_REG_HOLD_DATA(186+i*59,  uint16,  0,    3, RO, 1, pModularAir->m_eModularState);
        SLAVE_REG_HOLD_DATA(187+i*59,  uint16,  0,    3, RO, 1, pModularAir->m_eRunningMode);

        SLAVE_REG_HOLD_DATA(190+i*59,  uint16,   160,     350, RW, 1, pModularAir->m_usTempSet);
        SLAVE_REG_HOLD_DATA(191+i*59,  uint16,     0,    1000, RW, 1, pModularAir->m_usHumiSet);
        SLAVE_REG_HOLD_DATA(192+i*59,  uint16, 10000,   30000, RW, 1, pModularAir->m_usCO2Set);

        SLAVE_REG_HOLD_DATA(193+i*59,  uint16,  0,   1000, RO, 1, pModularAir->m_usExitAirDamperAng);
        SLAVE_REG_HOLD_DATA(194+i*59,  uint16,  0,   1000, RO, 1, pModularAir->m_usFreAirDamperAng);
        SLAVE_REG_HOLD_DATA(195+i*59,  uint16,  0,   1000, RO, 1, pModularAir->m_usDivideDamperAng);
        SLAVE_REG_HOLD_DATA(196+i*59,  uint16,  0,   1000, RO, 1, pModularAir->m_usRetAirDamperAng);
        SLAVE_REG_HOLD_DATA(197+i*59,  uint16,  0,   1000, RO, 1, pModularAir->m_usCoilerDamperAng);

        SLAVE_REG_HOLD_DATA(198+i*59,  uint16,  0,    500, RO, 1, pModularAir->m_usSupAirFanFreg);
        SLAVE_REG_HOLD_DATA(199+i*59,  uint16,  0,    500, RO, 1, pModularAir->m_usExitAirFanFreg);

        SLAVE_REG_HOLD_DATA(200+i*59,  int16,  -400,  700, RO, 1, pModularAir->m_sSupAirTemp);
        SLAVE_REG_HOLD_DATA(201+i*59,  uint16,  0,   1000, RO, 1, pModularAir->m_usSupAirHumi);
        SLAVE_REG_HOLD_DATA(202+i*59,  uint16,  0,  65535, RO, 1, pModularAir->m_usExitAirVolume);
        SLAVE_REG_HOLD_DATA(203+i*59,  uint16,  0,  65535, RO, 1, pModularAir->m_usFreAirVolume);

        SLAVE_REG_HOLD_DATA(206+i*59,  uint16,    0, 50000, RO, 1, pModularAir->m_pCO2Sensors[0]->m_usCO2ppm);
        SLAVE_REG_HOLD_DATA(207+i*59,  uint16,    0, 50000, RO, 1, pModularAir->m_pCO2Sensors[1]->m_usCO2ppm);
        SLAVE_REG_HOLD_DATA(211+i*59,  int16,  -400,   700, RO, 1, pModularAir->m_pTempHumiOutSensor->m_sTemp);
        SLAVE_REG_HOLD_DATA(212+i*59,  uint16,    0,  1000, RO, 1, pModularAir->m_pTempHumiOutSensor->m_usHumi);

        SLAVE_REG_HOLD_DATA(216+i*59,  int16, -400, 700, RO, 1, pModularAir->m_pTempHumiInSensors[0]->m_sTemp);
        SLAVE_REG_HOLD_DATA(217+i*59,  int16, -400, 700, RO, 1, pModularAir->m_pTempHumiInSensors[1]->m_sTemp);
        SLAVE_REG_HOLD_DATA(218+i*59,  int16, -400, 700, RO, 1, pModularAir->m_pTempHumiInSensors[2]->m_sTemp);
        SLAVE_REG_HOLD_DATA(219+i*59,  int16, -400, 700, RO, 1, pModularAir->m_pTempHumiInSensors[3]->m_sTemp);

        SLAVE_REG_HOLD_DATA(223+i*59,  uint16,  0,  1000,  RO, 1, pModularAir->m_pTempHumiInSensors[0]->m_usHumi);
        SLAVE_REG_HOLD_DATA(224+i*59,  uint16,  0,  1000,  RO, 1, pModularAir->m_pTempHumiInSensors[1]->m_usHumi);
        SLAVE_REG_HOLD_DATA(225+i*59,  uint16,  0,  1000,  RO, 1, pModularAir->m_pTempHumiInSensors[2]->m_usHumi);
        SLAVE_REG_HOLD_DATA(226+i*59,  uint16,  0,  1000,  RO, 1, pModularAir->m_pTempHumiInSensors[3]->m_usHumi);

        SLAVE_REG_HOLD_DATA(233+i*59,  uint16,  0,  65535,  RO, 1, pModularAir->m_sMeter.m_usPower);
        SLAVE_REG_HOLD_DATA(234+i*59,  uint16,  0,  65535,  RO, 1, pModularAir->m_sMeter.m_usTotalEnergy_H);
        SLAVE_REG_HOLD_DATA(235+i*59,  uint16,  0,  65535,  RO, 1, pModularAir->m_sMeter.m_usTotalEnergy_L);
    }

    pModularChiller = pSystem->m_pModularChillers[0];

    SLAVE_REG_HOLD_DATA(301, uint16,   85,  170, RW, 1, pModularChiller->m_eSwitchCmd);
    SLAVE_REG_HOLD_DATA(302, uint16,    1,    3, RW, 1, pModularChiller->m_eRunningModeCmd);
    SLAVE_REG_HOLD_DATA(305, int16,  -300, 1400, RO, 1, pModularChiller->m_sRetWaterTemp);

    SLAVE_REG_HOLD_DATA(306,  uint16,  100,  250,  RW, 1, pModularChiller->m_usChillerCoolInTemp);
    SLAVE_REG_HOLD_DATA(307,  uint16,   50,  200,  RW, 1, pModularChiller->m_usChillerCoolOutTemp);
    SLAVE_REG_HOLD_DATA(308,  uint16,  300,  450,  RW, 1, pModularChiller->m_usChillerHeatInTemp);
    SLAVE_REG_HOLD_DATA(309,  uint16,  350,  500,  RW, 1, pModularChiller->m_usChillerHeatOutTemp);

    SLAVE_REG_HOLD_DATA(310,  uint16,  0,  4, 0,  RO,  pModularChiller->m_Modulars[0]->m_eModularState);
    SLAVE_REG_HOLD_DATA(311,  uint16,  0,  4, 0,  RO,  pModularChiller->m_Modulars[1]->m_eModularState);
    SLAVE_REG_HOLD_DATA(312,  uint16,  0,  4, 0,  RO,  pModularChiller->m_Modulars[2]->m_eModularState);

    SLAVE_REG_HOLD_DATA(313,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[0]->m_sInputWaterTemp);
    SLAVE_REG_HOLD_DATA(314,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[0]->m_sOutputWaterTemp);
    SLAVE_REG_HOLD_DATA(315,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[1]->m_sInputWaterTemp);
    SLAVE_REG_HOLD_DATA(316,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[1]->m_sOutputWaterTemp);
    SLAVE_REG_HOLD_DATA(317,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[2]->m_sInputWaterTemp);
    SLAVE_REG_HOLD_DATA(318,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[2]->m_sOutputWaterTemp);

    pModularChiller = pSystem->m_pModularChillers[1];

    SLAVE_REG_HOLD_DATA(326, uint16,   85,  170, RW, 1,  pModularChiller->m_eSwitchCmd);
    SLAVE_REG_HOLD_DATA(327, uint16,    1,    3, RW, 1,  pModularChiller->m_eRunningModeCmd);
    SLAVE_REG_HOLD_DATA(330, int16,  -300, 1400, RO, 1,  pModularChiller->m_sRetWaterTemp);

    SLAVE_REG_HOLD_DATA(331,  uint16,  100,  250,  RW, 1, pModularChiller->m_usChillerCoolInTemp);
    SLAVE_REG_HOLD_DATA(332,  uint16,   50,  200,  RW, 1, pModularChiller->m_usChillerCoolOutTemp);
    SLAVE_REG_HOLD_DATA(333,  uint16,  300,  450,  RW, 1, pModularChiller->m_usChillerHeatInTemp);
    SLAVE_REG_HOLD_DATA(334,  uint16,  350,  500,  RW, 1, pModularChiller->m_usChillerHeatOutTemp);

    SLAVE_REG_HOLD_DATA(335,  uint16,  0,  4,  RO, 1, pModularChiller->m_Modulars[0]->m_eModularState);
    SLAVE_REG_HOLD_DATA(336,  uint16,  0,  4,  RO, 1, pModularChiller->m_Modulars[1]->m_eModularState);
    SLAVE_REG_HOLD_DATA(337,  uint16,  0,  4,  RO, 1, pModularChiller->m_Modulars[2]->m_eModularState);
    SLAVE_REG_HOLD_DATA(338,  uint16,  0,  4,  RO, 1, pModularChiller->m_Modulars[3]->m_eModularState);

    SLAVE_REG_HOLD_DATA(339,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[0]->m_sInputWaterTemp);
    SLAVE_REG_HOLD_DATA(340,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[0]->m_sOutputWaterTemp);
    SLAVE_REG_HOLD_DATA(341,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[1]->m_sInputWaterTemp);
    SLAVE_REG_HOLD_DATA(342,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[1]->m_sOutputWaterTemp);
    SLAVE_REG_HOLD_DATA(343,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[2]->m_sInputWaterTemp);
    SLAVE_REG_HOLD_DATA(344,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[2]->m_sOutputWaterTemp);
    SLAVE_REG_HOLD_DATA(345,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[3]->m_sInputWaterTemp);
    SLAVE_REG_HOLD_DATA(346,  int16,  -300,  700,  RO, 1, pModularChiller->m_Modulars[3]->m_sOutputWaterTemp);

    for(uint8_t i = 0; i < pSystem->m_pAxialFans.count(); i++)
    {
        pAxialFan = pSystem->m_pAxialFans[i];
        SLAVE_REG_HOLD_DATA(351+i*6,  uint16,  0,  500, RW, 1, pAxialFan->m_usFreqSet);
        SLAVE_REG_HOLD_DATA(352+i*6,  uint16,  0,  500, RO, 1, pAxialFan->m_usFreq);
    }
    for(uint8_t i = 0; i < pSystem->m_pChilledBumps.count(); i++)
    {
        pChilledBump = pSystem->m_pChilledBumps[i];
        SLAVE_REG_HOLD_DATA(375+i*6,  uint16,  0,  500, RW, 1, pChilledBump->m_usFreqSet);
        SLAVE_REG_HOLD_DATA(376+i*6,  uint16,  0,  500, RO, 1, pChilledBump->m_usFreq);
    }
    SLAVE_REG_HOLD_DATA(393,  uint16, 0, 100, RW, 1, pSystem->m_pWindowFans[0]->m_usAngSet);
    SLAVE_REG_HOLD_DATA(394,  uint16, 0, 100, RW, 1, pSystem->m_pWindowFans[1]->m_usAngSet);
    SLAVE_REG_HOLD_DATA(395,  uint16, 0, 100, RO, 1, pSystem->m_pWindowFans[0]->m_usAng);
    SLAVE_REG_HOLD_DATA(396,  uint16, 0, 100, RO, 1, pSystem->m_pWindowFans[1]->m_usAng);

    SLAVE_REG_HOLD_DATA(399,  uint16, 1970, 9999,  WO, 1, pSystem->m_usSysYear);
    SLAVE_REG_HOLD_DATA(400,  uint16,    1,   12,  WO, 1, pSystem->m_usSysMon);
    SLAVE_REG_HOLD_DATA(401,  uint16,    1,   31,  WO, 1, pSystem->m_usSysDay);
    SLAVE_REG_HOLD_DATA(403,  uint16,    0,   23,  WO, 1, pSystem->m_usSysHour);
    SLAVE_REG_HOLD_DATA(404,  uint16,    0,   59,  WO, 1, pSystem->m_usSysMin);
    //SLAVE_REG_HOLD_DATA(405,  uint16,    0,   59,    0,  WO, 1, pSystem->m_usSysSec);

    SLAVE_REG_HOLD_DATA(410,  uint16,  0,  65535,  RO, 1, pSystem->m_pBumpMeter->m_usPower);
    SLAVE_REG_HOLD_DATA(411,  uint16,  0,  65535,  RO, 1, pSystem->m_pBumpMeter->m_usTotalEnergy_H);
    SLAVE_REG_HOLD_DATA(412,  uint16,  0,  65535,  RO, 1, pSystem->m_pBumpMeter->m_usTotalEnergy_L);

    SLAVE_REG_HOLD_DATA(417,  uint16,  0,  65535,  RO, 1, pSystem->m_pChillerMeter->m_usPower);
    SLAVE_REG_HOLD_DATA(418,  uint16,  0,  65535,  RO, 1, pSystem->m_pChillerMeter->m_usTotalEnergy_H);
    SLAVE_REG_HOLD_DATA(419,  uint16,  0,  65535,  RO, 1, pSystem->m_pChillerMeter->m_usTotalEnergy_L);

SLAVE_END_DATA_BUF(0, 419)

SLAVE_BEGIN_DATA_BUF(m_psBitCoilBuf, m_sBMSCommData.sMBCoilTable, m_usBitCoilIndex)

    SLAVE_COIL_BIT_DATA(4, RW, pSystem->m_xAlarmEnable);
    SLAVE_COIL_BIT_DATA(5, RW, pSystem->m_xAlarmClean);

    for(uint8_t i = 0; i < pSystem->m_pModularAirs.count(); i++)
    {
        pModularAir = pSystem->m_pModularAirs[i];
        SLAVE_COIL_BIT_DATA(44+i*40, RO, pModularAir->m_xAlarmFlag);
        SLAVE_COIL_BIT_DATA(45+i*40, RO, pModularAir->m_xCommErr);
        SLAVE_COIL_BIT_DATA(46+i*40, RO, pModularAir->m_xSupAirFan);
        SLAVE_COIL_BIT_DATA(47+i*40, RO, pModularAir->m_xExitAirFan);
        SLAVE_COIL_BIT_DATA(48+i*40, RO, pModularAir->m_xWetMode);
        SLAVE_COIL_BIT_DATA(49+i*40, RO, pModularAir->m_xRecycleMode);
        SLAVE_COIL_BIT_DATA(50+i*40, RO, pModularAir->m_xExitAirSenErr);
        SLAVE_COIL_BIT_DATA(51+i*40, RO, pModularAir->m_xFreAirSenErr);

        SLAVE_COIL_BIT_DATA(56+i*40, RO, pModularAir->m_pCO2Sensors[0]->m_xError);
        SLAVE_COIL_BIT_DATA(57+i*40, RO, pModularAir->m_pCO2Sensors[1]->m_xError);

        SLAVE_COIL_BIT_DATA(58+i*40, RO, pModularAir->m_pTempHumiOutSensor->m_xTempErr);
        SLAVE_COIL_BIT_DATA(59+i*40, RO, pModularAir->m_pTempHumiOutSensor->m_xHumiErr);

        SLAVE_COIL_BIT_DATA(60+i*40, RO, pModularAir->m_pTempHumiInSensors[0]->m_xTempErr);
        SLAVE_COIL_BIT_DATA(61+i*40, RO, pModularAir->m_pTempHumiInSensors[1]->m_xTempErr);
        SLAVE_COIL_BIT_DATA(62+i*40, RO, pModularAir->m_pTempHumiInSensors[2]->m_xTempErr);
        SLAVE_COIL_BIT_DATA(63+i*40, RO, pModularAir->m_pTempHumiInSensors[3]->m_xTempErr);

        SLAVE_COIL_BIT_DATA(64+i*40, RO, pModularAir->m_pTempHumiInSensors[0]->m_xHumiErr);
        SLAVE_COIL_BIT_DATA(65+i*40, RO, pModularAir->m_pTempHumiInSensors[1]->m_xHumiErr);
        SLAVE_COIL_BIT_DATA(66+i*40, RO, pModularAir->m_pTempHumiInSensors[2]->m_xHumiErr);
        SLAVE_COIL_BIT_DATA(67+i*40, RO, pModularAir->m_pTempHumiInSensors[3]->m_xHumiErr);
    }

    pModularChiller = pSystem->m_pModularChillers[0];

    SLAVE_COIL_BIT_DATA(121, RO, pModularChiller->m_xCommErr);
    for(uint8_t j = 0; j < pModularChiller->m_Modulars.count(); j++)
    {
        pModular = pModularChiller->m_Modulars[j];
        SLAVE_COIL_BIT_DATA(122+j*6, RO, pModular->m_xErrorFlag);
        SLAVE_COIL_BIT_DATA(123+j*6, RO, pModular->m_xAlarmFlag);
        SLAVE_COIL_BIT_DATA(124+j*6, RO, pModular->m_xRunningFlag);
        SLAVE_COIL_BIT_DATA(125+j*6, RO, pModular->m_xCompRunnnig_1);
        SLAVE_COIL_BIT_DATA(126+j*6, RO, pModular->m_xCompRunnnig_2);
        SLAVE_COIL_BIT_DATA(127+j*6, RO, pModular->m_xWaterValve);
    }

    pModularChiller = pSystem->m_pModularChillers[1];

    SLAVE_COIL_BIT_DATA(145, RO, pModularChiller->m_xCommErr);
    for(uint8_t j = 0; j < pModularChiller->m_Modulars.count(); j++)
    {
        pModular = pModularChiller->m_Modulars[j];
        SLAVE_COIL_BIT_DATA(146+j*6, RO, pModular->m_xErrorFlag);
        SLAVE_COIL_BIT_DATA(147+j*6, RO, pModular->m_xAlarmFlag);
        SLAVE_COIL_BIT_DATA(148+j*6, RO, pModular->m_xRunningFlag);
        SLAVE_COIL_BIT_DATA(149+j*6, RO, pModular->m_xCompRunnnig_1);
        SLAVE_COIL_BIT_DATA(150+j*6, RO, pModular->m_xCompRunnnig_2);
        SLAVE_COIL_BIT_DATA(151+j*6, RO, pModular->m_xWaterValve);
    }

    for(uint8_t i = 0; i < pSystem->m_pChilledBumps.count(); i++)
    {
        pChilledBump = pSystem->m_pChilledBumps[i];
        SLAVE_COIL_BIT_DATA(176+i*8, RW, pChilledBump->m_xSwitchCmd);
        SLAVE_COIL_BIT_DATA(177+i*8, RW, pChilledBump->m_xErrClean);
        SLAVE_COIL_BIT_DATA(178+i*8, RO, pChilledBump->m_xRunningFlag);
        SLAVE_COIL_BIT_DATA(179+i*8, RO, pChilledBump->m_xControlFlag);
        SLAVE_COIL_BIT_DATA(180+i*8, RO, pChilledBump->m_xErrorFlag);
        SLAVE_COIL_BIT_DATA(181+i*8, RO, pChilledBump->m_xRemote);
    }

    for(uint8_t i = 0; i < pSystem->m_pAxialFans.count(); i++)
    {
        pAxialFan = pSystem->m_pAxialFans[i];
        SLAVE_COIL_BIT_DATA(200+i*8, RW, pAxialFan->m_xSwitchCmd);
        SLAVE_COIL_BIT_DATA(201+i*8, RW, pAxialFan->m_xErrClean);
        SLAVE_COIL_BIT_DATA(202+i*8, RO, pAxialFan->m_xRunningFlag);
        SLAVE_COIL_BIT_DATA(203+i*8, RO, pAxialFan->m_xControlFlag);
        SLAVE_COIL_BIT_DATA(204+i*8, RO, pAxialFan->m_xErrorFlag);
        SLAVE_COIL_BIT_DATA(205+i*8, RO, pAxialFan->m_xRemote);
    }

    for(uint8_t i = 0; i < pSystem->m_pButterflyValves.count(); i++)
    {
        pValve = pSystem->m_pButterflyValves[i];
        SLAVE_COIL_BIT_DATA(232+i*8, RW, pValve->m_xSwitchCmd);
        SLAVE_COIL_BIT_DATA(233+i*8, RW, pValve->m_xErrClean);
        SLAVE_COIL_BIT_DATA(234+i*8, RO, pValve->m_xOpened);
        SLAVE_COIL_BIT_DATA(235+i*8, RO, pValve->m_xClosed);
        SLAVE_COIL_BIT_DATA(236+i*8, RO, pValve->m_xRemote);
        SLAVE_COIL_BIT_DATA(237+i*8, RO, pValve->m_xErrorFlag);
    }

    SLAVE_COIL_BIT_DATA(264, RW, pSystem->m_pWindowFans[0]->m_xSwitchCmd);
    SLAVE_COIL_BIT_DATA(265, RW, pSystem->m_pWindowFans[1]->m_xSwitchCmd);

    SLAVE_COIL_BIT_DATA(272, RW, pSystem->m_pBypassValve->m_xControlMode);
    SLAVE_COIL_BIT_DATA(274, RO, pSystem->m_pBypassValve->m_xErrorFlag);

    SLAVE_COIL_BIT_DATA(276, RO, pSystem->m_pChillerMeter->m_xCommErr);
    SLAVE_COIL_BIT_DATA(277, RO, pSystem->m_pModularAirs[0]->m_sMeter.m_xCommErr);
    SLAVE_COIL_BIT_DATA(278, RO, pSystem->m_pModularAirs[1]->m_sMeter.m_xCommErr);
    SLAVE_COIL_BIT_DATA(279, RO, pSystem->m_pBumpMeter->m_xCommErr);

    SLAVE_COIL_BIT_DATA(280, RO, pSystem->m_pCHWPressureSensors[0]->m_xError);
    SLAVE_COIL_BIT_DATA(281, RO, pSystem->m_pCHWPressureSensors[1]->m_xError);
    SLAVE_COIL_BIT_DATA(282, RO, pSystem->m_pCHWTempSensors[0]->m_xError);
    SLAVE_COIL_BIT_DATA(283, RO, pSystem->m_pCHWTempSensors[1]->m_xError);

SLAVE_END_DATA_BUF(0, 290)

    m_sBMSCommData.pRegInIndex = m_usRegHoldIndex;
    m_sBMSCommData.pBitCoilIndex = m_usBitCoilIndex;
    m_sBMSCommData.pxSlaveDataMapIndex = nullptr;
}

void BMS::initComm()
{
    if(!m_xCommDataInited)
    {
        registCommData();
        m_xCommDataInited = true;
    }
}

