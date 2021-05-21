#ifndef TEMPSETPAGE_H
#define TEMPSETPAGE_H

#include <QWidget>
#include <QFile>
#include <QPointF>
#include "tempunit.h"
#include "datamonitor.h"

namespace Ui {
class TempSetPage;
}

class TempSetPage : public QWidget
{
    Q_OBJECT

public:
    int16_t       m_sPreSetDay;      //上次设置的天数
    int16_t       m_sCurrentDay;     //当前是第几天
    int16_t       m_sTemp;           //温度

private:
    QVector<TempUnit*>   m_TempUnits;
    Monitor    *m_pDayMonitor;
    QFile       m_TempSetFile;
    QStringList m_TempSetPoint;

    QVector<QPoint> m_TempPoint;
private:
    void initLabel();
    void initButton();
    void readTempSetFile();

public:
    explicit TempSetPage(QWidget *parent = nullptr);
    ~TempSetPage();

private slots:
    void growDayChangedSlot(Monitor* pMonitor);
    void systemTimeChangedSlot();
    void tempChangedSlot(TempUnit *);

private:
    Ui::TempSetPage *ui;
};

#endif // TEMPSETPAGE_H
