#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include "tempsetpage.h"
#include "userparampage.h"
#include "manuparampage.h"

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

private:
    QVector<QWidget*>   m_Widgets;

    TempSetPage      *m_pTempSetPage;
    UserParamPage    *m_pUserParamPage;
    ManuParamPage    *m_pManuParamPage;

private:
    void initLabel();
    void initButton();
    void initForm();

protected:
    //void paintEvent(QPaintEvent *e);
    void show();

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

private slots:
    void systemStackChanged(int32_t);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

public:
    Ui::Setting *ui;
};

#endif // SETTING_H
