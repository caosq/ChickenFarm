#ifndef AKEYBOARD_H
#define AKEYBOARD_H

#include "keybutton.h"
#include "skeyboard.h"
#include "textlabel.h"

class aKeyBoard : public skeyboard
{
    Q_OBJECT

public:

    
    //获取键盘输入的数据
    //value是默认显示的值
    //max是最大值，min最小值
    //dot是精度：0代表精度是1,1代表精度0.1，2代表精度0.01
    static int getValue(int value = 0, int max = 9999, int min = -9999, int dot = 0);

    //静态实例
    static aKeyBoard *instance();

private:
     explicit aKeyBoard(QWidget *parent = nullptr);

    void setupUi();
    int getLineEditValue();
    void initValueShow(double value = 0, double max = 9999, double min = -9999, int dot = 0);
    virtual void retranslateUi(QLocale::Language lang);
    virtual void setChinese();
    virtual void setEnglish();


private slots:
    virtual void digitClicked();
    void changeSign();
    void pointClicked();
    void clearClicked();

private:
    static aKeyBoard *core;
    static int       _value;

    int _dot;
    int _max;
    int _min;

    bool waitingForOperand;
    bool alwaysStay;


    TextLabel  *maxLabel;
    TextLabel  *maxValuelLabel;

    TextLabel  *minLabel;
    TextLabel  *minValuelLabel;

    keyButton   *pointButton;
    keyButton   *clearButton;
    keyButton   *cancelButton;
    keyButton   *enterButton;
    keyButton   *minusButton;

    QFontMetrics *_fontMetrics;

};

#endif // AKEYBOARD_H
