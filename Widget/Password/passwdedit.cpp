#include "passwdedit.h"
#include "password.h"

passwdEdit *passwdEdit::_instance = 0;

passwdEdit::passwdEdit(int width,int height,QWidget *parent) :
    dialogBase(parent)
{
    resize(width,height);

    msgBox = new messageBox;

    setupUi();
    //setLanguage(translate::instance()->currentLanguage());

    connect(cancelBtn,SIGNAL(clicked()),SLOT(hide()));
    connect(okBtn,SIGNAL(clicked()),SLOT(setPassword()));


    setChinese();
}

passwdEdit *passwdEdit::instance(int width,int height,QWidget *parent)
{
    if( _instance == 0 ){
        _instance = new passwdEdit(width,height,parent);
    }
    return _instance;
}

void passwdEdit::setPassword()
{
    int pwd0 = oldEdit->text().toInt();

    int pwd1 = newEdit->text().toInt();
    int pwd2 = newEditAgain->text().toInt();

    if( pwd1 == pwd2 ){
        if( password::instance()->reloadUserPassword(pwd1,pwd0) ){
            showSuccess();
        }else{
            showFaild();
        }
    }else{
        showFaild();
    }
    hide();
}

void passwdEdit::setupUi()
{
    oldPasswd = new TextLabel(this);
    oldPasswd->setGeometry(20,60,200,33);

    newPasswd = new TextLabel(this);
    newPasswd->setGeometry(20,110,200,33);

    newPasswdAgain = new TextLabel(this);
    newPasswdAgain->setGeometry(20,160,200,33);

    tipsLabel = new TextLabel(this);
    tipsLabel->setGeometry(80,220,350,33);

    okBtn = new ubuttonBK(this);
    okBtn->setGeometry(90,260,100,40);

    cancelBtn = new ubuttonBK(this);
    cancelBtn->setGeometry(270,260,100,40);

     okBtn->setTextColor(QPalette::ColorGroup::Current, QColor(0, 0, 0));
     cancelBtn->setTextColor(QPalette::ColorGroup::Current, QColor(0, 0, 0));

    oldEdit = new mEdit(this);
    oldEdit->setAlignment(Qt::AlignRight);
    oldEdit->setGeometry(220, 60, 200, 33);

    newEdit = new mEdit(this);
    newEdit->setAlignment(Qt::AlignRight);
    newEdit->setGeometry(220, 110, 200, 33);

    newEditAgain = new mEdit(this);
    newEditAgain->setAlignment(Qt::AlignRight);
    newEditAgain->setGeometry(220, 160, 200, 33);
}

void passwdEdit::showSuccess()
{
    msgBox->setInformativeText(success);
    msgBox->show();
}

void passwdEdit::showFaild()
{
    msgBox->setInformativeText(faild);
    msgBox->show();
}

void passwdEdit::setChinese()
{
    QPalette pe;

    oldPasswd->setText("请输入旧密码");
    newPasswd->setText("请输入新密码");
    newPasswdAgain->setText("请再次输入新密码");

    tipsLabel->setText("注意：请保管好您的密码");
    okBtn->setText("确定");
    cancelBtn->setText("取消");

    success = "密码修改成功！";
    faild = "密码错误，修改失败！";
    msgBox->setButtonText(messageBox::Yes,"关闭");
}

void passwdEdit::setEnglish()
{
    oldPasswd->setText("Enter password");
    newPasswd->setText("Enter new password");
    newPasswdAgain->setText("Retype new password");

    tipsLabel->setText("Warning：remember your password");
    okBtn->setText("Enter");
    cancelBtn->setText("Cancel");

    success = "Password updated successfully！";
    faild = "Sorry，passwords do not match.password unchanged!";
    msgBox->setButtonText(messageBox::Yes,"Close");
}


mEdit::mEdit(QWidget *parent):
    QLineEdit(parent)
{
    passwd = pwdKeyBoard::getWrongValue();

    setFocusPolicy(Qt::NoFocus);

    setEchoMode(PasswordEchoOnEdit);

    setText(QString::number(passwd));

    setStyleSheet("border:1px solid gray;border-radius:5px;font-size:28px;color:#dfd8c8;background-color:transparent;");

}

void mEdit::mousePressEvent(QMouseEvent * event)
{
//    buzzerControl::instance()->wantToBB();
    passwd = pwdKeyBoard::getValue(passwd);
    setText(QString::number(passwd));
}

void mEdit::showEvent(QShowEvent *e)
{
    QLineEdit::showEvent(e);
    passwd = pwdKeyBoard::getWrongValue();
    setText(QString::number(passwd));
}

int mEdit::getPassWord()
{
    return passwd;
}
