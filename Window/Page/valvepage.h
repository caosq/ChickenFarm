#ifndef VALVEPAGE_H
#define VALVEPAGE_H

#include <QWidget>

namespace Ui {
class ValvePage;
}

class ValvePage : public QWidget
{
    Q_OBJECT

public:
    explicit ValvePage(QWidget *parent = nullptr);
    ~ValvePage();

private:
    Ui::ValvePage *ui;
};

#endif // VALVEPAGE_H
