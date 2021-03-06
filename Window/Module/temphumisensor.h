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
    int16_t       m_sTemp = 0;         //温度
    uint16_t      m_usHumi = 0;        //湿度

    bool          m_xTempErr = 0;   //温度故障
    bool          m_xHumiErr = 0;   //湿度故障

private:
    QVector<QWidget*>   m_Widgets;
    DataLabel     *m_pTempLabel;    //温度
    DataLabel     *m_pHumiLabel;    //湿度
    Monitor       *m_pTempErrMonitor;  //故障监控；
    Monitor       *m_pHumiErrMonitor;  //故障监控；

private:
    void initLabel();
    void initButton();

public:
    explicit TempHumiSensor(const QString strSensorType, uint8_t ucIndex, QWidget *parent = nullptr);
    ~TempHumiSensor();

private slots:
    void tempValChangedSlot(Monitor* pMonitor);
    void humiValChangedSlot(Monitor* pMonitor);

private:
    Ui::TempHumiSensor *ui;
};

#endif // TEMPHUMISENSOR_H
