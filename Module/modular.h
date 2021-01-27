#ifndef MODULAR_H
#define MODULAR_H

#include <QWidget>

namespace Ui {
class Modular;
}

class Modular : public QWidget
{
    Q_OBJECT

public:
    explicit Modular(QWidget *parent = nullptr);
    ~Modular();

private:
    Ui::Modular *ui;
};

#endif // MODULAR_H
