#include "gzoomwidget.h"

#define ZOOM_PIX_OFF ":/images/baseFile/zoomOff.png"
#define ZOOM_PIX_ON ":/images/baseFile/zoomOn.png"
#define ZOOM_CLEAR ":/images/baseFile/zero.png"

#define BT_WIDTH 40
#define BT_HEIGHT 40

gZoomWidget::gZoomWidget(QWidget *parent) :
    QWidget(parent)
{
    _btnZoom = new gtoolButton(this);
    _btnZoom->setPixmap(gtoolButton::press,ZOOM_PIX_ON);
    _btnZoom->setPixmap(gtoolButton::release,ZOOM_PIX_OFF);
    _btnZoom->setGeometry(0,BT_HEIGHT * 0,BT_WIDTH,BT_HEIGHT);

    _clearZoom = new gtoolButton(this);
    _clearZoom->setPixmap(gtoolButton::press,ZOOM_CLEAR);
    _clearZoom->setPixmap(gtoolButton::release,ZOOM_CLEAR);
    _clearZoom->setGeometry(0,BT_HEIGHT * 1,BT_WIDTH,BT_HEIGHT);

    resize(BT_WIDTH,BT_HEIGHT * 2);

    connect( _btnZoom, SIGNAL( toggled( bool ) ),SIGNAL(zoomToggled(bool)));
    connect( _clearZoom, SIGNAL( released()),SIGNAL(clearZoom()));
}

QSize gZoomWidget::sizeHint()
{
    return QSize(BT_WIDTH,BT_HEIGHT * 2);
}
