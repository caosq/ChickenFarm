/********************************************************************************
** Form generated from reading UI file 'mainform.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFORM_H
#define UI_MAINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainForm
{
public:
    QStackedWidget *mainStackedWidget;
    QWidget *homePage;
    QWidget *systemPage;
    QStackedWidget *systemStackedWidget;
    QWidget *modularAirPage;
    QFrame *frame_3;
    QFrame *frame_4;
    QStackedWidget *modularAirStackedWidget;
    QWidget *page;
    QWidget *modularChillerPage;
    QFrame *frame_5;
    QPushButton *pushButton_7;
    QTableWidget *tableWidget;
    QFrame *frame_6;
    QPushButton *pushButton_9;
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
    QComboBox *comboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QWidget *paraPage;
    QFrame *frame;
    QPushButton *paraButton;
    QPushButton *eventButton;
    QPushButton *curveButton;
    QPushButton *systemButton;
    QPushButton *backButton;
    QFrame *frame_2;
    QPushButton *pushButton_8;

    void setupUi(QWidget *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName(QString::fromUtf8("MainForm"));
        MainForm->resize(1024, 768);
        MainForm->setStyleSheet(QString::fromUtf8(""));
        mainStackedWidget = new QStackedWidget(MainForm);
        mainStackedWidget->setObjectName(QString::fromUtf8("mainStackedWidget"));
        mainStackedWidget->setGeometry(QRect(0, 60, 920, 708));
        mainStackedWidget->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        homePage = new QWidget();
        homePage->setObjectName(QString::fromUtf8("homePage"));
        mainStackedWidget->addWidget(homePage);
        systemPage = new QWidget();
        systemPage->setObjectName(QString::fromUtf8("systemPage"));
        systemStackedWidget = new QStackedWidget(systemPage);
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
        pushButton_7 = new QPushButton(frame_5);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(64, 96, 89, 25));
        QPalette palette;
        QBrush brush(QColor(186, 189, 182, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        QBrush brush1(QColor(0, 0, 0, 0));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        pushButton_7->setPalette(palette);
        tableWidget = new QTableWidget(frame_5);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(40, 152, 161, 113));
        frame_6 = new QFrame(modularChillerPage);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setGeometry(QRect(16, 392, 249, 193));
        frame_6->setStyleSheet(QString::fromUtf8("#frame_6{\n"
"border-image: url(:/UI/Widget/System_Chiller_5.png);}"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        pushButton_9 = new QPushButton(frame_6);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(88, 96, 105, 41));
        QPalette palette1;
        QBrush brush2(QColor(191, 64, 64, 123));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush2);
        QBrush brush3(QColor(255, 255, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        QBrush brush4(QColor(190, 190, 190, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        pushButton_9->setPalette(palette1);
        pushButton_9->setStyleSheet(QString::fromUtf8("background-color: rgba(191, 64, 64, 123);"));
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
        comboBox = new QComboBox(sensorPage);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(456, 232, 105, 25));
        systemStackedWidget->addWidget(sensorPage);
        pushButton = new QPushButton(systemPage);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(24, 642, 105, 41));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush3);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        pushButton->setPalette(palette2);
        pushButton->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        pushButton_2 = new QPushButton(systemPage);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(168, 640, 105, 41));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush3);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush3);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        pushButton_2->setPalette(palette3);
        pushButton_2->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        pushButton_3 = new QPushButton(systemPage);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(320, 640, 105, 41));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush3);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        pushButton_3->setPalette(palette4);
        pushButton_3->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        pushButton_4 = new QPushButton(systemPage);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(472, 640, 105, 41));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush3);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush3);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        pushButton_4->setPalette(palette5);
        pushButton_4->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        pushButton_5 = new QPushButton(systemPage);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(632, 640, 105, 41));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush3);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush3);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        pushButton_5->setPalette(palette6);
        pushButton_5->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        pushButton_6 = new QPushButton(systemPage);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(776, 640, 105, 41));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush3);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush3);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        pushButton_6->setPalette(palette7);
        pushButton_6->setStyleSheet(QString::fromUtf8("font: 16pt \"Ubuntu\";\n"
"border-image: url(:/UI/Frame/\346\214\211\351\224\256.png);"));
        mainStackedWidget->addWidget(systemPage);
        paraPage = new QWidget();
        paraPage->setObjectName(QString::fromUtf8("paraPage"));
        mainStackedWidget->addWidget(paraPage);
        frame = new QFrame(MainForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 1024, 768));
        frame->setStyleSheet(QString::fromUtf8("#frame {\n"
"background-image: url(:/UI/Frame/\350\203\214\346\231\257.png);\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        paraButton = new QPushButton(frame);
        paraButton->setObjectName(QString::fromUtf8("paraButton"));
        paraButton->setGeometry(QRect(920, 520, 101, 101));
        paraButton->setStyleSheet(QString::fromUtf8("#paraButton {background-image: url(:/UI/Frame/\345\217\202\346\225\260\350\256\276\347\275\256.png);\n"
"background-color: transparent; }"));
        eventButton = new QPushButton(frame);
        eventButton->setObjectName(QString::fromUtf8("eventButton"));
        eventButton->setGeometry(QRect(920, 390, 101, 101));
        eventButton->setStyleSheet(QString::fromUtf8("#eventButton {background-image: url(:/UI/Frame/\344\272\213\344\273\266\350\256\260\345\275\225.png);\n"
"background-color: transparent; }"));
        curveButton = new QPushButton(frame);
        curveButton->setObjectName(QString::fromUtf8("curveButton"));
        curveButton->setGeometry(QRect(920, 260, 101, 101));
        curveButton->setStyleSheet(QString::fromUtf8("#curveButton {background-image: url(:/UI/Frame/\346\233\262\347\272\277\350\265\260\345\212\277.png);\n"
"background-color: transparent; }"));
        systemButton = new QPushButton(frame);
        systemButton->setObjectName(QString::fromUtf8("systemButton"));
        systemButton->setGeometry(QRect(920, 130, 101, 101));
        systemButton->setStyleSheet(QString::fromUtf8("#systemButton {background-image: url(:/UI/Frame/\347\263\273\347\273\237\347\233\221\346\216\247.png); \n"
"background-color: transparent; }"));
        backButton = new QPushButton(frame);
        backButton->setObjectName(QString::fromUtf8("backButton"));
        backButton->setGeometry(QRect(950, 50, 33, 35));
        backButton->setStyleSheet(QString::fromUtf8("#backButton {background-image: url(:/UI/Frame/\350\277\224\345\233\236\351\246\226\351\241\265.png);\n"
"background-color: transparent; }"));
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(230, 0, 361, 51));
        frame_2->setStyleSheet(QString::fromUtf8("#frame_2 {background-image: url(:/UI/Frame/\346\240\207\351\242\230.png);background-color: transparent; }"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        pushButton_8 = new QPushButton(frame);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(928, 640, 89, 25));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::Button, brush);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        pushButton_8->setPalette(palette8);
        pushButton_8->setStyleSheet(QString::fromUtf8(""));
        frame->raise();
        mainStackedWidget->raise();

        retranslateUi(MainForm);

        mainStackedWidget->setCurrentIndex(1);
        systemStackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QWidget *MainForm)
    {
        MainForm->setWindowTitle(QCoreApplication::translate("MainForm", "Form", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_7->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        pushButton_7->setText(QCoreApplication::translate("MainForm", "PushButton", nullptr));
        pushButton_9->setText(QCoreApplication::translate("MainForm", "\347\273\204  \347\251\272", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainForm", "New Item", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainForm", "New Item", nullptr));

        pushButton->setText(QCoreApplication::translate("MainForm", "\347\273\204  \347\251\272", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainForm", "\347\273\204  \347\251\272", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainForm", "\347\273\204  \347\251\272", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainForm", "\347\273\204  \347\251\272", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainForm", "\347\273\204  \347\251\272", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainForm", "\347\273\204  \347\251\272", nullptr));
        paraButton->setText(QString());
        eventButton->setText(QString());
        curveButton->setText(QString());
        systemButton->setText(QString());
        backButton->setText(QString());
#if QT_CONFIG(tooltip)
        pushButton_8->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        pushButton_8->setText(QCoreApplication::translate("MainForm", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
