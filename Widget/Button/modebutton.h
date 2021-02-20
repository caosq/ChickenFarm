#ifndef MODEBUTTON_H
#define MODEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QMenu>

#include "button.h"
#include "textcontrol.h"
#include "datamonitor.h"

class privateMenu : public QMenu
{
    Q_OBJECT
public:
    typedef struct{
        QColor _Background;
        QColor _Highlight;
        QColor _Text;
        QColor _HightlightText;
    }menuStyle;

    friend class ModeButton;
    explicit privateMenu(QWidget *parent = nullptr);

    void setColorStyle(menuStyle style);
    void setBackgroundColor(QColor color);
    void setHighlightColor(QColor color);
    void setTextColor(QColor color);
    void setHightlightTextColor(QColor color);

private:
    void setCurrentItem(int id);
    QList<int> getValueList();
    QString currentText(int id);
    void setItem(int id,QString text);
    bool replaceItemId(int old_id,int new_id);
    void removeItem(int id);

    void setMenuStyleSheet();

private slots:
    void actionSlot();

signals:
    void valueChange(int);

 private:
    QMap<int,QAction *> _acmap;

    menuStyle m_styleSheet;
};



class ModeButton : public Button
{
    Q_OBJECT
public:
    explicit ModeButton(QWidget *parent = nullptr);

    //获取祖父实例
    static ModeButton* forefather();

    //设置数据地址
    bool setMonitorData(void* pvVal = nullptr, Monitor::DataType emDataType = Monitor::Uint16t);

    //设置字体颜色，group决定color所属状态
    void setTextColor(QPalette::ColorGroup group,QColor color);

    //设置非触发状态下文字显示(不点击时的显示)
    void setText(const QString &text);

    //设置条目的文字显示(下拉的条目)
    void setItem(int id,QString text);

    //替换id
    bool replaceItemId(int old_id,int new_id);
    //移除条目
    void removeItem(int id);

    //设置字体
    void setFont(const QFont &font);

    //设置文字大小
    void setTextSize(int size);

    //设置字体大小，使用textControl的控制size
    //这个相当于使用一个变量来控制字体大小，当中英切换时
    //可以改变该变量的值来达到更改字号的目的
    //void setTextSize(textControl::textSize size = textControl::size18);

    //设置自动工作
    void enableAutomaticAttack(bool state);
    //设置点击，触发点击信号
    void setClicked();

    //设置文字显示的位置(居中，靠左，靠右)
    void setTextAlignment(Qt::Alignment flag);
    //设置文字边距
    void setTextMargin(int margin);

    void setButtumGap(int gap);

    //设置默认值
    void setDefaultValue(int defValue);

    //设置父类字号大小
    void setFatherFontSize(int size);

    //设置父类图片
    //state决定按下或抬起，filePath：图片位置
    //father这两个函数作用于全部abutton实体，但必须是在father初始化之后
    void setFatherPixmap(QString filePath);

    //设置祖父类的显示风格
    //menuStyle结构体决定了下拉框的风格
    //_Background 设置背景色
    //_Highlight 选中时的高亮颜色
    //_Text 下拉的文字颜色(非高亮颜色)
    //_HightlightText 下拉的文章颜色(高亮颜色)
    void setFatherItemStyle(privateMenu::menuStyle style);

    //使能默认值标记功能
    void enableDefaultMarker(bool ok){enableValMarker = ok;}
    //设置祖父的操作权限

protected:
    void paintEvent(QPaintEvent *e);

private:
    void initMenu();
    QString getElidedText();
    //virtual bool canPress();

public slots:
    void setValue(int32_t val);
    void setValue(Monitor* pMonitor);

private slots:
    void fontSlot();
    void clickedSlot();
    void valueSlot(int tempVal);
    void hideMenu();

signals:
    void fontChange();

private:
    Monitor*    m_pMonitor;

    static ModeButton *father;
    static QFont    fatherFont;
    static QPixmap *pixmap;

    static privateMenu::menuStyle fatherStyle;

    bool defValState;
    bool enableValMarker;
    bool save;
    bool automaticAttack;

    Monitor::DataType _dtype;

    privateMenu *_menu;

    int defVal;
    int currentValue;

    QString currentText;

    Qt::Alignment _alignmentFlag;
    int _margin;
    int _gap;
};

#endif // MODEBUTTON_H
