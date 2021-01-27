#include "pwdkeyboard.h"
#include "textlabel.h"
#include "textcontrol.h"
#include <qmath.h>

#define MAX_COUNT 8
#define BACKGROUND ":/images/baseFile/pwdkeyboard.png"

#define WRONG_PASSWORD -1000000

pwdKeyBoard   *pwdKeyBoard::core = NULL;
int pwdKeyBoard::_value = WRONG_PASSWORD;
bool pwdKeyBoard::exitState = true;

pwdKeyBoard::pwdKeyBoard(QWidget *parent) :
    skeyboard(parent)
{

    setupUi();
    connect(digitButton[0],SIGNAL(pressed()),this,SLOT(digitClicked()));
    connect(enterButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(exitSlot()));

    connect(clearButton,SIGNAL(clicked()),this,SLOT(clearClicked()));

    setAlwaysStay(false);

}

int pwdKeyBoard::getValue(int value, bool *exit)
{
    pwdKeyBoard::instance()->initValueShow(value);

    exitState = false;
    _value = value;
    if( pwdKeyBoard::instance()->exec() == QDialog::Accepted ){
        _value = pwdKeyBoard::instance()->getLineEditValue();
    }

    if(exit){
        *exit = exitState;
    }

    return _value;
}

QString pwdKeyBoard::getValueString(int value)
{
    pwdKeyBoard::instance()->initValueShow(value);

    QString valStr = QString::number(value);
    if( pwdKeyBoard::instance()->exec() == QDialog::Accepted ){
        valStr = pwdKeyBoard::instance()->lineEditText();
    }

    return valStr;
}

int pwdKeyBoard::getWrongValue()
{
    return WRONG_PASSWORD;
}

void pwdKeyBoard::initValueShow(int value)
{
    waitingForOperand = true;
    lineEdit->setText(QString::number(value));
}

void pwdKeyBoard::setupUi()
{
    QPixmap * img = new QPixmap(BACKGROUND);
    QPalette p = palette();
    p.setBrush(QPalette::Window, QBrush(*img));
    setPalette(p);

    lineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    titleLabel = new TextLabel(this);
    titleLabel->setGeometry(14,6,280,30);

    digitButton[0] = new keyButton(this);
    digitButton[0]->setText("0");
    digitButton[0]->setGeometry(12, 298, 138, 57);

    clearButton = new keyButton(this);
    clearButton->setGeometry(230, 102, 66, 88);

    cancelButton = new keyButton(this);
    cancelButton->setGeometry(230, 203, 66, 88);

    enterButton = new keyButton(this);
    enterButton->setGeometry(157, 298, 138, 56);

  //  retranslateUi(translate::instance()->currentLanguage());
}

int pwdKeyBoard::getLineEditValue()
{
    bool ret = false;
    int val = lineEdit->text().toInt(&ret,10);
    if(ret){
        return val;
    }else{
        return _value;
    }
}

QString pwdKeyBoard::lineEditText()
{
    return lineEdit->text();
}

void pwdKeyBoard::retranslateUi(QLocale::Language lang)
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

void pwdKeyBoard::setChinese()
{
    clearButton->setText(tr("清除"));
    cancelButton->setText(tr("退出"));
    enterButton->setText(tr("确认"));

    titleLabel->setTextSize(textControl::size20);
    titleLabel->setText("请输入密码:");
}

void pwdKeyBoard::setEnglish()
{
    clearButton->setText(tr("CR"));
    cancelButton->setText(tr("CE"));
    enterButton->setText(tr("ENT"));

    titleLabel->setTextSize(textControl::size20);
    titleLabel->setText("Enter password:");
}

void pwdKeyBoard::readyExit()
{
    exitState = true;
}

void pwdKeyBoard::digitClicked()
{

    keyButton   *button = qobject_cast<keyButton *>(sender());

    if( waitingForOperand ){
        lineEdit->clear();
        waitingForOperand = false;
    }

    QString str = lineEdit->text();

    if( str.size() >= MAX_COUNT )
        return;

    lineEdit->setText( lineEdit->text() + button->text() );

}

void pwdKeyBoard::clearClicked()
{
    lineEdit->setText("");
    waitingForOperand = true;
}

void pwdKeyBoard::exitSlot()
{
    exitState = true;
    hide();
}

pwdKeyBoard *pwdKeyBoard::instance()
{
    if( core == NULL ){
        core = new pwdKeyBoard;
    }
    return core;
}



