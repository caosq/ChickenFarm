#ifndef DATALABEL_H
#define DATALABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QMap>
#include "datamonitor.h"
#include "textcontrol.h"

enum eLabelType
{
    Data,
    Text,
    Color,
    Image
};

enum eLabelStyle
{
    Circular,   //画圆
    Rectangle,  //画矩形方块
    NoStyle     //没有风格
};

class DataLabel : public QLabel
{
    Q_OBJECT
public:

    explicit DataLabel(QWidget *parent = nullptr, eLabelType emLabelType = Data, eDataType emDataType = Uint16t);

    static DataLabel *forefather();

    //获取绑定的数据地址
    bool setMonitorData(void* pvVal = nullptr, eDataType emDataType = Uint16t);

    //设置数据类型
    void setDataType(eDataType emDataType);

    //设置数据单位
    void setUnit(QString unit = "");

    //设置数值精度，0代表数据精度为1,1代表数据精度0.1，2代表数据精度0.01
    void setDecPoint( uint8_t ucDecPoint = 0);

    //设置数值-字符串映射
    void setValueMap(int val,QString str);

    //设置数值-字符串-字体颜色映射
    void setValueMap(int val,QString str,QColor textColor);

    //设置数值-背景色映射(比如需要使用纯色来代表开关状态是就能这么使用)
    void setValueMap(int val,QColor backgroundColor);

    //设置数值-图像映射
    void setValueMap(int val,QPixmap *pixmap);

    //锁定文字颜色显示，锁定后只能使用这个函数改变文字颜色
    void lockTextColor(QColor color);

    //解锁文字颜色锁定
    void unlockTextColor();

    //设置文字颜色,text模式下会被setValueMap设置的颜色覆盖，不起作用
    void setTextColor(QColor color);

    //设置背景颜色
    void setBackGroundColor(QColor color);

    //设置color模式下，绘图的风格
    void setBackGroundColorStyle(eLabelStyle style);

    //设置字符串显示
    //void setText(const QString text);

    //设置字符串显示，同时设置字号大小
    void setText(const QString text, int size = 12);

    //设置字符串显示，同时设置字号大小
    void setText(const QString text,textControl::textSize size);

    //设置文字大小
    void setTextSize(int size);

    //设置文字大小
    void setTextSize(textControl::textSize size = textControl::size18);

    //刷新显示
    void flush();

    //设置默认字号大小
    //调用该函数后，默认新增的dlabel字号会变更，但在这之前的保持不变
    void setFatherFontSize(int size);

protected:
    void paintEvent(QPaintEvent *e);
private:
    void showData(unsigned int val);
    void showText(unsigned int val);
    void showColor(unsigned int val);
    void showImage(unsigned int val);

public slots:
    void setValue(unsigned int val);

private slots:
    void fontSlot();


signals:
    void fontChange();


private:
    typedef struct
    {
        QString  textStr;
        QColor   textColor;
        QColor   backgroundColor;
        QPixmap* pixmap;
    }sTextMap;

    Monitor*    m_pMonitor;

    eLabelType  m_eLabelType;
    eLabelStyle m_eLabelStyle;
    eDataType   m_eDataType;

    uint8_t     m_ucDecPoint;
    QString     m_strUnit;

    bool        m_xTextColorLock;

    QMap<int, sTextMap> m_TextMap;

    static DataLabel   *father;
    static QFont    *fatherFont;
    bool _textColorLock;
};

#endif // DATALABEL_H
