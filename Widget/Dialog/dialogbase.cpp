#include "dialogbase.h"

#include <QMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>

//#include "languageTranslate/translate.h"
#include "textcontrol.h"

#define TITLE "#3d3d3d"
#define BG_COLOR "#0d74bc"

QColor dialogBase::_backgroundColor(BG_COLOR);
QColor dialogBase::_frameLessColor(TITLE);

//信息弹框类
//提供设置信息，设置显示的按键，屏保时是否隐藏等功能
dialogBase::dialogBase(QWidget *parent) :
    QDialog(parent)
{
    //隐藏原有边框
    setWindowFlags(Qt::FramelessWindowHint);
    //设置模态显示
    setAttribute(Qt::WA_ShowModal,true);
    //自动填充背景
    setAutoFillBackground(true);

    //允许窗口移动标志
    moveState = true;
    //窗口按下标志
    pressed = false;

    //设置文字字号
    setFont(textControl::instance()->font(textControl::size18));
    //设置文字颜色
    setPalette(*(textControl::instance()->palette()));

    //外框抬头标题栏颜色
    frameLessColor = _frameLessColor;

    //设置背景色
    setBackGroundColor(_backgroundColor);
    //设置外框颜色
    setFramelessWindowColor(frameLessColor);

    //设置为屏保后隐藏，true为屏保后不隐藏
    setAlwaysStay(false);
//    connect(translate::instance(),SIGNAL(translatorIsReady()),this,SLOT(langChangeSlot()));
}

void dialogBase::setBackGroundColor(QColor color)
{
    QPalette palette = this->palette();
    palette.setBrush(this->backgroundRole(),QBrush(color));

    setPalette(palette);
}

void dialogBase::setFramelessWindowColor(QColor color)
{
    frameLessColor = color;
}

void dialogBase::disableMove(bool state)
{
    moveState = state;
}

void dialogBase::setAlwaysStay(bool state)
{
/*    if( state ){
        disconnect(ScreenSaver::instance(),SIGNAL(isTimeToBack()),this,SLOT(hide()));
    }else{
        connect(ScreenSaver::instance(),SIGNAL(isTimeToBack()),this,SLOT(hide()));
    }*/
}

void dialogBase::setLanguage(QLocale::Language lang)
{
    retranslateUi(lang);
}


void dialogBase::mouseMoveEvent ( QMouseEvent * event )
{
    if( !moveState )
        return;
    if ( pressed==true )
    {
        QPoint movePot = event->globalPos();
        if( movePot.y() < mousePosition.y())
            return;
        if( movePot.y() - mousePosition.y() > 735)
            return;
        if( movePot.x() + (this->width() / 2) - mousePosition.x() > 1024 )
            return;
        if( movePot.x() < mousePosition.x() - (this->width() / 2))
            return;
        movePot = event->globalPos() - mousePosition;
        move(movePot);
    }
}

void dialogBase::mousePressEvent ( QMouseEvent * event )
{
    mousePosition = event->pos();
    if(mousePosition.y() > 33)
        return;
    pressed = true;
}

void dialogBase::mouseReleaseEvent(QMouseEvent * event)
{
    pressed = false;
}

void dialogBase::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    QPen pen(frameLessColor,2);
    painter.setPen(pen);
    painter.drawRect(1,30,this->width() - 2,this->height() - 30 - 1);

    painter.setBrush(QBrush(frameLessColor));
    painter.drawRect(0,0,this->width(),33);

}

void dialogBase::showEvent(QShowEvent *e)
{
    move( (qApp->desktop()->width() - width()) / 2,
          (qApp->desktop()->height() - height()) / 2);
}

void dialogBase::retranslateUi(QLocale::Language lang)
{
    switch(lang)
    {
        case QLocale::Chinese:{
        setChinese();
    }
        break;
    case QLocale::English:{
        setEnglish();
    }
        break;
    default:
        break;
    }
}

void dialogBase::langChangeSlot()
{
//    retranslateUi(translate::instance()->currentLanguage());
//    retranslateUi();
}
