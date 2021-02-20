#ifndef CO2SENSOR_H
#define CO2SENSOR_H

#include <QWidget>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"

namespace Ui {
class CO2Sensor;
}

class CO2Sensor : public Device
{
    Q_OBJECT
public:
    uint16_t m_usCO2ppm = 0;      //CO2浓度
    bool     m_xError = 0;        //故障

    static uint8_t  m_usSensorCount;   //数量

private:
    QVector<QWidget*>   m_Widgets;
    DataLabel        *m_pCO2Label;    //CO2浓度

private:
    void initLabel();
    void initButton();

public:
    explicit CO2Sensor(QWidget *parent = nullptr);
    ~CO2Sensor();

private:
    Ui::CO2Sensor *ui;
};

#endif // CO2SENSOR_H
