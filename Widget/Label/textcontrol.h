#ifndef TEXTCONTROL_H
#define TEXTCONTROL_H

#include <QObject>
#include <QFont>
#include <QBrush>
#include <QPalette>

class textControl : public QObject
{
    Q_OBJECT
public:
    enum textSize{
        size10,
        size11,
        size12,
        size13,
        size14,
        size15,
        size16,
        size17,
        size18,
        size19,
        size20,
        size21,
        size22,
        size23,
        size24,
        size25,
        size26,
        size27,
        size28,
        size29,
        size30,
        size31,
        size32,
        size33,
        size34,
        size35,
        size36,
        size37,
        size38,
        size39
    };

    explicit textControl(QObject *parent = 0);

    ~textControl();
    //textControl的静态实例对象
    static textControl  *instance(){
        if( NULL == m_text ){
            m_text = new textControl;
        }
        return m_text;
    }

    //获得文字绘制的调色板
    QPalette *palette(){return m_palette;}

    //获得文字画刷
    QBrush  *brush(){return m_brush;}

    //获取某状态下的颜色值
    //colorRole决定状态值，函数返回对应状态颜色值
    const QColor color(QPalette::ColorRole role){return m_palette->color(role);}

    //获取一个指定字号的qfont
    QFont font(textSize size = size14 ){return m_font.at(size);}

private:
    void makePalette();
    void makeFont(int baseSize);

private slots:
    void languageChangeSlot();

private:
    static textControl  *m_text;
    QBrush  *m_brush;
    QPalette    *m_palette;
    QVector<QFont>  m_font;
    int baseSize;
    
};

#endif // TEXTCONTROL_H
