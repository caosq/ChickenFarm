#include "button.h"
#include <unistd.h>
#include "messagebox.h"

//#define USER_TIMER
//#define USER_SLEEP
//#define DELAY_TIME 50

/*BtnCheckData:: BtnCheckData(void* pCheckValAddr, int32_t  iCheckVal, Monitor::DataType eCheckDataType,
                            const QString  &strChecktext, QObject *parent) : QObject(parent)
{
    m_pCheckValAddr = pCheckValAddr;
    m_iCheckVal = iCheckVal;
    m_eCheckDataType = eCheckDataType;
    m_strChecktext = strChecktext;
}*/

bool Button::g_xDebugMode = false;
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

    int32_t  iCheckVal;
    QString  sChecktext;
    void*    pCheckValAddr = nullptr;
    Monitor::DataType eCheckDataType;

    if(m_xCheckMode)
    {
        for (uint8_t n = 0; n < m_BtnCheckDatas.count(); n++)
        {
            if(m_BtnCheckDatas[n]->pCheckValAddr == nullptr){continue;}

            eCheckDataType = m_BtnCheckDatas[n]->eCheckDataType;
            pCheckValAddr = m_BtnCheckDatas[n]->pCheckValAddr;
            sChecktext = m_BtnCheckDatas[n]->strChecktext;
            iCheckVal = m_BtnCheckDatas[n]->iCheckVal;

            if(eCheckDataType == Monitor::Uint8t)
            {
                ucValue = *static_cast<uint8_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(ucValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Uint16t)
            {
                usValue = *static_cast<uint16_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(usValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Int8t)
            {
                cValue = *static_cast<int8_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(cValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Int16t)
            {
                sValue = *static_cast<int16_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(sValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Uint32t)
            {
                uiValue = *static_cast<uint32_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(uiValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Int32t)
            {
                iValue = *static_cast<int32_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(iValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Boolean)
            {
                xValue = *static_cast<bool*>(pCheckValAddr);
                if(iCheckVal != int32_t(xValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
        }
    }
    if(xShowMsgBox && g_xDebugMode == false)
    {
        messageBox *pConfirmationBox = new messageBox(messageBox::Information);
        pConfirmationBox->setButtonText(messageBox::Yes,"确认");
        pConfirmationBox->setInformativeText(sChecktext);
        pConfirmationBox->exec();
        return;
    }
    else
    {
        if(m_xConfirmMode)
        {
            int32_t test;
            messageBox *pConfirmationBox = new messageBox(messageBox::Question);
            pConfirmationBox->setButtonText(messageBox::Yes,"确认");
            pConfirmationBox->setButtonText(messageBox::No,"取消");
            pConfirmationBox->setInformativeText(m_sConfirmtext);
            test = pConfirmationBox->exec();

            if(test == messageBox::No){return;}
        }
        if(m_xDelayMode)
        {
            m_DelayTimer.start(m_iDelayTimeMs);
        }
        emit buttonClicked();
    }
}

void Button::initMessage()
{
    /*confirmationBox = new messageBox(messageBox::Question);
    confirmationBox->setButtonText(messageBox::Yes,"重置");
    confirmationBox->setButtonText(messageBox::No,"取消");
    confirmationBox->setInformativeText(tr("请确认：是否重置运行时间？"));
    confirmationBox->hide();*/
}

void Button::setCheckMode(void* pCheckValAddr, int32_t iCheckVal, const QString &text, Monitor::DataType emDataType)
{
    m_xCheckMode    = true;

    BtnCheckData *pBtnCheckData = new BtnCheckData();

    pBtnCheckData->pCheckValAddr = pCheckValAddr;
    pBtnCheckData->iCheckVal = iCheckVal;
    pBtnCheckData->eCheckDataType = emDataType;
    pBtnCheckData->strChecktext = text;
    m_BtnCheckDatas.append(pBtnCheckData);
}

void Button::setCheckMode(QVector<BtnCheckData> *pBtnCheckDatas)
{
    /*m_pCheckValAddr = pCheckValAddr;
    m_iCheckVal     = iCheckVal;
    m_sChecktext    = text;
    m_eCheckDataType = emDataType;*/

    /*for (uint8_t n = 0; n < mBtnCheckDatas.count(); n++)
    {
        m_BtnCheckDatas.append( mBtnCheckDatas[n] );
    }
    m_xCheckMode    = true;*/
}

void Button::setCheckMode(int32_t iItemCount, ...)
{
    va_list ap;
    va_start(ap, iItemCount);

    BtnCheckData *pBtnCheckData = nullptr;
    BtnCheckData *mBtnCheckData = nullptr;

    for(uint8_t n = 0; n < iItemCount; n++)
    {
        mBtnCheckData = new BtnCheckData();
        pBtnCheckData = va_arg(ap, BtnCheckData*);

        mBtnCheckData->pCheckValAddr = pBtnCheckData->pCheckValAddr;
        mBtnCheckData->iCheckVal = pBtnCheckData->iCheckVal;
        mBtnCheckData->eCheckDataType = pBtnCheckData->eCheckDataType;
        mBtnCheckData->strChecktext = pBtnCheckData->strChecktext;
        m_BtnCheckDatas.append(mBtnCheckData);
    }
    va_end(ap);
    m_xCheckMode = true;
}

void Button::setDebugMode(bool xConfirmMode)
{
    Button::g_xDebugMode = xConfirmMode;
}

void Button::disableCheckMode()
{
    m_xCheckMode = false;
}

void Button::setConfirmMode(const QString &text)
{
    m_xConfirmMode = true;
    m_sConfirmtext = text;
}

void Button::disableConfirmMode()
{
     m_xConfirmMode = false;
}
