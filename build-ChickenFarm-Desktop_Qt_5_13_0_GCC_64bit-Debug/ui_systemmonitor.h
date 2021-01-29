/********************************************************************************
** Form generated from reading UI file 'systemmonitor.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSTEMMONITOR_H
#define UI_SYSTEMMONITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SystemMonitor
{
public:
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_6;
    QStackedWidget *systemStackedWidget;
    QWidget *modularAirPage;
    QFrame *frame_3;
    QFrame *frame_4;
    QStackedWidget *modularAirStackedWidget;
    QWidget *page;
    QWidget *modularChillerPage;
    QFrame *frame_5;
    QFrame *frame_6;
    QStackedWidget *modularChillerStackedWidget;
    QWidget *page_2;
    QWidget *bumpPage;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QFrame *frame_7;
    QFrame *frame_8;
    QWidget *axialFanPage;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QFrame *frame_10;
    QWidget *valvePage;
    QFrame *frame_11;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QWidget *sensorPage;
    QFrame *frame_9;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_4;
    QFrame *frame_12;
    QWidget *gridLayoutWidget_5;
    QGridLayout *gridLayout_5;
    QFrame *frame_13;
    QWidget *gridLayoutWidget_6;
    QGridLayout *gridLayout_6;
    QFrame *frame_14;
    QWidget *gridLayoutWidget_7;
    QGridLayout *gridLayout_7;
    QFrame *frame_15;
    QWidget *gridLayoutWidget_8;
    QGridLayout *gridLayout_8;
    QPushButton *pushButton_5;
    QPushButton *pushButton;
    QPushButton *pushButton_4;

    void setupUi(QWidget *SystemMonitor)
    {
        if (SystemMonitor->objectName().isEmpty())
            SystemMonitor->setObjectName(QString::fromUtf8("SystemMonitor"));
        SystemMonitor->resize(920, 708);
        pushButton_2 = new QPushButton(SystemMonitor);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(168, 640, 105, 41));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 0));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        QBrush brush2(QColor(190, 190, 190, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        pushButton_2->setPalette(palette);
        pushButton_2->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        pushButton_3 = new QPushButton(SystemMonitor);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(320, 640, 105, 41));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Button, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        pushButton_3->setPalette(palette1);
        pushButton_3->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        pushButton_6 = new QPushButton(SystemMonitor);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(776, 640, 105, 41));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Button, brush);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush);
        pushButton_6->setPalette(palette2);
        pushButton_6->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        systemStackedWidget = new QStackedWidget(SystemMonitor);
        systemStackedWidget->setObjectName(QString::fromUtf8("systemStackedWidget"));
        systemStackedWidget->setGeometry(QRect(0, 0, 920, 633));
        modularAirPage = new QWidget();
        modularAirPage->setObjectName(QString::fromUtf8("modularAirPage"));
        frame_3 = new QFrame(modularAirPage);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(10, 60, 253, 283));
        frame_3->setStyleSheet(QString::fromUtf8("#frame_3{\n"
"border-image: url(:/UI/Widget/System_Air_2.png);}"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        frame_4 = new QFrame(modularAirPage);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(10, 430, 241, 171));
        frame_4->setStyleSheet(QString::fromUtf8("#frame_4{\n"
"border-image: url(:/UI/Widget/System_Air_3.png);}"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        modularAirStackedWidget = new QStackedWidget(modularAirPage);
        modularAirStackedWidget->setObjectName(QString::fromUtf8("modularAirStackedWidget"));
        modularAirStackedWidget->setGeometry(QRect(270, 20, 641, 581));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        modularAirStackedWidget->addWidget(page);
        systemStackedWidget->addWidget(modularAirPage);
        modularChillerPage = new QWidget();
        modularChillerPage->setObjectName(QString::fromUtf8("modularChillerPage"));
        frame_5 = new QFrame(modularChillerPage);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(16, 24, 249, 313));
        frame_5->setStyleSheet(QString::fromUtf8("#frame_5{\n"
"border-image: url(:/UI/Widget/System_Chiller_1.png);}"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        frame_6 = new QFrame(modularChillerPage);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setGeometry(QRect(16, 392, 249, 193));
        frame_6->setStyleSheet(QString::fromUtf8("#frame_6{\n"
"border-image: url(:/UI/Widget/System_Chiller_5.png);}"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        modularChillerStackedWidget = new QStackedWidget(modularChillerPage);
        modularChillerStackedWidget->setObjectName(QString::fromUtf8("modularChillerStackedWidget"));
        modularChillerStackedWidget->setGeometry(QRect(264, 16, 641, 581));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        modularChillerStackedWidget->addWidget(page_2);
        systemStackedWidget->addWidget(modularChillerPage);
        bumpPage = new QWidget();
        bumpPage->setObjectName(QString::fromUtf8("bumpPage"));
        gridLayoutWidget = new QWidget(bumpPage);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(288, 16, 609, 593));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        frame_7 = new QFrame(bumpPage);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setGeometry(QRect(8, 56, 265, 305));
        frame_7->setStyleSheet(QString::fromUtf8("#frame_7\n"
"{border-image: url(:/UI/Widget/System_Bump_1.png);}\n"
""));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        frame_8 = new QFrame(bumpPage);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setGeometry(QRect(8, 416, 273, 177));
        frame_8->setStyleSheet(QString::fromUtf8("#frame_8\n"
"{border-image: url(:/UI/Widget/System_Bump_2.png);}"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        systemStackedWidget->addWidget(bumpPage);
        axialFanPage = new QWidget();
        axialFanPage->setObjectName(QString::fromUtf8("axialFanPage"));
        gridLayoutWidget_2 = new QWidget(axialFanPage);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(288, 8, 609, 593));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_10 = new QFrame(axialFanPage);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setGeometry(QRect(8, 48, 265, 377));
        frame_10->setStyleSheet(QString::fromUtf8("#frame_10\n"
"{border-image: url(:/UI/Widget/System_Bump_1.png);}"));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        systemStackedWidget->addWidget(axialFanPage);
        valvePage = new QWidget();
        valvePage->setObjectName(QString::fromUtf8("valvePage"));
        frame_11 = new QFrame(valvePage);
        frame_11->setObjectName(QString::fromUtf8("frame_11"));
        frame_11->setGeometry(QRect(16, 48, 265, 377));
        frame_11->setStyleSheet(QString::fromUtf8("#frame_11\n"
"{border-image: url(:/UI/Widget/System_Bump_1.png);}"));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        gridLayoutWidget_3 = new QWidget(valvePage);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(296, 8, 609, 593));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        systemStackedWidget->addWidget(valvePage);
        sensorPage = new QWidget();
        sensorPage->setObjectName(QString::fromUtf8("sensorPage"));
        frame_9 = new QFrame(sensorPage);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setGeometry(QRect(16, 48, 417, 385));
        frame_9->setStyleSheet(QString::fromUtf8("#frame_9{\n"
"border-image: url(:/UI/Widget/System_Sensor_1.png);}\n"
""));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        gridLayoutWidget_4 = new QWidget(frame_9);
        gridLayoutWidget_4->setObjectName(QString::fromUtf8("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(16, 16, 385, 353));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        frame_12 = new QFrame(sensorPage);
        frame_12->setObjectName(QString::fromUtf8("frame_12"));
        frame_12->setGeometry(QRect(15, 464, 417, 137));
        frame_12->setStyleSheet(QString::fromUtf8("#frame_12{\n"
"border-image: url(:/UI/Widget/System_Sensor_4.png);}"));
        frame_12->setFrameShape(QFrame::StyledPanel);
        frame_12->setFrameShadow(QFrame::Raised);
        gridLayoutWidget_5 = new QWidget(frame_12);
        gridLayoutWidget_5->setObjectName(QString::fromUtf8("gridLayoutWidget_5"));
        gridLayoutWidget_5->setGeometry(QRect(8, 8, 401, 121));
        gridLayout_5 = new QGridLayout(gridLayoutWidget_5);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        frame_13 = new QFrame(sensorPage);
        frame_13->setObjectName(QString::fromUtf8("frame_13"));
        frame_13->setGeometry(QRect(448, 48, 441, 161));
        frame_13->setStyleSheet(QString::fromUtf8("#frame_13{\n"
"border-image: url(:/UI/Widget/System_Sensor_4.png);}"));
        frame_13->setFrameShape(QFrame::StyledPanel);
        frame_13->setFrameShadow(QFrame::Raised);
        gridLayoutWidget_6 = new QWidget(frame_13);
        gridLayoutWidget_6->setObjectName(QString::fromUtf8("gridLayoutWidget_6"));
        gridLayoutWidget_6->setGeometry(QRect(8, 8, 425, 153));
        gridLayout_6 = new QGridLayout(gridLayoutWidget_6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        frame_14 = new QFrame(sensorPage);
        frame_14->setObjectName(QString::fromUtf8("frame_14"));
        frame_14->setGeometry(QRect(456, 272, 417, 137));
        frame_14->setStyleSheet(QString::fromUtf8("#frame_14{\n"
"border-image: url(:/UI/Widget/System_Sensor_4.png);}"));
        frame_14->setFrameShape(QFrame::StyledPanel);
        frame_14->setFrameShadow(QFrame::Raised);
        gridLayoutWidget_7 = new QWidget(frame_14);
        gridLayoutWidget_7->setObjectName(QString::fromUtf8("gridLayoutWidget_7"));
        gridLayoutWidget_7->setGeometry(QRect(8, 8, 401, 121));
        gridLayout_7 = new QGridLayout(gridLayoutWidget_7);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        frame_15 = new QFrame(sensorPage);
        frame_15->setObjectName(QString::fromUtf8("frame_15"));
        frame_15->setGeometry(QRect(456, 456, 417, 137));
        frame_15->setStyleSheet(QString::fromUtf8("#frame_15{\n"
"border-image: url(:/UI/Widget/System_Sensor_4.png);}"));
        frame_15->setFrameShape(QFrame::StyledPanel);
        frame_15->setFrameShadow(QFrame::Raised);
        gridLayoutWidget_8 = new QWidget(frame_15);
        gridLayoutWidget_8->setObjectName(QString::fromUtf8("gridLayoutWidget_8"));
        gridLayoutWidget_8->setGeometry(QRect(8, 8, 401, 121));
        gridLayout_8 = new QGridLayout(gridLayoutWidget_8);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        systemStackedWidget->addWidget(sensorPage);
        pushButton_5 = new QPushButton(SystemMonitor);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(632, 640, 105, 41));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Button, brush);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush);
        pushButton_5->setPalette(palette3);
        pushButton_5->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        pushButton = new QPushButton(SystemMonitor);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(24, 642, 105, 41));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Button, brush);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush);
        pushButton->setPalette(palette4);
        pushButton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        pushButton_4 = new QPushButton(SystemMonitor);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(472, 640, 105, 41));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Button, brush);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush);
        pushButton_4->setPalette(palette5);
        pushButton_4->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));

        retranslateUi(SystemMonitor);

        systemStackedWidget->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(SystemMonitor);
    } // setupUi

    void retranslateUi(QWidget *SystemMonitor)
    {
        SystemMonitor->setWindowTitle(QCoreApplication::translate("SystemMonitor", "Form", nullptr));
        pushButton_2->setText(QCoreApplication::translate("SystemMonitor", "\347\273\204  \347\251\272", nullptr));
        pushButton_3->setText(QCoreApplication::translate("SystemMonitor", "\347\273\204  \347\251\272", nullptr));
        pushButton_6->setText(QCoreApplication::translate("SystemMonitor", "\347\273\204  \347\251\272", nullptr));
        pushButton_5->setText(QCoreApplication::translate("SystemMonitor", "\347\273\204  \347\251\272", nullptr));
        pushButton->setText(QCoreApplication::translate("SystemMonitor", "\347\273\204  \347\251\272", nullptr));
        pushButton_4->setText(QCoreApplication::translate("SystemMonitor", "\347\273\204  \347\251\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SystemMonitor: public Ui_SystemMonitor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMMONITOR_H
