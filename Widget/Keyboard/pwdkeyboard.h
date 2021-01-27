#ifndef PWDKEYBOARD_H
#define PWDKEYBOARD_H

#include "keybutton.h"
//#include "dataManage/dataCore/database.h"
//#include "languageTranslate/translate.h"
#include "skeyboard.h"
#include "textlabel.h"


class pwdKeyBoard : public skeyboard
{
    Q_OBJECT
public:
    explicit pwdKeyBoard(QWidget *parent = 0);
    
    //获取键盘录入的数据
    //获取数据时需要传入bool变量地址，用于判断数据是否成功获取
    //因为即使是点击退出也会返回数值，但这时并不希望程序处理这个返回值
    static int getValue(int value,bool *exit = 0);
    //获取字符串
    static QString getValueString(int value);
    //获取错误数据，当点击取消后再点退出或者没有输入数据时
    //其默认返回可以设置为这个错误值
    static int getWrongValue();
    //静态实例
    static pwdKeyBoard *instance();

private:
    void setupUi();
    int getLineEditValue();
    QString lineEditText();
    void initValueShow(int value);

    virtual void retranslateUi(QLocale::Language lang);
    virtual void setChinese();
    virtual void setEnglish();
    virtual void readyExit();

private slots:
    virtual void digitClicked();
    void clearClicked();
    void exitSlot();
    
private:
    static pwdKeyBoard    *core;
    static int  _value;
    static bool exitState;

    bool waitingForOperand;
    bool alwaysStay;

    TextLabel  *titleLabel;

    keyButton   *clearButton;
    keyButton   *cancelButton;
    keyButton   *enterButton;

};

#endif // PWDKEYBOARD_H
