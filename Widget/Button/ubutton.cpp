#include "ubutton.h"

#include <QPainter>
//#include "universalSet/buzzercontrol.h"
#include <unistd.h>

ubutton *ubutton::father = nullptr;
QFont ubutton::fatherFont = textControl::instance()->font(textControl::size20);
QPixmap *ubutton::_pix0 = nullptr;
QPixmap *ubutton::_pix1 = nullptr;

ubutton::ubutton(QWidget *parent) :
    Button(parent)
{

    if( father ){
        connect(father,SIGNAL(fontChange()),this,SLOT(fontSlot()));
    }

    setFont(fatherFont);

    pix0 = NULL;
    pix1 = NULL;

    _alignmentFlag = Qt::AlignCenter;
    _margin = 0;

    privateData = 0;

    connect(this,SIGNAL(clicked()),this,SLOT(sendDataSlot()));
}

void ubutton::setTextColor(QPalette::ColorGroup group, QColor color)
{
    QPalette    palette = this->palette();
    palette.setBrush(group,QPalette::ButtonText,QBrush(color));
    setPalette(palette);
}

//void ubutton::setTextColor(QColor color)
//{
//    QPalette    palette = this->palette();
//    palette.setBrush(QPalette::Normal,QPalette::ButtonText,QBrush(color));
//    setPalette(palette);
//}

void ubutton::setText(const QString &text)
{
    currentText = text;
    update();
}

void ubutton::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);
    setFont(font);
}

void ubutton::setTextSize(textControl::textSize size)
{
    setFont(textControl::instance()->font(size));
}

void ubutton::setFatherFontSize(int size)
{
    if( this == father ){
        if( fatherFont.pointSize() != size ){
            fatherFont.setPointSize(size);
            emit fontChange();
        }
    }
}

void ubutton::fontSlot()
{
    setFont(fatherFont);
}

void ubutton::sendDataSlot()
{
    emit sendData(privateData);
}

void ubutton::paintEvent(QPaintEvent *e)
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
    }else if( _pix0 && _pix1 ){
        QPainter painter(this);
        if( !isEnabled() ){
            painter.setPen(this->palette().color(QPalette::Inactive,QPalette::ButtonText));
        }else{
            painter.setPen(this->palette().color(QPalette::Active,QPalette::ButtonText));
        }
        if( isDown() ){
            painter.drawPixmap(1,1,this->width(),this->height(),*_pix0);
            painter.drawText(_margin + 1,1,this->width(),this->height(),_alignmentFlag,currentText);
        }else{
            painter.drawPixmap(0,0,this->width(),this->height(),*_pix1);
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

bool ubutton::canPress()
{
//    return permissionSet::permissionIsOk(permissionSet::HMI,permissionLevel());
}

ubutton *ubutton::forefather()
{
    if( father == NULL ){
        father = new ubutton;
    }
    return father;
}

void ubutton::setFatherPixmap(STATE state, QString filePath)
{
    if( father == this ){
        if( press == state ){
            if( _pix0 ){
                _pix0->load(filePath);
            }else{
                _pix0 = new QPixmap(filePath);
            }
        }else if( release == state ){
            if( _pix1 ){
                _pix1->load(filePath);
            }else{
                _pix1 = new QPixmap(filePath);
            }
        }
    }
}

void ubutton::setPixmap(ubutton::STATE state, QString filePath)
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

void ubutton::setTextAlignment(Qt::Alignment flag)
{
    _alignmentFlag = flag;
}

void ubutton::setTextMargin(int margin)
{
    _margin = margin;
}
