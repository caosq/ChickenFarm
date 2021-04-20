#ifndef ANALOGVALBUTTON_H
#define ANALOGVALBUTTON_H

#include <QObject>
#include <QWidget>
#include "button.h"
#include "datamonitor.h"

class AnalogValButton : public Button
{
    Q_OBJECT
public:

    enum STATE{
        press = 0,
        release = 1
    };

    explicit AnalogValButton(QWidget *parent = nullptr);

    //获取祖父静态实例
    //通过这个对象能够控制所有abutton实例的一些属性
    //如：按键icon,字号等等
    static AnalogValButton *forefather();

    //绑定数据地址
    bool setMonitorData(void* pvVal = nullptr, Monitor::DataType emDataType = Monitor::Uint16t, bool xSyncMode = false);

    //当某个数据的最大最小是变量时可以使用该函数
    bool setMaxValMonitor(void* pvVa = nullptr, Monitor::DataType emDataType = Monitor::Uint16t);

    //设置最小值的数据地址
    bool setMinValMonitor(void* pvVal = nullptr, Monitor::DataType emDataType = Monitor::Uint16t);

    //设置数据参数
    void setDataParameter(QString strUnit, uint8_t ucDecPoint, int32_t iDefaultVal,
                          int32_t iMaxVal, int32_t iMinVal, Monitor::DataType emDataType);

    //设置数据类型
    void setDataType(Monitor::DataType emDataType);

    //设置数据单位
    void setUnit(QString unit = "");

    //设置数值精度，0代表数据精度为1,1代表数据精度0.1，2代表数据精度0.01
    void setDecPoint( uint8_t ucDecPoint = 0);

    //设置数值范围
    void setValueRange(int max = 65535,int min = -65535);

    //设置默认值
    void setDefaultValue(int defValue);

    //设置字体颜色
    //group决定color所属状态组，如激活，非激活，正常，禁止等
    void setTextColor(QPalette::ColorGroup group,QColor color);

    //设置背景颜色
    void setBackGroundColor(QColor color);

    //设置字符串显示
    //void setText(const QString text);

    //设置字符串显示，同时设置字号大小
    void setText(const QString text, int size = 16);

    //设置文字大小
    void setTextSize(int size);

    //设置文字显示的位置(居中，靠左，靠右)
    void setTextAlignment(Qt::Alignment flag);

    //设置文字边距
    void setTextMargin(int margin);

    //设置图片
    //state决定按下或抬起，filePath：图片位置
    void setPixmap(STATE state, QString filePath);

    //设置点击，触发点击信号
    void setClicked();

    //刷新显示
    void flush();

    //设置默认字号大小
    //调用该函数后，默认新增的dlabel字号会变更，但在这之前的保持不变
    void setFatherFontSize(int size);

    //设置父类图片
    //state决定按下或抬起，filePath：图片位置
    //father这两个函数作用于全部abutton实体，但必须是在father初始化之后
    void setFatherPixmap(STATE state, QString filePath);

    //使能默认值标记
    void enableDefaultMarker(bool ok){enableValMarker = ok;}

    int32_t getCurrentValue();

protected:
    void paintEvent(QPaintEvent *e);

private:
    void showData(uint32_t val);
    void showText(uint32_t val);
    void showColor(uint32_t val);
    void showImage(uint32_t val);

signals:
    void fontChange();


public slots:
    void setValue(int32_t val);
    void setValue(Monitor* pMonitor);
    void setMaxValue(Monitor* pMonitor);
    void setMinValue(Monitor* pMonitor);

    void clickedSlot();

private slots:
    void fontSlot();

private:

    static AnalogValButton *father;
    static QFont    fatherFont;
    static QPixmap *_pix0;
    static QPixmap *_pix1;

    Monitor*    m_pMonitor;
    Monitor*    m_pMaxMonitor;
    Monitor*    m_pMinMonitor;

    Monitor::DataType   m_eDataType;
    QString     m_strUnit;
    QString     m_strCurrentText;

    uint8_t     m_ucDecPoint;
    int32_t     m_iMaxVal;
    int32_t     m_iMinVal;

    QPixmap     *m_pPressImg;
    QPixmap     *m_pRelaseImg;

    Qt::Alignment m_alignment;
    int           m_iMargin;

    bool defValState;
    bool enableValMarker;
    bool save;
    bool automaticAttack;
};

#endif // ANALOGVALBUTTON_H
