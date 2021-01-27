#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include "modularair.h"
#include "modularchiller.h"
#include "chilledbump.h"

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
    ModularAir*     m_psModularAir;
    ModularChiller* m_psModularChiller;
    ChilledBump*    m_psChilledBump_1;
    ChilledBump*    m_psChilledBump_2;
    ChilledBump*    m_psChilledBump_3;

};

#endif // MAINFORM_H
