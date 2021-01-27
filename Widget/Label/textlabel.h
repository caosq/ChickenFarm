#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class TextLabel : public QLabel
{
public:
    explicit TextLabel(QWidget* parent = nullptr);
    TextLabel(int size = 12, QColor textColor = Qt::white, QWidget *parent = nullptr);

    void setText(const QString text, int size = 12, QColor textColor = Qt::white);

    void setTextColor(QColor color);
    //设置背景颜色
    void setBackGroundColor(QColor color);

    //设置字号大小
    void setTextSize(int size);

    //统一设置字号大小
    void setFatherFontSize(int size);

};

#endif // TEXTLABEL_H
