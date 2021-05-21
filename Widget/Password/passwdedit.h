#ifndef PASSWDEDIT_H
#define PASSWDEDIT_H

#include <QLineEdit>

#include "dialogbase.h"
#include "textlabel.h"
#include "ubuttonbk.h"
#include "messagebox.h"
#include "pwdkeyboard.h"

class mEdit;
class passwdEdit : public dialogBase
{
    Q_OBJECT
public:
    explicit passwdEdit(int width = 450,int height = 320,QWidget *parent = 0);
    
    static passwdEdit *instance(int width = 450,int height = 320,QWidget *parent = 0);

private:
    void setupUi();
    void showSuccess();
    void showFaild();
    virtual void setChinese();
    virtual void setEnglish();
    
private slots:
    void setPassword();

private:
    static passwdEdit *_instance;
    messageBox  *msgBox;

    TextLabel *oldPasswd;
    TextLabel *newPasswd;
    TextLabel *newPasswdAgain;

    TextLabel *tipsLabel;

    mEdit *oldEdit;
    mEdit *newEdit;
    mEdit *newEditAgain;

    ubuttonBK *okBtn;
    ubuttonBK *cancelBtn;

    QString success;
    QString faild;

};

class mEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit mEdit(QWidget *parent = 0);

    int getPassWord();

protected:
    void mousePressEvent(QMouseEvent *event);
    void showEvent(QShowEvent *e);

private:
    int passwd;
};

#endif // PASSWDEDIT_H
