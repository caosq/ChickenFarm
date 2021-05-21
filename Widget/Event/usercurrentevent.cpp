#include "usercurrentevent.h"

#define BG ":/UI/baseFile/eventBg.png"

UserCurrentEvent::UserCurrentEvent(QString saveName, QWidget *parent) :
    QWidget(parent)
{
    table = new CurrentEventTable(4,saveName,this);

    _bg = new QPixmap(BG);

    table->setItemTextSize(16);

    init();

    //retranslateUi(translate::instance()->currentLanguage());

    //connect(translate::instance(),SIGNAL(translatorIsReady()),this,SLOT(langChangeSlot()));
}

//void userCurrentEvent::setGeometry(int x, int y, int w, int h)
//{
//    move(x,y);
//}

void UserCurrentEvent::init()
{
    resize(800,580);
    table->setGeometry(0,50,900,545);
    table->setColumnWidth(0, 60);
    table->setColumnWidth(1, 200);
    table->setColumnWidth(2, 200);
    table->setColumnWidth(3, 400);

    table->setBackgroundPixmap(_bg);

    _num = new TextLabel(15, QColor(255,255,255), this);
    _num->setAlignment(Qt::AlignLeft);
    _num->setGeometry(0,10,80,30);

    _time0 = new TextLabel(15, QColor(255,255,255), this);
    _time0->setAlignment(Qt::AlignLeft);
    _time0->setGeometry(60,10,240,30);

    _time1 = new TextLabel(15, QColor(255,255,255), this);
    _time1->setAlignment(Qt::AlignLeft);
    _time1->setGeometry(260,10,240,30);

    _detail = new TextLabel(15, QColor(255,255,255), this);
    _detail->setAlignment(Qt::AlignLeft);
    _detail->setGeometry(460,10,240,30);

    _num->setText("序号");
    _time0->setText("发生时间");
    _time1->setText("结束时间");
    _detail->setText("事件信息");
}

//void userCurrentEvent::paintEvent(QPaintEvent *e)
//{
//    QPainter painter(this);

//    QPen m_pen;
//    m_pen.setColor("#68838b");
//    m_pen.setWidth(2);

//    painter.setPen(m_pen);
//    painter.drawLine(QPoint(0, 30), QPoint(800, 30));
//    painter.drawRect(QRect(1,38,width() - 2,height() - 40));

//}

void UserCurrentEvent::retranslateUi(QLocale::Language lang)
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

void UserCurrentEvent::langChangeSlot()
{
    //retranslateUi(translate::instance()->currentLanguage());
}

void UserCurrentEvent::setChinese()
{
    _num->setText("序号");
    _time0->setText("发生时间");
    _time1->setText("恢复时间");
    _detail->setText("事件信息");
}

void UserCurrentEvent::setEnglish()
{
    _num->setText("No.");
    _time0->setText("Occur");
    _time1->setText("Resume");
    _detail->setText("Event Information");
}
