#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>
#include "ubuttonbk.h"
#include "ubutton.h"

//#include "universalSet/screensaver.h"

class messageBox : public QMessageBox
{
    Q_OBJECT
public:
    enum msgType{
        Information = 0,
        Warning = 1,
        Question = 2
    };

    enum btRole{
        Yes,
        No
    };

    //消息提示框类，有警告，一般信息，询问模式
    explicit messageBox(msgType type = Information,QWidget *parent = 0);
    
    //设置消息字体大小
    void setTextSize(int size);
    //设置字体颜色
    void setTextColor(QColor color);
    //设置按键文件，role决定button类型
    void setButtonText(btRole role,QString str);

    //设置默认的按键图片，type决定消息leix
    //在该函数调用后，按键的统一图片设置才会生效
    static void setDefIconPixmap(msgType type,QString filePath);
    static void setDefIconPixmap(msgType type,QPixmap *pixmap);

    //设置消息外框和抬头颜色
    void setFramelessWindowColor(QColor color);
    //设置背景颜色
    void setBackGroundColor(QColor color);

    //使能窗体移动，false将导致弹框消息不能移动
    void disableMove(bool state);
    //设置屏保后是否隐藏
    void setAlwaysStay(bool state);
    //隐藏按键
    void hideButton(btRole role);
    //显示按键
    void showButton(btRole role);
    //设置默认的初始背景颜色
    static void setDefBackGroundColor(QColor color){_backgroundColor = color;}
    //设置默认的边框颜色
    static void setDefFramelessWindowColor(QColor color){_frameLessColor = color;}
    //设置弹框信息格式原型字符
    void setInformativeFormatStr(QString format){_informativeFormatStr = format;}
    //获取弹框信息原型字符
    QString getInformativeFormatStr()const{return _informativeFormatStr;}

    static messageBox *instance();
private:
    void initButton();
    void initIcon();

protected:
    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *o, QEvent *e);
    void showEvent(QShowEvent *event);

signals:
    
public slots:
    void tryReject();

private:
    bool moveState;
    bool pressed;
    QPoint mousePosition;

    static messageBox *g_pUniversalMsg;

    static QColor _frameLessColor;
    static QColor _backgroundColor;
    static QMap<msgType,QPixmap*> iconMap;

    QColor frameLessColor;
    
    msgType _type;
    ubuttonBK *yes;
    ubuttonBK *no;

    QString _informativeFormatStr;
};

#endif // MESSAGEBOX_H
