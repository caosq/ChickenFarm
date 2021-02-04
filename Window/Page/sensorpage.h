#ifndef SENSORPAGE_H
#define SENSORPAGE_H

#include <QWidget>

namespace Ui {
class SensorPage;
}

class SensorPage : public QWidget
{
    Q_OBJECT

public:
    explicit SensorPage(QWidget *parent = nullptr);
    ~SensorPage();

private:
    Ui::SensorPage *ui;
};

#endif // SENSORPAGE_H
