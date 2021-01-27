#ifndef TEMPHUMISENSOR_H
#define TEMPHUMISENSOR_H

#include <QWidget>

namespace Ui {
class TempHumiSensor;
}

class TempHumiSensor : public QWidget
{
    Q_OBJECT

public:
    explicit TempHumiSensor(QWidget *parent = nullptr);
    ~TempHumiSensor();

private:
    Ui::TempHumiSensor *ui;
};

#endif // TEMPHUMISENSOR_H
