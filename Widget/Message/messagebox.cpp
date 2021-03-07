#include "messagebox.h"
#include "textcontrol.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLayout>

#include <QLabel>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QSizePolicy>

#define TITLE "#3d3d3d"
#define BG_COLOR "#0d74bc"
#define UB_RELEAS_PIX ":UI/baseFile/abRelease.png"
#define UB_PRESS_PIX ":UI/baseFile/abPress.png"

QColor messageBox::_backgroundColor(BG_COLOR);
QColor messageBox::_frameLessColor(TITLE);
QMap<messageBox::msgType,QPixmap *> messageBox::iconMap;

messageBox* messageBox::g_pUniversalMsg = nullptr;

messageBox::messageBox(msgType type, QWidget *parent) :
    QMessageBox(parent),_type(type)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_ShowModal,true);
    setAutoFillBackground(true);

    moveState = true;
    pressed = false;

    _informativeFormatStr = "";

    //设置文字字号
    setFont(textControl::instance()->font(textControl::size20));
    //配置文字调色板
    setPalette(*(textControl::instance()->palette()));

    frameLessColor = _frameLessColor;

    initButton();
    initIcon();
    setBackGroundColor(_backgroundColor);
    setFramelessWindowColor(frameLessColor);

    //设置边距
    layout()->setContentsMargins(0,10,10,10);

    setAlwaysStay(false);
}


messageBox *messageBox::instance()
{
    if( g_pUniversalMsg == nullptr)
    {
        g_pUniversalMsg = new messageBox;
        g_pUniversalMsg->setButtonText(messageBox::Yes,tr("确认"));
        g_pUniversalMsg->setInformativeText("");
    }
    return g_pUniversalMsg;
}

void messageBox::setBackGroundColor(QColor color)
{
    QPalette palette = this->palette();
    palette.setBrush(this->backgroundRole(),QBrush(color));
    setPalette(palette);
}

void messageBox::disableMove(bool state)
{
    moveState = state;
}

void messageBox::setAlwaysStay(bool state)
{
/*    if( state ){
        disconnect(ScreenSaver::instance(),SIGNAL(isTimeToBack()),this,SLOT(tryReject()));
    }else{
        connect(ScreenSaver::instance(),SIGNAL(isTimeToBack()),this,SLOT(tryReject()));
    }
*/
}

void messageBox::hideButton(messageBox::btRole role)
{
    switch(role)
    {
    case Yes:{
        if( yes ){
            yes->hide();
        }
    }
        break;
    case No:{
        if( no ){
            no->hide();
        }
    }
        break;
    default:
        break;
    }
}

void messageBox::showButton(messageBox::btRole role)
{
    switch(role)
    {
    case Yes:{
        if( yes ){
            yes->show();
        }
    }
        break;
    case No:{
        if( no ){
            no->show();
        }
    }
        break;
    default:
        break;
    }
}

void messageBox::initButton()
{
    QPalette pe;
    pe.setColor(QPalette::ButtonText,Qt::black);


    switch(_type){
    case Information:
    case Warning:{
        yes = new ubuttonBK(this);
        yes->setMaximumSize(100,35);
        yes->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        yes->setText("Yes");
        yes->setPalette(pe);
        addButton(yes,QMessageBox::YesRole);
        no = NULL;
    }
        break;
    case Question:{
        no = new ubuttonBK(this);
        no->setMaximumSize(100,35);
        no->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        no->setText("No");
        no->setPalette(pe);
        addButton(no,QMessageBox::NoRole);

        yes = new ubuttonBK(this);
        yes->setMaximumSize(100,35);
        yes->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        yes->setText("Yes");
        yes->setPalette(pe);
        addButton(yes,QMessageBox::YesRole);
    }
        break;
    case Message:{
        yes = new ubuttonBK(this);
        yes->setMaximumSize(100,35);
        yes->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        yes->setText("Yes");
        yes->setPalette(pe);
        addButton(yes,QMessageBox::YesRole);
        yes->hide();

    }
        break;
    default:
        break;
    }

}

//初始化图标，这里禁用了icon
//启用后弹框消息默认会有个小图标(感叹号,问号..跟配置的具体消息类型有关)
void messageBox::initIcon()
{
    setIcon(QMessageBox::NoIcon);
    return;

    if( NULL != iconMap.value(_type,NULL) ){
        setIconPixmap(*(iconMap.value(_type)));
    }else{
        switch(_type){
        case Warning:
            setIcon(QMessageBox::Warning);
            break;
        case Information:
            setIcon(QMessageBox::Information);
            break;
        case Question:
            setIcon(QMessageBox::Question);
            break;
        default:
            setIcon(QMessageBox::NoIcon);
            break;
        }
    }
}

void messageBox::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);
    setFont(font);
}

void messageBox::setTextColor(QColor color)
{
    QPalette    palette = this->palette();
    palette.setBrush(QPalette::Active,QPalette::WindowText,QBrush(color));
    palette.setBrush(QPalette::Inactive,QPalette::WindowText,QBrush(color));
    setPalette(palette);
}

void messageBox::setButtonText(messageBox::btRole role, QString str)
{
    switch(role){
    case Yes:{
        if( yes ){
            yes->setText(str);
        }
    }
        break;
    case No:{
        if( no ){
            no->setText(str);
        }
    }
        break;
    }
}

void messageBox::setFramelessWindowColor(QColor color)
{
    frameLessColor = color;
}

void messageBox::setDefIconPixmap(messageBox::msgType type, QString filePath)
{
    if( NULL == iconMap.value(type,NULL) ){
        iconMap.insert(type,new QPixmap(filePath));
    }else{
        QPixmap *temp = iconMap.value(type);
        temp->load(filePath);
    }
}

void messageBox::setDefIconPixmap(messageBox::msgType type, QPixmap *pixmap)
{
    iconMap.insert(type,pixmap);
}

void messageBox::mouseMoveEvent ( QMouseEvent * event )
{
    if( !moveState )
        return;
    if ( pressed==true )
    {
        QPoint movePot = event->globalPos();
        if( movePot.y() < mousePosition.y())
            return;
        if( movePot.y() - mousePosition.y() > 735)
            return;
        if( movePot.x() + (this->width() / 2) - mousePosition.x() > 1024 )
            return;
        if( movePot.x() < mousePosition.x() - (this->width() / 2))
            return;
        movePot = event->globalPos() - mousePosition;
        move(movePot);
    }
}

bool messageBox::eventFilter(QObject *o, QEvent *e)
{
    if( o == this ){
        if( e->type() == QEvent::UpdateRequest )
            return true;
    }

    return false;
}

//设置消息弹框的最小大小
void messageBox::showEvent(QShowEvent *event)
{
    //查找QMessageBox的子类
    QWidget *widget = findChild<QWidget *>("qt_msgbox_label");
    if(widget != 0){
        //设置最小大小
        widget->setMinimumWidth(400);
    }

    QMessageBox::showEvent(event);

    //设置初始显示位置
    move( (qApp->desktop()->width() - width()) / 2,
          (qApp->desktop()->height() - height()) / 2);

}

void messageBox::tryReject()
{
    if( !isHidden() ){
        reject();
    }
}


void messageBox::mousePressEvent ( QMouseEvent * event )
{
    mousePosition = event->pos();
    if(mousePosition.y() > 33)
        return;
    pressed = true;
}

void messageBox::mouseReleaseEvent(QMouseEvent * event)
{
    pressed = false;
}

void messageBox::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    QPen pen(frameLessColor,2);
    painter.setPen(pen);
    painter.drawRect(1,30,this->width() - 2,this->height() - 30 - 1);

    painter.setBrush(QBrush(frameLessColor));
    painter.drawRect(0,0,this->width(),33);

}
