#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include "modularair.h"

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();

private:
    Ui::MainForm *ui;
    ModularAir* m_psModularAir;

};

#endif // MAINFORM_H
