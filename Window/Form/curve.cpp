#include "curve.h"
#include "ui_curve.h"
#include "system.h"

#define HY1 610
#define HY2 650
#define HY3 670
#define HY4 730

#define VX1 30
#define VX2 150
#define VX3 270
#define VX4 630

#define VX5 700
#define VX6 770

#define SIZE 100,30
#define LABELSIZE 150,22


Curve::Curve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Curve)
{
    ui->setupUi(this);

    initPolt();
    initLabel();
    initButton();
}

Curve::~Curve()
{
    delete ui;
}

void Curve::initPolt()
{
    setCurrCurve();
    setHistoCurve();

    ui->curveStackedWidget->addWidget(curveTemp);
    ui->curveStackedWidget->addWidget(curveHumi);
    ui->curveStackedWidget->addWidget(curveCO2);
    ui->curveStackedWidget->addWidget(curveAir);
     ui->curveStackedWidget->addWidget(curvePower);

    ui->curveStackedWidget->addWidget(curveTempH);
    ui->curveStackedWidget->addWidget(curveHumiH);
    ui->curveStackedWidget->addWidget(curveCO2H);
    ui->curveStackedWidget->addWidget(curveAirH);
    ui->curveStackedWidget->addWidget(curvePowerH);

    startTimer(3*1000);
}

//初始化按键显示，初始化窗口显示，初始化按键效果
void Curve::initButton()
{
    pHistoryBtn = new ubutton(this);
    pHistoryBtn->setText(tr("历史曲线"));
    pHistoryBtn->setGeometry(VX6,HY3,SIZE);

    pCurrentBtn = new ubutton(this);
    pCurrentBtn->setText(tr("当前曲线"));
    pCurrentBtn->setGeometry(VX5,HY3,SIZE);

    connect(pHistoryBtn,SIGNAL(clicked()),this,SLOT(historyClick()));
    connect(pCurrentBtn,SIGNAL(clicked()),this,SLOT(currentClick()));

    on_tempButton_clicked();
}

void Curve::initLabel()
{
    red_show= new gQwtLegendItem(this);
    red_show->setIconStyle(gQwtLegendItem::drawIcon);
    red_show->setIconColor(Qt::red);
    red_show->setNoBackground(true);
    red_show->setGeometry(25,HY1,22,22);

    yellow_show= new gQwtLegendItem(this);
    yellow_show->setIconStyle(gQwtLegendItem::drawIcon);
    yellow_show->setIconColor(Qt::yellow);
    yellow_show->setNoBackground(true);
    yellow_show->setGeometry(175,HY1,22,22);

    gree_show= new gQwtLegendItem(this);
    gree_show->setIconStyle(gQwtLegendItem::drawIcon);
    gree_show->setIconColor(Qt::green);
    gree_show->setNoBackground(true);
    gree_show->setGeometry(375,HY1,22,22);

    white_show= new gQwtLegendItem(this);
    white_show->setIconStyle(gQwtLegendItem::drawIcon);
    white_show->setIconColor(Qt::white);
    white_show->setNoBackground(true);
    white_show->setGeometry(575,HY1,22,22);

    _labelFirst = new TextLabel(this);
    _labelSecond = new TextLabel(this);
    _labelThird = new TextLabel(this);
    _labelFour = new TextLabel(this);

    _labelFirst->setText(tr("室内温度"),12);
    _labelSecond->setText(tr("室外温度"),12);
    _labelThird->setText(tr("冷冻总管供水温度"),12);
    _labelFour->setText(tr("冷冻总管回水温度"),15);

    _labelFirst->setGeometry(50,HY1,LABELSIZE);
    _labelSecond->setGeometry(200,HY1,LABELSIZE);
    _labelThird->setGeometry(400,HY1,LABELSIZE);
    _labelFour->setGeometry(600,HY1,LABELSIZE);
}

void Curve::setCurrCurve()
{
    //温度曲线窗口
    curveTemp = new qrealTimeCurve();
    curveTemp->setYAxisScale(-200,500,40);
    curveTemp->setCurveAttribute(1," ℃");
    curveTemp->enableTouchMark(true);
    curveTemp->setCurve(0, &m_fAmbientIn_T, "AmbientInTemp", Qt::red);
    curveTemp->setCurve(1, &m_fAmbientOut_T, "AmbientOutTemp", Qt::yellow);
    curveTemp->setSaveDirName("Temp");
    curveTemp->setSamplingTime(10);
    curveTemp->startSampling();

    //湿度曲线窗口
    curveHumi = new qrealTimeCurve;
    curveHumi->setYAxisScale(0,1000,40);
    curveHumi->setCurveAttribute(1," %");
    curveHumi->enableTouchMark(true);
    curveHumi->setCurve(0, &m_usAmbientIn_H, "AmbientInHumi", Qt::red);
    curveHumi->setCurve(1, &m_usAmbientOut_H, "AmbientOutHumi", Qt::yellow);
    curveHumi->setSaveDirName("Humi");
    curveHumi->setSamplingTime(10);
    curveHumi->startSampling();

    //CO2曲线窗口
    curveCO2 = new qrealTimeCurve;
    curveCO2->setYAxisScale(0,50000,2500);
    curveCO2->setCurveAttribute(1," ppm");
    curveCO2->enableTouchMark(true);
    curveCO2->setCurve(0, &m_usCO2PPM, "CO2PPM", Qt::red);
    curveCO2->setSaveDirName("CO2");
    curveCO2->setSamplingTime(10);
    curveCO2->startSampling();

    //新风量曲线窗口
    curveAir = new qrealTimeCurve;
    curveAir->setYAxisScale(0,130000,10000);
    curveAir->setCurveAttribute(0," m³/h");
    curveAir->enableTouchMark(true);
    curveAir->setCurve(0, &m_ulTotalFreAir, "TotalFreAir",Qt::red);
    curveAir->setSaveDirName("FreAir");
    curveAir->setSamplingTime(10);
    curveAir->startSampling();

    //功率曲线窗口
    curvePower = new qrealTimeCurve;
    curvePower->setYAxisScale(0,10000,500);
    curvePower->setCurveAttribute(0," kW");
    curvePower->enableTouchMark(true);
    curvePower->setCurve(0, &m_usChillerPower, "ChillerPower",Qt::red);
    curvePower->setCurve(1, &m_usBumpPower,    "BumpPower",   Qt::yellow);
    curvePower->setCurve(2, &m_usAirPower_1,   "AirPower_1",  Qt::white);
    curvePower->setCurve(3, &m_usAirPower_2,   "AirPower_2",  Qt::green);
    curvePower->setSaveDirName("Power");
    curvePower->setSamplingTime(10);
    curvePower->startSampling();
}

void Curve::setHistoCurve()
{
    curveTempH = new qHistoryCurve(this);
    curveTempH->setYAxisScale(-200,500,40);
    curveTempH->setCurveAttribute(1," ℃");
    curveTempH->setSamplingTime(10);
    curveTempH->setHistoryDir("Temp");
    curveTempH->addCurve(0,Qt::red);
    curveTempH->addCurve(1,Qt::yellow);

    curveHumiH = new qHistoryCurve(this);
    curveHumiH->setYAxisScale(0,1000,50);
    curveHumiH->setCurveAttribute(1," %");
    curveHumiH->setSamplingTime(10);
    curveHumiH->setHistoryDir("Humi");
    curveHumiH->addCurve(0,Qt::red);
    curveHumiH->addCurve(1,Qt::yellow);

    curveCO2H = new qHistoryCurve(this);
    curveCO2H->setYAxisScale(0,50000,2500);
    curveCO2H->setCurveAttribute(1,"ppm");
    curveCO2H->setSamplingTime(10);
    curveCO2H->setHistoryDir("CO2");
    curveCO2H->addCurve(0,Qt::red);

    curveAirH = new qHistoryCurve(this);
    curveAirH->setYAxisScale(0,130000,10000);
    curveAirH->setCurveAttribute(0," m³/h");
    curveAirH->setSamplingTime(10);
    curveAirH->setHistoryDir("TotalFreAir");
    curveAirH->addCurve(0,Qt::red);

    curvePowerH = new qHistoryCurve(this);
    curvePowerH->setYAxisScale(0,10000,500);
    curvePowerH->setCurveAttribute(1," kW");
    curvePowerH->setSamplingTime(10);
    curvePowerH->setHistoryDir("Power");
    curvePowerH->addCurve(0,Qt::red);
    curvePowerH->addCurve(1,Qt::yellow);
    curvePowerH->addCurve(2,Qt::white);
    curvePowerH->addCurve(3,Qt::green);

}

void Curve::timerEvent(QTimerEvent *)
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    m_fAmbientIn_T = pSystem->m_sAmbientIn_T;
    m_fAmbientOut_T = pSystem->m_sAmbientOut_T;

    m_usAmbientIn_H = pSystem->m_usAmbientIn_H;
    m_usAmbientOut_H = pSystem->m_usAmbientOut_H;

    m_usCO2PPM = pSystem->m_usCO2PPM;
    m_ulTotalFreAir = pSystem->m_ulTotalFreAir;

    m_usChillerPower = pSystem->m_pChillerMeter->m_usPower;
    m_usBumpPower = pSystem->m_pBumpMeter->m_usPower;
    m_usAirPower_1 = pSystem->m_pModularAirs[0]->m_sMeter.m_usPower;
    m_usAirPower_2 = pSystem->m_pModularAirs[1]->m_sMeter.m_usPower;

}

void Curve::historyClick()
{
    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    pHistoryBtn->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    pCurrentBtn->setPalette(pe);

    ui->previousButton->show();
    ui->nextButton->show();

    if(m_usIndex == 0)
    {
        ui->curveStackedWidget->setCurrentWidget(curveTempH);
    }
    else if (m_usIndex == 1)
    {
        ui->curveStackedWidget->setCurrentWidget(curveHumiH);
    }
    else if (m_usIndex == 2)
    {
        ui->curveStackedWidget->setCurrentWidget(curveCO2H);
    }
    else if (m_usIndex == 3)
    {
        ui->curveStackedWidget->setCurrentWidget(curveAirH);
    }
    else if (m_usIndex == 4)
    {
        ui->curveStackedWidget->setCurrentWidget(curvePowerH);
    }
    System::getInstance()->m_uiOffLogCount = 0;
}

void Curve::currentClick()
{
    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    pCurrentBtn->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    pHistoryBtn->setPalette(pe);

    ui->previousButton->hide();
    ui->nextButton->hide();

    if(m_usIndex == 0)
    {
        ui->curveStackedWidget->setCurrentWidget(curveTemp);
    }
    else if (m_usIndex == 1)
    {
        ui->curveStackedWidget->setCurrentWidget(curveHumi);
    }
    else if (m_usIndex == 2)
    {
        ui->curveStackedWidget->setCurrentWidget(curveCO2);
    }
    else if (m_usIndex == 3)
    {
        ui->curveStackedWidget->setCurrentWidget(curveAir);
    }
    else if (m_usIndex == 4)
    {
        ui->curveStackedWidget->setCurrentWidget(curvePower);
    }
    System::getInstance()->m_uiOffLogCount = 0;
}

void Curve::on_tempButton_clicked()
{
    m_usIndex = 0;
    currentClick();

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->tempButton->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    ui->co2Button->setPalette(pe);
    ui->airButton->setPalette(pe);
    ui->humiButton->setPalette(pe);
    ui->powerButton->setPalette(pe);

    red_show->show();
    yellow_show->show();
    gree_show->hide();
    white_show->hide();

    _labelFirst->show();
    _labelSecond->show();
    _labelThird->hide();
    _labelFour->hide();

    _labelFirst->setText(tr("室内环境温度"),14);
    _labelSecond->setText(tr("室外环境温度"),14);

    System::getInstance()->m_uiOffLogCount = 0;
}

void Curve::on_humiButton_clicked()
{
    m_usIndex = 1;
    currentClick();

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->humiButton->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    ui->co2Button->setPalette(pe);
    ui->airButton->setPalette(pe);
    ui->tempButton->setPalette(pe);
    ui->powerButton->setPalette(pe);

    red_show->show();
    yellow_show->show();
    gree_show->hide();
    white_show->hide();

    _labelFirst->show();
    _labelSecond->show();
    _labelThird->hide();
    _labelFour->hide();

    _labelFirst->setText(tr("室内环境湿度"),14);
    _labelSecond->setText(tr("室外环境湿度"),14);

     System::getInstance()->m_uiOffLogCount = 0;
}


void Curve::on_co2Button_clicked()
{
    m_usIndex = 2;
    currentClick();

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->co2Button->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    ui->airButton->setPalette(pe);
    ui->tempButton->setPalette(pe);
    ui->humiButton->setPalette(pe);
    ui->powerButton->setPalette(pe);

    red_show->show();
    yellow_show->hide();
    gree_show->hide();
    white_show->hide();

    _labelFirst->show();
    _labelSecond->hide();
    _labelThird->hide();
    _labelFour->hide();

    _labelFirst->setText(tr("室内CO2浓度"),14);
    System::getInstance()->m_uiOffLogCount = 0;
}

void Curve::on_airButton_clicked()
{
    m_usIndex = 3;
    currentClick();

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->airButton->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    ui->co2Button->setPalette(pe);
    ui->tempButton->setPalette(pe);
    ui->humiButton->setPalette(pe);
    ui->powerButton->setPalette(pe);

    red_show->show();
    yellow_show->hide();
    gree_show->hide();
    white_show->hide();

    _labelFirst->show();
    _labelSecond->hide();
    _labelThird->hide();
    _labelFour->hide();

    _labelFirst->setText(tr("系统新风量"),14);
    System::getInstance()->m_uiOffLogCount = 0;
}

void Curve::on_powerButton_clicked()
{
    m_usIndex = 4;
    currentClick();

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->powerButton->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    ui->co2Button->setPalette(pe);
    ui->tempButton->setPalette(pe);
    ui->humiButton->setPalette(pe);
    ui->airButton->setPalette(pe);

    red_show->show();
    yellow_show->show();
    gree_show->show();
    white_show->show();

    _labelFirst->show();
    _labelSecond->show();
    _labelThird->show();
    _labelFour->show();

    _labelFirst->setText(tr("机组耗电功率"),14);
    _labelSecond->setText(tr("冷冻水泵耗电功率"),14);
    _labelThird->setText(tr("1#组合柜耗电功率"),14);
    _labelFour->setText(tr("2#组合柜耗电功率"),14);

    System::getInstance()->m_uiOffLogCount = 0;

}

void Curve::on_previousButton_clicked()
{
    if(m_usIndex == 0)
    {
        curveTempH->previousDay();
    }
    else if (m_usIndex == 1)
    {
        curveHumiH->previousDay();
    }
    else if (m_usIndex == 2)
    {
        curveCO2H->previousDay();
    }
    else if (m_usIndex == 3)
    {
        curveAirH->previousDay();
    }
    else if (m_usIndex == 3)
    {
        curveAirH->previousDay();
    }
    else if (m_usIndex == 4)
    {
        curvePowerH->previousDay();
    }
    System::getInstance()->m_uiOffLogCount = 0;
}

void Curve::on_nextButton_clicked()
{
    if(m_usIndex == 0)
    {
        curveTempH->nextDay();
    }
    else if (m_usIndex == 1)
    {
        curveHumiH->nextDay();
    }
    else if (m_usIndex == 2)
    {
        curveCO2H->nextDay();
    }
    else if (m_usIndex == 3)
    {
        curveAirH->nextDay();
    }
    else if (m_usIndex == 4)
    {
        curvePowerH->nextDay();
    }
    System::getInstance()->m_uiOffLogCount = 0;
}



