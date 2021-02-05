#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QWidget>
#include "modularairpage.h"
#include "modularchillerpage.h"
#include "bumppage.h"
#include "axialfanpage.h"

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

private:
    void initLabel();
    void initButton();
    void initForm();

public:
    explicit SystemMonitor(QWidget *parent = nullptr);
    ~SystemMonitor();

private:
    Ui::SystemMonitor *ui;
};

#endif // SYSTEMMONITOR_H
