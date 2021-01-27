#include "keybutton.h"
#include <QEvent>
#include <QPainter>
#include <unistd.h>

#include "textcontrol.h"
//#include "universalSet/buzzercontrol.h"

#define TEXT_COLOR  "#dfd8c8"

#define USER_TIMER
//#define USER_SLEEP
#define DELAY_TIME 50

keyButton::keyButton(QWidget *parent) :
    QPushButton(parent)
{
    pressDown = false;

    QFont font;
    font.setPointSize(22);

    setFont(font);

    setPalette(*(textControl::instance()->palette()));
    setFocusPolicy(Qt::NoFocus);
    setFlat(true);

#ifdef USER_TIMER

#else
#ifdef USER_SLEEP
    connect(this,SIGNAL(pressed()),SLOT(privatePressSlot()));
#endif
#endif

}

bool keyButton::isDown()
{
#ifdef USER_TIMER
    return pressDown;
#else
    return QPushButton::isDown();
#endif
}

void keyButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawText(0,0,this->width(),this->height(),Qt::AlignCenter,this->text());
    painter.setPen(Qt::NoPen);
    if( isDown() ){
        painter.setBrush(QColor(191,191,191,150));
        painter.drawRoundRect(0,0,this->width(),this->height(),12,12);
    }
}

void keyButton::timerEvent(QTimerEvent *e)
{
    killTimer(e->timerId());
#ifdef USER_TIMER
    pressDown = false;
    update();
    emit clicked();
#endif
}

void keyButton::mousePressEvent(QMouseEvent *e)
{
#ifdef USER_TIMER
    if( pressDown )
        return;
#endif

   // buzzerControl::instance()->wantToBB();

#ifdef USER_TIMER
        pressDown = true;
        update();
        emit pressed();
        startTimer(DELAY_TIME);
#else
        QPushButton::mousePressEvent(e);
#endif

}

void keyButton::mouseReleaseEvent(QMouseEvent *e)
{
#ifdef USER_TIMER

#else
    QPushButton::mouseReleaseEvent(e);
#endif
}

void keyButton::privatePressSlot()
{
#ifdef USER_SLEEP
    usleep(DELAY_TIME * 1000);
#endif
}

