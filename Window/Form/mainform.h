#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include "systemmonitor.h"
#include "curve.h"


namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT


private:
    void initLabel();
    void initButton();
    void initForm();

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();

private:
    Ui::MainForm *ui;

    SystemMonitor  *m_pSystemMonitor;
    Curve          *m_pCurve;
};

#endif // MAINFORM_H
