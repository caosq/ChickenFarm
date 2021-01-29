#ifndef GQWTLEGENDBASE_H
#define GQWTLEGENDBASE_H

#include <QLabel>
#include <QPainter>

class gQwtLegendBase : public QLabel
{
    Q_OBJECT
public:
    enum iconStyle{
        noIcon,
        pixmap,
        drawIcon
    };

    //绘图图例
    explicit gQwtLegendBase(QWidget *parent = 0);

    //设置图例显示的图像
    void setPixmap(QString path = "");
    //设置图片显示风格，noicon不显示图像，pixmap显示给定的图像
    //drawicon:自己画
    void setIconStyle(iconStyle style);
    //设置坐标和大小
    void setGeometry(int x, int y, int w, int h);
    //设置icon的颜色
    void setIconColor(QColor color);
    //设置背景颜色
    void setBackgroundColor(QColor color);
    //设置是否显示背景颜色
    void setNoBackground(bool ok = false);
    //获取是否使能背景颜色
    bool noBackground(){return _bgState;}

    //获取图例icon颜色
    QColor iconColor();
    //获取当前icon显示风格
    iconStyle iconShowStyle();
    //获取背景颜色
    QColor backgroundColor();
    //获取文字显示坐标
    QRect textRect();
    //获取icon显示的图像
    QPixmap *iconPixmap(){return _pixmap;}


private:
    iconStyle _iStyle;
    QPixmap *_pixmap;
    QColor _icolor;
    QColor _bcolor;
    bool _bgState;

signals:
    
public slots:
    
};

#endif // GQWTLEGENDBASE_H
