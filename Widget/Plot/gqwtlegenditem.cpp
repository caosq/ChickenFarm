#include "gqwtlegenditem.h"

#define HS 4
#define WS 4
#define ROUND_RADIUS 4

#define DEF_HEIGHT 22
#define DEF_WIDTH   80

QSize   gQwtLegendItem::_sizeHint = QSize(DEF_WIDTH,DEF_HEIGHT);

gQwtLegendItem::gQwtLegendItem(QWidget *parent) :
    gQwtLegendBase(parent)
{

}

void gQwtLegendItem::setSizeHint(QSize size)
{
    _sizeHint = size;
}

QSize gQwtLegendItem::legendSizeHint()
{
    return _sizeHint;
}

void gQwtLegendItem::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing,true);

    //background
    if( !noBackground() ){
        p.setPen(Qt::NoPen);
        p.setBrush(backgroundColor());
        p.drawRoundedRect(0,0,this->width(),this->height(),ROUND_RADIUS,ROUND_RADIUS);
    }

    //icon
    if( drawIcon == iconShowStyle() ){
        qreal radius = (height() - HS * 2) / 2;
        qreal dia = radius * 2;
        qreal cx = radius + WS;
        qreal cy = radius + HS;

        QRadialGradient tb(cx,cy,radius,cx + radius / 2 ,cy - radius / 2);

        tb.setColorAt(0.0,Qt::white);
        tb.setColorAt(0.356,iconColor());

        p.setPen(QPen(Qt::gray,1));
        p.setBrush(QBrush(tb));

        p.drawEllipse(WS,HS,dia,dia);
    }

    if( pixmap == iconShowStyle() ){
        QPixmap *temp = iconPixmap();
        if( temp ){
            if( temp->height() < height() ){
                int sy = (height() - temp->height()) / 2;
                p.drawPixmap(0,sy,*temp);
            }else{
                p.drawPixmap(0,0,*temp);
            }
        }
    }
    //text

    p.setPen(Qt::white);
    p.drawText(textRect(),Qt::AlignLeft | Qt::AlignVCenter,text());
}

