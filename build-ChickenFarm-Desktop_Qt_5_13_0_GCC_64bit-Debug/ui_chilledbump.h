/********************************************************************************
** Form generated from reading UI file 'chilledbump.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHILLEDBUMP_H
#define UI_CHILLEDBUMP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChilledBump
{
public:
    QFrame *frame;
    QLabel *label_36;
    QLabel *label_37;
    QLabel *label_38;
    QLabel *label_39;
    QLabel *label_40;
    QLabel *label_41;
    QLabel *label_42;
    QLabel *label_43;
    QLabel *label_44;

    void setupUi(QWidget *ChilledBump)
    {
        if (ChilledBump->objectName().isEmpty())
            ChilledBump->setObjectName(QString::fromUtf8("ChilledBump"));
        ChilledBump->resize(308, 271);
        frame = new QFrame(ChilledBump);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 16, 308, 257));
        frame->setStyleSheet(QString::fromUtf8("#frame{\n"
"border-image: url(:/UI/Widget/System_Bump_3.png);}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_36 = new QLabel(frame);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(16, 106, 81, 17));
        label_37 = new QLabel(frame);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(16, 166, 81, 17));
        label_38 = new QLabel(frame);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(16, 226, 81, 17));
        label_39 = new QLabel(frame);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(16, 76, 81, 17));
        label_40 = new QLabel(frame);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(16, 196, 81, 17));
        label_41 = new QLabel(frame);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setGeometry(QRect(16, 46, 81, 17));
        label_42 = new QLabel(frame);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(16, 136, 81, 17));
        label_43 = new QLabel(frame);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(16, 16, 81, 17));
        label_44 = new QLabel(ChilledBump);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(120, 0, 49, 21));

        retranslateUi(ChilledBump);

        QMetaObject::connectSlotsByName(ChilledBump);
    } // setupUi

    void retranslateUi(QWidget *ChilledBump)
    {
        ChilledBump->setWindowTitle(QCoreApplication::translate("ChilledBump", "Form", nullptr));
        label_36->setText(QCoreApplication::translate("ChilledBump", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_37->setText(QCoreApplication::translate("ChilledBump", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_38->setText(QCoreApplication::translate("ChilledBump", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_39->setText(QCoreApplication::translate("ChilledBump", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_40->setText(QCoreApplication::translate("ChilledBump", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_41->setText(QCoreApplication::translate("ChilledBump", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_42->setText(QCoreApplication::translate("ChilledBump", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_43->setText(QCoreApplication::translate("ChilledBump", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_44->setText(QCoreApplication::translate("ChilledBump", "\346\250\241\345\235\227 1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChilledBump: public Ui_ChilledBump {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHILLEDBUMP_H
