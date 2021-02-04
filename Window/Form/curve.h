#ifndef CURVE_H
#define CURVE_H

#include <QWidget>
#include "qrealtimecurve.h"
#include "qhistorycurve.h"
#include "ubutton.h"
#include "textlabel.h"

namespace Ui {
class Curve;
}

class Curve : public QWidget
{
    Q_OBJECT

public:
    explicit Curve(QWidget *parent = nullptr);

    void initPolt();
    void initButton();
    void initLabel();

    ~Curve();

protected:
    void timerEvent(QTimerEvent *e);
private:
    void setCurrCurve();
    void setHistoCurve();

private:
    Ui::Curve *ui;

    double fGoTdata;
    double fBackTdata;
    double fGoPdata;
    double fBackPdata;

    double cGoTdata;
    double cBackTdata;
    double crrentEER;
    double dEER;
    double mEER;
    double yEER;
    double totalEER;

private:

    qrealTimeCurve *curveTempWidget;
    qrealTimeCurve *curveCO2Widget;
    qrealTimeCurve *curveAirWidget;

    qHistoryCurve *curveTempWidgetH;
    qHistoryCurve *curveCO2WidgetH;
    qHistoryCurve *curveAirWidgetH;

    ubutton *TempBtt;
    ubutton *CO2Btt;
    ubutton *AirBtt;

    ubutton *historyBtt;
    ubutton *currentBtt;

    ubutton *nextBtt;
    ubutton *previousBtt;

    gQwtLegendItem *red_show;
    gQwtLegendItem *yellow_show;
    gQwtLegendItem *gree_show;
    gQwtLegendItem *white_show;


    TextLabel *_labelFirst;
    TextLabel *_labelSecond;
    TextLabel *_labelThird;
    TextLabel *_labelFour;


};

#endif // CURVE_H
