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
    DataLabel *m_pLogLabel;
    QLabel *m_pTitleLabel;

private:
    void initLabel();
    void initForm();

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();

    static void initButton();

private slots:
    void systemTimeChangedSlot();
    void systemLogChangedSlot(bool);

    void on_systemButton_clicked();
    void on_curveButton_clicked();
    void on_paraButton_clicked();
    void on_backButton_clicked();
    void on_eventButton_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainForm *ui;

    SystemMonitor m_SystemMonitor;
    Home          m_Home;
    Curve         m_Curve;
    Setting       m_Setting;
    Event         m_Event;
};

#endif // MAINFORM_H
