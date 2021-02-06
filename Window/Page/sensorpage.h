#ifndef SENSORPAGE_H
#define SENSORPAGE_H

#include <QWidget>
#include "temphumisensor.h"
#include "tempsensor.h"
#include "co2sensor.h"
#include "pressuresensor.h"

namespace Ui {
class SensorPage;
}

class SensorPage : public QWidget
{
    Q_OBJECT

private:
    QVector<QWidget*>   m_iTempHumiWidgets;
    QVector<QWidget*>   m_TempWidgets;
    QVector<QWidget*>   m_CO2Widgets;
    QVector<QWidget*>   m_PressureWidgets;
    QVector<QWidget*>   m_oTempHumiWidgets;

private:
    void initDevice();
    void initLabel();
    void initButton();

public:
    explicit SensorPage(QWidget *parent = nullptr);
    ~SensorPage();

private:
    Ui::SensorPage *ui;
};

#endif // SENSORPAGE_H
