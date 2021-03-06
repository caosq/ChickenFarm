#include "skeyboard.h"

#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>

//#include "languageTranslate/translate.h"

#define  KEY_X  308
#define  KEY_Y  370
#define  EDIT_STYLE      "font-size:28px;color:#dfd8c8;border:0px;background-color:transparent;"

skeyboard::skeyboard(QWidget *parent) :
    QDialog(parent)
{
    moveState = false;

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_ShowModal);
    setAutoFillBackground(true);

    setGeometry(300, 200, KEY_X, KEY_Y);

    digitButton.resize(10);

    int x[4] = {0,12,85,157};
    int y[4] = {0,233,168,102};
    int k = 1;

    for(int i = 1; i < 4; i++){
        for(int j = 1; j < 4; j++ ){
            digitButton[i * j]  = new keyButton(this);
            digitButton[i * j]->setText(QString::number(k));
            digitButton[i * j]->setGeometry(x[j],y[i],66,58);
            k++;
            connect(digitButton[i * j],SIGNAL(pressed()),this,SLOT(digitClicked()));
        }
    }

    lineEdit = new QLineEdit(this);
    lineEdit->setEnabled(false);
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setStyleSheet(EDIT_STYLE);
    lineEdit->setGeometry(14, 44, 280, 48);

    //connect(translate::instance(),SIGNAL(translatorIsReady()),this,SLOT(langChangeSlot()));
}

void skeyboard::mousePressEvent(QMouseEvent *e)
{
    mousePosition = e->pos();
    if(mousePosition.y() > 42)
        return;
    moveState = true;
}

void skeyboard::mouseReleaseEvent(QMouseEvent *e)
{
    moveState = false;
}

void skeyboard::mouseMoveEvent(QMouseEvent *e)
{
    if ( moveState==true )
    {
        QPoint movePot = e->globalPos();
        if( movePot.y() < mousePosition.y())
            return;
        if( movePot.y() - mousePosition.y() > 735)
            return;
        if( movePot.x() + (154) - mousePosition.x() > 1024 )
            return;
        if( movePot.x() < mousePosition.x() - (154))
            return;
        movePot = e->globalPos() - mousePosition;
        move(movePot);
    }
}

void skeyboard::showEvent(QShowEvent *e)
{
    move( (qApp->desktop()->width() - width()) / 2,
          (qApp->desktop()->height() - height()) / 2);
}

void skeyboard::hideSlot()
{
    readyExit();
    hide();
}

void skeyboard::setAlwaysStay(bool state)
{
    if(state)
    {
//        disconnect(ScreenSaver::instance(),SIGNAL(isTimeToBack()),this,SLOT(hideSlot()));
    }
    else
    {
//        connect(ScreenSaver::instance(),SIGNAL(isTimeToBack()),this,SLOT(hideSlot()));
    }
}

void skeyboard::setLanguage(QLocale::Language lang)
{
    retranslateUi(lang);
}

void skeyboard::langChangeSlot()
{
 //   retranslateUi(translate::instance()->currentLanguage());
    retranslateUi();
}

