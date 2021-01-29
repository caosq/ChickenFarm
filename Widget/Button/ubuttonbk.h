#ifndef UBUTTONBK_H
#define UBUTTONBK_H

//#include <QPushButton>
#include "button.h"
#include "textcontrol.h"


class ubuttonBK : public Button
{
    Q_OBJECT
public:
    enum STATE{
        press = 0,
        release = 1
    };

    //通用按键
    explicit ubuttonBK(QWidget *parent = 0);

    //设置字体颜色
    void setTextColor(QPalette::ColorGroup group,QColor color);
    //void setTextColor(QColor color);
    //设置文字显示
    void setText(const QString &text);
    //设置字体大小
    void setTextSize(int size);
    //设置字体大小，使用textControl的控制size
    //这个相当于使用一个变量来控制字体大小，当中英切换时
    //可以改变该变量的值来达到更改字号的目的
    void setTextSize(textControl::textSize size = textControl::size18);

    //设置父类字号大小
    void setFatherFontSize(int size);

    //设置父类图片
    //state决定按下或抬起，filePath：图片位置
    //father这两个函数作用于全部abutton实体，但必须是在father初始化之后
    void setFatherPixmap(STATE state,QString filePath);

    //设置图片
    //state决定按下或抬起，filePath：图片位置
    void setPixmap(STATE state,QString filePath);

    //设置文字显示的位置(居中，靠左，靠右)
    void setTextAlignment(Qt::Alignment flag);

    //设置文字边距
    void setTextMargin(int margin);

    //设置点击信号触发后发送的数据
    void setSendData(int data){privateData = data;}

    //获取祖父对象
    static ubuttonBK *forefather();

protected:
    void paintEvent(QPaintEvent *e);

private:
    virtual bool canPress();

private slots:
    void fontSlot();
    void sendDataSlot();

signals:
    void fontChange();
    void sendData(int);

private:
    static ubuttonBK *fatherBK;
    static QFont    fatherFontBK;
    static QPixmap *_pix0BK;
    static QPixmap *_pix1BK;

    QPixmap *pix0;
    QPixmap *pix1;

    QString currentText;
    int privateData;

    Qt::Alignment _alignmentFlag;
    int _margin;

};

#endif // UBUTTONBK_H
