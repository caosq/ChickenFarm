#ifndef AXIALFANPAGE_H
#define AXIALFANPAGE_H

#include <QWidget>

namespace Ui {
class AxialFanPage;
}

class AxialFanPage : public QWidget
{
    Q_OBJECT

public:
    explicit AxialFanPage(QWidget *parent = nullptr);
    ~AxialFanPage();

private:
    Ui::AxialFanPage *ui;
};

#endif // AXIALFANPAGE_H
