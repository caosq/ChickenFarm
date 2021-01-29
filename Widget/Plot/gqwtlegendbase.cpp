#include "gqwtlegendbase.h"

#include <QDebug>

gQwtLegendBase::gQwtLegendBase(QWidget *parent) :
    QLabel(parent)
{
    _iStyle = drawIcon;
    _pixmap = 0;
    _icolor = QColor(Qt::red);
    _bcolor = QColor(Qt::gray);
    _bcolor.setAlpha(100);
    _bgState = false;
}

void gQwtLegendBase::setPixmap(QString path)
{
    if( _pixmap ){
        _pixmap->load(path);
    }else{
        _pixmap = new QPixmap(path);
    }
}

void gQwtLegendBase::setIconStyle(gQwtLegendBase::iconStyle style)
{
    _iStyle = style;
    update();
}

void gQwtLegendBase::setGeometry(int x, int y, int w, int h)
{

    QLabel::setGeometry(x,y,w,h);
}

void gQwtLegendBase::setIconColor(QColor color)
{
    _icolor = color;
}

void gQwtLegendBase::setBackgroundColor(QColor color)
{
    _bcolor = color;
}

void gQwtLegendBase::setNoBackground(bool ok)
{
    _bgState = ok;
    update();
}

QColor gQwtLegendBase::iconColor()
{
    return _icolor;
}

gQwtLegendBase::iconStyle gQwtLegendBase::iconShowStyle()
{
    return _iStyle;
}

QColor gQwtLegendBase::backgroundColor()
{
    return _bcolor;
}

QRect gQwtLegendBase::textRect()
{
    QRect tRect(height(),0,width() - height(),height());
    return tRect;
}

