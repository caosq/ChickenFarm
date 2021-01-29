#include "gtoolbutton.h"

#define DEF_WIDTH 40
#define DEF_HEIGHT 40

gtoolButton::gtoolButton(QWidget *parent) :
    QToolButton(parent)
{
    pixOn = NULL;
    pixOff = NULL;

    state = false;

    setCheckable( true );
    setToolButtonStyle( Qt::ToolButtonIconOnly );

    connect(this,SIGNAL(toggled(bool)),SLOT(stateChange(bool)));
}

void gtoolButton::setPixmap(STATE state, QString filePath)
{
    if( press == state ){
        if( pixOn ){
            pixOn->load(filePath);
        }else{
            pixOn = new QPixmap(filePath);
        }
    }else if( release == state ){
        if( pixOff ){
            pixOff->load(filePath);
        }else{
            pixOff = new QPixmap(filePath);
        }
    }
}

QSize gtoolButton::sizeHint()
{
    if( pixOn && pixOff ){
        int width = qMax(pixOn->size().width(),pixOff->size().width());
        int height = qMax(pixOn->size().height(),pixOff->size().height());
        return QSize(width,height);
    }else{
        return QSize(DEF_WIDTH,DEF_HEIGHT);
    }
}

void gtoolButton::paintEvent(QPaintEvent *e)
{
    if( pixOn && pixOff ){
        QPainter painter(this);
        if( state ){
            painter.drawPixmap(0,0,this->width(),this->height(),*pixOn);
        }else{
            painter.drawPixmap(0,0,this->width(),this->height(),*pixOff);
        }
    }
}

void gtoolButton::stateChange(bool _state)
{
    state = _state;
    update();
}
