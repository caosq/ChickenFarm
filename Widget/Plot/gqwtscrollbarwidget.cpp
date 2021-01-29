#include "gqwtscrollbarwidget.h"

#define CHILDREN_SPEC 5
#define DEF_LENGTH 750

#define RIGHT_BT_PATH ":/images/baseFile/scRightBt.png"
#define LEFT_BT_PATH ":/images/baseFile/scLeftBt.png"

#include <QDebug>

gqwtScrollBarWidget::gqwtScrollBarWidget(QWidget *parent) :
    QWidget(parent)
{
    _scrollBar = new gqwtScrollBar(this);
    _scrollBar->setOrientation(Qt::Horizontal);

    _rBt = new qwtScrollBarButton(this);
    _rBt->setPixmap(RIGHT_BT_PATH);

    _lBt =new qwtScrollBarButton(this);
    _lBt->setPixmap(LEFT_BT_PATH);

    autoLength(DEF_LENGTH);

    connect(_lBt,SIGNAL(clicked()),_scrollBar,SLOT(gotoMin()));
    connect(_rBt,SIGNAL(clicked()),_scrollBar,SLOT(gotoMax()));
    connect(_scrollBar,SIGNAL(valueChanged(int)),this,SIGNAL(valueChanged(int)));
}

void gqwtScrollBarWidget::setGeometry(int x, int y, int w, int h)
{
    move(x,y);
    autoLength(w);
}

void gqwtScrollBarWidget::reloadScrollBar()
{
    _scrollBar->setMinimum(0);
    _scrollBar->setMaximum(0);
    _scrollBar->setValue(0);
}

void gqwtScrollBarWidget::setMaximumVal(int max)
{
    bool goMax = false;
    if( _scrollBar->value() == _scrollBar->maximum() ){
        goMax = true;
    }

    _scrollBar->setMaximum(max);

    if( goMax ){
        _scrollBar->setValue(max);
    }
}

int gqwtScrollBarWidget::maximum()
{
    return _scrollBar->maximum();
}

void gqwtScrollBarWidget::goMax()
{
    _scrollBar->gotoMax();
}

void gqwtScrollBarWidget::goMin()
{
    _scrollBar->gotoMin();
}

int gqwtScrollBarWidget::currentValue()
{
    return _scrollBar->value();
}

void gqwtScrollBarWidget::autoLength(int length)
{
    int height = qMax(_scrollBar->height(),_rBt->height());
    int width = length;
    int barLength = width - _rBt->width() - _lBt->width() - CHILDREN_SPEC * 2;

    resize(width,height);

    _lBt->setGeometry(0,0,_lBt->width(),_lBt->height());
    _scrollBar->setGeometry(_lBt->width() + CHILDREN_SPEC,0,
                            barLength,_scrollBar->height());
    _rBt->setGeometry(width - _rBt->width(),0,
                      _rBt->width(),_rBt->height());
}


