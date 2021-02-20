#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QLabel>
#include "systemmonitor.h"
#include "curve.h"
#include "setting.h"
#include "event.h"
#include "home.h"


namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    QLabel *m_pLogLabel;
    QLabel *m_pTitleLabel;

private:
    void initLabel();
    void initButton();
    void initForm();

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();

private slots:
    void on_systemButton_clicked();

    void on_curveButton_clicked();

    void on_paraButton_clicked();

    void on_backButton_clicked();

    void on_eventButton_clicked();

private:
    Ui::MainForm *ui;

    SystemMonitor  *m_pSystemMonitor;
    Curve          *m_pCurve;
    Setting        *m_pSetting;
    Event          *m_pEvent;
    Home           *m_pHome;

};

#endif // MAINFORM_H
