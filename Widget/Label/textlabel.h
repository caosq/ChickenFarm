#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>

#include "textcontrol.h"

class TextLabel : public QLabel
{
    Q_OBJECT
public:
    //一般文字显示模块，要显示文字或者图片可以使用这个类
    //这个类不具备数据绑定监视功能
    explicit TextLabel(QWidget *parent = nullptr);

    TextLabel(int size = 20,QColor textColor = Qt::white,QWidget *parent = nullptr);

    static TextLabel *forefather();

    //设置文字显示
    void setText(const QString text);

    //设置文字显示和设置文字大小
    void setText(const QString text,int size);

    //设置文字显示和设置文字大小，不过是使用textSize枚举类型来决定字号大小
    //使用枚举类型的好处是可以改变实际字号的大小
    //即使是同一个枚举类型，只有改变其映射的数值就能改变实际的字号
    //这个可以在中英切换时统一更改字号
    void setText(const QString text,textControl::textSize size);

    //设置文字的颜色
    void setTextColor(QColor color);

    //设置背景颜色
    void setBackGroundColor(QColor color);

    //设置字号大小
    void setTextSize(int size);
    void setTextSize(textControl::textSize size = textControl::size18);

    //统一设置字号大小
    void setFatherFontSize(int size);
private:
    static TextLabel   *father;
    static QFont    fatherFont;

private slots:
    void fontSlot();

signals:
    void fontChange();
};

#endif // TEXTLABEL_H
