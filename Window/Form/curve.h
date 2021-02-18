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

    uint8_t m_usIndex;

private:
    qrealTimeCurve *curveTemp;
    qrealTimeCurve *curveCO2;
    qrealTimeCurve *curveAir;

    qHistoryCurve *curveTempH;
    qHistoryCurve *curveCO2H;
    qHistoryCurve *curveAirH;

    ubutton *pTempBtn;
    ubutton *pCO2Btn;
    ubutton *pAirBtn;

    ubutton *pHistoryBtn;
    ubutton *pCurrentBtn;

    gQwtLegendItem *red_show;
    gQwtLegendItem *yellow_show;
    gQwtLegendItem *gree_show;
    gQwtLegendItem *white_show;


    TextLabel *_labelFirst;
    TextLabel *_labelSecond;
    TextLabel *_labelThird;
    TextLabel *_labelFour;

private:
    void initPolt();
    void initButton();
    void initLabel();

public:
    explicit Curve(QWidget *parent = nullptr);
    ~Curve();

protected:
    void timerEvent(QTimerEvent *e);
private:
    void setCurrCurve();
    void setHistoCurve();

private slots:

    void historyClick();
    void currentClick();

    void on_tempButton_clicked();
    void on_co2Button_clicked();
    void on_airButton_clicked();
};

#endif // CURVE_H
