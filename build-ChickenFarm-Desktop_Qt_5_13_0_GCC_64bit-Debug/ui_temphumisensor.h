/********************************************************************************
** Form generated from reading UI file 'temphumisensor.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEMPHUMISENSOR_H
#define UI_TEMPHUMISENSOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TempHumiSensor
{
public:
    QFrame *frame;
    QLabel *label_52;
    QLabel *label_53;
    QLabel *label_54;
    QLabel *label_55;

    void setupUi(QWidget *TempHumiSensor)
    {
        if (TempHumiSensor->objectName().isEmpty())
            TempHumiSensor->setObjectName(QString::fromUtf8("TempHumiSensor"));
        TempHumiSensor->resize(205, 45);
        TempHumiSensor->setStyleSheet(QString::fromUtf8(""));
        frame = new QFrame(TempHumiSensor);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 205, 45));
        frame->setStyleSheet(QString::fromUtf8("#frame{\n"
"border-image: url(:/UI/Widget/System_Sensor_5.png);}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_52 = new QLabel(frame);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(8, 16, 57, 17));
        label_53 = new QLabel(frame);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(72, 16, 49, 17));
        label_54 = new QLabel(frame);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(136, 16, 17, 17));
        label_55 = new QLabel(frame);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(152, 16, 33, 17));

        retranslateUi(TempHumiSensor);

        QMetaObject::connectSlotsByName(TempHumiSensor);
    } // setupUi

    void retranslateUi(QWidget *TempHumiSensor)
    {
        TempHumiSensor->setWindowTitle(QCoreApplication::translate("TempHumiSensor", "Form", nullptr));
        label_52->setText(QCoreApplication::translate("TempHumiSensor", "ITH-01", nullptr));
        label_53->setText(QCoreApplication::translate("TempHumiSensor", "31.4  \342\204\203", nullptr));
        label_54->setText(QCoreApplication::translate("TempHumiSensor", "/", nullptr));
        label_55->setText(QCoreApplication::translate("TempHumiSensor", "71 %", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TempHumiSensor: public Ui_TempHumiSensor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEMPHUMISENSOR_H
