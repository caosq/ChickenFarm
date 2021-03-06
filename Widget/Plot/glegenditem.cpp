#include "glegenditem.h"

#include <QDebug>
#include <qwt_painter_command.h>

#define EXTEND_SPACE 4
#define REVISE_X 2

gLegendItem::gLegendItem() :
    QwtPlotLegendItem()
{
    _style = style0;

    setRenderHint(QwtPlotItem::RenderAntialiased);
    QColor  color(Qt::white);

    setTextPen( color );
    QColor c( Qt::gray );
    c.setAlpha( 100 );

    setBorderPen( c );
    setBackgroundBrush( c );
}

QSize gLegendItem::minimumSize(const QwtLegendData &data) const
{
    QSize size = QwtPlotLegendItem::minimumSize(data);
    if( _style == style1){
        size.setWidth(size.width() + EXTEND_SPACE);
    }
    return size;
}

void gLegendItem::setStyle(gLegendItem::legendItemStyle style)
{
    _style = style;
}

void gLegendItem::drawIcon(QPainter *p, QColor baseColor, QRectF rectf) const
{
    QRadialGradient tb(rectf.center().x(), rectf.center().y(),
                       rectf.width(), rectf.center().x() + rectf.width() / 4,
                       rectf.center().y() - rectf.height() / 4);

    tb.setColorAt(0.0,Qt::white);
    tb.setColorAt(0.256,baseColor);

//    QConicalGradient board(rectf.center(),60);
//    board.setColorAt(0.0,QColor(210,210,210));
//    board.setColorAt(0.5,QColor(140,140,140));
//    board.setColorAt(1,QColor(210,210,210));
//    p->setPen(QPen(board,1));
    p->setPen(QPen(Qt::gray,1));

    p->setBrush(QBrush(tb));

    p->drawEllipse(rectf.center(),
                         rectf.width() / 2,
                         rectf.height() / 2);
}

void gLegendItem::drawLegendData(QPainter *painter,
                                 const QwtPlotItem *plotItem, const QwtLegendData &data,
                                 const QRectF &rect) const
{
    if( _style == style0 ){
        QwtPlotLegendItem::drawLegendData(painter,
                                          plotItem,data,rect);
    }else{
        Q_UNUSED( plotItem );

        const int m = itemMargin();
        const QRectF r = rect.toRect().adjusted( m, m, -m, -m );

        painter->setClipRect( r, Qt::IntersectClip );

        int titleOff = 0;

        const QwtGraphic graphic = data.icon();

        if ( !graphic.isEmpty() ){
            QSizeF size = QSizeF(graphic.defaultSize().width() + EXTEND_SPACE,
                                 graphic.defaultSize().height() + EXTEND_SPACE);

            QRectF iconRect( r.topLeft(), size );

            iconRect.moveCenter(
                        QPoint( iconRect.center().x() + REVISE_X, rect.center().y() ) );

            QVector< QwtPainterCommand > array = graphic.commands();

            if( !array.isEmpty() ){
                QColor bc = array.at(0).stateData()->brush.color();
                drawIcon(painter,bc,iconRect);
            }

            //graphic.render( painter, iconRect, Qt::KeepAspectRatio );
            titleOff += iconRect.width() + itemSpacing();
        }

        const QwtText text = data.title();
        if ( !text.isEmpty() ){
            painter->setPen( textPen() );
            painter->setFont( font() );

            const QRectF textRect = r.adjusted( titleOff, 0, 0, 0 );
            text.draw( painter, textRect );
        }
    }

}


