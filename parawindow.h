#ifndef PARAWINDOW_H
#define PARAWINDOW_H

#include <QMainWindow>

namespace Ui {
class ParaWindow;
}

class ParaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ParaWindow(QWidget *parent = nullptr);
    ~ParaWindow();

private:
    Ui::ParaWindow *ui;
};

#endif // PARAWINDOW_H
