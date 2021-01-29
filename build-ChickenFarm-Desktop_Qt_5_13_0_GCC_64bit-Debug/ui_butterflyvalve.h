/********************************************************************************
** Form generated from reading UI file 'butterflyvalve.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTERFLYVALVE_H
#define UI_BUTTERFLYVALVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ButterflyValve
{
public:
    QLabel *label_44;
    QFrame *frame;
    QLabel *label_45;
    QLabel *label_54;
    QLabel *label_55;
    QLabel *label_56;
    QLabel *label_57;
    QLabel *label_58;
    QLabel *label_59;
    QLabel *label_60;

    void setupUi(QWidget *ButterflyValve)
    {
        if (ButterflyValve->objectName().isEmpty())
            ButterflyValve->setObjectName(QString::fromUtf8("ButterflyValve"));
        ButterflyValve->resize(308, 271);
        label_44 = new QLabel(ButterflyValve);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(120, 0, 49, 21));
        frame = new QFrame(ButterflyValve);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 16, 308, 257));
        frame->setStyleSheet(QString::fromUtf8("#frame{\n"
"border-image: url(:/UI/Widget/System_Bump_3.png);}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_45 = new QLabel(frame);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(16, 106, 81, 17));
        label_54 = new QLabel(frame);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(16, 166, 81, 17));
        label_55 = new QLabel(frame);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(16, 226, 81, 17));
        label_56 = new QLabel(frame);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        label_56->setGeometry(QRect(16, 76, 81, 17));
        label_57 = new QLabel(frame);
        label_57->setObjectName(QString::fromUtf8("label_57"));
        label_57->setGeometry(QRect(16, 196, 81, 17));
        label_58 = new QLabel(frame);
        label_58->setObjectName(QString::fromUtf8("label_58"));
        label_58->setGeometry(QRect(16, 46, 81, 17));
        label_59 = new QLabel(frame);
        label_59->setObjectName(QString::fromUtf8("label_59"));
        label_59->setGeometry(QRect(16, 136, 81, 17));
        label_60 = new QLabel(frame);
        label_60->setObjectName(QString::fromUtf8("label_60"));
        label_60->setGeometry(QRect(16, 16, 81, 17));

        retranslateUi(ButterflyValve);

        QMetaObject::connectSlotsByName(ButterflyValve);
    } // setupUi

    void retranslateUi(QWidget *ButterflyValve)
    {
        ButterflyValve->setWindowTitle(QCoreApplication::translate("ButterflyValve", "Form", nullptr));
        label_44->setText(QCoreApplication::translate("ButterflyValve", "\346\250\241\345\235\227 1", nullptr));
        label_45->setText(QCoreApplication::translate("ButterflyValve", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_54->setText(QCoreApplication::translate("ButterflyValve", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_55->setText(QCoreApplication::translate("ButterflyValve", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_56->setText(QCoreApplication::translate("ButterflyValve", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_57->setText(QCoreApplication::translate("ButterflyValve", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_58->setText(QCoreApplication::translate("ButterflyValve", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_59->setText(QCoreApplication::translate("ButterflyValve", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_60->setText(QCoreApplication::translate("ButterflyValve", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ButterflyValve: public Ui_ButterflyValve {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTERFLYVALVE_H
