#ifndef MODULARAIR_H
#define MODULARAIR_H

#include <QWidget>

namespace Ui {
class ModularAir;
}

class ModularAir : public QWidget
{
    Q_OBJECT

public:
    explicit ModularAir(QWidget *parent = nullptr);
    ~ModularAir();

private:
    Ui::ModularAir *ui;
};

#endif // MODULARAIR_H
