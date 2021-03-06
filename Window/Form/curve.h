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

    double   m_fAmbientIn_T = 0;
    double   m_fAmbientOut_T = 0;

    double   m_usAmbientIn_H = 0;
    double   m_usAmbientOut_H = 0;

    double   m_usCO2PPM = 0;
    double   m_ulTotalFreAir = 0;

    double   m_usChillerPower = 0;
    double   m_usBumpPower = 0;
    double   m_usAirPower_1 = 0;
    double   m_usAirPower_2 = 0;

    uint8_t m_usIndex;

private:
    qrealTimeCurve *curveTemp;
    qrealTimeCurve *curveHumi;
    qrealTimeCurve *curveCO2;
    qrealTimeCurve *curveAir;
    qrealTimeCurve *curvePower;

    qHistoryCurve *curveTempH;
    qHistoryCurve *curveHumiH;
    qHistoryCurve *curveCO2H;
    qHistoryCurve *curveAirH;
    qHistoryCurve *curvePowerH;

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
    void on_previousButton_clicked();
    void on_nextButton_clicked();
    void on_humiButton_clicked();
    void on_powerButton_clicked();
};

#endif // CURVE_H
