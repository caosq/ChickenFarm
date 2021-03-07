#include "controller.h"
#include "system.h"
#include "mbmap_m.h"
#include "messagebox.h"

#define PROTOCOL_TYPE_ID   0x302A+10
#define COMM_ERR_DELAY_S   30
#define OFFLINE_CONTAIN_S  60

Controller::Controller(QObject *parent) :
     QObject(parent)
{
    m_sMBSlaveDev.usProtocolID = 0;
    m_sMBSlaveDev.xDevOnTimeout = 0;
    m_sMBSlaveDev.xSynchronized = 0;
    m_sMBSlaveDev.ucOfflineTimes = 0;
    m_sMBSlaveDev.xStateTestRequest = 0;
    m_sMBSlaveDev.xDataReady = 0;
    m_sMBSlaveDev.xOnLine = 0;
    m_sMBSlaveDev.psMBMasterInfo = nullptr;
    m_sMBSlaveDev.psDevDataInfo = nullptr;
    m_sMBSlaveDev.psDevCurData = nullptr;
    m_sMBSlaveDev.pLast = nullptr;
    m_sMBSlaveDev.pNext = nullptr;

    m_DelayTimer.setSingleShot(true);
    connect(&m_DelayTimer, SIGNAL(timeout()), this, SLOT(delayTimeOutSlot()));
}

void Controller::delayTimeOutSlot()
{
    if(m_xOffline && m_xCommErr)
    {
        emit offlineChanged(m_xOffline);
    }
    if(m_sMBSlaveDev.xOnLine)
    {
        m_xCommErr = false;
        m_xOffline = false;
    }
    else
    {
        m_xCommErr = true;
        m_xOffline = true;
        m_DelayTimer.start(OFFLINE_CONTAIN_S*1000);
    }
}

void Controller::initComm(uint8_t ucDevAddr)
{
    registDevCommData();

    m_sMBSlaveDev.ucDevAddr = ucDevAddr;
    m_pCommErrMonitor = DataMonitor::monitorRegist(&m_sMBSlaveDev.xOnLine, Monitor::DataType::Boolean);
    m_pSyncMonitor = DataMonitor::monitorRegist(&m_sMBSlaveDev.xSynchronized, Monitor::DataType::Boolean);

    connect(m_pCommErrMonitor, SIGNAL(valChanged(Monitor*)), this, SLOT(dataChanged(Monitor*)));
    connect(m_pSyncMonitor, SIGNAL(valChanged(Monitor*)), this, SLOT(syncChangedSlot(Monitor*)));
}

void Controller::dataChanged(Monitor*)
{
     if(m_sMBSlaveDev.xOnLine)
     {
         m_xCommErr = false;
         m_xOffline = false;
         emit syncChanged(false);
     }
     else
     {
         m_xCommErr = true;
         m_xOffline = true;
         m_DelayTimer.start(OFFLINE_CONTAIN_S*1000);
     }
}

void Controller::syncChangedSlot(Monitor*)
{
     emit syncChanged(m_sMBSlaveDev.xSynchronized);
}

void Controller::registDevCommData()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    ModularChiller *pModularChiller = nullptr;
    ModularAir     *pModularAir = nullptr;
    Modular        *pModular = nullptr;

    ChilledBump    *pChilledBump = nullptr;
    AxialFan       *pAxialFan    = nullptr;
    ButterflyValve *pValve       = nullptr;
    WindowFan      *psWindowFan  = nullptr;

MASTER_PBUF_INDEX_ALLOC

MASTER_TEST_CMD_INIT(m_sDevCommData.sMBDevCmdTable, 0, READ_REG_HOLD, 0x302A, FALSE)

MASTER_BEGIN_DATA_BUF(m_psRegHoldBuf, m_sDevCommData.sMBRegHoldTable, m_usRegHoldIndex)

    MASTER_REG_HOLD_DATA(0,  uint16,  0,  65535,  0x302A, RO, 1, pSystem->m_usUnitID);
    MASTER_REG_HOLD_DATA(1,  uint16,  1,  65535,  10,     RO, 1, pSystem->m_usProtocolVer);
    MASTER_REG_HOLD_DATA(2,  uint16,  0,      3,  1,      RW, 1, pSystem->m_eSystemModeCmd);
    MASTER_REG_HOLD_DATA(4,  uint16,  0,      7,  0,      RO, 1, pSystem->m_eSystemState);

    MASTER_REG_HOLD_DATA(11,   int16,    -2,    462,      0,  RW, 1, pSystem->m_sChickenGrowDay);
    MASTER_REG_HOLD_DATA(12,  uint16,  1000,  30000,  20000,  RW, 1, pSystem->m_usCO2PPMSet);
    MASTER_REG_HOLD_DATA(13,  uint16,   160,    350,    240,  RW, 1, pSystem->m_usTempSet);
    MASTER_REG_HOLD_DATA(14,  uint16,     0,   1000,    500,  RW, 1, pSystem->m_usHumiSet);

    MASTER_REG_HOLD_DATA(22,  uint16,  0,   450,  250,  RW, 1, pSystem->m_usEnergyTemp);
    MASTER_REG_HOLD_DATA(23,  uint16,  0,   550,  450,  RW, 1, pSystem->m_usSupAirMaxTemp);
    MASTER_REG_HOLD_DATA(24,  uint16,  0,   100,    5,  RW, 1, pSystem->m_usTempDeviat);

    MASTER_REG_HOLD_DATA(27,  uint16,  0,  1000,  350,  RW, 1, pSystem->m_usCoolWaterDamperMinAng_1);
    MASTER_REG_HOLD_DATA(28,  uint16,  0,  1000,   50,  RW, 1, pSystem->m_usCoolWaterDamperMinAng_2);
    MASTER_REG_HOLD_DATA(29,  uint16,  0,  1000,    0,  RW, 1, pSystem->m_usHeatWaterDamperMinAng);
    MASTER_REG_HOLD_DATA(30,  uint16,  0,  1000,  100,  RW, 1, pSystem->m_usExAirDamperMinAng);

    MASTER_REG_HOLD_DATA(33,  uint16,  0,  10000,  5000,  RW, 1, pSystem->m_usCO2AdjustDeviat_Up);
    MASTER_REG_HOLD_DATA(34,  uint16,  0,  10000,  5000,  RW, 1, pSystem->m_usCO2AdjustDeviat_Down);
    MASTER_REG_HOLD_DATA(35,  uint16,  0,  35000,  2000,  RW, 1, pSystem->m_usCO2ExAirDeviat_1);
    MASTER_REG_HOLD_DATA(36,  uint16,  0,  35000,  2000,  RW, 1, pSystem->m_usCO2ExAirDeviat_2);
    MASTER_REG_HOLD_DATA(37,  uint16,  0,  35000, 27000,  RW, 1, pSystem->m_usCO2AdjustThr);
    MASTER_REG_HOLD_DATA(38,  uint16,  0,  35000, 30000,  RW, 1, pSystem->m_usCO2PPMAlarm);

    MASTER_REG_HOLD_DATA(45,  uint16,  0,  500,      350,  RW, 1, pSystem->m_usExAirFanMinFreq);
    MASTER_REG_HOLD_DATA(46,  uint16,  0,  500,      500,  RW, 1, pSystem->m_usExAirFanMaxFreq);
    MASTER_REG_HOLD_DATA(47,  uint16,  0,  500,       10,  RW, 1, pSystem->m_usExAirFanAdjustFreq);
    MASTER_REG_HOLD_DATA(48,  uint16,  0,  1,          0,  RW, 1, pSystem->m_usExAirFanRatedVol_H);
    MASTER_REG_HOLD_DATA(49,  uint16,  0,  65535,  30000,  RW, 1, pSystem->m_usExAirFanRatedVol_L);
    MASTER_REG_HOLD_DATA(50,  uint16,  0,  7200,    1200,  RW, 1, pSystem->m_usExAirFanFreqRunPeriod);
    MASTER_REG_HOLD_DATA(51,  uint16,  0,  1440,      60,  RW, 1, pSystem->m_usExAirFanIntervalMin);
    MASTER_REG_HOLD_DATA(52,  uint16,  0,  7200,     300,  RW, 1, pSystem->m_usExAirFanRunTimeMin);
    MASTER_REG_HOLD_DATA(53,  uint16,  0,  7200,     180,  RW, 1, pSystem->m_usExAirFanPlusTime);
    MASTER_REG_HOLD_DATA(54,  uint16,  0,  7200,      30,  RW, 1, pSystem->m_usExAirFanFreqAdjustTime);
    MASTER_REG_HOLD_DATA(55,  uint16,  0,  7200,     180,  RW, 1, pSystem->m_usExAirFanSubTime);
    MASTER_REG_HOLD_DATA(56,  uint16,  0,   500,     350,  RW, 1, pSystem->m_usExAirFanPlusFreq_1);
    MASTER_REG_HOLD_DATA(57,  uint16,  0,   500,     380,  RW, 1, pSystem->m_usExAirFanPlusFreq_2);
    MASTER_REG_HOLD_DATA(58,  uint16,  0,   500,     350,  RW, 1, pSystem->m_usExAirFanSubFreq_1);
    MASTER_REG_HOLD_DATA(59,  uint16,  0,   500,     380,  RW, 1, pSystem->m_usExAirFanSubFreq_2);
    MASTER_REG_HOLD_DATA(60,  uint16,  0,    50,      10,  RW, 1, pSystem->m_usExAirFanTempDeviat);
    MASTER_REG_HOLD_DATA(61,  uint16,  0,   200,      90,  RW, 1, pSystem->m_usExAirFanRatio);

    MASTER_REG_HOLD_DATA(65,  uint16,  0,  500,   350,  RW, 1, pSystem->m_usCHWBumpMinFreq);
    MASTER_REG_HOLD_DATA(66,  uint16,  0,  500,   500,  RW, 1, pSystem->m_usCHWBumpMaxFreq);
    MASTER_REG_HOLD_DATA(67,  uint16,  0,  300,    10,  RW, 1, pSystem->m_usChilledDevsOpenDelay);
    MASTER_REG_HOLD_DATA(68,  uint16,  0,  300,    10,  RW, 1, pSystem->m_usChilledDevsCloseDelay);
    MASTER_REG_HOLD_DATA(69,  uint16,  0,  300,    10,  RW, 1, pSystem->m_usCHWBumpCloseDelay);
    MASTER_REG_HOLD_DATA(70,  uint16,  0,  300,    10,  RW, 1, pSystem->m_usButterflyValveCloseDelay);
    MASTER_REG_HOLD_DATA(71,  uint16,  0,  500,   300,  RW, 1, pSystem->m_usCHWBumpAdjustPeriod);
    MASTER_REG_HOLD_DATA(72,  uint16,  0,  500,    10,  RW, 1, pSystem->m_usCHWBumpAdjustFreq_1);
    MASTER_REG_HOLD_DATA(73,  uint16,  0,  500,     5,  RW, 1, pSystem->m_usCHWBumpAdjustFreq_2);
    MASTER_REG_HOLD_DATA(74,  uint16,  0,  500,    10,  RW, 1, pSystem->m_usCHWBumpAdjustFreq_3);
    MASTER_REG_HOLD_DATA(75,  uint16,  0,  500,     5,  RW, 1, pSystem->m_usCHWBumpAdjustFreq_4);

    MASTER_REG_HOLD_DATA(79,  uint16,  0,   30,  10,  RW, 1, pSystem->m_usChillerPlusPeriod);
    MASTER_REG_HOLD_DATA(80,  uint16,  0,   30,  10,  RW, 1, pSystem->m_usChillerSubPeriod);
    MASTER_REG_HOLD_DATA(81,  uint16,  0,   30,  10,  RW, 1, pSystem->m_usChillerPlusRemain);
    MASTER_REG_HOLD_DATA(82,  uint16,  0,   30,  10,  RW, 1, pSystem->m_usChillerSubRemain);

    MASTER_REG_HOLD_DATA(85,  uint16,  0,   60,  10,  RW, 1, pSystem->m_usModeChangePeriod_1);
    MASTER_REG_HOLD_DATA(86,  uint16,  0,   60,  10,  RW, 1, pSystem->m_usModeChangePeriod_2);
    MASTER_REG_HOLD_DATA(87,  uint16,  0,   60,  10,  RW, 1, pSystem->m_usModeChangePeriod_3);
    MASTER_REG_HOLD_DATA(88,  uint16,  0,   60,  10,  RW, 1, pSystem->m_usModeChangePeriod_4);
    MASTER_REG_HOLD_DATA(89,  uint16,  0,   60,  10,  RW, 1, pSystem->m_usModeChangePeriod_5);
    MASTER_REG_HOLD_DATA(90,  uint16,  0,   60,  10,  RW, 1, pSystem->m_usModeChangePeriod_6);

    MASTER_REG_HOLD_DATA(96,   uint16,  0,   100,  10,  RW, 1, pSystem->m_usModeAdjustTemp_1);
    MASTER_REG_HOLD_DATA(97,   uint16,  0,   100,  10,  RW, 1, pSystem->m_usModeAdjustTemp_2);
    MASTER_REG_HOLD_DATA(98,   uint16,  0,   100,  10,  RW, 1, pSystem->m_usModeAdjustTemp_3);
    MASTER_REG_HOLD_DATA(99,   uint16,  0,   100,  10,  RW, 1, pSystem->m_usModeAdjustTemp_4);
    MASTER_REG_HOLD_DATA(100,  uint16,  0,   100,  10,  RW, 1, pSystem->m_usModeAdjustTemp_5);
    MASTER_REG_HOLD_DATA(101,  uint16,  0,   100,  10,  RW, 1, pSystem->m_usModeAdjustTemp_6);

    MASTER_REG_HOLD_DATA(106,  uint16,   50,  200,   70,  RW, 1, pSystem->m_usCHWOutletCoolTemp);
    MASTER_REG_HOLD_DATA(107,  uint16,  350,  500,  500,  RW, 1, pSystem->m_usCHWOutletHeatTemp);
    MASTER_REG_HOLD_DATA(108,  uint16,   25,   60,   50,  RW, 1, pSystem->m_usCHWTempDiff);
    MASTER_REG_HOLD_DATA(109,  uint16,  100,  500,  240,  RW, 1, pSystem->m_usCHWPressureDiff);
    MASTER_REG_HOLD_DATA(110,  uint16,    2,   15,    5,  RW, 1, pSystem->m_usCHWPressureDeviat);
    MASTER_REG_HOLD_DATA(111,  uint16,   1,    20,    5,  RW, 1, pSystem->m_usCHWBypassRange);
    MASTER_REG_HOLD_DATA(112,  uint16,  50,   100,  100,  RW, 1, pSystem->m_usCHWBypassMaxAng);
    MASTER_REG_HOLD_DATA(113,  uint16,   0,    20,    0,  RW, 1, pSystem->m_usCHWBypassMinAng);
    MASTER_REG_HOLD_DATA(114,  uint16,   0,   300,   10,  RW, 1, pSystem->m_usCHWBypassAdjustTime);
    MASTER_REG_HOLD_DATA(115,  uint16,   0,   100,    0,  RW, 1, pSystem->m_pBypassValve->m_usAngSet);
    MASTER_REG_HOLD_DATA(116,  uint16,   0,   100,    0,  RW, 1, pSystem->m_pBypassValve->m_usAng);

    MASTER_REG_HOLD_DATA(120,  uint16,  100,  250, 120,  RW, 1, pSystem->m_usChillerCoolInTemp);
    MASTER_REG_HOLD_DATA(121,  uint16,   50,  200,  70,  RW, 1, pSystem->m_usChillerCoolOutTemp);
    MASTER_REG_HOLD_DATA(122,  uint16,  300,  450, 450,  RW, 1, pSystem->m_usChillerHeatInTemp);
    MASTER_REG_HOLD_DATA(123,  uint16,  350,  500, 500,  RW, 1, pSystem->m_usChillerHeatOutTemp);

    MASTER_REG_HOLD_DATA(128,  uint16,    0,  50,   25,  RW, 1, pSystem->m_usCHCoolPlusSupTempDeviat);
    MASTER_REG_HOLD_DATA(129,  uint16,    0,  50,   21,  RW, 1, pSystem->m_usCHCoolPlusTempDiff);
    MASTER_REG_HOLD_DATA(130,  uint16,    0,  50,   25,  RW, 1, pSystem->m_usCHHeatPlusSupTempDeviat);
    MASTER_REG_HOLD_DATA(131,  uint16,    0,  50,   21,  RW, 1, pSystem->m_usCHHeatPlusTempDiff);

    MASTER_REG_HOLD_DATA(132,  uint16,    0,  50,   20,  RW, 1, pSystem->m_usCHCoolSubSupTempDeviat);
    MASTER_REG_HOLD_DATA(133,  uint16,    0,  50,   21,  RW, 1, pSystem->m_usCHCoolSubTempDiff);
    MASTER_REG_HOLD_DATA(134,  uint16,    0,  50,   20,  RW, 1, pSystem->m_usCHHeatSubSupTempDeviat);
    MASTER_REG_HOLD_DATA(135,  uint16,    0,  50,   21,  RW, 1, pSystem->m_usCHHeatSubTempDiff);

    MASTER_REG_HOLD_DATA(140,  uint16,    0, 100,   50,  RW, 1, pSystem->m_usCHWBCoolPlusFreqTempDeviat);
    MASTER_REG_HOLD_DATA(141,  uint16,    0,  50,   10,  RW, 1, pSystem->m_usCHWBCoolPlusFreqTempDiff_1);
    MASTER_REG_HOLD_DATA(142,  uint16,    0, 100,    5,  RW, 1, pSystem->m_usCHWBCoolPlusFreqTempDiff_2);
    MASTER_REG_HOLD_DATA(143,  uint16,    0,  50,   50,  RW, 1, pSystem->m_usCHWBHeatPlusFreqTempDeviat);
    MASTER_REG_HOLD_DATA(144,  uint16,    0,  50,   10,  RW, 1, pSystem->m_usCHWBHeatPlusFreqTempDiff_1);
    MASTER_REG_HOLD_DATA(145,  uint16,    0,  50,    5,  RW, 1, pSystem->m_usCHWBHeatPlusFreqTempDiff_2);

    MASTER_REG_HOLD_DATA(146,  uint16,    0,  50,   10,  RW, 1, pSystem->m_usCHWBCoolSubFreqTempDeviat);
    MASTER_REG_HOLD_DATA(147,  uint16,    0,  50,   10,  RW, 1, pSystem->m_usCHWBCoolSubFreqTempDiff_1);
    MASTER_REG_HOLD_DATA(148,  uint16,    0,  50,    5,  RW, 1, pSystem->m_usCHWBCoolSubFreqTempDiff_2);
    MASTER_REG_HOLD_DATA(149,  uint16,    0,  50,   10,  RW, 1, pSystem->m_usCHWBHeatSubFreqTempDeviat);
    MASTER_REG_HOLD_DATA(150,  uint16,    0,  50,   10,  RW, 1, pSystem->m_usCHWBHeatSubFreqTempDiff_1);
    MASTER_REG_HOLD_DATA(151,  uint16,    0,  50,    5,  RW, 1, pSystem->m_usCHWBHeatSubFreqTempDiff_2);

    MASTER_REG_HOLD_DATA(157,  uint16,    0,  65535,   0,  RW, 1, pSystem->m_usTotalFreAir_H);
    MASTER_REG_HOLD_DATA(158,  uint16,    0,  65535,   0,  RW, 1, pSystem->m_usTotalFreAir_L);
    MASTER_REG_HOLD_DATA(159,  uint16,    0,  50000,   0,  RW, 1, pSystem->m_usCO2PPM);
    MASTER_REG_HOLD_DATA(160,  int16,  -400,    700,   0,  RW, 1, pSystem->m_sAmbientIn_T);
    MASTER_REG_HOLD_DATA(161,  int16,  -400,    700,   0,  RW, 1, pSystem->m_sAmbientOut_T);
    MASTER_REG_HOLD_DATA(162,  uint16,    0,   1000,   0,  RW, 1, pSystem->m_usAmbientIn_H);
    MASTER_REG_HOLD_DATA(163,  uint16,    0,   1000,   0,  RW, 1, pSystem->m_usAmbientOut_H);

    MASTER_REG_HOLD_DATA(167,  int16,  -200,   1000,   0,  RW, 1, pSystem->m_pCHWTempSensors[0]->m_sTemp);
    MASTER_REG_HOLD_DATA(168,  int16,  -200,   1000,   0,  RW, 1, pSystem->m_pCHWTempSensors[1]->m_sTemp);
    MASTER_REG_HOLD_DATA(169,  uint16,    0,  16000,   0,  RW, 1, pSystem->m_pCHWPressureSensors[0]->m_usPressure);
    MASTER_REG_HOLD_DATA(170,  uint16,    0,  16000,   0,  RW, 1, pSystem->m_pCHWPressureSensors[1]->m_usPressure);

    for(uint8_t i = 0; i < pSystem->m_pModularAirs.count(); i++)
    {
        pModularAir = pSystem->m_pModularAirs[i];

        MASTER_REG_HOLD_DATA(183+i*59,  uint16, 85,  170, 85,  RW, 1, pModularAir->m_eSwitchCmd);
        MASTER_REG_HOLD_DATA(184+i*59,  uint16,  0,    3,  0,  RW, 1, pModularAir->m_eRunningModeCmd);
        MASTER_REG_HOLD_DATA(185+i*59,  uint16,  0,    1,  0,  RO, 1, pModularAir->m_eControlMode);
        MASTER_REG_HOLD_DATA(186+i*59,  uint16,  0,    3,  0,  RO, 1, pModularAir->m_eModularState);
        MASTER_REG_HOLD_DATA(187+i*59,  uint16,  0,    3,  0,  RO, 1, pModularAir->m_eRunningMode);

        MASTER_REG_HOLD_DATA(190+i*59,  uint16,   160,     350,   240,  RW, 1, pModularAir->m_usTempSet);
        MASTER_REG_HOLD_DATA(191+i*59,  uint16,     0,    1000,   600,  RW, 1, pModularAir->m_usHumiSet);
        MASTER_REG_HOLD_DATA(192+i*59,  uint16, 10000,   30000, 20000,  RW, 1, pModularAir->m_usCO2Set);

        MASTER_REG_HOLD_DATA(193+i*59,  uint16,  0,   1000,   0,  RO, 1, pModularAir->m_usExitAirDamperAng);
        MASTER_REG_HOLD_DATA(194+i*59,  uint16,  0,   1000,   0,  RO, 1, pModularAir->m_usFreAirDamperAng);
        MASTER_REG_HOLD_DATA(195+i*59,  uint16,  0,   1000,   0,  RO, 1, pModularAir->m_usDivideDamperAng);
        MASTER_REG_HOLD_DATA(196+i*59,  uint16,  0,   1000,   0,  RO, 1, pModularAir->m_usRetAirDamperAng);
        MASTER_REG_HOLD_DATA(197+i*59,  uint16,  0,   1000,   0,  RO, 1, pModularAir->m_usCoilerDamperAng);

        MASTER_REG_HOLD_DATA(198+i*59,  uint16,  0,    500,   0,  RO, 1, pModularAir->m_usSupAirFanFreg);
        MASTER_REG_HOLD_DATA(199+i*59,  uint16,  0,    500,   0,  RO, 1, pModularAir->m_usExitAirFanFreg);

        MASTER_REG_HOLD_DATA(200+i*59,  int16,  -400,  700,   0,  RO, 1, pModularAir->m_sSupAirTemp);
        MASTER_REG_HOLD_DATA(201+i*59,  uint16,  0,   1000,   0,  RO, 1, pModularAir->m_usSupAirHumi);
        MASTER_REG_HOLD_DATA(202+i*59,  uint16,  0,  65535,   0,  RO, 1, pModularAir->m_usExitAirVolume);
        MASTER_REG_HOLD_DATA(203+i*59,  uint16,  0,  65535,   0,  RO, 1, pModularAir->m_usFreAirVolume);

        MASTER_REG_HOLD_DATA(206+i*59,  uint16,    0, 50000, 0,  RO, 1, pModularAir->m_pCO2Sensors[0]->m_usCO2ppm);
        MASTER_REG_HOLD_DATA(207+i*59,  uint16,    0, 50000, 0,  RO, 1, pModularAir->m_pCO2Sensors[1]->m_usCO2ppm);
        MASTER_REG_HOLD_DATA(211+i*59,  int16, -400,   700, 0,  RO, 1, pModularAir->m_pTempHumiOutSensor->m_sTemp);
        MASTER_REG_HOLD_DATA(212+i*59,  uint16,    0,  1000, 0,  RO, 1, pModularAir->m_pTempHumiOutSensor->m_usHumi);

        MASTER_REG_HOLD_DATA(216+i*59,  int16, -400, 700, 0,  RO, 1, pModularAir->m_pTempHumiInSensors[0]->m_sTemp);
        MASTER_REG_HOLD_DATA(217+i*59,  int16, -400, 700, 0,  RO, 1, pModularAir->m_pTempHumiInSensors[1]->m_sTemp);
        MASTER_REG_HOLD_DATA(218+i*59,  int16, -400, 700, 0,  RO, 1, pModularAir->m_pTempHumiInSensors[2]->m_sTemp);
        MASTER_REG_HOLD_DATA(219+i*59,  int16, -400, 700, 0,  RO, 1, pModularAir->m_pTempHumiInSensors[3]->m_sTemp);

        MASTER_REG_HOLD_DATA(223+i*59,  uint16,  0,  1000, 0,  RO, 1, pModularAir->m_pTempHumiInSensors[0]->m_usHumi);
        MASTER_REG_HOLD_DATA(224+i*59,  uint16,  0,  1000, 0,  RO, 1, pModularAir->m_pTempHumiInSensors[1]->m_usHumi);
        MASTER_REG_HOLD_DATA(225+i*59,  uint16,  0,  1000, 0,  RO, 1, pModularAir->m_pTempHumiInSensors[2]->m_usHumi);
        MASTER_REG_HOLD_DATA(226+i*59,  uint16,  0,  1000, 0,  RO, 1, pModularAir->m_pTempHumiInSensors[3]->m_usHumi);

        MASTER_REG_HOLD_DATA(233+i*59,  uint16,  0,  65535, 0,  RO, 1, pModularAir->m_sMeter.m_usPower);
        MASTER_REG_HOLD_DATA(234+i*59,  uint16,  0,  65535, 0,  RO, 1, pModularAir->m_sMeter.m_usTotalEnergy_H);
        MASTER_REG_HOLD_DATA(235+i*59,  uint16,  0,  65535, 0,  RO, 1, pModularAir->m_sMeter.m_usTotalEnergy_L);
    }

    pModularChiller = pSystem->m_pModularChillers[0];

    MASTER_REG_HOLD_DATA(301, uint16,   85,  170,  85, RW, 1,  pModularChiller->m_eSwitchCmd);
    MASTER_REG_HOLD_DATA(302, uint16,    1,    3,  1,  RW, 1,  pModularChiller->m_eRunningModeCmd);
    MASTER_REG_HOLD_DATA(305, int16,  -300, 1400,  0,  RO, 1,  pModularChiller->m_sRetWaterTemp);

    MASTER_REG_HOLD_DATA(306,  uint16,  100,  250, 120,  RW, 1, pModularChiller->m_usChillerCoolInTemp);
    MASTER_REG_HOLD_DATA(307,  uint16,   50,  200,  70,  RW, 1, pModularChiller->m_usChillerCoolOutTemp);
    MASTER_REG_HOLD_DATA(308,  uint16,  300,  450, 450,  RW, 1, pModularChiller->m_usChillerHeatInTemp);
    MASTER_REG_HOLD_DATA(309,  uint16,  350,  500, 500,  RW, 1, pModularChiller->m_usChillerHeatOutTemp);

    MASTER_REG_HOLD_DATA(310,  uint16,  0,  4, 0,  RO, 1, pModularChiller->m_Modulars[0]->m_eModularState);
    MASTER_REG_HOLD_DATA(311,  uint16,  0,  4, 0,  RO, 1, pModularChiller->m_Modulars[1]->m_eModularState);
    MASTER_REG_HOLD_DATA(312,  uint16,  0,  4, 0,  RO, 1, pModularChiller->m_Modulars[2]->m_eModularState);

    MASTER_REG_HOLD_DATA(313,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[0]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(314,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[0]->m_sOutputWaterTemp);
    MASTER_REG_HOLD_DATA(315,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[1]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(316,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[1]->m_sOutputWaterTemp);
    MASTER_REG_HOLD_DATA(317,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[2]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(318,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[2]->m_sOutputWaterTemp);

    pModularChiller = pSystem->m_pModularChillers[1];

    MASTER_REG_HOLD_DATA(326, uint16,   85,  170,  85, RW, 1,  pModularChiller->m_eSwitchCmd);
    MASTER_REG_HOLD_DATA(327, uint16,    1,    3,  1,  RW, 1,  pModularChiller->m_eRunningModeCmd);
    MASTER_REG_HOLD_DATA(330, int16,  -300, 1400,  0,  RO, 1,  pModularChiller->m_sRetWaterTemp);

    MASTER_REG_HOLD_DATA(331,  uint16,  100,  250, 120,  RW, 1, pModularChiller->m_usChillerCoolInTemp);
    MASTER_REG_HOLD_DATA(332,  uint16,   50,  200,  70,  RW, 1, pModularChiller->m_usChillerCoolOutTemp);
    MASTER_REG_HOLD_DATA(333,  uint16,  300,  450, 450,  RW, 1, pModularChiller->m_usChillerHeatInTemp);
    MASTER_REG_HOLD_DATA(334,  uint16,  350,  500, 500,  RW, 1, pModularChiller->m_usChillerHeatOutTemp);

    MASTER_REG_HOLD_DATA(335,  uint16,  0,  4, 0,  RO, 1, pModularChiller->m_Modulars[0]->m_eModularState);
    MASTER_REG_HOLD_DATA(336,  uint16,  0,  4, 0,  RO, 1, pModularChiller->m_Modulars[1]->m_eModularState);
    MASTER_REG_HOLD_DATA(337,  uint16,  0,  4, 0,  RO, 1, pModularChiller->m_Modulars[2]->m_eModularState);
    MASTER_REG_HOLD_DATA(338,  uint16,  0,  4, 0,  RO, 1, pModularChiller->m_Modulars[3]->m_eModularState);

    MASTER_REG_HOLD_DATA(339,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[0]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(340,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[0]->m_sOutputWaterTemp);
    MASTER_REG_HOLD_DATA(341,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[1]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(342,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[1]->m_sOutputWaterTemp);
    MASTER_REG_HOLD_DATA(343,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[2]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(344,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[2]->m_sOutputWaterTemp);
    MASTER_REG_HOLD_DATA(345,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[3]->m_sInputWaterTemp);
    MASTER_REG_HOLD_DATA(346,  int16,  -300,  700, uint16_t(-300),  RO, 1, pModularChiller->m_Modulars[3]->m_sOutputWaterTemp);

    for(uint8_t i = 0; i < pSystem->m_pAxialFans.count(); i++)
    {
        pAxialFan = pSystem->m_pAxialFans[i];
        MASTER_REG_HOLD_DATA(351+i*6,  uint16,  0,  500,   0,  RW, 1, pAxialFan->m_usFreqSet);
        MASTER_REG_HOLD_DATA(352+i*6,  uint16,  0,  500,   0,  RO, 1, pAxialFan->m_usFreq);
    }
    for(uint8_t i = 0; i < pSystem->m_pChilledBumps.count(); i++)
    {
        pChilledBump = pSystem->m_pChilledBumps[i];
        MASTER_REG_HOLD_DATA(375+i*6,  uint16,  0,  500,   0,  RW, 1, pChilledBump->m_usFreqSet);
        MASTER_REG_HOLD_DATA(376+i*6,  uint16,  0,  500,   0,  RO, 1, pChilledBump->m_usFreq);
    }
    MASTER_REG_HOLD_DATA(393,  uint16, 0, 100, 0,  RW, 1, pSystem->m_pWindowFans[0]->m_usAngSet);
    MASTER_REG_HOLD_DATA(394,  uint16, 0, 100, 0,  RW, 1, pSystem->m_pWindowFans[1]->m_usAngSet);
    MASTER_REG_HOLD_DATA(395,  uint16, 0, 100, 0,  RO, 1, pSystem->m_pWindowFans[0]->m_usAng);
    MASTER_REG_HOLD_DATA(396,  uint16, 0, 100, 0,  RO, 1, pSystem->m_pWindowFans[1]->m_usAng);

    MASTER_REG_HOLD_DATA(399,  uint16, 1970, 9999, 1970,  WO, 1, pSystem->m_usSysYear);
    MASTER_REG_HOLD_DATA(400,  uint16,    1,   12,    1,  WO, 1, pSystem->m_usSysMon);
    MASTER_REG_HOLD_DATA(401,  uint16,    1,   31,    1,  WO, 1, pSystem->m_usSysDay);
    MASTER_REG_HOLD_DATA(403,  uint16,    0,   23,    0,  WO, 1, pSystem->m_usSysHour);
    MASTER_REG_HOLD_DATA(404,  uint16,    0,   59,    0,  WO, 1, pSystem->m_usSysMin);
    //MASTER_REG_HOLD_DATA(405,  uint16,    0,   59,    0,  WO, 1, pSystem->m_usSysSec);

    MASTER_REG_HOLD_DATA(410,  uint16,  0,  65535, 0,  RO, 1, pSystem->m_pBumpMeter->m_usPower);
    MASTER_REG_HOLD_DATA(411,  uint16,  0,  65535, 0,  RO, 1, pSystem->m_pBumpMeter->m_usTotalEnergy_H);
    MASTER_REG_HOLD_DATA(412,  uint16,  0,  65535, 0,  RO, 1, pSystem->m_pBumpMeter->m_usTotalEnergy_L);

    MASTER_REG_HOLD_DATA(417,  uint16,  0,  65535, 0,  RO, 1, pSystem->m_pChillerMeter->m_usPower);
    MASTER_REG_HOLD_DATA(418,  uint16,  0,  65535, 0,  RO, 1, pSystem->m_pChillerMeter->m_usTotalEnergy_H);
    MASTER_REG_HOLD_DATA(419,  uint16,  0,  65535, 0,  RO, 1, pSystem->m_pChillerMeter->m_usTotalEnergy_L);

MASTER_END_DATA_BUF(0, 419)

MASTER_BEGIN_DATA_BUF(m_psBitCoilBuf, m_sDevCommData.sMBCoilTable, m_usBitCoilIndex)

    MASTER_COIL_BIT_DATA(4, 0, RW, pSystem->m_xAlarmEnable);
    MASTER_COIL_BIT_DATA(5, 0, RW, pSystem->m_xAlarmClean);

    for(uint8_t i = 0; i < pSystem->m_pModularAirs.count(); i++)
    {
        pModularAir = pSystem->m_pModularAirs[i];
        MASTER_COIL_BIT_DATA(43+i*40, 0, RO, pModularAir->m_xAlarmFlag);
        MASTER_COIL_BIT_DATA(45+i*40, 0, RO, pModularAir->m_xCommErr);
        MASTER_COIL_BIT_DATA(46+i*40, 0, RO, pModularAir->m_xSupAirFan);
        MASTER_COIL_BIT_DATA(47+i*40, 0, RO, pModularAir->m_xExitAirFan);
        MASTER_COIL_BIT_DATA(48+i*40, 0, RO, pModularAir->m_xWetMode);
        MASTER_COIL_BIT_DATA(49+i*40, 0, RO, pModularAir->m_xRecycleMode);
        MASTER_COIL_BIT_DATA(50+i*40, 0, RO, pModularAir->m_xExitAirSenErr);
        MASTER_COIL_BIT_DATA(51+i*40, 0, RO, pModularAir->m_xFreAirSenErr);

        MASTER_COIL_BIT_DATA(56+i*40, 0, RO, pModularAir->m_pCO2Sensors[0]->m_xError);
        MASTER_COIL_BIT_DATA(57+i*40, 0, RO, pModularAir->m_pCO2Sensors[1]->m_xError);

        MASTER_COIL_BIT_DATA(58+i*40, 0, RO, pModularAir->m_pTempHumiOutSensor->m_xTempErr);
        MASTER_COIL_BIT_DATA(59+i*40, 0, RO, pModularAir->m_pTempHumiOutSensor->m_xHumiErr);

        MASTER_COIL_BIT_DATA(60+i*40, 0, RO, pModularAir->m_pTempHumiInSensors[0]->m_xTempErr);
        MASTER_COIL_BIT_DATA(61+i*40, 0, RO, pModularAir->m_pTempHumiInSensors[1]->m_xTempErr);
        MASTER_COIL_BIT_DATA(62+i*40, 0, RO, pModularAir->m_pTempHumiInSensors[2]->m_xTempErr);
        MASTER_COIL_BIT_DATA(63+i*40, 0, RO, pModularAir->m_pTempHumiInSensors[3]->m_xTempErr);

        MASTER_COIL_BIT_DATA(64+i*40, 0, RO, pModularAir->m_pTempHumiInSensors[0]->m_xHumiErr);
        MASTER_COIL_BIT_DATA(65+i*40, 0, RO, pModularAir->m_pTempHumiInSensors[1]->m_xHumiErr);
        MASTER_COIL_BIT_DATA(66+i*40, 0, RO, pModularAir->m_pTempHumiInSensors[2]->m_xHumiErr);
        MASTER_COIL_BIT_DATA(67+i*40, 0, RO, pModularAir->m_pTempHumiInSensors[3]->m_xHumiErr);
    }

    pModularChiller = pSystem->m_pModularChillers[0];

    MASTER_COIL_BIT_DATA(121, 0, RO, pModularChiller->m_xCommErr);
    for(uint8_t j = 0; j < pModularChiller->m_Modulars.count(); j++)
    {
        pModular = pModularChiller->m_Modulars[j];
        MASTER_COIL_BIT_DATA(122+j*6, 0, RO, pModular->m_xErrorFlag);
        MASTER_COIL_BIT_DATA(123+j*6, 0, RO, pModular->m_xAlarmFlag);
        MASTER_COIL_BIT_DATA(124+j*6, 0, RO, pModular->m_xRunningFlag);
        MASTER_COIL_BIT_DATA(125+j*6, 0, RO, pModular->m_xCompRunnnig_1);
        MASTER_COIL_BIT_DATA(126+j*6, 0, RO, pModular->m_xCompRunnnig_2);
        MASTER_COIL_BIT_DATA(127+j*6, 0, RO, pModular->m_xWaterValve);
    }

    pModularChiller = pSystem->m_pModularChillers[1];

    MASTER_COIL_BIT_DATA(145, 0, RO, pModularChiller->m_xCommErr);
    for(uint8_t j = 0; j < pModularChiller->m_Modulars.count(); j++)
    {
        pModular = pModularChiller->m_Modulars[j];
        MASTER_COIL_BIT_DATA(146+j*6, 0, RO, pModular->m_xErrorFlag);
        MASTER_COIL_BIT_DATA(147+j*6, 0, RO, pModular->m_xAlarmFlag);
        MASTER_COIL_BIT_DATA(148+j*6, 0, RO, pModular->m_xRunningFlag);
        MASTER_COIL_BIT_DATA(149+j*6, 0, RO, pModular->m_xCompRunnnig_1);
        MASTER_COIL_BIT_DATA(150+j*6, 0, RO, pModular->m_xCompRunnnig_2);
        MASTER_COIL_BIT_DATA(151+j*6, 0, RO, pModular->m_xWaterValve);
    }

    for(uint8_t i = 0; i < pSystem->m_pChilledBumps.count(); i++)
    {
        pChilledBump = pSystem->m_pChilledBumps[i];
        MASTER_COIL_BIT_DATA(176+i*8, 0, RW, pChilledBump->m_xSwitchCmd);
        MASTER_COIL_BIT_DATA(177+i*8, 0, RW, pChilledBump->m_xErrClean);
        MASTER_COIL_BIT_DATA(178+i*8, 0, RO, pChilledBump->m_xRunningFlag);
        MASTER_COIL_BIT_DATA(179+i*8, 0, RO, pChilledBump->m_xControlFlag);
        MASTER_COIL_BIT_DATA(180+i*8, 0, RO, pChilledBump->m_xErrorFlag);
        MASTER_COIL_BIT_DATA(181+i*8, 0, RO, pChilledBump->m_xRemote);
    }

    for(uint8_t i = 0; i < pSystem->m_pAxialFans.count(); i++)
    {
        pAxialFan = pSystem->m_pAxialFans[i];
        MASTER_COIL_BIT_DATA(200+i*8, 0, RW, pAxialFan->m_xSwitchCmd);
        MASTER_COIL_BIT_DATA(201+i*8, 0, RW, pAxialFan->m_xErrClean);
        MASTER_COIL_BIT_DATA(202+i*8, 0, RO, pAxialFan->m_xRunningFlag);
        MASTER_COIL_BIT_DATA(203+i*8, 0, RO, pAxialFan->m_xControlFlag);
        MASTER_COIL_BIT_DATA(204+i*8, 0, RO, pAxialFan->m_xErrorFlag);
        MASTER_COIL_BIT_DATA(205+i*8, 0, RO, pAxialFan->m_xRemote);
    }

    for(uint8_t i = 0; i < pSystem->m_pButterflyValves.count(); i++)
    {
        pValve = pSystem->m_pButterflyValves[i];
        MASTER_COIL_BIT_DATA(232+i*8, 0, RW, pValve->m_xSwitchCmd);
        MASTER_COIL_BIT_DATA(233+i*8, 0, RW, pValve->m_xErrClean);
        MASTER_COIL_BIT_DATA(234+i*8, 0, RO, pValve->m_xOpened);
        MASTER_COIL_BIT_DATA(235+i*8, 0, RO, pValve->m_xClosed);
        MASTER_COIL_BIT_DATA(236+i*8, 0, RO, pValve->m_xRemote);
        MASTER_COIL_BIT_DATA(237+i*8, 0, RO, pValve->m_xErrorFlag);
    }

    MASTER_COIL_BIT_DATA(264, 0, RW, pSystem->m_pWindowFans[0]->m_xSwitchCmd);
    MASTER_COIL_BIT_DATA(265, 0, RW, pSystem->m_pWindowFans[1]->m_xSwitchCmd);

    MASTER_COIL_BIT_DATA(272, 0, RW, pSystem->m_pBypassValve->m_xControlMode);
    MASTER_COIL_BIT_DATA(274, 0, RO, pSystem->m_pBypassValve->m_xErrorFlag);

    MASTER_COIL_BIT_DATA(276, 0, RO, pSystem->m_pChillerMeter->m_xCommErr);
    MASTER_COIL_BIT_DATA(277, 0, RO, pSystem->m_pModularAirs[0]->m_sMeter.m_xCommErr);
    MASTER_COIL_BIT_DATA(278, 0, RO, pSystem->m_pModularAirs[1]->m_sMeter.m_xCommErr);
    MASTER_COIL_BIT_DATA(279, 0, RO, pSystem->m_pBumpMeter->m_xCommErr);

MASTER_END_DATA_BUF(0, 279)

    m_sDevCommData.pNext = nullptr;
    m_sDevCommData.usProtocolID = PROTOCOL_TYPE_ID;
    m_sDevCommData.pRegHoldIndex = m_usRegHoldIndex;  //绑定映射
    m_sDevCommData.pBitCoilIndex = m_usBitCoilIndex;  //绑定映射
    m_sDevCommData.pxDevDataMapIndex = devDataMapIndex;  //绑定映射函数
    m_sMBSlaveDev.psDevDataInfo = &m_sDevCommData;

    m_DelayTimer.start(COMM_ERR_DELAY_S*1000);
}

uint8_t Controller::devDataMapIndex(eDataType eDataType, uint8_t ucProtocolID, uint16_t usAddr, uint16_t* psIndex)
{
    uint16_t i = 0;
    switch(ucProtocolID)
    {
    case PROTOCOL_TYPE_ID:
        if(eDataType == RegHoldData)
        {
            switch(usAddr)
            {
                case 0 :  i = 0 ;  break;
                case 1 :  i = 1 ;  break;
                case 2 :  i = 2 ;  break;
                case 3 :  i = 3 ;  break;
                case 4 :  i = 4 ;  break;

                default:
                    return false;
                break;
            }
        }
        else if(eDataType == CoilData)
        {
            switch(usAddr)
            {
                case 0  :  i = 0 ;  break;
                case 1  :  i = 1 ;  break;

                default:
                    return FALSE;
                break;
            }
        }
    break;
    default: break;
    }

    *psIndex = i;
    return true;
}

