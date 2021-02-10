#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QWidget>
#include "modularairpage.h"
#include "modularchillerpage.h"
#include "bumppage.h"
#include "axialfanpage.h"
#include "valvepage.h"
#include "sensorpage.h"

namespace Ui {
class SystemMonitor;
}

class SystemMonitor : public QWidget
{
    Q_OBJECT


private:
    ModularAirPage      *m_pModularAirPage;
    ModularChillerPage  *m_pModularChillerPage;
    BumpPage            *m_pBumpPage;
    AxialFanPage        *m_pAxialFanPage;
    ValvePage           *m_pValvePage;
    SensorPage          *m_pSensorPage;

private:
    void initLabel();
    void initButton();
    void initForm();

public:
    explicit SystemMonitor(QWidget *parent = nullptr);
    ~SystemMonitor();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::SystemMonitor *ui;
};

#endif // SYSTEMMONITOR_H
