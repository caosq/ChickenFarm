#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QWidget>

namespace Ui {
class SystemMonitor;
}

class SystemMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit SystemMonitor(QWidget *parent = nullptr);
    ~SystemMonitor();

private:
    Ui::SystemMonitor *ui;
};

#endif // SYSTEMMONITOR_H
