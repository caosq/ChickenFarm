/********************************************************************************
** Form generated from reading UI file 'tempsensor.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEMPSENSOR_H
#define UI_TEMPSENSOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TempSensor
{
public:
    QFrame *frame;
    QLabel *label_52;
    QLabel *label_53;

    void setupUi(QWidget *TempSensor)
    {
        if (TempSensor->objectName().isEmpty())
            TempSensor->setObjectName(QString::fromUtf8("TempSensor"));
        TempSensor->resize(205, 45);
        frame = new QFrame(TempSensor);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 205, 45));
        frame->setStyleSheet(QString::fromUtf8("#frame{\n"
"border-image: url(:/UI/Widget/System_Sensor_5.png);}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_52 = new QLabel(frame);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(24, 16, 65, 17));
        label_53 = new QLabel(frame);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(120, 16, 65, 17));

        retranslateUi(TempSensor);

        QMetaObject::connectSlotsByName(TempSensor);
    } // setupUi

    void retranslateUi(QWidget *TempSensor)
    {
        TempSensor->setWindowTitle(QCoreApplication::translate("TempSensor", "Form", nullptr));
        label_52->setText(QCoreApplication::translate("TempSensor", "CHW-ST", nullptr));
        label_53->setText(QCoreApplication::translate("TempSensor", "31.4  \342\204\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TempSensor: public Ui_TempSensor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEMPSENSOR_H
