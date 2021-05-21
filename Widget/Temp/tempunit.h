#ifndef TEMPUNIT_H
#define TEMPUNIT_H

#include <QWidget>
#include "datalabel.h"
#include "analogvalbutton.h"
#include "datamonitor.h"

#define AB_PRESS_PIX ":UI/baseFile/abPress.png"
#define AB_RELEASE_PIX ":UI/baseFile/abRelease.png"

namespace Ui {
class TempUnit;
}

class TempUnit : public QWidget
{
    Q_OBJECT

public:
    int16_t       m_sDayIndex;            //第几天
    bool          m_xIsToday = false;     //是否为当天
    uint16_t      m_usTemp = 190;           //温度

    static uint8_t  m_usDayCount;    //天数

private:
    QVector<QWidget*>   m_Widgets;
    AnalogValButton     *m_pTempBtn;    //温度设定

private:
    void initLabel();
    void initButton();

signals:
    void tempChanged(TempUnit *pTempUnit);

private slots:
    void valChangedSlot(void*);

public: 
    explicit TempUnit(QWidget *parent = nullptr);
    ~TempUnit();

    void setCurrentDay(bool xIsToday);

private:
    Ui::TempUnit *ui;
};

#endif // TEMPUNIT_H
