#include "g_qwtscaledraw.h"

#define DEF_POINT_SIZE  12
#define DEF_TEXT_COLOR "#ffffff"
#define DEF_FONT_FAMILEY "Bitstream Charter"

#include <QDebug>
#include <qmath.h>

g_qwtScaleDraw::g_qwtScaleDraw() :
    QwtScaleDraw()
{
    _style = style1;
    _val_style = valueStyle;
    _font.setFamily(DEF_FONT_FAMILEY);
    _font.setPointSize(DEF_POINT_SIZE);
    _font.setBold(true);

    _color = QColor(DEF_TEXT_COLOR);

    _dot = 0;
    _baseTime.setHMS(0,0,0,0);

}

QwtText g_qwtScaleDraw::label(double v) const
{
    QwtText text;

    if( QwtScaleDraw::BottomScale == alignment() ){
        QTime updateTime = _baseTime.addSecs(static_cast<int>(v));
        text = updateTime.toString();
        text.setFont(_font);
        text.setColor(_color);
    }else if( QwtScaleDraw::LeftScale == alignment() ){
        text = QwtScaleDraw::label(v / qPow(10,_dot));
        text.setFont(_font);
        text.setColor(_color);
    }
    return text;
}

void g_qwtScaleDraw::setScaleDrawStyle(g_qwtScaleDraw::scaleDrawStyle style)
{
    _style = style;
}

void g_qwtScaleDraw::setScaleValStyle(g_qwtScaleDraw::scaleValStyle style)
{
    _val_style  = style;
}

void g_qwtScaleDraw::setScaleLabelTextSize(int size)
{
    _font.setPointSize(size);
}

void g_qwtScaleDraw::setScaleLabelTextColor(QColor color)
{
    _color = color;
}

void g_qwtScaleDraw::setScaleLabelTextFamily(QString family)
{
    _font.setFamily(family);
}

void g_qwtScaleDraw::setScaleRaised(int dot)
{
    _dot = dot;
}

int g_qwtScaleDraw::widgetWidth()
{
    return this->maxLabelWidth(_font) + this->maxTickLength();
}

void g_qwtScaleDraw::drawBackbone(QPainter *p) const
{
    switch( _style )
    {
    case style0:
        break;
    case style1:
    {
        QPen pen;
        pen.setWidth(3);

        QLinearGradient linear;
        linear.setStart(0,0);

        if( QwtScaleDraw::BottomScale == alignment() ){
            linear.setFinalStop(length(),0);
        }else if( QwtScaleDraw::LeftScale == alignment() ){
            linear.setFinalStop(0,length());
        }
        linear.setColorAt(0.0,QColor(24,77,160));
        linear.setColorAt(0.5,QColor(49,183,226));
        linear.setColorAt(1.0,QColor(24,77,160));

        pen.setBrush(QBrush(linear));
        p->setPen(pen);
    }
        break;
    default:
        break;
    }

    QwtScaleDraw::drawBackbone(p);
}

void g_qwtScaleDraw::drawTick(QPainter *p, double val, double len) const
{

    switch( _style )
    {
    case style0:
        break;
    case style1:
    {
        QPen pen;
        pen.setWidth(3);

        QLinearGradient linear;
        linear.setStart(0,0);

        if( QwtScaleDraw::BottomScale == alignment() ){
            linear.setFinalStop(length(),0);
        }else if( QwtScaleDraw::LeftScale == alignment() ){
            linear.setFinalStop(0,length());
        }
        linear.setColorAt(0.0,QColor(24,77,160));
        linear.setColorAt(0.5,QColor(49,183,226));
        linear.setColorAt(1.0,QColor(24,77,160));

        pen.setBrush(QBrush(linear));
        p->setPen(pen);
    }
        break;
    default:
        break;
    }

    QwtScaleDraw::drawTick(p,val,len);
}
