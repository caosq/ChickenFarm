#ifndef SETTINGBUTTON_H
#define SETTINGBUTTON_H

#include <QObject>
#include <QWidget>
#include "button.h"


enum STATE
{
   press = 0,
   release = 1
};

class SettingButton : public Button
{
    Q_OBJECT
public:
    SettingButton();


    //模拟量按键
    explicit SettingButton(QWidget *parent = nullptr);

    //获取祖父静态实例
    //通过这个对象能够控制所有abutton实例的一些属性
    //如：按键icon,字号等等
    static SettingButton *forefather();

    //绑定的数据id
    QString memoryId();

    //绑定的数据地址
    int memoryAddr();

    //是否自动触发事件
    //自动触发模式下默认会弹出输入键盘，同时会执行预备状态逻辑判定
    //非自动模式下不会弹出键盘，也不会执行任何逻辑
    bool isAutomaticAttack(){return automaticAttack;}

    //绑定数据地址
    bool setMonitorData(void* pvVal = nullptr, uint8_t ucDataType = Uint16t);

    //设置数据参数
    void setDataParameter(QString unit = "", int dot = 0, int max = 9999, int min = -9999, int defValue = 0, eDataType dtype = Uint16t);

    //设置最大值的数据地址
    //当某个数据的最大最小是变量时可以使用该函数
    bool setMaxValMonitorAddress(QString Id,int addr,int dot);
    //设置最小值的数据地址
    bool setMinValMonitorAddress(QString Id,int addr,int dot);

    //设置数据类型
  //  void setDataType(eDataType type){_dtype = type;}
    //设置数据单位
    void setUnit(QString unit = "");
    //设置数据精度，0=整型，1=有一位小数，2=有两位小数
    void setDot(int dot = 0);
    //设置数值范围
    void setValueRange(int max = 9999,int min = -9999);

    //使能存储，数据是否存储还与该数据的id有关
    //必须数据id允许存储，同时该单独数据也允许存储才能记忆
    void disableSave(bool state = false);

    //设置默认值
    void setDefaultValue(int defValue);

    //设置字体颜色
    //group决定color所属状态组，如激活，非激活，正常，禁止等
    void setTextColor(QPalette::ColorGroup group,QColor color);
    //设置文字显示
    void setText(const QString &text);

    //设置字体大小
    void setTextSize(int size);
    //设置字体大小，使用textControl的控制size
    //这个相当于使用一个变量来控制字体大小，当中英切换时
    //可以改变该变量的值来达到更改字号的目的
  //  void setTextSize(textControl::textSize size = textControl::size18);

    //设置自动工作
    void enableAutomaticAttack(bool state);
    //设置点击，触发点击信号
    void setClicked();

    //设置文字显示的位置(居中，靠左，靠右)
    void setTextAlignment(Qt::Alignment flag);

    //设置文字边距
    void setTextMargin(int margin);

    //设置父类字号大小
    void setFatherFontSize(int size);

    //设置父类图片
    //state决定按下或抬起，filePath：图片位置
    //father这两个函数作用于全部abutton实体，但必须是在father初始化之后
    void setFatherPixmap(STATE state, QString filePath);

    //设置图片
    //state决定按下或抬起，filePath：图片位置
    void setPixmap(STATE state, QString filePath);

    //使能默认值标记
    void enableDefaultMarker(bool ok){enableValMarker = ok;}

    //设置默认的操作权限
  //  void setFatherDefPermission(permissionSet::permissionLevel level){fatherLevel = level;}

protected:
    void paintEvent(QPaintEvent *e);

private:
    virtual bool canPress();

private slots:
    void fontSlot();


public slots:
    void setValue(unsigned int val);
    void setMaxValue(unsigned int val);
    void setMinValue(unsigned int val);
    void clickedSlot();

signals:
    void fontChange();

private:
    //static abutton *father;
    static QFont    fatherFont;
    static QPixmap *_pix0;
    static QPixmap *_pix1;
   // static permissionSet::permissionLevel fatherLevel;

    QPixmap *pix0;
    QPixmap *pix1;

 //   dataMonitor *monitor;
 //   dataMonitor *monitorMax;
 //   dataMonitor *monitorMin;

    bool defValState;
    bool enableValMarker;
    bool save;
    bool automaticAttack;

 //   dataBase::types _dtype;

    int _dot;
    int maxVal;
    int minVal;
    int defVal;
    int currentValue;

    QString _unit;
    QString currentText;

    Qt::Alignment _alignmentFlag;
    int _margin;
    int adjustDot;
};

#endif // SETTINGBUTTON_H
