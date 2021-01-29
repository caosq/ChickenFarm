#include "gqwtscrollbar.h"
#include <QStyle>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QPaintEvent>
#include <QDebug>

//600
#define DEF_WIDTH 680
#define DEF_HEIGHT 32

gqwtScrollBar::gqwtScrollBar(QWidget *parent) :
    QScrollBar(parent)
{
    setStyleSheet("QScrollBar:horizontal"
                      "{"
                      "border-image:url(:/images/baseFile/scbg.png);"
                      "margin:0px 0px 0px 0px;"
                      "}"
                      "QScrollBar::handle:horizontal"
                      "{"
                      "background-image:url(:/images/baseFile/sch.png);"
                      "margin:4px 3px 10px 3px;"
                      "border:1px solid #000000;"
                      "border-radius:8px;"
                      "min-width:60px;"
                      "}"
                      "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal"
                      "{"
                      "background:none;"
                      "}"
                      "QScrollBar::add-line:horizontal"
                      "{"
                      "height:0px;"
                      "width:0px;"
                      "}"
                      "QScrollBar::sub-line:horizontal"
                      "{"
                      "height:0px;"
                      "width:0px;"
                      "}"
                  );
    resize(DEF_WIDTH,DEF_HEIGHT);

    setPageStep(1);
    setTracking(false);
}

QSize gqwtScrollBar::sizeHint()
{
    return QSize(DEF_WIDTH,DEF_HEIGHT);
}

void gqwtScrollBar::timerEvent(QTimerEvent *e)
{

}

void gqwtScrollBar::gotoMax()
{
    setValue(maximum());
}

void gqwtScrollBar::gotoMin()
{
    setValue(minimum());
}
