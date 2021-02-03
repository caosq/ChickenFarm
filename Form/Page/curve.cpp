#include "curve.h"
#include "ui_curve.h"

#define HY1 610
#define HY2 670
#define HY3 600
#define HY4 630

#define VX1 50
#define VX2 150
#define VX3 250
#define VX4 750

#define VX5 630
#define VX6 750

#define SIZE 100,30
#define LABELSIZE 130,22


Curve::Curve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Curve)
{
    ui->setupUi(this);

    fGoTdata = 50;
    fBackTdata = 100;
    fGoPdata = 150;
    fBackPdata = 200;

    initPolt();
    initButton();
    initLabel();
}

Curve::~Curve()
{
    delete ui;
}

void Curve::initPolt()
{
    setCurrCurve();
    setHistoCurve();

    ui->curveStackedWidget->insertWidget(0, curveTempWidget);
    ui->curveStackedWidget->insertWidget(1, curveCO2Widget);//压力
    ui->curveStackedWidget->insertWidget(2, curveAirWidget);//比率
    ui->curveStackedWidget->insertWidget(3, curveTempWidgetH);//溫度历史
    ui->curveStackedWidget->insertWidget(4, curveCO2WidgetH);//压力历史
    ui->curveStackedWidget->insertWidget(5, curveAirWidgetH);//比率历史

}

//初始化按键显示，初始化窗口显示，初始化按键效果
void Curve::initButton()
{
    historyBtt = new ubutton(this);
    currentBtt = new ubutton(this);
    TempBtt = new ubutton(this);
    CO2Btt = new ubutton(this);
    AirBtt = new ubutton(this);
    nextBtt = new ubutton(this);
    previousBtt = new ubutton(this);



    historyBtt->setGeometry(VX4,HY2,SIZE);
    currentBtt->setGeometry(VX4,HY2,SIZE);
    TempBtt->setGeometry(VX1,HY2,SIZE);
    CO2Btt->setGeometry(VX2,HY2,SIZE);
    AirBtt->setGeometry(VX3,HY2,SIZE);
    previousBtt->setGeometry(VX5,HY1,SIZE);
    nextBtt->setGeometry(VX6,HY1,SIZE);


    historyBtt->setText(tr("历史曲线"));
    currentBtt->setText(tr("当前曲线"));
    TempBtt->setText(tr("温度"));
    CO2Btt->setText(tr("压力"));
    AirBtt->setText(tr("能效比"));
    previousBtt->setText(tr("上一天"));
    nextBtt->setText(tr("下一天"));
}

void Curve::initLabel()
{
    red_show= new gQwtLegendItem(this);
    red_show->setIconStyle(gQwtLegendItem::drawIcon);
    red_show->setIconColor(Qt::red);
    red_show->setNoBackground(true);
    red_show->setGeometry(25,HY3,22,22);

    yellow_show= new gQwtLegendItem(this);
    yellow_show->setIconStyle(gQwtLegendItem::drawIcon);
    yellow_show->setIconColor(Qt::yellow);
    yellow_show->setNoBackground(true);
    yellow_show->setGeometry(225,HY3,22,22);

    gree_show= new gQwtLegendItem(this);
    gree_show->setIconStyle(gQwtLegendItem::drawIcon);
    gree_show->setIconColor(Qt::green);
    gree_show->setNoBackground(true);
    gree_show->setGeometry(25,HY4,22,22);

    white_show= new gQwtLegendItem(this);
    white_show->setIconStyle(gQwtLegendItem::drawIcon);
    white_show->setIconColor(Qt::white);
    white_show->setNoBackground(true);
    white_show->setGeometry(225,HY4,22,22);



    _labelFirst = new TextLabel(this);
    _labelSecond = new TextLabel(this);
    _labelThird = new TextLabel(this);
    _labelFour = new TextLabel(this);


    _labelFirst->setText(tr("冷冻总管供水温度"),10);
    _labelSecond->setText(tr("冷冻总管回水温度"),12);
    _labelThird->setText(tr("冷冻总管供水温度"),12);
    _labelFour->setText(tr("冷冻总管回水温度"),15);


    _labelFirst->setGeometry(50,HY3,LABELSIZE);
    _labelSecond->setGeometry(250,HY3,LABELSIZE);
    _labelThird->setGeometry(50,HY4,LABELSIZE);
    _labelFour->setGeometry(250,HY4,LABELSIZE);

}

void Curve::setCurrCurve()
{
    //温度曲线窗口
    curveTempWidget = new qrealTimeCurve();
    curveTempWidget->setYAxisScale(-200,1000,100);
    curveTempWidget->setCurveAttribute(1,"℃");
    curveTempWidget->enableTouchMark(true);

    curveTempWidget->setCurve(0,&fGoTdata,"FreezeGoTemp",Qt::red);
    curveTempWidget->setCurve(1,&fBackTdata,"FreezeBackTemp",Qt::yellow);
    curveTempWidget->setCurve(2,&fGoPdata,"coolGoTemp",Qt::green);
    curveTempWidget->setCurve(3,&fBackPdata,"coolBackTemp",Qt::white);


    curveTempWidget->setSaveDirName("temperature");
    curveTempWidget->setSamplingTime(10);
    curveTempWidget->startSampling();

    //CO2曲线窗口
    curveCO2Widget = new qrealTimeCurve;
    curveCO2Widget->setYAxisScale(0,32000,1000);
    curveCO2Widget->setCurveAttribute(1,"kPa");
    curveCO2Widget->enableTouchMark(true);

    curveCO2Widget->setCurve(0,&fGoPdata,"FreezeGoPressure",Qt::red);

    curveCO2Widget->setSaveDirName("Pressure");
    curveCO2Widget->setSamplingTime(10);
    curveCO2Widget->startSampling();

    //新风量曲线窗口
    curveAirWidget = new qrealTimeCurve;
    curveAirWidget->setYAxisScale(0,32000,1000);
    curveAirWidget->setCurveAttribute(1,"kPa");
    curveAirWidget->enableTouchMark(true);

    curveAirWidget->setCurve(0,&fGoPdata,"FreezeGoPressure",Qt::red);

    curveAirWidget->setSaveDirName("Pressure");
    curveAirWidget->setSamplingTime(10);
    curveAirWidget->startSampling();
}

void Curve::setHistoCurve()
{
    curveTempWidgetH = new qHistoryCurve(this);
    curveTempWidgetH->setYAxisScale(-200,1000,100);
    curveTempWidgetH->setCurveAttribute(1,"℃");
    curveTempWidgetH->setSamplingTime(10);
    curveTempWidgetH->setHistoryDir("temperature");
    curveTempWidgetH->addCurve(0,Qt::red);
    curveTempWidgetH->addCurve(1,Qt::yellow);
    curveTempWidgetH->addCurve(2,Qt::green);
    curveTempWidgetH->addCurve(3,Qt::white);

    curveCO2WidgetH = new qHistoryCurve(this);
    curveCO2WidgetH->setYAxisScale(0,32000,1000);
    curveCO2WidgetH->setCurveAttribute(1,"ppm");
    curveCO2WidgetH->setSamplingTime(10);
    curveCO2WidgetH->setHistoryDir("Pressure");
    curveCO2WidgetH->addCurve(0,Qt::red);
    curveCO2WidgetH->addCurve(1,Qt::yellow);

    curveAirWidgetH = new qHistoryCurve(this);
    curveAirWidgetH->setYAxisScale(0,100,10);
    curveAirWidgetH->setCurveAttribute(1,"");
    curveAirWidgetH->setSamplingTime(10);
    curveAirWidgetH->setHistoryDir("Ratio");
    curveAirWidgetH->addCurve(0,Qt::red);
    curveAirWidgetH->addCurve(1,Qt::yellow);
    curveAirWidgetH->addCurve(3,Qt::green);
    curveAirWidgetH->addCurve(4,Qt::white);
}

void Curve::timerEvent(QTimerEvent *)
{

}
