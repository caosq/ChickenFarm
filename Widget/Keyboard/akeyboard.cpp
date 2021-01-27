#include "akeyboard.h"
#include <QDialog>
#include <qmath.h>

#define OVERFLOW_STR "******"
#define MAX_COUNT 9
#define BACKGROUND ":/UI/baseFile/akeyboard.png"

aKeyBoard   *aKeyBoard::core = nullptr;
int aKeyBoard::_value = 0;

aKeyBoard::aKeyBoard(QWidget *parent) :
    skeyboard(parent)
{
    setupUi();

    connect(digitButton[0],SIGNAL(pressed()),this,SLOT(digitClicked()));
    connect(enterButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(hide()));

    connect(pointButton,SIGNAL(pressed()),this,SLOT(pointClicked()));
    connect(minusButton,SIGNAL(pressed()),this,SLOT(changeSign()));
    connect(clearButton,SIGNAL(pressed()),this,SLOT(clearClicked()));

    setAlwaysStay(false);
}


int aKeyBoard::getValue(int value, int max, int min, int dot)
{
    //初始化显示
    aKeyBoard::instance()->initValueShow(value,max,min,dot);

    _value = value;
    //键盘退出返回值判断
    if( aKeyBoard::instance()->exec() == QDialog::Accepted ){
        _value = aKeyBoard::instance()->getLineEditValue();
    }

    return _value;
}

void aKeyBoard::initValueShow(double value, double max, double min, int dot)
{
    //输入标志，判断是否已经输入数值
    waitingForOperand = true;
    _dot = dot;

    //设置输入框显示
    lineEdit->setText(QString::number(value/qPow(10,dot),'.',dot));

    //最大值标签
    maxValuelLabel->setText(QString::number(max/qPow(10,dot),'.',dot));
    //最小值标签
    minValuelLabel->setText(QString::number(min/qPow(10,dot),'.',dot));

    if( _fontMetrics->width(maxValuelLabel->text()) > maxValuelLabel->width() ||
            _fontMetrics->width(minValuelLabel->text()) > minValuelLabel->width()){
        maxValuelLabel->setText(OVERFLOW_STR);
        minValuelLabel->setText(OVERFLOW_STR);
    }
}

void aKeyBoard::setupUi()
{
    QPixmap * img = new QPixmap(BACKGROUND);
    QPalette p = palette();
    p.setBrush(QPalette::Window, QBrush(*img));
    setPalette(p);

    maxLabel = new TextLabel(this);
    maxLabel->setAlignment(Qt::AlignCenter);
    maxLabel->setGeometry(12,6,50,30);

    maxValuelLabel = new TextLabel(this);
    maxValuelLabel->setTextSize(22);
    maxValuelLabel->setAlignment(Qt::AlignCenter);
    maxValuelLabel->setGeometry(55,6,95,30);

    minLabel = new TextLabel(this);
    minLabel->setAlignment(Qt::AlignCenter);
    minLabel->setGeometry(153,6,50,30);

    minValuelLabel = new TextLabel(this);
    minValuelLabel->setTextSize(22);
    minValuelLabel->setAlignment(Qt::AlignCenter);
    minValuelLabel->setGeometry(203,6,95,30);

    _fontMetrics = new QFontMetrics(minValuelLabel->font());

    //数值0
    digitButton[0] = new keyButton(this);
    digitButton[0]->setText("0");
    digitButton[0]->setGeometry(85, 298, 66, 58);

    //正负切换按键
    minusButton = new keyButton(this);
    minusButton->setText("-");
    minusButton->setGeometry(229, 102, 66, 58);

    //小数点
    pointButton = new keyButton(this);
    pointButton->setText(".");
    pointButton->setGeometry(12, 298, 66, 58);

    //清除
    clearButton = new keyButton(this);
    clearButton->setGeometry(229, 168, 66, 58);

    //取消
    cancelButton = new keyButton(this);
    cancelButton->setGeometry(229, 233, 66, 58);

    //确认
    enterButton = new keyButton(this);
    enterButton->setGeometry(157, 299, 138, 56);

   // retranslateUi(translate::instance()->currentLanguage());
}

int aKeyBoard::getLineEditValue()
{
    bool ret = false;
    int temp = lineEdit->text().toDouble(&ret) * qPow(10,_dot);
    return ret ? temp : _value;
}

void aKeyBoard::retranslateUi(QLocale::Language lang)
{
    switch(lang)
    {
        case QLocale::Chinese:{
        setChinese();
    }
        break;
    case QLocale::English:{
        setEnglish();
    }
        break;
    default:
        break;
    }

}

void aKeyBoard::setChinese()
{
    clearButton->setText("清除");
    cancelButton->setText("退出");
    enterButton->setText("确认");

    //maxLabel->setTextSize(textControl::size20);
    maxLabel->setText("最大");

   // minLabel->setTextSize(textControl::size20);
    minLabel->setText("最小");
}

void aKeyBoard::setEnglish()
{
    clearButton->setText("CR");
    cancelButton->setText("CE");
    enterButton->setText("ENT");

  //  maxLabel->setTextSize(textControl::size20);
    maxLabel->setText("Max");

//    minLabel->setTextSize(textControl::size20);
    minLabel->setText("Min");
}

//数值按键处理函数
void aKeyBoard::digitClicked()
{

    keyButton   *button = qobject_cast<keyButton *>(sender());
    int val = button->text().toInt();

    //如果当前值为0,再输入0就不响应
    if( lineEdit->text() == "0" && val == 0.0 )
        return;

    //如果还没有输入过任何数据则先清除原数据，并设置输入标志为false
    if( waitingForOperand ){
        lineEdit->clear();
        waitingForOperand = false;
    }

    QString str = lineEdit->text();

    if( str.contains(".") ){
        //判断数值精度是否已达上限
        if( str.size() - str.indexOf(".") > _dot ){
            return;
        }
        //判断数值最大长度是否已达上限，已达上限则不输入小数点
        if( str.indexOf(".") >= MAX_COUNT ){
            return;
        }
    }else{
        //判断数值是否已达最大上限
        if( str.size() >= MAX_COUNT )
            return;
    }

    lineEdit->setText( lineEdit->text() + QString::number(val) );

}

//更改数值正负
void aKeyBoard::changeSign()
{
    if( waitingForOperand ){
        lineEdit->setText("-");
        waitingForOperand = false;
    }

    QString text = lineEdit->text();

    if( text == "-" ){
        return;
    }
    else{
        //    QString text = lineEdit->text();
        if( text.contains("-") ){
            text.remove(0,1);
        }else{
            text.prepend("-");
        }
//        double val = text.toDouble();

//        if( val > 0.0 ){
//            text.prepend("-");
//        }else if( val < 0.0 ){
//            text.remove(0,1);
//        }

        lineEdit->setText(text);
    }
}

//输入小数点
void aKeyBoard::pointClicked()
{
    //精度1则返回
    if( _dot <= 0 ){
        return;
    }

    //还没有输入过数值，则清楚并输入0
    if( waitingForOperand ){
        lineEdit->setText("0");
    }

    if( lineEdit->text() == "-" ){
        lineEdit->setText("-0");
    }

    //如果不包含. 则输入.
    if( !lineEdit->text().contains(".") ){
        lineEdit->setText(lineEdit->text() + ".");
    }

    waitingForOperand = false;
}

//清楚数据
void aKeyBoard::clearClicked()
{
    lineEdit->setText("0");
    waitingForOperand = true;
}

aKeyBoard *aKeyBoard::instance()
{
    if( core == NULL ){
        core = new aKeyBoard;
    }
    return core;
}



