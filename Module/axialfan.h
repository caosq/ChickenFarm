#ifndef AXIALFAN_H
#define AXIALFAN_H

#include <QWidget>

namespace Ui {
class AxialFan;
}

class AxialFan : public QWidget
{
    Q_OBJECT

public:
    explicit AxialFan(QWidget *parent = nullptr);
    ~AxialFan();

private:
    Ui::AxialFan *ui;
};

#endif // AXIALFAN_H
