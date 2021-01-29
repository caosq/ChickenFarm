#include "qwtscrollbarbutton.h"

#define DELAY_TIME 50

#define DEF_WIDTH 59
#define DEF_HEIGHT 25

qwtScrollBarButton::qwtScrollBarButton(QWidget *parent) :
    QPushButton(parent)
{
    _pressDown = false;

    _pix = 0;

    setFocusPolicy(Qt::NoFocus);
    resize(DEF_WIDTH,DEF_HEIGHT);
}

QSize qwtScrollBarButton::sizeHint()
{
    return QSize(DEF_WIDTH,DEF_HEIGHT);
}

bool qwtScrollBarButton::isDown()
{
    return _pressDown;
}

bool qwtScrollBarButton::setPixmap(QString path)
{
    if( _pix == 0 ){
        _pix = new QPixmap(path);
    }else{
       _pix->load(path);
    }
}

void qwtScrollBarButton::timerEvent(QTimerEvent *e)
{
    killTimer(e->timerId());

    _pressDown = false;
    update();
    emit clicked();
}

void qwtScrollBarButton::mousePressEvent(QMouseEvent *e)
{
    if( _pressDown )
        return;
    _pressDown = true;
    update();
    emit pressed();
    startTimer(DELAY_TIME);
}

void qwtScrollBarButton::mouseReleaseEvent(QMouseEvent *e)
{

}

void qwtScrollBarButton::paintEvent(QPaintEvent *e)
{
    if( _pix == 0 ){
        QPushButton::paintEvent(e);
    }else{
        QPainter painter(this);
        if( isDown() ){
            painter.drawPixmap(1,1,this->width(),this->height(),*_pix);
        }else{
            painter.drawPixmap(0,0,this->width(),this->height(),*_pix);
        }
    }
}

