#include "button.h"
#include <unistd.h>
#include "messagebox.h"

//#define USER_TIMER
//#define USER_SLEEP
//#define DELAY_TIME 50

Button::Button(QWidget *parent) :
    QPushButton(parent)
{
    pressDown = false;
    setFocusPolicy(Qt::NoFocus);
    setPalette(*(textControl::instance()->palette()));

    m_DelayTimer.setSingleShot(true);
    connect(this, SIGNAL(clicked()), this, SLOT(clickedSlot()));
    connect(&m_DelayTimer, SIGNAL(timeout()), this, SLOT(delayTimeOutSlot()));

#ifdef USER_TIMER
#else
#ifdef USER_SLEEP
    connect(this,SIGNAL(pressed()),SLOT(privatePressSlot()));
#endif
#endif
}

bool Button::isDown()
{
#ifdef USER_TIMER
    return pressDown;
#else
    return QPushButton::isDown();
#endif
}

void Button::timerEvent(QTimerEvent *e)
{
    killTimer(e->timerId());
#ifdef USER_TIMER
    pressDown = false;
    update();
    emit clicked();
#endif
}

void Button::mousePressEvent(QMouseEvent *e)
{
#ifdef USER_TIMER
    if( pressDown )
        return;
#endif

   // buzzerControl::instance()->wantToBB();

    if( canPress() )
    {
#ifdef USER_TIMER
        pressDown = true;
        update();
        emit pressed();
        startTimer(DELAY_TIME);
#else
        QPushButton::mousePressEvent(e);
#endif
    }
}

void Button::mouseReleaseEvent(QMouseEvent *e)
{
#ifdef USER_TIMER

#else
    QPushButton::mouseReleaseEvent(e);
#endif
}

void Button::privatePressSlot()
{
#ifdef USER_SLEEP
    usleep(DELAY_TIME * 1000);
#endif
}

void Button::setDelayMode(int32_t iDelayTimeMs, int32_t iInitValue)
{
    m_xDelayMode = true;
    m_iDelayTimeMs = iDelayTimeMs;
    m_iInitValue = iInitValue;
}

void Button::disableDelayMode()
{
    m_xDelayMode = false;
}

void Button::delayTimeOutSlot()
{
    emit delayTimeOut(m_iInitValue);
}

void Button::clickedSlot()
{
    bool     xValue = 0;
    bool     xShowMsgBox = false;
    uint8_t  ucValue = 0;
    uint16_t usValue = 0;
    uint32_t uiValue  = 0;

    int8_t   cValue  = 0;
    int16_t  sValue  = 0;
    int32_t  iValue  = 0;

    if(m_xCheckMode)
    {
        if(m_CheckDataType == Monitor::Uint8t)
        {
            ucValue = *static_cast<uint8_t*>(m_pCheckValAddr);
            if(m_iCheckVal != int32_t(ucValue)){ xShowMsgBox = true;}
        }
        else if(m_CheckDataType == Monitor::Uint16t)
        {
            usValue = *static_cast<uint16_t*>(m_pCheckValAddr);
            if(m_iCheckVal != int32_t(usValue)){ xShowMsgBox = true;}
        }
        else if(m_CheckDataType == Monitor::Int8t)
        {
            cValue = *static_cast<int8_t*>(m_pCheckValAddr);
            if(m_iCheckVal != int32_t(cValue)){ xShowMsgBox = true;}
        }
        else if(m_CheckDataType == Monitor::Int16t)
        {
            sValue = *static_cast<int16_t*>(m_pCheckValAddr);
            if(m_iCheckVal != int32_t(sValue)){ xShowMsgBox = true;}
        }
        else if(m_CheckDataType == Monitor::Uint32t)
        {
            uiValue = *static_cast<uint32_t*>(m_pCheckValAddr);
            if(m_iCheckVal != int32_t(uiValue)){ xShowMsgBox = true;}
        }
        else if(m_CheckDataType == Monitor::Int32t)
        {
            iValue = *static_cast<int32_t*>(m_pCheckValAddr);
            if(m_iCheckVal != int32_t(iValue)){ xShowMsgBox = true;}
        }
        else if(m_CheckDataType == Monitor::Boolean)
        {
            xValue = *static_cast<bool*>(m_pCheckValAddr);
            if(m_iCheckVal != int32_t(xValue)){ xShowMsgBox = true;}
        }
    }
    if(xShowMsgBox)
    {
        confirmationBox->show();
    }
    else
    {
        emit buttonClicked();
    }
    if(m_xDelayMode)
    {
        m_DelayTimer.start(m_iDelayTimeMs);
    }
}

void Button::initMessage()
{
    confirmationBox = new messageBox(messageBox::Question);
    confirmationBox->setButtonText(messageBox::Yes,"重置");
    confirmationBox->setButtonText(messageBox::No,"取消");
    confirmationBox->setInformativeText(tr("请确认：是否重置运行时间？"));
    confirmationBox->hide();
}

void Button::setCheckMode(void* pCheckValAddr, int32_t iCheckVal, const QString &text, Monitor::DataType emDataType)
{
    m_pCheckValAddr = pCheckValAddr;
    m_iCheckVal    = iCheckVal;
    m_xCheckMode    = true;
    m_CheckDataType = emDataType;

    confirmationBox = new messageBox(messageBox::Question);
    confirmationBox->setButtonText(messageBox::Yes,"确认");
    confirmationBox->setButtonText(messageBox::No,"取消");
    confirmationBox->setInformativeText(text);
    confirmationBox->hide();
}
