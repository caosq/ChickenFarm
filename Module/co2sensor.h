#ifndef CO2SENSOR_H
#define CO2SENSOR_H

#include <QWidget>

namespace Ui {
class CO2Sensor;
}

class CO2Sensor : public QWidget
{
    Q_OBJECT

public:
    explicit CO2Sensor(QWidget *parent = nullptr);
    ~CO2Sensor();

private:
    Ui::CO2Sensor *ui;
};

#endif // CO2SENSOR_H
