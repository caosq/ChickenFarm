/********************************************************************************
** Form generated from reading UI file 'axialfan.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AXIALFAN_H
#define UI_AXIALFAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AxialFan
{
public:
    QLabel *label_44;
    QFrame *frame;
    QLabel *label_45;
    QLabel *label_46;
    QLabel *label_47;
    QLabel *label_48;
    QLabel *label_49;
    QLabel *label_50;
    QLabel *label_51;
    QLabel *label_52;

    void setupUi(QWidget *AxialFan)
    {
        if (AxialFan->objectName().isEmpty())
            AxialFan->setObjectName(QString::fromUtf8("AxialFan"));
        AxialFan->resize(308, 271);
        label_44 = new QLabel(AxialFan);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(120, 0, 49, 21));
        frame = new QFrame(AxialFan);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 16, 308, 257));
        frame->setStyleSheet(QString::fromUtf8("#frame{\n"
"border-image: url(:/UI/Widget/System_Bump_3.png);}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_45 = new QLabel(frame);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(16, 106, 81, 17));
        QFont font;
        font.setPointSize(11);
        label_45->setFont(font);
        label_46 = new QLabel(frame);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(16, 166, 81, 17));
        label_47 = new QLabel(frame);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setGeometry(QRect(16, 226, 81, 17));
        label_48 = new QLabel(frame);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setGeometry(QRect(16, 76, 81, 17));
        label_49 = new QLabel(frame);
        label_49->setObjectName(QString::fromUtf8("label_49"));
        label_49->setGeometry(QRect(16, 196, 81, 17));
        label_50 = new QLabel(frame);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        label_50->setGeometry(QRect(16, 46, 81, 17));
        label_51 = new QLabel(frame);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setGeometry(QRect(16, 136, 81, 17));
        label_52 = new QLabel(frame);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(16, 16, 81, 17));

        retranslateUi(AxialFan);

        QMetaObject::connectSlotsByName(AxialFan);
    } // setupUi

    void retranslateUi(QWidget *AxialFan)
    {
        AxialFan->setWindowTitle(QCoreApplication::translate("AxialFan", "Form", nullptr));
        label_44->setText(QCoreApplication::translate("AxialFan", "\346\250\241\345\235\227 1", nullptr));
        label_45->setText(QCoreApplication::translate("AxialFan", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_46->setText(QCoreApplication::translate("AxialFan", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_47->setText(QCoreApplication::translate("AxialFan", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_48->setText(QCoreApplication::translate("AxialFan", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_49->setText(QCoreApplication::translate("AxialFan", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_50->setText(QCoreApplication::translate("AxialFan", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_51->setText(QCoreApplication::translate("AxialFan", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_52->setText(QCoreApplication::translate("AxialFan", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AxialFan: public Ui_AxialFan {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AXIALFAN_H
