#include "userhistoryevent.h"

#define BG ":/UI/baseFile/eventBg.png"

UserHistoryEvent::UserHistoryEvent(QString saveName, QWidget *parent) :
    QWidget(parent)
{
    table = new HistoryEventTable(3,saveName,this);

    _bg = new QPixmap(BG);

    table->setItemTextSize(16);

    init();

//    retranslateUi(translate::instance()->currentLanguage());
//    connect(translate::instance(),SIGNAL(translatorIsReady()),this,SLOT(langChangeSlot()));
}

void UserHistoryEvent::setGeometry(int x, int y, int w, int h)
{
    move(x,y);
}

void UserHistoryEvent::init()
{
    resize(800,580);
    table->setGeometry(0,50,900,545);
    table->setColumnWidth(0, 60);
    table->setColumnWidth(1, 200);
    table->setColumnWidth(2, 600);

    table->setBackgroundPixmap(_bg);

    _num = new TextLabel(15, QColor(255,255,255), this);
    _num->setAlignment(Qt::AlignLeft);
    _num->setGeometry(0,10,80,30);

    _time0 = new TextLabel(15, QColor(255,255,255), this);
    _time0->setAlignment(Qt::AlignLeft);
    _time0->setGeometry(60,10,240,30);

    _detail = new TextLabel(15, QColor(255,255,255), this);
    _detail->setAlignment(Qt::AlignLeft);
    _detail->setGeometry(260,10,480,30);

    _num->setText("序号");
    _time0->setText("发生时间");
    _detail->setText("事件信息");
}

//void userHistoryEvent::paintEvent(QPaintEvent *e)
//{
//    QPainter painter(this);

//    QPen m_pen;
//    m_pen.setColor("#68838b");
//    m_pen.setWidth(2);

//    painter.setPen(m_pen);
//    painter.drawLine(QPoint(0, 30), QPoint(800, 30));
//    painter.drawRect(QRect(1,38,width() - 2,height() - 40));

//}

void UserHistoryEvent::retranslateUi(QLocale::Language lang)
{
    switch(lang)
    {
    case QLocale::Chinese:
    {
        setChinese();
    }
        break;
    case QLocale::English:
    {
        setEnglish();
    }
        break;
    default:
        break;
    }
}

void UserHistoryEvent::langChangeSlot()
{
//    retranslateUi(translate::instance()->currentLanguage());
}

void UserHistoryEvent::setChinese()
{
    _num->setText("序号");
    _time0->setText("发生时间");
    _detail->setText("事件信息");
}

void UserHistoryEvent::setEnglish()
{
    _num->setText("No.");
    _time0->setText("Occur");
    _detail->setText("Event Information");
}
