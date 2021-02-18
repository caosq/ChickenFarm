#include "curve.h"
#include "ui_curve.h"

#define HY1 610
#define HY2 650
#define HY3 670
#define HY4 730

#define VX1 30
#define VX2 150
#define VX3 270
#define VX4 630

#define VX5 700
#define VX6 750

#define SIZE 100,30
#define LABELSIZE 130,22


Curve::Curve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Curve)
{
    ui->setupUi(this);

    fGoTdata = 500;
    fBackTdata = 100;
    fGoPdata = 2000;
    fBackPdata = 100000;

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

    ui->curveStackedWidget->insertWidget(0, curveTemp);
    ui->curveStackedWidget->insertWidget(1, curveCO2);
    ui->curveStackedWidget->insertWidget(2, curveAir);
    ui->curveStackedWidget->insertWidget(3, curveTempH);
    ui->curveStackedWidget->insertWidget(4, curveCO2H);
    ui->curveStackedWidget->insertWidget(5, curveAirH);
}

//初始化按键显示，初始化窗口显示，初始化按键效果
void Curve::initButton()
{
    pHistoryBtn = new ubutton(this);
    pHistoryBtn->setText(tr("历史曲线"));
    pHistoryBtn->setGeometry(VX6,HY3,SIZE);

    pCurrentBtn = new ubutton(this);
    pCurrentBtn->setText(tr("当前曲线"));
    pCurrentBtn->setGeometry(VX4,HY3,SIZE);

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
    curveTemp->setYAxisScale(-200,500,50);
    curveTemp->setCurveAttribute(1," ℃");
    curveTemp->enableTouchMark(true);
    curveTemp->setCurve(0,&fGoTdata,"FreezeGoTemp",Qt::red);
    curveTemp->setCurve(1,&fBackTdata,"FreezeBackTemp",Qt::yellow);
    curveTemp->setSaveDirName("Temp");
    curveTemp->setSamplingTime(10);
    curveTemp->startSampling();

    //CO2曲线窗口
    curveCO2 = new qrealTimeCurve;
    curveCO2->setYAxisScale(0,5000,200);
    curveCO2->setCurveAttribute(0," ppm");
    curveCO2->enableTouchMark(true);
    curveCO2->setCurve(0,&fGoPdata,"FreezeGoPressure",Qt::red);
    curveCO2->setSaveDirName("CO2");
    curveCO2->setSamplingTime(10);
    curveCO2->startSampling();

    //新风量曲线窗口
    curveAir = new qrealTimeCurve;
    curveAir->setYAxisScale(0,130000,10000);
    curveAir->setCurveAttribute(0," m³/h");
    curveAir->enableTouchMark(true);

    curveAir->setCurve(0,&fGoPdata,"FreezeGoPressure",Qt::red);

    curveAir->setSaveDirName("Air");
    curveAir->setSamplingTime(10);
    curveAir->startSampling();
}

void Curve::setHistoCurve()
{
    curveTempH = new qHistoryCurve(this);
    curveTempH->setYAxisScale(-200,500,50);
    curveTempH->setCurveAttribute(1," ℃");
    curveTempH->setSamplingTime(10);
    curveTempH->setHistoryDir("Temp");
    curveTempH->addCurve(0,Qt::red);
    curveTempH->addCurve(1,Qt::yellow);

    curveCO2H = new qHistoryCurve(this);
    curveCO2H->setYAxisScale(0,5000,200);
    curveCO2H->setCurveAttribute(0," ppm");
    curveCO2H->setSamplingTime(10);
    curveCO2H->setHistoryDir("CO2");
    curveCO2H->addCurve(0,Qt::red);


    curveAirH = new qHistoryCurve(this);
    curveAirH->setYAxisScale(0,130000,10000);
    curveAirH->setCurveAttribute(0," m³/h");
    curveAirH->setSamplingTime(10);
    curveAirH->setHistoryDir("Air");
    curveAirH->addCurve(0,Qt::red);
}

void Curve::timerEvent(QTimerEvent *)
{

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
        ui->curveStackedWidget->setCurrentWidget(curveCO2H);
    }
    else if (m_usIndex == 2)
    {
        ui->curveStackedWidget->setCurrentWidget(curveAirH);
    }
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
        ui->curveStackedWidget->setCurrentWidget(curveCO2);
    }
    else if (m_usIndex == 2)
    {
        ui->curveStackedWidget->setCurrentWidget(curveAir);
    }
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

    red_show->show();
    yellow_show->show();
    gree_show->hide();
    white_show->hide();

    _labelFirst->show();
    _labelSecond->show();
    _labelThird->hide();
    _labelFour->hide();

    _labelFirst->setText(tr("室内温度"),14);
    _labelSecond->setText(tr("室外温度"),14);
}

void Curve::on_co2Button_clicked()
{
    m_usIndex = 1;
    currentClick();

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->co2Button->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    ui->airButton->setPalette(pe);
    ui->tempButton->setPalette(pe);

    red_show->show();
    yellow_show->hide();
    gree_show->hide();
    white_show->hide();

    _labelFirst->show();
    _labelSecond->hide();
    _labelThird->hide();
    _labelFour->hide();

    _labelFirst->setText(tr("CO2浓度"),14);
}

void Curve::on_airButton_clicked()
{
    m_usIndex = 2;
    currentClick();

    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::white);
    ui->airButton->setPalette(pe);

    pe.setColor(QPalette::ButtonText,Qt::gray);
    ui->co2Button->setPalette(pe);
    ui->tempButton->setPalette(pe);

    red_show->show();
    yellow_show->hide();
    gree_show->hide();
    white_show->hide();

    _labelFirst->show();
    _labelSecond->hide();
    _labelThird->hide();
    _labelFour->hide();

    _labelFirst->setText(tr("新风量"),14);
}
