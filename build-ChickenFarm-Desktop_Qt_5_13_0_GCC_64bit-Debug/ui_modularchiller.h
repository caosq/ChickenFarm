/********************************************************************************
** Form generated from reading UI file 'modularchiller.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODULARCHILLER_H
#define UI_MODULARCHILLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModularChiller
{
public:
    QFrame *frame;
    QFrame *frame_2;
    QLabel *label_13;
    QLabel *label_15;
    QLabel *label_10;
    QLabel *label_16;
    QLabel *label_24;
    QLabel *label_17;
    QLabel *label_14;
    QLabel *label_12;
    QLabel *label_11;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QLabel *label;

    void setupUi(QWidget *ModularChiller)
    {
        if (ModularChiller->objectName().isEmpty())
            ModularChiller->setObjectName(QString::fromUtf8("ModularChiller"));
        ModularChiller->resize(641, 581);
        frame = new QFrame(ModularChiller);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(-10, 8, 661, 581));
        frame->setStyleSheet(QString::fromUtf8("#frame{border-image: url(:/UI/Widget/System_Air_1.png);}\n"
""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(16, 16, 625, 145));
        frame_2->setStyleSheet(QString::fromUtf8("#frame_2{\n"
"border-image: url(:/UI/Widget/System_Chiller_4.png);}"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_13 = new QLabel(frame_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(104, 108, 81, 17));
        label_15 = new QLabel(frame_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(354, 18, 81, 17));
        label_10 = new QLabel(frame_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(104, 18, 81, 17));
        label_16 = new QLabel(frame_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(354, 48, 81, 17));
        label_24 = new QLabel(frame_2);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(24, 28, 21, 91));
        label_24->setToolTipDuration(-1);
        label_24->setStyleSheet(QString::fromUtf8(""));
        label_24->setAlignment(Qt::AlignJustify|Qt::AlignTop);
        label_17 = new QLabel(frame_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(354, 108, 81, 17));
        label_14 = new QLabel(frame_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(354, 78, 81, 17));
        label_12 = new QLabel(frame_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(104, 78, 81, 17));
        label_11 = new QLabel(frame_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(104, 48, 81, 17));
        horizontalLayoutWidget = new QWidget(frame);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(16, 160, 625, 409));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(190, 190, 190, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        pushButton->setPalette(palette);
        pushButton->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(pushButton);

        label = new QLabel(ModularChiller);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(280, 0, 101, 21));
        label->setStyleSheet(QString::fromUtf8("font: 63 oblique 16pt \"URW Gothic L\";"));

        retranslateUi(ModularChiller);

        QMetaObject::connectSlotsByName(ModularChiller);
    } // setupUi

    void retranslateUi(QWidget *ModularChiller)
    {
        ModularChiller->setWindowTitle(QCoreApplication::translate("ModularChiller", "Form", nullptr));
        label_13->setText(QCoreApplication::translate("ModularChiller", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_15->setText(QCoreApplication::translate("ModularChiller", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_10->setText(QCoreApplication::translate("ModularChiller", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_16->setText(QCoreApplication::translate("ModularChiller", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_24->setText(QCoreApplication::translate("ModularChiller", "\346\234\272\n"
"\347\273\204\n"
"\345\217\202\n"
"\346\225\260", nullptr));
        label_17->setText(QCoreApplication::translate("ModularChiller", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_14->setText(QCoreApplication::translate("ModularChiller", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_12->setText(QCoreApplication::translate("ModularChiller", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        label_11->setText(QCoreApplication::translate("ModularChiller", "\345\220\257\345\201\234\345\221\275\344\273\244\357\274\232", nullptr));
        pushButton->setText(QCoreApplication::translate("ModularChiller", "PushButton", nullptr));
        label->setText(QCoreApplication::translate("ModularChiller", "xxx #\346\234\272\347\273\204", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModularChiller: public Ui_ModularChiller {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODULARCHILLER_H
