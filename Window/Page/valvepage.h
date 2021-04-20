#ifndef VALVEPAGE_H
#define VALVEPAGE_H

#include <QWidget>
#include "butterflyvalve.h"
#include "bypassvalve.h"

namespace Ui {
class ValvePage;
}

class ValvePage : public QWidget
{
    Q_OBJECT
public:
    QVector<ButterflyValve*> m_ButterflyValves;
    BypassValve *m_BypassValve;

private:
    QVector<TextLabel*> m_Labels;
    QVector<QWidget*>   m_Widgets;

    StateButton     *m_pSwitchCmdBtn;      //启停命令
    StateButton     *m_pErrorCleanCmdBtn;  //故障清除

private:
    void initDevice();
    void initLabel();
    void initButton();

public:
    explicit ValvePage(QWidget *parent = nullptr);
    ~ValvePage();

private slots:
    void paramSetBtnValChanged(void*);

private:
    Ui::ValvePage *ui;
};

#endif // VALVEPAGE_H
