#ifndef MODULARCHILLERWIDGET_H
#define MODULARCHILLERWIDGET_H

#include <QWidget>

namespace Ui {
class ModularChillerWidget;
}

class ModularChillerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModularChillerWidget(QWidget *parent = nullptr);
    ~ModularChillerWidget();

private:
    Ui::ModularChillerWidget *ui;
};

#endif // MODULARCHILLERWIDGET_H
