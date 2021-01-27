#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#include <QWidget>

namespace Ui {
class TempSensor;
}

class TempSensor : public QWidget
{
    Q_OBJECT

public:
    explicit TempSensor(QWidget *parent = nullptr);
    ~TempSensor();

private:
    Ui::TempSensor *ui;
};

#endif // TEMPSENSOR_H
