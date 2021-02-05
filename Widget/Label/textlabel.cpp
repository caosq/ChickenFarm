#include "textlabel.h"

TextLabel  *TextLabel::father = nullptr;
QFont TextLabel::fatherFont;

TextLabel::TextLabel(QWidget *parent) :
    QLabel(parent)
{
    if( father ){
        setFont(fatherFont);
        connect(father,SIGNAL(fontChange()),this,SLOT(fontSlot()));
    }else{
        setFont(textControl::instance()->font(textControl::size18));
    }
    setPalette(*(textControl::instance()->palette()));
}

TextLabel::TextLabel(int size, QColor textColor, QWidget *parent) :
    QLabel(parent)
{
    if( father ){
        setFont(fatherFont);
        connect(father,SIGNAL(fontChange()),this,SLOT(fontSlot()));
    }else{
        setFont(textControl::instance()->font(textControl::size18));
    }
    setPalette(*(textControl::instance()->palette()));

    setTextSize(size);
    setTextColor(textColor);
}

TextLabel *TextLabel::forefather()
{
    if( nullptr == father ){
        father = new TextLabel(0);
    }
    return father;
}

void TextLabel::setText(const QString text)
{
    setText(text, m_iTextSize);
}

void TextLabel::setText(const QString text, int size)
{
    setTextSize(size);
    QLabel::setText(text);
}

void TextLabel::setText(const QString text, textControl::textSize size)
{
    setTextSize(size);
    QLabel::setText(text);
}

void TextLabel::setTextColor(QColor color)
{
    QPalette    palette = this->palette();
    palette.setBrush(QPalette::Active,QPalette::WindowText,QBrush(color));
    palette.setBrush(QPalette::Inactive,QPalette::WindowText,QBrush(color));
    setPalette(palette);
}

void TextLabel::setBackGroundColor(QColor color)
{
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Normal,QPalette::Window,QBrush(color));
    palette.setBrush(QPalette::Inactive,QPalette::Window,QBrush(color));

    setPalette(palette);
}

void TextLabel::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);

    setFont(font);
    m_iTextSize = size;
}

void TextLabel::setTextSize(textControl::textSize size)
{
    setFont(textControl::instance()->font(size));
}

void TextLabel::setFatherFontSize(int size)
{
    if( father == this ){
        fatherFont.setPointSize(size);
        emit fontChange();
    }
}

void TextLabel::fontSlot()
{
    setFont(fatherFont);
}
