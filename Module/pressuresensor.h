#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include <QWidget>

namespace Ui {
class PressureSensor;
}

class PressureSensor : public QWidget
{
    Q_OBJECT

public:
    explicit PressureSensor(QWidget *parent = nullptr);
    ~PressureSensor();

private:
    Ui::PressureSensor *ui;
};

#endif // PRESSURESENSOR_H
