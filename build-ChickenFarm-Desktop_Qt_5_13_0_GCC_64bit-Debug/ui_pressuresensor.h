/********************************************************************************
** Form generated from reading UI file 'pressuresensor.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRESSURESENSOR_H
#define UI_PRESSURESENSOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PressureSensor
{
public:
    QFrame *frame;
    QLabel *label_52;
    QLabel *label_53;

    void setupUi(QWidget *PressureSensor)
    {
        if (PressureSensor->objectName().isEmpty())
            PressureSensor->setObjectName(QString::fromUtf8("PressureSensor"));
        PressureSensor->resize(205, 45);
        frame = new QFrame(PressureSensor);
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

        retranslateUi(PressureSensor);

        QMetaObject::connectSlotsByName(PressureSensor);
    } // setupUi

    void retranslateUi(QWidget *PressureSensor)
    {
        PressureSensor->setWindowTitle(QCoreApplication::translate("PressureSensor", "Form", nullptr));
        label_52->setText(QCoreApplication::translate("PressureSensor", "CHW-SP", nullptr));
        label_53->setText(QCoreApplication::translate("PressureSensor", "2000  kPa", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PressureSensor: public Ui_PressureSensor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRESSURESENSOR_H
