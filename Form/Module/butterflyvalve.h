#ifndef BUTTERFLYVALVE_H
#define BUTTERFLYVALVE_H

#include <QWidget>

namespace Ui {
class ButterflyValve;
}

class ButterflyValve : public QWidget
{
    Q_OBJECT

public:
    explicit ButterflyValve(QWidget *parent = nullptr);
    ~ButterflyValve();

private:
    Ui::ButterflyValve *ui;
};

#endif // BUTTERFLYVALVE_H
