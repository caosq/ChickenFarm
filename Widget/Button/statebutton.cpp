#include "statebutton.h"

#define activeColor_COLOR "#dfd8c8"
#define INactiveColor_COLOR "#2116f7"

#define SP_X 3
#define SP_Y 3

StateButton *StateButton::father = nullptr;
QFont StateButton::fatherFont = textControl::instance()->font(textControl::size18);
QPixmap *StateButton::pixmap0 = nullptr;
QPixmap *StateButton::pixmap1 = nullptr;

StateButton::StateButton(QWidget *parent) :
    Button(parent),m_pMonitor(nullptr)
{
    defValState = false;
    enableValMarker = true;

    save = true;
    automaticAttack = true;
    m_DefaultState = State0;
    m_CurrentState = State0;
    m_CurrentValue = 0;

//    StateStyle strTextuct0 = {pixmap0, "", QColor(activeColor_COLOR), QColor(INactiveColor_COLOR), 0};

//    StateStyle strTextuct1 = {pixmap1, "", QColor(activeColor_COLOR), QColor(INactiveColor_COLOR), 1};

    StateStyle strTextuct0 = {pixmap0, "", Qt::white, QColor(INactiveColor_COLOR), 0};
    StateStyle strTextuct1 = {pixmap1, "", Qt::green, QColor(INactiveColor_COLOR), 1};

    m_StateStyleVector.resize(0);
    m_StateStyleVector.append(strTextuct0);
    m_StateStyleVector.append(strTextuct1);

    m_StateValMap.insert(State0, 0);
    m_StateValMap.insert(State1, 1);

    //setPermission(fatherLevel);
    if(father)
    {
        connect(father,SIGNAL(fontChange()),this,SLOT(fontSlot()));
    }
    setFont(fatherFont);

    connect(this,SIGNAL(buttonClicked()),this,SLOT(clickedSlot()));
    connect(this,SIGNAL(delayTimeOut(int32_t)),this,SLOT(setValue(int32_t)));
}

StateButton *StateButton::forefather()
{
    if(father == nullptr)
    {
        father = new StateButton;
    }
    return father;
}

bool StateButton::setMonitorData(void* pvVal, Monitor::DataType emDataType)
{
    m_pMonitor = DataMonitor::monitorRegist(pvVal, emDataType);

    if(m_pMonitor != nullptr)
    {
        connect(m_pMonitor, SIGNAL(valChanged(Monitor*)), this, SLOT(setValue(Monitor*)));
        setValue(m_pMonitor->getCurVal());
        return true;
    }
    return false;
}

void StateButton::setDeafultState(ButtonState state)
{
    m_DefaultState = state;
    defValState = (m_DefaultState == m_CurrentState) ? true:false;
    update();
}

bool StateButton::setValueMap(ButtonState state,int val)
{
    m_StateValMap.insert(state,val);
    return true;
}

void StateButton::setStateText(ButtonState state, QString strText)
{
    m_StateStyleVector[state].strText = strText;
    update();
}

void StateButton::setPixmap(ButtonState state, QPixmap *pixmap)
{
    m_StateStyleVector[state].pImage = pixmap;
    update();
}

void StateButton::setStateActiveTextColor(ButtonState state, QColor color)
{
     m_StateStyleVector[state].activeColor = color;
     update();
}

void StateButton::setStateInactiveTextColor(ButtonState state, QColor color)
{
     m_StateStyleVector[state].inactiveColor = color;
     update();
}

void StateButton::setFatherFontSize(int size)
{
    if(this == father)
    {
        if( fatherFont.pointSize() != size )
        {
            fatherFont.setPointSize(size);
            emit fontChange();
        }
    }
}

void StateButton::setFatherPixmap(ButtonState state, QString filePath)
{
    if(father == this)
    {
        switch(state)
        {
        case State0:
        {
            if(pixmap0)
            {
                pixmap0->load(filePath);
            }
            else
            {
                pixmap0 = new QPixmap(filePath);
            }
        }
            break;
        case State1:
        {
            if(pixmap1)
            {
                pixmap1->load(filePath);
            }
            else
            {
                pixmap1 = new QPixmap(filePath);
            }
        }
            break;
        default:
            break;
        }
    }
}

int32_t StateButton::getCurrentValue()
{
    return m_CurrentValue;
}

void StateButton::setValue(Monitor* pMonitor)
{
    int32_t val = pMonitor->getCurVal();
    ButtonState state =  m_StateValMap.key(val, Error);
    if(Error != state)
    {
        m_CurrentState = state;
        m_CurrentValue = val;
        defValState = (m_DefaultState == state) ? true:false;
    }
    update();
    emit valChanged(this);
}

void StateButton::setValue(int32_t val)
{
    ButtonState state =  m_StateValMap.key(val, Error);
    if(Error != state)
    {
        m_CurrentState = state;
        m_CurrentValue = val;
        defValState = (m_DefaultState == state) ? true:false;
    }
    if(m_pMonitor)
    {
        m_pMonitor->setValue(val);
    }
    else
    {
        emit valChanged(this);
    }
    update();

}

void StateButton::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);
    setFont(font);
}

void StateButton::setText(ButtonState state, const QString &text)
{
    if( State0 == state )
    {
        m_StateStyleVector[State0].strText = text;
    }
    else if(State1 == state)
    {
        m_StateStyleVector[State1].strText = text;
    }
    if(m_pMonitor)
    {
        //m_pMonitor->setDatastrTexting(m_StateStyleVector[currentState].strText);
    }
    update();
}

void StateButton::setClicked()
{
    clickedSlot();
}

void StateButton::fontSlot()
{
    setFont(fatherFont);
}

void StateButton::clickedSlot()
{
    int tempVal = 0;
    if( m_CurrentState == ButtonState::State0)
    {
        tempVal = m_StateValMap.value(ButtonState::State1);
    }
    else if( m_CurrentState == ButtonState::State1)
    {
        tempVal = m_StateValMap.value(ButtonState::State0);
    }
    else
    {
        return;
    }
    setValue(tempVal);
}

void StateButton::paintEvent(QPaintEvent *e)
{
    if(m_StateStyleVector[State0].pImage  && m_StateStyleVector[State1].pImage)
    {
        QPainter painter(this);
        if(this->isDown())
        {
            if(State0 == m_CurrentState)
            {
                painter.drawPixmap(1,1,this->width(),this->height(),*(m_StateStyleVector[State0].pImage));
                painter.setPen(m_StateStyleVector[State0].activeColor);
                /*if(isEnabled())
                {
                    painter.setPen(m_StateStyleVector[State0].activeColor);
                }
                else
                {
                    painter.setPen(m_StateStyleVector[State0].inactiveColor);
                }*/
                painter.drawText(1,1,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State0].strText);
                painter.setPen(m_StateStyleVector[State1].inactiveColor);
                painter.drawText(this->width() / 2,1,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State1].strText);
            }
            else
            {
                painter.drawPixmap(1,1,this->width(),this->height(),*(m_StateStyleVector[State1].pImage));
                painter.setPen(m_StateStyleVector[State0].inactiveColor);
                painter.drawText(1,1,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State0].strText);

                painter.setPen(m_StateStyleVector[State1].activeColor);
                /*if(isEnabled())
                {
                    painter.setPen(m_StateStyleVector[State1].activeColor);
                }
                else
                {
                    painter.setPen(m_StateStyleVector[State1].inactiveColor);
                }*/
                painter.drawText(this->width() / 2,1,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State1].strText);
            }
        }
        else
        {
            if(State0 == m_CurrentState)
            {
                painter.drawPixmap(0,0,this->width(),this->height(),*(m_StateStyleVector[State0].pImage));

                painter.setPen(m_StateStyleVector[State0].activeColor);
                /*if(isEnabled())
                {
                    painter.setPen(m_StateStyleVector[State0].activeColor);
                }
                else
                {
                    painter.setPen(m_StateStyleVector[State0].inactiveColor);
                }*/
                painter.drawText(0,0,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State0].strText);
                painter.setPen(m_StateStyleVector[State1].inactiveColor);
                painter.drawText(this->width() / 2,0,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State1].strText);
            }
            else
            {
                painter.drawPixmap(0,0,this->width(),this->height(),*(m_StateStyleVector[State1].pImage));
                painter.setPen(m_StateStyleVector[State0].inactiveColor);
                painter.drawText(0,0,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State0].strText);

                painter.setPen(m_StateStyleVector[State1].activeColor);
                /*if(isEnabled())
                {
                    painter.setPen(m_StateStyleVector[State1].activeColor);
                }
                else
                {
                    painter.setPen(m_StateStyleVector[State1].inactiveColor);
                }*/
                painter.drawText(this->width() / 2,0,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State1].strText);
            }
        }
    }
    else
    {
        QPushButton::paintEvent(e);
        QPainter painter(this);
        if(State0 == m_CurrentState)
        {
            painter.setPen(m_StateStyleVector[State0].activeColor);
            /*if(isEnabled())
            {
                painter.setPen(m_StateStyleVector[State0].activeColor);
            }
            else
            {
                painter.setPen(m_StateStyleVector[State0].inactiveColor);
            }*/
            painter.drawText(0,0,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State0].strText);
            painter.setPen(m_StateStyleVector[State1].inactiveColor);
            painter.drawText(this->width() / 2,0,this->width() / 2,this->width(),Qt::AlignCenter,m_StateStyleVector[State1].strText);
        }
        else
        {
            painter.setPen(m_StateStyleVector[State0].inactiveColor);
            painter.drawText(0,0,this->width() / 2,this->height(),Qt::AlignCenter,m_StateStyleVector[State0].strText);

            painter.setPen(m_StateStyleVector[State1].activeColor);
            /*if(isEnabled())
            {
                painter.setPen(m_StateStyleVector[State1].activeColor);
            }
            else
            {
                painter.setPen(m_StateStyleVector[State1].inactiveColor);
            }*/
            painter.drawText(this->width() / 2,0,this->width() / 2,this->width(),Qt::AlignCenter,m_StateStyleVector[State1].strText);
        }
    }
    if(!defValState && enableValMarker){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::green);
        painter.drawEllipse(SP_X, SP_Y, 5, 5);
    }
}
