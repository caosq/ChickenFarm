#include "delaybutton.h"

DelayButton::DelayButton(QWidget *parent) :
    StateButton(parent)
{
    delayTimer = new QTimer(this);
    delayTime = 2;
    initMessage();

    chooseMessage();

    disconnect(this,SIGNAL(clicked()),this,SLOT(clickedSlot()));
    connect(this,SIGNAL(clicked()),this,SLOT(messageBoxShow()));
    connect(delayTimer,SIGNAL(timeout()),this,SLOT(delayTimeSlot()));

    connect(temp,SIGNAL(finished(int)),this,SLOT(delayClicked(int)));
}

void DelayButton::setDelayTime(char time)
{
    delayTime =time;
}

void DelayButton::chooseMessage(char id )
{
    temp=msg[id];
}

void DelayButton::initMessage()
{
    temp = new messageBox;
    confirmationBox = new messageBox(messageBox::Question);
    confirmationBox->setButtonText(messageBox::Yes,"重置");
    confirmationBox->setButtonText(messageBox::No,"取消");
    confirmationBox->setInformativeText(tr("请确认：是否重置运行时间？"));
    confirmationBox->hide();

    if( msg.size() != MAX_MSG ){
        msg.resize(MAX_MSG);
    }

    msg[0] = confirmationBox;


}

void DelayButton::delayClicked(int val)
{
    if(0 == val)
        return;

    clickedSlot();
    if(NULL != monitor)
    {
        if(0 == dataInstance::Data()->getValue(monitor->id(),monitor->addr()));
        {
            delayTimer->start(delayTime*1000);
          this->setDisabled(true);
        }
//          msgManage::instance()->showUniversalMsg(52);//    msg[52] = tr("系统设备存在故障，禁止操作！");
    }else
    {
        qDebug("Doesn't init this memory!");
    }

}

void DelayButton::delayTimeSlot()
{

      dataInstance::Data()->setValue(monitor->id(),monitor->addr(),0,false);
      this->setEnabled(true);
}

void DelayButton::messageBoxShow()
{
        temp->show();
}
