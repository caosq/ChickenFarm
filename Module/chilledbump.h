#ifndef CHILLEDBUMP_H
#define CHILLEDBUMP_H

#include <QWidget>

namespace Ui {
class ChilledBump;
}

class ChilledBump : public QWidget
{
    Q_OBJECT

public:
    explicit ChilledBump(QWidget *parent = nullptr);
    ~ChilledBump();

private:
    Ui::ChilledBump *ui;
};

#endif // CHILLEDBUMP_H
