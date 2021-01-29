#include "ubuttonbk.h"
#include <QPainter>
//#include "universalSet/buzzercontrol.h"
#include <unistd.h>


ubuttonBK *ubuttonBK::fatherBK = NULL;
QFont ubuttonBK::fatherFontBK = textControl::instance()->font(textControl::size20);
QPixmap *ubuttonBK::_pix0BK = NULL;
QPixmap *ubuttonBK::_pix1BK = NULL;

ubuttonBK::ubuttonBK(QWidget *parent) :
    Button(parent)
{

    if( fatherBK ){
        connect(fatherBK,SIGNAL(fontChange()),this,SLOT(fontSlot()));
    }

    setFont(fatherFontBK);

    pix0 = NULL;
    pix1 = NULL;

    _alignmentFlag = Qt::AlignCenter;
    _margin = 0;

    privateData = 0;

    connect(this,SIGNAL(clicked()),this,SLOT(sendDataSlot()));
}

void ubuttonBK::setTextColor(QPalette::ColorGroup group, QColor color)
{
    QPalette    palette = this->palette();
    palette.setBrush(group,QPalette::ButtonText,QBrush(color));
    setPalette(palette);
}

//void ubuttonBK::setTextColor(QColor color)
//{
//    QPalette    palette = this->palette();
//    palette.setBrush(QPalette::Normal,QPalette::ButtonText,QBrush(color));
//    setPalette(palette);
//}

void ubuttonBK::setText(const QString &text)
{
    currentText = text;
    update();
}

void ubuttonBK::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);
    setFont(font);
}

void ubuttonBK::setTextSize(textControl::textSize size)
{
    setFont(textControl::instance()->font(size));
}

void ubuttonBK::setFatherFontSize(int size)
{
    if( this == fatherBK ){
        if( fatherFontBK.pointSize() != size ){
            fatherFontBK.setPointSize(size);
            emit fontChange();
        }
    }
}

void ubuttonBK::fontSlot()
{
    setFont(fatherFontBK);
}

void ubuttonBK::sendDataSlot()
{
    emit sendData(privateData);
}

void ubuttonBK::paintEvent(QPaintEvent *e)
{
    if( pix0 && pix1 ){
        QPainter painter(this);
        if( !isEnabled() ){
            painter.setPen(this->palette().color(QPalette::Inactive,QPalette::ButtonText));
        }else{
            painter.setPen(this->palette().color(QPalette::Active,QPalette::ButtonText));
        }
        if( isDown() ){
            painter.drawPixmap(1,1,this->width(),this->height(),*pix0);
            painter.drawText(_margin + 1,1,this->width(),this->height(),_alignmentFlag,currentText);
        }else{
            painter.drawPixmap(0,0,this->width(),this->height(),*pix1);
            painter.drawText(_margin,0,this->width(),this->height(),_alignmentFlag,currentText);
        }
    }else if( _pix0BK && _pix1BK ){
        QPainter painter(this);
        if( !isEnabled() ){
            painter.setPen(this->palette().color(QPalette::Inactive,QPalette::ButtonText));
        }else{
            painter.setPen(this->palette().color(QPalette::Active,QPalette::ButtonText));
        }
        if( isDown() ){
            painter.drawPixmap(1,1,this->width(),this->height(),*_pix0BK);
            painter.drawText(_margin + 1,1,this->width(),this->height(),_alignmentFlag,currentText);
        }else{
            painter.drawPixmap(0,0,this->width(),this->height(),*_pix1BK);
            painter.drawText(_margin,0,this->width(),this->height(),_alignmentFlag,currentText);
        }
    }else{
        QPushButton::paintEvent(e);
        QPainter painter(this);
        if( !isEnabled() ){
            painter.setPen(this->palette().color(QPalette::Inactive,QPalette::ButtonText));
        }else{
            painter.setPen(this->palette().color(QPalette::Active,QPalette::ButtonText));
        }
        painter.drawText(_margin,0,this->width(),this->height(),_alignmentFlag,currentText);
    }
}

bool ubuttonBK::canPress()
{
//    return permissionSet::permissionIsOk(permissionSet::HMI,permissionLevel());
}

ubuttonBK *ubuttonBK::forefather()
{
    if( fatherBK == NULL ){
        fatherBK = new ubuttonBK;
    }
    return fatherBK;
}

void ubuttonBK::setFatherPixmap(STATE state, QString filePath)
{
    if( fatherBK == this ){
        if( press == state ){
            if( _pix0BK ){
                _pix0BK->load(filePath);
            }else{
                _pix0BK = new QPixmap(filePath);
            }
        }else if( release == state ){
            if( _pix1BK ){
                _pix1BK->load(filePath);
            }else{
                _pix1BK = new QPixmap(filePath);
            }
        }
    }
}

void ubuttonBK::setPixmap(ubuttonBK::STATE state, QString filePath)
{
    if( press == state ){
        if( pix0 ){
            pix0->load(filePath);
        }else{
            pix0 = new QPixmap(filePath);
        }
    }else if( release == state ){
        if( pix1 ){
            pix1->load(filePath);
        }else{
            pix1 = new QPixmap(filePath);
        }
    }
}

void ubuttonBK::setTextAlignment(Qt::Alignment flag)
{
    _alignmentFlag = flag;
}

void ubuttonBK::setTextMargin(int margin)
{
    _margin = margin;
}
