#ifndef TEMPHUMISENSOR_H
#define TEMPHUMISENSOR_H

#include <QWidget>
#include <QMap>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"

namespace Ui {
class TempHumiSensor;
}

class TempHumiSensor : public Device
{
    Q_OBJECT

public:

    QString       m_strSensorType; //传感器类型
    int16_t       m_sTemp;         //温度
    uint16_t      m_usHumi;        //湿度

    bool          m_xTempErr;   //温度故障
    bool          m_xHumiErr;   //湿度故障

private:
    QVector<QWidget*>   m_Widgets;

    DataLabel     *m_pTempLabel;    //温度
    DataLabel     *m_pHumiLabel;    //湿度

private:
    void initLabel();
    void initButton();

public:
    explicit TempHumiSensor(const QString strSensorType, uint8_t ucIndex, QWidget *parent = nullptr);
    ~TempHumiSensor();

private:
    Ui::TempHumiSensor *ui;
};

#endif // TEMPHUMISENSOR_H
