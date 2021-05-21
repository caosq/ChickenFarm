#include "manuparampage.h"
#include "ui_manuparampage.h"
#include "system.h"

#define LABEL_COLUMNS  2
#define LABEL_ROWS     16

#define LABEL_SIZE       300, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   40
#define LABEL_INTERVAL_H    450
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  130, 28

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  300
#define DATA_LABEL_INTERVAL_H   420
#define DATA_LABEL_INTERVAL_V   35

ManuParamPage::ManuParamPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManuParamPage)
{
    ui->setupUi(this);

    initLabel();
    initButton();
}

ManuParamPage::~ManuParamPage()
{
    delete ui;
}

void ManuParamPage::initLabel()
{
    TextLabel *pLabel = nullptr;

    for(uint8_t m = 0; m < LABEL_COLUMNS; m++)
    {
        for(uint8_t n = 0; n < LABEL_ROWS; n++)
        {
            pLabel = new TextLabel(ui->frame);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels.append(pLabel);
        }
    }
    m_Labels[0]->setText(tr("水阀制冷最小开度1"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("水阀制冷最小开度2"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("水阀制热最小开度"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("排风阀最小开度"), LABEL_FONT_SIZE);
    m_Labels[4]->setText(tr("CO2调节上容差"), LABEL_FONT_SIZE);
    m_Labels[5]->setText(tr("CO2调节下容差"), LABEL_FONT_SIZE);
    m_Labels[6]->setText(tr("CO2浓度排风控制偏差1"), LABEL_FONT_SIZE);
    m_Labels[7]->setText(tr("CO2浓度排风控制偏差2"), LABEL_FONT_SIZE);
    m_Labels[8]->setText(tr("CO2浓度调节阈值"), LABEL_FONT_SIZE);
    m_Labels[9]->setText(tr("畜牧风机最小频率"), LABEL_FONT_SIZE);

    m_Labels[10]->setText(tr("畜牧风机最大频率"), LABEL_FONT_SIZE);
    m_Labels[11]->setText(tr("畜牧风机频率调节值"), LABEL_FONT_SIZE);
    m_Labels[12]->setText(tr("畜牧风机额定风量"), LABEL_FONT_SIZE);
    m_Labels[13]->setText(tr("畜牧风机运行周期 "), LABEL_FONT_SIZE);
    m_Labels[14]->setText(tr("畜牧风机最小启停间隔"), LABEL_FONT_SIZE);
    m_Labels[15]->setText(tr("畜牧风机最小运行时间"), LABEL_FONT_SIZE);
    m_Labels[16]->setText(tr("畜牧风机加机时间"), LABEL_FONT_SIZE);
    m_Labels[17]->setText(tr("畜牧风机减机时间"), LABEL_FONT_SIZE);
    m_Labels[18]->setText(tr("畜牧风机频率调节时间"), LABEL_FONT_SIZE);
    m_Labels[19]->setText(tr("畜牧风机加机频率1"), LABEL_FONT_SIZE);

    m_Labels[20]->setText(tr("畜牧风机加机频率2"), LABEL_FONT_SIZE);
    m_Labels[21]->setText(tr("畜牧风机减机频率1"), LABEL_FONT_SIZE);
    m_Labels[22]->setText(tr("畜牧风机减机频率2"), LABEL_FONT_SIZE);
    m_Labels[23]->setText(tr("畜牧风机调节温度容差"), LABEL_FONT_SIZE);
    m_Labels[24]->setText(tr("畜牧风机排风百分比"), LABEL_FONT_SIZE);
    m_Labels[25]->setText(tr("冷冻泵最小频率"), LABEL_FONT_SIZE);
    m_Labels[26]->setText(tr("冷冻泵最大频率"), LABEL_FONT_SIZE);
    m_Labels[27]->setText(tr("冷冻水泵延迟关闭时间"), LABEL_FONT_SIZE);
    m_Labels[28]->setText(tr("冷冻泵频率调节间隔时间"), LABEL_FONT_SIZE);
    m_Labels[29]->setText(tr("冷冻水泵频率调节值_1"), LABEL_FONT_SIZE);

    m_Labels[30]->setText(tr("冷冻水泵频率调节值_2"), LABEL_FONT_SIZE);
    m_Labels[31]->setText(tr("冷冻水泵频率调节值_3"), LABEL_FONT_SIZE);



    for(uint8_t m = 0; m < LABEL_COLUMNS; m++)
    {
        for(uint8_t n = 0; n < LABEL_ROWS; n++)
        {
            pLabel = new TextLabel(ui->frame_2);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels.append(pLabel);
        }
    }
    m_Labels[32]->setText(tr("冷冻水泵频率调节值_4"), LABEL_FONT_SIZE);
    m_Labels[33]->setText(tr("冷冻设备延迟开启时间"), LABEL_FONT_SIZE);
    m_Labels[34]->setText(tr("冷冻设备延迟关闭时间"), LABEL_FONT_SIZE);
    m_Labels[35]->setText(tr("蝶阀延迟关闭时间"), LABEL_FONT_SIZE);
    m_Labels[36]->setText(tr("机组加机间隔时间"), LABEL_FONT_SIZE);
    m_Labels[37]->setText(tr("机组减机间隔时间"), LABEL_FONT_SIZE);
    m_Labels[38]->setText(tr("机组加机持续判断时间"), LABEL_FONT_SIZE);
    m_Labels[39]->setText(tr("机组减机持续判断时间"), LABEL_FONT_SIZE);

    m_Labels[40]->setText(tr("机组响应时间"), LABEL_FONT_SIZE);
    m_Labels[41]->setText(tr("模式切换间隔时间1"), LABEL_FONT_SIZE);
    m_Labels[42]->setText(tr("模式切换间隔时间2"), LABEL_FONT_SIZE);
    m_Labels[43]->setText(tr("模式切换间隔时间3"), LABEL_FONT_SIZE);
    m_Labels[44]->setText(tr("模式切换间隔时间4"), LABEL_FONT_SIZE);
    m_Labels[45]->setText(tr("模式切换间隔时间5"), LABEL_FONT_SIZE);
    m_Labels[46]->setText(tr("模式切换间隔时间6"), LABEL_FONT_SIZE);
    m_Labels[47]->setText(tr("模式调节温度T0"), LABEL_FONT_SIZE);
    m_Labels[48]->setText(tr("模式调节温度T1"), LABEL_FONT_SIZE);
    m_Labels[49]->setText(tr("模式调节温度T2"), LABEL_FONT_SIZE);

    m_Labels[50]->setText(tr("模式调节温度T3"), LABEL_FONT_SIZE);
    m_Labels[51]->setText(tr("模式调节温度T4"), LABEL_FONT_SIZE);
    m_Labels[52]->setText(tr("模式调节温度T5"), LABEL_FONT_SIZE);
    m_Labels[53]->setText(tr("模式调节温度T6"), LABEL_FONT_SIZE);
    m_Labels[54]->setText(tr("冷冻总管制冷供水温度"), LABEL_FONT_SIZE);
    m_Labels[55]->setText(tr("冷冻总管制热供水温度"), LABEL_FONT_SIZE);
    m_Labels[56]->setText(tr("冷冻总管供回水温差"), LABEL_FONT_SIZE);
    m_Labels[57]->setText(tr("冷冻总管压差设定"), LABEL_FONT_SIZE);
    m_Labels[58]->setText(tr("冷冻压差偏差百分比"), LABEL_FONT_SIZE);
    m_Labels[59]->setText(tr("冷冻旁通阀调节范围"), LABEL_FONT_SIZE);

    m_Labels[60]->setText(tr("冷冻旁通阀最大开度"), LABEL_FONT_SIZE);
    m_Labels[61]->setText(tr("冷冻旁通阀最小开度"), LABEL_FONT_SIZE);
    m_Labels[62]->setText(tr("冷冻旁通阀调节时间"), LABEL_FONT_SIZE);
    m_Labels[63]->setText(tr("制冷加机供水温度偏差"), LABEL_FONT_SIZE);


    for(uint8_t m = 0; m < LABEL_COLUMNS; m++)
    {
        for(uint8_t n = 0; n < LABEL_ROWS; n++)
        {
            pLabel = new TextLabel(ui->frame_3);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels.append(pLabel);
        }
    }
    m_Labels[64]->setText(tr("制冷加机供回水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[65]->setText(tr("制热加机供水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[66]->setText(tr("制热加机供回水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[67]->setText(tr("制冷减机供水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[68]->setText(tr("制冷减机供回水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[69]->setText(tr("制热减机供水温度偏差"), LABEL_FONT_SIZE);

    m_Labels[70]->setText(tr("制热减机供回水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[71]->setText(tr("制冷水泵升频供水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[72]->setText(tr("制冷水泵升频供回水温度偏差_1"), LABEL_FONT_SIZE);
    m_Labels[73]->setText(tr("制冷水泵升频供回水温度偏差_2"), LABEL_FONT_SIZE);
    m_Labels[74]->setText(tr("制热水泵升频供水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[75]->setText(tr("制热水泵升频供回水温度偏差_1"), LABEL_FONT_SIZE);
    m_Labels[76]->setText(tr("制热水泵升频供回水温度偏差_2"), LABEL_FONT_SIZE);
    m_Labels[77]->setText(tr("制冷水泵降频供水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[78]->setText(tr("制冷水泵降频供回水温度偏差_1"), LABEL_FONT_SIZE);
    m_Labels[79]->setText(tr("制冷水泵降频供回水温度偏差_2"), LABEL_FONT_SIZE);

    m_Labels[80]->setText(tr("制热水泵降频供水温度偏差"), LABEL_FONT_SIZE);
    m_Labels[81]->setText(tr("制热水泵降频供回水温度偏差_1"), LABEL_FONT_SIZE);
    m_Labels[82]->setText(tr("制热水泵降频供回水温度偏差_2"), LABEL_FONT_SIZE);

    m_Labels[83]->setText(tr("产品条码一"), LABEL_FONT_SIZE);
    m_Labels[84]->setText(tr("产品条码二"), LABEL_FONT_SIZE);
    m_Labels[85]->setText(tr("产品条码三"), LABEL_FONT_SIZE);
    m_Labels[86]->setText(tr("产品条码四"), LABEL_FONT_SIZE);
    m_Labels[87]->setText(tr("产品条码五"), LABEL_FONT_SIZE);
    m_Labels[88]->setText(tr("产品条码六"), LABEL_FONT_SIZE);
    m_Labels[89]->setText(tr("产品条码七"), LABEL_FONT_SIZE);

    m_Labels[90]->setText(tr("产品条码八"), LABEL_FONT_SIZE);
}

void ManuParamPage::initButton()
{
    System   *pSystem = System::getInstance();

    if(pSystem == nullptr)
    {
        return;
    }

    //水阀制冷最小开度1
    m_pCoolWaterDamperMinAng_1 = new AnalogValButton(ui->frame);
    m_pCoolWaterDamperMinAng_1->setDataParameter("%", 1, 350, 1000, 0, Monitor::Uint16t);
    m_pCoolWaterDamperMinAng_1->setMonitorData(&pSystem->m_usCoolWaterDamperMinAng_1, Monitor::Uint16t);
    m_Widgets.append(m_pCoolWaterDamperMinAng_1);
    //水阀制冷最小开度2
    m_pCoolWaterDamperMinAng_2 = new AnalogValButton(ui->frame);
    m_pCoolWaterDamperMinAng_2->setDataParameter("%", 1, 50, 1000, 0, Monitor::Uint16t);
    m_pCoolWaterDamperMinAng_2->setMonitorData(&pSystem->m_usCoolWaterDamperMinAng_2, Monitor::Uint16t);
    m_Widgets.append(m_pCoolWaterDamperMinAng_2);
    //水阀制热最小开度
    m_pHeatWaterDamperMinAng = new AnalogValButton(ui->frame);
    m_pHeatWaterDamperMinAng->setDataParameter("%", 1, 0, 1000, 0, Monitor::Uint16t);
    m_pHeatWaterDamperMinAng->setMonitorData(&pSystem->m_usHeatWaterDamperMinAng, Monitor::Uint16t);
    m_Widgets.append(m_pHeatWaterDamperMinAng);
    //排风阀最小开度
    m_pExAirDamperMinAng = new AnalogValButton(ui->frame);
    m_pExAirDamperMinAng->setDataParameter("%", 1, 100, 1000, 0, Monitor::Uint16t);
    m_pExAirDamperMinAng->setMonitorData(&pSystem->m_usExAirDamperMinAng, Monitor::Uint16t);
    m_Widgets.append(m_pExAirDamperMinAng);

    //CO2浓度调节上偏差
    m_pCO2AdjustDeviat_Up = new AnalogValButton(ui->frame);
    m_pCO2AdjustDeviat_Up->setDataParameter("ppm", 1, 5000, 10000, 0, Monitor::Uint16t);
    m_pCO2AdjustDeviat_Up->setMonitorData(&pSystem->m_usCO2AdjustDeviat_Up, Monitor::Uint16t);
    m_Widgets.append(m_pCO2AdjustDeviat_Up);
    //CO2浓度调节下偏差
    m_pCO2AdjustDeviat_Down = new AnalogValButton(ui->frame);
    m_pCO2AdjustDeviat_Down->setDataParameter("ppm", 1, 5000, 10000, 0, Monitor::Uint16t);
    m_pCO2AdjustDeviat_Down->setMonitorData(&pSystem->m_usCO2AdjustDeviat_Down, Monitor::Uint16t);
    m_Widgets.append(m_pCO2AdjustDeviat_Down);
    //CO2浓度排风控制偏差1
    m_pCO2ExAirDeviat_1 = new AnalogValButton(ui->frame);
    m_pCO2ExAirDeviat_1->setDataParameter("ppm", 1, 2000, 35000, 0, Monitor::Uint16t);
    m_pCO2ExAirDeviat_1->setMonitorData(&pSystem->m_usCO2ExAirDeviat_1, Monitor::Uint16t);
    m_Widgets.append(m_pCO2ExAirDeviat_1);
    //CO2浓度排风控制偏差2
    m_pCO2ExAirDeviat_2 = new AnalogValButton(ui->frame);
    m_pCO2ExAirDeviat_2->setDataParameter("ppm", 1, 2000, 35000, 0, Monitor::Uint16t);
    m_pCO2ExAirDeviat_2->setMonitorData(&pSystem->m_usCO2ExAirDeviat_2, Monitor::Uint16t);
    m_Widgets.append(m_pCO2ExAirDeviat_2);
    //CO2浓度调节阈值
    m_pCO2AdjustThr = new AnalogValButton(ui->frame);
    m_pCO2AdjustThr->setDataParameter("ppm", 1, 27000, 35000, 0, Monitor::Uint16t);
    m_pCO2AdjustThr->setMonitorData(&pSystem->m_usCO2AdjustThr, Monitor::Uint16t);
    m_Widgets.append(m_pCO2AdjustThr);

    //排风机最小频率
    m_pExAirFanMinFreq = new AnalogValButton(ui->frame);
    m_pExAirFanMinFreq->setDataParameter("Hz", 1, 350, 500, 0, Monitor::Uint16t);
    m_pExAirFanMinFreq->setMonitorData(&pSystem->m_usExAirFanMinFreq, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanMinFreq);
    //排风机最大频率
    m_pExAirFanMaxFreq = new AnalogValButton(ui->frame);
    m_pExAirFanMaxFreq->setDataParameter("Hz", 1, 500, 500, 0, Monitor::Uint16t);
    m_pExAirFanMaxFreq->setMonitorData(&pSystem->m_usExAirFanMaxFreq, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanMaxFreq);
    //排风机频率调节值
    m_pExAirFanAdjustFreq = new AnalogValButton(ui->frame);
    m_pExAirFanAdjustFreq->setDataParameter("Hz", 1, 10, 500, 0, Monitor::Uint16t);
    m_pExAirFanAdjustFreq->setMonitorData(&pSystem->m_usExAirFanAdjustFreq, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanAdjustFreq);
    //排风机额定风量
    m_pExAirFanRatedVol = new AnalogValButton(ui->frame);
    m_pExAirFanRatedVol->setDataParameter("m³/h", 0, 30000, 100000, 0, Monitor::Uint32t);
    m_pExAirFanRatedVol->setMonitorData(&pSystem->m_ulExAirFanRatedVol, Monitor::Uint32t);
    m_Widgets.append(m_pExAirFanRatedVol);

    //排风机运行周期
    m_pExAirFanFreqRunPeriod = new AnalogValButton(ui->frame);
    m_pExAirFanFreqRunPeriod->setDataParameter("s", 0, 1200, 7200, 0, Monitor::Uint16t);
    m_pExAirFanFreqRunPeriod->setMonitorData(&pSystem->m_usExAirFanFreqRunPeriod, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanFreqRunPeriod);
    //排风机最小启停间隔
    m_pExAirFanIntervalMin = new AnalogValButton(ui->frame);
    m_pExAirFanIntervalMin->setDataParameter("min", 0, 60, 1440, 0, Monitor::Uint16t);
    m_pExAirFanIntervalMin->setMonitorData(&pSystem->m_usExAirFanIntervalMin, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanIntervalMin);
    //排风机最小运行时间
    m_pExAirFanRunTimeMin = new AnalogValButton(ui->frame);
    m_pExAirFanRunTimeMin->setDataParameter("s", 0, 300, 7200, 0, Monitor::Uint16t);
    m_pExAirFanRunTimeMin->setMonitorData(&pSystem->m_usExAirFanRunTimeMin, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanRunTimeMin);
    //排风机加机时间
    m_pExAirFanPlusTime = new AnalogValButton(ui->frame);
    m_pExAirFanPlusTime->setDataParameter("s", 0, 180, 7200, 0, Monitor::Uint16t);
    m_pExAirFanPlusTime->setMonitorData(&pSystem->m_usExAirFanPlusTime, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanPlusTime);
    //排风机减机时间
    m_pExAirFanSubTime = new AnalogValButton(ui->frame);
    m_pExAirFanSubTime->setDataParameter("s", 0, 180, 7200, 0, Monitor::Uint16t);
    m_pExAirFanSubTime->setMonitorData(&pSystem->m_usExAirFanSubTime, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanSubTime);
    //排风机频率调节时间
    m_pExAirFanFreqAdjustTime = new AnalogValButton(ui->frame);
    m_pExAirFanFreqAdjustTime->setDataParameter("s", 0, 30, 7200, 0, Monitor::Uint16t);
    m_pExAirFanFreqAdjustTime->setMonitorData(&pSystem->m_usExAirFanFreqAdjustTime, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanFreqAdjustTime);
    //排风机加机频率1
    m_pExAirFanPlusFreq_1 = new AnalogValButton(ui->frame);
    m_pExAirFanPlusFreq_1->setDataParameter("Hz", 1, 350, 500, 0, Monitor::Uint16t);
    m_pExAirFanPlusFreq_1->setMonitorData(&pSystem->m_usExAirFanPlusFreq_1, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanPlusFreq_1);
    //排风机加机频率2
    m_pExAirFanPlusFreq_2 = new AnalogValButton(ui->frame);
    m_pExAirFanPlusFreq_2->setDataParameter("Hz", 1, 380, 500, 0, Monitor::Uint16t);
    m_pExAirFanPlusFreq_2->setMonitorData(&pSystem->m_usExAirFanPlusFreq_2, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanPlusFreq_2);
    //排风机减机频率1
    m_pExAirFanSubFreq_1 = new AnalogValButton(ui->frame);
    m_pExAirFanSubFreq_1->setDataParameter("Hz", 1, 350, 500, 0, Monitor::Uint16t);
    m_pExAirFanSubFreq_1->setMonitorData(&pSystem->m_usExAirFanSubFreq_1, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanSubFreq_1);
    //排风机减机频率2
    m_pExAirFanSubFreq_2 = new AnalogValButton(ui->frame);
    m_pExAirFanSubFreq_2->setDataParameter("Hz", 1, 380, 500, 0, Monitor::Uint16t);
    m_pExAirFanSubFreq_2->setMonitorData(&pSystem->m_usExAirFanSubFreq_2, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanSubFreq_2);
    //排风机调节温度偏差
    m_pExAirFanTempDeviat = new AnalogValButton(ui->frame);
    m_pExAirFanTempDeviat->setDataParameter("℃", 1, 10, 50, 0, Monitor::Uint16t);
    m_pExAirFanTempDeviat->setMonitorData(&pSystem->m_usExAirFanTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanTempDeviat);
    //排风机排风百分比
    m_pExAirFanRatio = new AnalogValButton(ui->frame);
    m_pExAirFanRatio->setDataParameter("%", 0, 90, 200, 0, Monitor::Uint16t);
    m_pExAirFanRatio->setMonitorData(&pSystem->m_usExAirFanRatio, Monitor::Uint16t);
    m_Widgets.append(m_pExAirFanRatio);

    //冷冻泵最小频率
    m_pCHWBumpMinFreq = new AnalogValButton(ui->frame);
    m_pCHWBumpMinFreq->setDataParameter("Hz", 1, 350, 500, 0, Monitor::Uint16t);
    m_pCHWBumpMinFreq->setMonitorData(&pSystem->m_usCHWBumpMinFreq, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBumpMinFreq);
    //冷冻泵最大频率
    m_pCHWBumpMaxFreq = new AnalogValButton(ui->frame);
    m_pCHWBumpMaxFreq->setDataParameter("Hz", 1, 500, 500, 0, Monitor::Uint16t);
    m_pCHWBumpMaxFreq->setMonitorData(&pSystem->m_usCHWBumpMaxFreq, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBumpMaxFreq);
    //冷冻泵延迟关闭时间
    m_pCHWBumpCloseDelay = new AnalogValButton(ui->frame);
    m_pCHWBumpCloseDelay->setDataParameter("s", 0, 10, 300, 0, Monitor::Uint16t);
    m_pCHWBumpCloseDelay->setMonitorData(&pSystem->m_usCHWBumpCloseDelay, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBumpCloseDelay);
    //冷冻泵频率调节间隔
    m_pCHWBumpAdjustPeriod = new AnalogValButton(ui->frame);
    m_pCHWBumpAdjustPeriod->setDataParameter("s", 0, 300, 500, 0, Monitor::Uint16t);
    m_pCHWBumpAdjustPeriod->setMonitorData(&pSystem->m_usCHWBumpAdjustPeriod, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBumpAdjustPeriod);

    //冷冻泵频率调节值
    m_pCHWBumpAdjustFreq_1 = new AnalogValButton(ui->frame);
    m_pCHWBumpAdjustFreq_1->setDataParameter("Hz", 1, 10, 500, 0, Monitor::Uint16t);
    m_pCHWBumpAdjustFreq_1->setMonitorData(&pSystem->m_usCHWBumpAdjustFreq_1, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBumpAdjustFreq_1);

    //冷冻泵频率调节值
    m_pCHWBumpAdjustFreq_2 = new AnalogValButton(ui->frame);
    m_pCHWBumpAdjustFreq_2->setDataParameter("Hz", 1, 5, 500, 0, Monitor::Uint16t);
    m_pCHWBumpAdjustFreq_2->setMonitorData(&pSystem->m_usCHWBumpAdjustFreq_2, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBumpAdjustFreq_2);

    //冷冻泵频率调节值
    m_pCHWBumpAdjustFreq_3 = new AnalogValButton(ui->frame);
    m_pCHWBumpAdjustFreq_3->setDataParameter("Hz", 1, 10, 500, 0, Monitor::Uint16t);
    m_pCHWBumpAdjustFreq_3->setMonitorData(&pSystem->m_usCHWBumpAdjustFreq_3, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBumpAdjustFreq_3);

    //冷冻泵频率调节值
    m_pCHWBumpAdjustFreq_4 = new AnalogValButton(ui->frame_2);
    m_pCHWBumpAdjustFreq_4->setDataParameter("Hz", 1, 5, 500, 0, Monitor::Uint16t);
    m_pCHWBumpAdjustFreq_4->setMonitorData(&pSystem->m_usCHWBumpAdjustFreq_4, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBumpAdjustFreq_4);

    //冷冻设备延迟开启时间
    m_pChilledDevsOpenDelay = new AnalogValButton(ui->frame_2);
    m_pChilledDevsOpenDelay->setDataParameter("s", 0, 10, 300, 0, Monitor::Uint16t);
    m_pChilledDevsOpenDelay->setMonitorData(&pSystem->m_usChilledDevsOpenDelay, Monitor::Uint16t);
    m_Widgets.append(m_pChilledDevsOpenDelay);
    //冷冻设备延迟关闭时间
    m_pChilledDevsCloseDelay = new AnalogValButton(ui->frame_2);
    m_pChilledDevsCloseDelay->setDataParameter("s", 0, 10, 300, 0, Monitor::Uint16t);
    m_pChilledDevsCloseDelay->setMonitorData(&pSystem->m_usChilledDevsCloseDelay, Monitor::Uint16t);
    m_Widgets.append(m_pChilledDevsCloseDelay);
    //蝶阀延迟关闭时间
    m_pButterflyValveCloseDelay = new AnalogValButton(ui->frame_2);
    m_pButterflyValveCloseDelay->setDataParameter("s", 0, 10, 300, 0, Monitor::Uint16t);
    m_pButterflyValveCloseDelay->setMonitorData(&pSystem->m_usButterflyValveCloseDelay, Monitor::Uint16t);
    m_Widgets.append(m_pButterflyValveCloseDelay);

    //机组加机间隔
    m_pChillerPlusPeriod = new AnalogValButton(ui->frame_2);
    m_pChillerPlusPeriod->setDataParameter("min", 0, 10, 30, 0, Monitor::Uint16t);
    m_pChillerPlusPeriod->setMonitorData(&pSystem->m_usChillerPlusPeriod, Monitor::Uint16t);
    m_Widgets.append(m_pChillerPlusPeriod);
    //机组减机间隔
    m_pChillerSubPeriod = new AnalogValButton(ui->frame_2);
    m_pChillerSubPeriod->setDataParameter("min", 0, 10, 30, 0, Monitor::Uint16t);
    m_pChillerSubPeriod->setMonitorData(&pSystem->m_usChillerSubPeriod, Monitor::Uint16t);
    m_Widgets.append(m_pChillerSubPeriod);
    //机组加机持续判断时间
    m_pChillerPlusRemain = new AnalogValButton(ui->frame_2);
    m_pChillerPlusRemain->setDataParameter("min", 0, 10, 30, 0, Monitor::Uint16t);
    m_pChillerPlusRemain->setMonitorData(&pSystem->m_usChillerPlusRemain, Monitor::Uint16t);
    m_Widgets.append(m_pChillerPlusRemain);
    //机组减机持续判断时间
    m_pChillerSubRemain = new AnalogValButton(ui->frame_2);
    m_pChillerSubRemain->setDataParameter("min", 0, 10, 30, 0, Monitor::Uint16t);
    m_pChillerSubRemain->setMonitorData(&pSystem->m_usChillerSubRemain, Monitor::Uint16t);
    m_Widgets.append(m_pChillerSubRemain);
    //机组响应时间
    m_pChillerRespond = new AnalogValButton(ui->frame_2);
    m_pChillerRespond->setDataParameter("min", 0, 10, 60, 0, Monitor::Uint16t);
    m_pChillerRespond->setMonitorData(&pSystem->m_usChillerRespond, Monitor::Uint16t);
    m_Widgets.append(m_pChillerRespond);

    //模式切换间隔时间t1(min)
    m_pModeChangePeriod_1 = new AnalogValButton(ui->frame_2);
    m_pModeChangePeriod_1->setDataParameter("min", 0, 10, 60, 0, Monitor::Uint16t);
    m_pModeChangePeriod_1->setMonitorData(&pSystem->m_usModeChangePeriod_1, Monitor::Uint16t);
    m_Widgets.append(m_pModeChangePeriod_1);
    //模式切换间隔时间t2(min)
    m_pModeChangePeriod_2 = new AnalogValButton(ui->frame_2);
    m_pModeChangePeriod_2->setDataParameter("min", 0, 10, 60, 0, Monitor::Uint16t);
    m_pModeChangePeriod_2->setMonitorData(&pSystem->m_usModeChangePeriod_2, Monitor::Uint16t);
    m_Widgets.append(m_pModeChangePeriod_2);
    //模式切换间隔时间t3(min)
    m_pModeChangePeriod_3 = new AnalogValButton(ui->frame_2);
    m_pModeChangePeriod_3->setDataParameter("min", 0, 10, 60, 0, Monitor::Uint16t);
    m_pModeChangePeriod_3->setMonitorData(&pSystem->m_usModeChangePeriod_3, Monitor::Uint16t);
    m_Widgets.append(m_pModeChangePeriod_3);
    //模式切换间隔时间t4(min)
    m_pModeChangePeriod_4 = new AnalogValButton(ui->frame_2);
    m_pModeChangePeriod_4->setDataParameter("min", 0, 10, 60, 0, Monitor::Uint16t);
    m_pModeChangePeriod_4->setMonitorData(&pSystem->m_usModeChangePeriod_4, Monitor::Uint16t);
    m_Widgets.append(m_pModeChangePeriod_4);
    //模式切换间隔时间t5(min)
    m_pModeChangePeriod_5 = new AnalogValButton(ui->frame_2);
    m_pModeChangePeriod_5->setDataParameter("min", 0, 10, 60, 0, Monitor::Uint16t);
    m_pModeChangePeriod_5->setMonitorData(&pSystem->m_usModeChangePeriod_5, Monitor::Uint16t);
    m_Widgets.append(m_pModeChangePeriod_5);
    //模式切换间隔时间t6(min)
    m_pModeChangePeriod_6 = new AnalogValButton(ui->frame_2);
    m_pModeChangePeriod_6->setDataParameter("min", 0, 10, 60, 0, Monitor::Uint16t);
    m_pModeChangePeriod_6->setMonitorData(&pSystem->m_usModeChangePeriod_6, Monitor::Uint16t);
    m_Widgets.append(m_pModeChangePeriod_6);


    //模式调节温度T0
    m_pModeAdjustTemp_0 = new AnalogValButton(ui->frame_2);
    m_pModeAdjustTemp_0->setDataParameter("℃", 1, 10, 100, 0, Monitor::Uint16t);
    m_pModeAdjustTemp_0->setMonitorData(&pSystem->m_usModeAdjustTemp_0, Monitor::Uint16t);
    m_Widgets.append(m_pModeAdjustTemp_0);
    //模式调节温度T1
    m_pModeAdjustTemp_1 = new AnalogValButton(ui->frame_2);
    m_pModeAdjustTemp_1->setDataParameter("℃", 1, 10, 100, 0, Monitor::Uint16t);
    m_pModeAdjustTemp_1->setMonitorData(&pSystem->m_usModeAdjustTemp_1, Monitor::Uint16t);
    m_Widgets.append(m_pModeAdjustTemp_1);
    //模式调节温度T2
    m_pModeAdjustTemp_2 = new AnalogValButton(ui->frame_2);
    m_pModeAdjustTemp_2->setDataParameter("℃", 1, 10, 100, 0, Monitor::Uint16t);
    m_pModeAdjustTemp_2->setMonitorData(&pSystem->m_usModeAdjustTemp_2, Monitor::Uint16t);
    m_Widgets.append(m_pModeAdjustTemp_2);
    //模式调节温度T3
    m_pModeAdjustTemp_3 = new AnalogValButton(ui->frame_2);
    m_pModeAdjustTemp_3->setDataParameter("℃", 1, 10, 100, 0, Monitor::Uint16t);
    m_pModeAdjustTemp_3->setMonitorData(&pSystem->m_usModeAdjustTemp_3, Monitor::Uint16t);
    m_Widgets.append(m_pModeAdjustTemp_3);
    //模式调节温度T4
    m_pModeAdjustTemp_4 = new AnalogValButton(ui->frame_2);
    m_pModeAdjustTemp_4->setDataParameter("℃", 1, 10, 100, 0, Monitor::Uint16t);
    m_pModeAdjustTemp_4->setMonitorData(&pSystem->m_usModeAdjustTemp_4, Monitor::Uint16t);
    m_Widgets.append(m_pModeAdjustTemp_4);
    //模式调节温度T5
    m_pModeAdjustTemp_5 = new AnalogValButton(ui->frame_2);
    m_pModeAdjustTemp_5->setDataParameter("℃", 1, 10, 100, 0, Monitor::Uint16t);
    m_pModeAdjustTemp_5->setMonitorData(&pSystem->m_usModeAdjustTemp_5, Monitor::Uint16t);
    m_Widgets.append(m_pModeAdjustTemp_5);
    //模式调节温度T6
    m_pModeAdjustTemp_6 = new AnalogValButton(ui->frame_2);
    m_pModeAdjustTemp_6->setDataParameter("℃", 1, 10, 100, 0, Monitor::Uint16t);
    m_pModeAdjustTemp_6->setMonitorData(&pSystem->m_usModeAdjustTemp_6, Monitor::Uint16t);
    m_Widgets.append(m_pModeAdjustTemp_6);

    //冷冻总管制冷供水温度设定值
    m_pCHWOutletCoolTemp = new AnalogValButton(ui->frame_2);
    m_pCHWOutletCoolTemp->setDataParameter("℃", 1, 70, 200, 50, Monitor::Uint16t);
    m_pCHWOutletCoolTemp->setMonitorData(&pSystem->m_usCHWOutletCoolTemp, Monitor::Uint16t);
    m_Widgets.append(m_pCHWOutletCoolTemp);
    //冷冻总管制热供水温度设定值
    m_pCHWOutletHeatTemp = new AnalogValButton(ui->frame_2);
    m_pCHWOutletHeatTemp->setDataParameter("℃", 1, 500, 500, 350, Monitor::Uint16t);
    m_pCHWOutletHeatTemp->setMonitorData(&pSystem->m_usCHWOutletHeatTemp, Monitor::Uint16t);
    m_Widgets.append(m_pCHWOutletHeatTemp);
    //冷冻总管供回水温差设定值
    m_pCHWTempDiff = new AnalogValButton(ui->frame_2);
    m_pCHWTempDiff->setDataParameter("℃", 1, 50, 60, 25, Monitor::Uint16t);
    m_pCHWTempDiff->setMonitorData(&pSystem->m_usCHWTempDiff, Monitor::Uint16t);
    m_Widgets.append(m_pCHWTempDiff);

    //冷冻总管压差设定
    m_pCHWPressureDiff = new AnalogValButton(ui->frame_2);
    m_pCHWPressureDiff->setDataParameter("kPa", 0, 240, 500, 100, Monitor::Uint16t);
    m_pCHWPressureDiff->setMonitorData(&pSystem->m_usCHWPressureDiff, Monitor::Uint16t);
    m_Widgets.append(m_pCHWPressureDiff);
    //冷冻压差偏差百分比设定
    m_pCHWPressureDeviat = new AnalogValButton(ui->frame_2);
    m_pCHWPressureDeviat->setDataParameter("%", 0, 5, 15, 2, Monitor::Uint16t);
    m_pCHWPressureDeviat->setMonitorData(&pSystem->m_usCHWPressureDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pCHWPressureDeviat);

    //冷冻旁通阀调节范围设定
    m_pCHWBypassRange = new AnalogValButton(ui->frame_2);
    m_pCHWBypassRange->setDataParameter("%", 0, 5, 20, 1, Monitor::Uint16t);
    m_pCHWBypassRange->setMonitorData(&pSystem->m_usCHWBypassRange, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBypassRange);
    //冷冻旁通阀最大开度设定
    m_pCHWBypassMaxAng = new AnalogValButton(ui->frame_2);
    m_pCHWBypassMaxAng->setDataParameter("%", 0, 100, 100, 50, Monitor::Uint16t);
    m_pCHWBypassMaxAng->setMonitorData(&pSystem->m_usCHWBypassMaxAng, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBypassMaxAng);
    //冷冻旁通阀最小开度设定
    m_pCHWBypassMinAng = new AnalogValButton(ui->frame_2);
    m_pCHWBypassMinAng->setDataParameter("%", 0, 0, 20, 0, Monitor::Uint16t);
    m_pCHWBypassMinAng->setMonitorData(&pSystem->m_usCHWBypassMinAng, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBypassMinAng);
    //冷冻旁通阀调节时间设定
    m_pCHWBypassAdjustTime = new AnalogValButton(ui->frame_2);
    m_pCHWBypassAdjustTime->setDataParameter("s", 0, 10, 300, 0, Monitor::Uint16t);
    m_pCHWBypassAdjustTime->setMonitorData(&pSystem->m_usCHWBypassAdjustTime, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBypassAdjustTime);

    //制冷加机供水温度偏差设定值
    m_pCHCoolPlusSupTempDeviat = new AnalogValButton(ui->frame_2);
    m_pCHCoolPlusSupTempDeviat->setDataParameter("℃", 1, 25, 50, 0, Monitor::Uint16t);
    m_pCHCoolPlusSupTempDeviat->setMonitorData(&pSystem->m_usCHCoolPlusSupTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pCHCoolPlusSupTempDeviat);
    //制冷加机供回水温差偏差设定值
    m_pCHCoolPlusTempDiff = new AnalogValButton(ui->frame_3);
    m_pCHCoolPlusTempDiff->setDataParameter("℃", 1, 21, 50, 0, Monitor::Uint16t);
    m_pCHCoolPlusTempDiff->setMonitorData(&pSystem->m_usCHCoolPlusTempDiff, Monitor::Uint16t);
    m_Widgets.append(m_pCHCoolPlusTempDiff);
    //制热加机供水温度偏差设定值
    m_pCHHeatPlusSupTempDeviat = new AnalogValButton(ui->frame_3);
    m_pCHHeatPlusSupTempDeviat->setDataParameter("℃", 1, 25, 50, 0, Monitor::Uint16t);
    m_pCHHeatPlusSupTempDeviat->setMonitorData(&pSystem->m_usCHHeatPlusSupTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pCHHeatPlusSupTempDeviat);
    //制热加机供回水温差偏差设定值
    m_pCHHeatPlusTempDiff = new AnalogValButton(ui->frame_3);
    m_pCHHeatPlusTempDiff->setDataParameter("℃", 1, 21, 50, 0, Monitor::Uint16t);
    m_pCHHeatPlusTempDiff->setMonitorData(&pSystem->m_usCHHeatPlusTempDiff, Monitor::Uint16t);
    m_Widgets.append(m_pCHHeatPlusTempDiff);

    //制冷减机供水温度偏差设定值
    m_pCHCoolSubSupTempDeviat = new AnalogValButton(ui->frame_3);
    m_pCHCoolSubSupTempDeviat->setDataParameter("℃", 1, 20, 50, 0, Monitor::Uint16t);
    m_pCHCoolSubSupTempDeviat->setMonitorData(&pSystem->m_usCHCoolSubSupTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pCHCoolSubSupTempDeviat);
    //制冷减机供回水温差偏差设定值
    m_pCHCoolPlusTempDiff = new AnalogValButton(ui->frame_3);
    m_pCHCoolPlusTempDiff->setDataParameter("℃", 1, 21, 50, 0, Monitor::Uint16t);
    m_pCHCoolPlusTempDiff->setMonitorData(&pSystem->m_usCHCoolSubTempDiff, Monitor::Uint16t);
    m_Widgets.append(m_pCHCoolPlusTempDiff);
    //制热减机供水温度偏差设定值
    m_pCHHeatSubSupTempDeviat = new AnalogValButton(ui->frame_3);
    m_pCHHeatSubSupTempDeviat->setDataParameter("℃", 1, 20, 50, 0, Monitor::Uint16t);
    m_pCHHeatSubSupTempDeviat->setMonitorData(&pSystem->m_usCHHeatSubSupTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pCHHeatSubSupTempDeviat);
    //制热减机供回水温差偏差设定值
    m_pCHHeatSubTempDiff = new AnalogValButton(ui->frame_3);
    m_pCHHeatSubTempDiff->setDataParameter("℃", 1, 21, 50, 0, Monitor::Uint16t);
    m_pCHHeatSubTempDiff->setMonitorData(&pSystem->m_usCHHeatSubTempDiff, Monitor::Uint16t);
    m_Widgets.append(m_pCHHeatSubTempDiff);

    //制冷水泵升频供水温度偏差设定值
    m_pCHWBCoolPlusFreqTempDeviat = new AnalogValButton(ui->frame_3);
    m_pCHWBCoolPlusFreqTempDeviat->setDataParameter("℃", 1, 50, 100, 0, Monitor::Uint16t);
    m_pCHWBCoolPlusFreqTempDeviat->setMonitorData(&pSystem->m_usCHWBCoolPlusFreqTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBCoolPlusFreqTempDeviat);
    //制冷水泵升频供回水温差偏差设定值
    m_pCHWBCoolPlusFreqTempDiff_1 = new AnalogValButton(ui->frame_3);
    m_pCHWBCoolPlusFreqTempDiff_1->setDataParameter("℃", 1, 10, 50, 0, Monitor::Uint16t);
    m_pCHWBCoolPlusFreqTempDiff_1->setMonitorData(&pSystem->m_usCHWBCoolPlusFreqTempDiff_1, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBCoolPlusFreqTempDiff_1);
    //制冷水泵升频供回水温差偏差设定值
    m_pCHWBCoolPlusFreqTempDiff_2 = new AnalogValButton(ui->frame_3);
    m_pCHWBCoolPlusFreqTempDiff_2->setDataParameter("℃", 1, 5, 50, 0, Monitor::Uint16t);
    m_pCHWBCoolPlusFreqTempDiff_2->setMonitorData(&pSystem->m_usCHWBCoolPlusFreqTempDiff_2, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBCoolPlusFreqTempDiff_2);

    //制热水泵升频供水温度偏差设定值
    m_pCHWBHeatPlusFreqTempDeviat = new AnalogValButton(ui->frame_3);
    m_pCHWBHeatPlusFreqTempDeviat->setDataParameter("℃", 1, 50, 100, 0, Monitor::Uint16t);
    m_pCHWBHeatPlusFreqTempDeviat->setMonitorData(&pSystem->m_usCHWBHeatPlusFreqTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBHeatPlusFreqTempDeviat);
    //制热水泵升频供回水温差偏差设定值
    m_pCHWBHeatPlusFreqTempDiff_1 = new AnalogValButton(ui->frame_3);
    m_pCHWBHeatPlusFreqTempDiff_1->setDataParameter("℃", 1, 10, 50, 0, Monitor::Uint16t);
    m_pCHWBHeatPlusFreqTempDiff_1->setMonitorData(&pSystem->m_usCHWBHeatPlusFreqTempDiff_1, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBHeatPlusFreqTempDiff_1);
    //制热水泵升频供回水温差偏差设定值
    m_pCHWBHeatPlusFreqTempDiff_2 = new AnalogValButton(ui->frame_3);
    m_pCHWBHeatPlusFreqTempDiff_2->setDataParameter("℃", 1, 5, 50, 0, Monitor::Uint16t);
    m_pCHWBHeatPlusFreqTempDiff_2->setMonitorData(&pSystem->m_usCHWBHeatPlusFreqTempDiff_2, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBHeatPlusFreqTempDiff_2);



    //制冷水泵降频供水温度偏差设定值
    m_pCHWBCoolSubFreqTempDeviat = new AnalogValButton(ui->frame_3);
    m_pCHWBCoolSubFreqTempDeviat->setDataParameter("℃", 1, 10, 50, 0, Monitor::Uint16t);
    m_pCHWBCoolSubFreqTempDeviat->setMonitorData(&pSystem->m_usCHWBCoolSubFreqTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBCoolSubFreqTempDeviat);
    //制冷水泵降频供回水温差偏差设定值
    m_pCHWBCoolSubFreqTempDiff_1 = new AnalogValButton(ui->frame_3);
    m_pCHWBCoolSubFreqTempDiff_1->setDataParameter("℃", 1, 10, 50, 0, Monitor::Uint16t);
    m_pCHWBCoolSubFreqTempDiff_1->setMonitorData(&pSystem->m_usCHWBCoolSubFreqTempDiff_1, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBCoolSubFreqTempDiff_1);
    //制冷水泵降频供回水温差偏差设定值
    m_pCHWBCoolSubFreqTempDiff_2 = new AnalogValButton(ui->frame_3);
    m_pCHWBCoolSubFreqTempDiff_2->setDataParameter("℃", 1, 5, 50, 0, Monitor::Uint16t);
    m_pCHWBCoolSubFreqTempDiff_2->setMonitorData(&pSystem->m_usCHWBCoolSubFreqTempDiff_2, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBCoolSubFreqTempDiff_2);
    //制热水泵降频供水温度偏差设定值
    m_pCHWBHeatSubFreqTempDeviat = new AnalogValButton(ui->frame_3);
    m_pCHWBHeatSubFreqTempDeviat->setDataParameter("℃", 1, 10, 50, 0, Monitor::Uint16t);
    m_pCHWBHeatSubFreqTempDeviat->setMonitorData(&pSystem->m_usCHWBHeatSubFreqTempDeviat, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBHeatSubFreqTempDeviat);
    //制热水泵降频供回水温差偏差设定值
    m_pCHWBHeatSubFreqTempDiff_1 = new AnalogValButton(ui->frame_3);
    m_pCHWBHeatSubFreqTempDiff_1->setDataParameter("℃", 1, 10, 50, 0, Monitor::Uint16t);
    m_pCHWBHeatSubFreqTempDiff_1->setMonitorData(&pSystem->m_usCHWBHeatSubFreqTempDiff_1, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBHeatSubFreqTempDiff_1);
    //制热水泵降频供回水温差偏差设定值
    m_pCHWBHeatSubFreqTempDiff_2 = new AnalogValButton(ui->frame_3);
    m_pCHWBHeatSubFreqTempDiff_2->setDataParameter("℃", 1, 5, 50, 0, Monitor::Uint16t);
    m_pCHWBHeatSubFreqTempDiff_2->setMonitorData(&pSystem->m_usCHWBHeatSubFreqTempDiff_2, Monitor::Uint16t);
    m_Widgets.append(m_pCHWBHeatSubFreqTempDiff_2);

    //产品条码1
    m_pProductID_1 = new AnalogValButton(ui->frame_3);
    m_pProductID_1->setDataParameter("", 0, 0, 65535, 0, Monitor::Uint16t);
    m_pProductID_1->setMonitorData(&pSystem->m_DTU.m_usProductID_1, Monitor::Uint16t);
    m_Widgets.append(m_pProductID_1);

    //产品条码2
    m_pProductID_2 = new AnalogValButton(ui->frame_3);
    m_pProductID_2->setDataParameter("", 0, 0, 65535, 0, Monitor::Uint16t);
    m_pProductID_2->setMonitorData(&pSystem->m_DTU.m_usProductID_2, Monitor::Uint16t);
    m_Widgets.append(m_pProductID_2);

    //产品条码3
    m_pProductID_3 = new AnalogValButton(ui->frame_3);
    m_pProductID_3->setDataParameter("", 0, 0, 65535, 0, Monitor::Uint16t);
    m_pProductID_3->setMonitorData(&pSystem->m_DTU.m_usProductID_3, Monitor::Uint16t);
    m_Widgets.append(m_pProductID_3);

    //产品条码4
    m_pProductID_4 = new AnalogValButton(ui->frame_3);
    m_pProductID_4->setDataParameter("", 0, 0, 65535, 0, Monitor::Uint16t);
    m_pProductID_4->setMonitorData(&pSystem->m_DTU.m_usProductID_4, Monitor::Uint16t);
    m_Widgets.append(m_pProductID_4);

    //产品条码5
    m_pProductID_5 = new AnalogValButton(ui->frame_3);
    m_pProductID_5->setDataParameter("", 0, 0, 65535, 0, Monitor::Uint16t);
    m_pProductID_5->setMonitorData(&pSystem->m_DTU.m_usProductID_5, Monitor::Uint16t);
    m_Widgets.append(m_pProductID_5);

    //产品条码6
    m_pProductID_6 = new AnalogValButton(ui->frame_3);
    m_pProductID_6->setDataParameter("", 0, 0, 65535, 0, Monitor::Uint16t);
    m_pProductID_6->setMonitorData(&pSystem->m_DTU.m_usProductID_6, Monitor::Uint16t);
    m_Widgets.append(m_pProductID_6);

    //产品条码7
    m_pProductID_7 = new AnalogValButton(ui->frame_3);
    m_pProductID_7->setDataParameter("", 0, 0, 65535, 0, Monitor::Uint16t);
    m_pProductID_7->setMonitorData(&pSystem->m_DTU.m_usProductID_7, Monitor::Uint16t);
    m_Widgets.append(m_pProductID_7);

    //产品条码8
    m_pProductID_8 = new AnalogValButton(ui->frame_3);
    m_pProductID_8->setDataParameter("", 0, 0, 65535, 0, Monitor::Uint16t);
    m_pProductID_8->setMonitorData(&pSystem->m_DTU.m_usProductID_8, Monitor::Uint16t);
    m_Widgets.append(m_pProductID_8);

    for (uint8_t i = 0, m = 0, n = 0, p=0; i < m_Widgets.count(); i++)
    {
        p = i % (LABEL_COLUMNS * LABEL_ROWS);
        m = p / LABEL_ROWS;
        n = p % LABEL_ROWS;

        if(m_Widgets[i]->parent() == ui->frame_3)
        {
            m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN  + m * DATA_LABEL_INTERVAL_H + 30,
                                      DATA_LABEL_UP_MARGIN + n * DATA_LABEL_INTERVAL_V,
                                      DATA_LABEL_SIZE);
        }
        else
        {
            m_Widgets[i]->setGeometry(DATA_LABEL_LEFT_MARGIN + m * DATA_LABEL_INTERVAL_H,
                                      DATA_LABEL_UP_MARGIN + n * DATA_LABEL_INTERVAL_V,
                                      DATA_LABEL_SIZE);
        }

    }
    connect(m_pExAirFanRatedVol, SIGNAL(valChanged(void*)), this, SLOT(paramSetBtnValChanged(void*)));
}


void ManuParamPage::paramSetBtnValChanged(void*)
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    pSystem->m_ulExAirFanRatedVol = uint32_t(m_pExAirFanRatedVol->getCurrentValue());
    pSystem->m_usExAirFanRatedVol_H = uint16_t(pSystem->m_ulExAirFanRatedVol / 65536);
    pSystem->m_usExAirFanRatedVol_L = uint16_t(pSystem->m_ulExAirFanRatedVol % 65536);

}

void ManuParamPage::on_pushButton_clicked()
{
    if(ui->stackedWidget->currentIndex() < ui->stackedWidget->count()-1)
    {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
    }
    System::getInstance()->m_uiOffLogCount = 0;
}

void ManuParamPage::on_pushButton_2_clicked()
{
    if(ui->stackedWidget->currentIndex() > 0)
    {
        ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
    }
    System::getInstance()->m_uiOffLogCount = 0;
}
