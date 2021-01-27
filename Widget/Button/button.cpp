#include "button.h"
#include <unistd.h>

#define USER_TIMER
//#define USER_SLEEP
#define DELAY_TIME 50

Button::Button(QWidget *parent) :
    QPushButton(parent)
{
    pressDown = false;

   // _level = permissionSet::Level0;

    setFocusPolicy(Qt::NoFocus);
    setPalette(*(textControl::instance()->palette()));

#ifdef USER_TIMER

#else
#ifdef USER_SLEEP
    connect(this,SIGNAL(pressed()),SLOT(privatePressSlot()));
#endif
#endif
}

bool Button::isDown()
{
#ifdef USER_TIMER
    return pressDown;
#else
    return QPushButton::isDown();
#endif
}

void Button::timerEvent(QTimerEvent *e)
{
    killTimer(e->timerId());
#ifdef USER_TIMER
    pressDown = false;
    update();
    emit clicked();
#endif
}

void Button::mousePressEvent(QMouseEvent *e)
{
#ifdef USER_TIMER
    if( pressDown )
        return;
#endif

   // buzzerControl::instance()->wantToBB();

    if( canPress() ){
#ifdef USER_TIMER
        pressDown = true;
        update();
        emit pressed();
        startTimer(DELAY_TIME);
#else
        QPushButton::mousePressEvent(e);
#endif
    }
}

void Button::mouseReleaseEvent(QMouseEvent *e)
{
#ifdef USER_TIMER

#else
    QPushButton::mouseReleaseEvent(e);
#endif
}

void Button::privatePressSlot()
{
#ifdef USER_SLEEP
    usleep(DELAY_TIME * 1000);
#endif
}
