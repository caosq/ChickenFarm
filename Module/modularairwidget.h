#ifndef MODULARAIRWIDGET_H
#define MODULARAIRWIDGET_H

#include <QWidget>
#include "modularair.h"

namespace Ui {
class ModularAirWidget;
}

class ModularAirWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModularAirWidget(QWidget *parent = nullptr);
    explicit ModularAirWidget(QWidget *parent = nullptr, ModularAir* psModularAir = nullptr);

    ~ModularAirWidget();

private:
    Ui::ModularAirWidget *ui;
    ModularAir* m_psModularAir;
};

#endif // MODULARAIRWIDGET_H
