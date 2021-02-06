#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#include <QWidget>
#include "device.h"
#include "datalabel.h"
#include "textlabel.h"

namespace Ui {
class TempSensor;
}

class TempSensor : public Device
{
    Q_OBJECT
public:
    int16_t       m_sTemp;         //温度
    bool          m_xError;        //故障

    static uint8_t  m_usSensorCount;   //数量

private:
    QVector<QWidget*>   m_Widgets;

    DataLabel     *m_pTempLabel;    //温度

private:
    void initLabel();
    void initButton();

public:
    explicit TempSensor(QWidget *parent = nullptr);
    ~TempSensor();

private:
    Ui::TempSensor *ui;
};

#endif // TEMPSENSOR_H
