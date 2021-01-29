/********************************************************************************
** Form generated from reading UI file 'co2sensor.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CO2SENSOR_H
#define UI_CO2SENSOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CO2Sensor
{
public:
    QFrame *frame;
    QLabel *label_52;
    QLabel *label_53;

    void setupUi(QWidget *CO2Sensor)
    {
        if (CO2Sensor->objectName().isEmpty())
            CO2Sensor->setObjectName(QString::fromUtf8("CO2Sensor"));
        CO2Sensor->resize(205, 45);
        frame = new QFrame(CO2Sensor);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 205, 45));
        frame->setStyleSheet(QString::fromUtf8("#frame{\n"
"border-image: url(:/UI/Widget/System_Sensor_5.png);}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_52 = new QLabel(frame);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(24, 16, 57, 17));
        label_53 = new QLabel(frame);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(104, 16, 89, 17));

        retranslateUi(CO2Sensor);

        QMetaObject::connectSlotsByName(CO2Sensor);
    } // setupUi

    void retranslateUi(QWidget *CO2Sensor)
    {
        CO2Sensor->setWindowTitle(QCoreApplication::translate("CO2Sensor", "Form", nullptr));
        label_52->setText(QCoreApplication::translate("CO2Sensor", "ICO2-01", nullptr));
        label_53->setText(QCoreApplication::translate("CO2Sensor", "2000  ppm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CO2Sensor: public Ui_CO2Sensor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CO2SENSOR_H
