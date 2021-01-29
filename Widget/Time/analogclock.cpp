#include "analogclock.h"
//#include "universalSet/buzzercontrol.h"
#include  <QDebug>

#define CLOCK_X 200
#define CLOCK_Y 200
#define TIMER_STEP  500

#define KEY_STYLE "QPushButton{\n"\
                "font-size:22px;\n"\
                "color:#dfd8c8;\n"\
                "background-color: transparent;\n"\
                "}"

analogclock *analogclock::my_clock = NULL;
QTimer *analogclock::systemdTimer = NULL;


clockBase::clockBase(QWidget *parent, int w, int h) :
    QWidget(parent)
{
    resize(w, h);

    basePen = new QPen(QColor("#efeeeb"));

}

void clockBase::paintEvent(QPaintEvent *)
{
    int side = qMin(width(), height());
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    basePen->setWidth(3);
    painter.setPen(*basePen);

    //画分钟刻度
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
        {
            painter.drawPoint(94,0);
        }
        painter.rotate(6.0);
    }

    //画整点刻度
    basePen->setWidth(2);
    painter.setPen(*basePen);

    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }
}

//模拟时钟
//提供时，分，秒设置功能
analogclock::analogclock(QWidget *parent, int w, int h) :
    QWidget(parent)
{
    resize(w, h);
    my_base = new clockBase(this,w,h);
    my_base->setGeometry(0,0,w,h);

    m_hour = new QPushButton(this);
    m_hour->setFocusPolicy(Qt::NoFocus);
    m_hour->setGeometry(40,210,40,30);
    m_hour->setStyleSheet(KEY_STYLE);

    m_min = new QPushButton(this);
    m_min->setFocusPolicy(Qt::NoFocus);
    m_min->setGeometry(80,210,40,30);
    m_min->setStyleSheet(KEY_STYLE);

    m_sec = new QPushButton(this);
    m_sec->setFocusPolicy(Qt::NoFocus);
    m_sec->setGeometry(120,210,40,30);
    m_sec->setStyleSheet(KEY_STYLE);


    InitMember();

    m_connect();
}

QTimer *analogclock::sysTimer()
{
    if( systemdTimer == NULL ){
        systemdTimer = new QTimer;
        systemdTimer->setInterval(TIMER_STEP);
        systemdTimer->start();
    }
    return systemdTimer;
}

analogclock *analogclock::instance(QWidget *parent, int w, int h)
{
    if(NULL == my_clock)
    {
        my_clock = new analogclock(parent,w,h);
    }
    return my_clock;
}

void analogclock::InitMember()
{
    //时，分，秒绘制坐标
    hourHand[0] = QPoint(-2,0);
    hourHand[1] = QPoint(-2,-50);
    hourHand[2] = QPoint(2,-50);
    hourHand[3] = QPoint(2,0);

    minuteHand[0] = QPoint(-2,0);
    minuteHand[1] = QPoint(-2,-70);
    minuteHand[2] = QPoint(2,-70);
    minuteHand[3] = QPoint(2,0);

    secondHand[0] = QPoint(-1,30);
    secondHand[1] = QPoint(-1,-90);
    secondHand[2] = QPoint(1,-90);
    secondHand[3] = QPoint(1,30);
    //

    //时，分，秒及中心圆点颜色
    hourColor = new QColor("#efeeeb");
    minuteColor = new QColor("#efeeeb");
    secondColor = new QColor(Qt::red);
    ellipseColor = new QColor("#efeeeb");

    Running = true;
    changeState = false;

    systemdTimer = new QTimer;
    systemdTimer->setInterval(TIMER_STEP);
    systemdTimer->start();
}

void analogclock::m_connect()
{
    connect(m_hour,SIGNAL(clicked()),SLOT(hourBtSlot()));
    connect(m_min,SIGNAL(clicked()),SLOT(minBtSlot()));
    connect(m_sec,SIGNAL(clicked()),SLOT(secBtSlot()));
    connect(systemdTimer,SIGNAL(timeout()),this,SLOT(update()));
}

QTime analogclock::getTime()
{
    return clocktime;
}

bool analogclock::runningState()
{
    return Running;
}

bool analogclock::timeChanged()
{
    return changeState;
}

void analogclock::flushClock(bool yes)
{
    Running = yes;
}

//画时钟
void analogclock::paintEvent(QPaintEvent *)
{
    if( isHidden() )
    {
        return;
    }

    int side = 200;

    if( Running )
    {
        clocktime = QTime::currentTime();
    }

    QPainter painter(this);

    m_hour->setText(clocktime.toString("hh:"));
    m_min->setText(clocktime.toString("mm:"));
    m_sec->setText(clocktime.toString("ss"));

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(200 / 2, 200 / 2);
    painter.scale(side / 200.0, side / 200.0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(*hourColor);

    painter.save();
    painter.rotate(30.0 * ((clocktime.hour() + clocktime.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 4);
    painter.restore();

    painter.setPen(Qt::NoPen);
    painter.setBrush(*minuteColor);

    painter.save();
    painter.rotate(6.0 * (clocktime.minute() + clocktime.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 4);
    painter.restore();

    painter.setPen(Qt::NoPen);
    painter.setBrush(*secondColor);

    painter.save();
    painter.rotate(6.0 * clocktime.second());
    painter.drawConvexPolygon(secondHand,4);
    painter.restore();

    painter.setBrush(QBrush(*ellipseColor));
    painter.drawEllipse(QPoint(0,0),5,5);

}

void analogclock::showEvent(QShowEvent *e)
{
    flushClock(true);
    changeState = false;
}

void analogclock::hideEvent(QHideEvent *e)
{
    flushClock(false);
}

//设置小时
void analogclock::hourBtSlot()
{
//    buzzerControl::instance()->wantToBB();
    t_hour = clocktime.hour();

   int hour =  aKeyBoard::getValue(clocktime.hour(),23,0,0);

   if( hour >= 0 && hour <= 23 ){
       if( !changeState ){
           changeState = ( hour == t_hour) ? false:true;
       }

       if( Running ){
           Running = !changeState;
       }

       t_hour = hour;
   }
   clocktime.setHMS(t_hour,clocktime.minute(),clocktime.second());
   update();
}

//设置分钟
void analogclock::minBtSlot()
{
//    buzzerControl::instance()->wantToBB();
    t_min = clocktime.minute();

    int min =  aKeyBoard::getValue(t_min,59,0,0);
    if( min >= 0 && min <= 59 ){
        if( !changeState ){
            changeState = (min == t_min) ? false:true;
        }

        if( Running ){
            Running = !changeState;
        }

        t_min = min;
    }
    clocktime.setHMS(clocktime.hour(),t_min,clocktime.second());
    update();
}

//设置秒
void analogclock::secBtSlot()
{
//    buzzerControl::instance()->wantToBB();
    t_sec = clocktime.second();

    int sec =  aKeyBoard::getValue(t_sec,59,0,0);

    if( sec >= 0 && sec <= 59 ){
        if( !changeState ){
            changeState = (sec == t_sec) ? false:true;
        }

        if( Running ){
            Running = !changeState;
        }
        t_sec = sec;
    }

    clocktime.setHMS(clocktime.hour(),clocktime.minute(),t_sec);
    update();
}

