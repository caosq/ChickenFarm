#include "textlabel.h"

TextLabel::TextLabel(QWidget *parent) :
    QLabel(parent)
{

}

void TextLabel::setText(const QString text, int size,  QColor textColor)
{
     QLabel::setText(text);
     setTextSize(size);
     setTextColor(textColor);
}

void TextLabel::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);

    setFont(font);
}

void TextLabel::setTextColor(QColor color)
{
    QPalette    palette = this->palette();
    palette.setBrush(QPalette::Active, QPalette::WindowText, QBrush(color));
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, QBrush(color));
    setPalette(palette);
}
