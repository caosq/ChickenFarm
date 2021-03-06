#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include <QWidget>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"

namespace Ui {
class PressureSensor;
}

class PressureSensor : public Device
{
    Q_OBJECT
public:
    uint16_t      m_usPressure = 0;      //压力
    bool          m_xError = 0;          //故障
    static uint8_t  m_usSensorCount;    //数量

private:
    QVector<QWidget*>   m_Widgets;
    DataLabel     *m_pPressureLabel;    //压力
    Monitor    *m_pErrMonitor;  //故障监控；

private:
    void initLabel();
    void initButton();

public:
    explicit PressureSensor(QWidget *parent = nullptr);
    ~PressureSensor();

private slots:
    void valChangedSlot(Monitor* pMonitor);

private:
    Ui::PressureSensor *ui;
};

#endif // PRESSURESENSOR_H
