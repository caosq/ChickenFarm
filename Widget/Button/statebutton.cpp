#include "statebutton.h"

StateButton::StateButton()
{

}

bool StateButton::setMonitorData(void* pvVal, eDataType emDataType)
{
    m_pMonitor = DataMonitor::monitorRegist(pvVal, emDataType);

    if(m_pMonitor != nullptr)
    {
        connect(m_pMonitor, SIGNAL(valChange(uint)), this, SLOT(setValue(uint)));
        setValue(m_pMonitor->getCurVal());
        return true;
    }
    return false;
}

bool StateButton::setValueMap(eButtonState state,int val)
{
    m_StateValMap.insert(state,val);
    return true;
}

void StateButton::setStateText(eButtonState state, QString str)
{
    m_StateStyleVector[state].strText = str;
    update();
}


void StateButton::setPixmap(eButtonState state, QPixmap *pixmap)
{
    m_StateStyleVector[state].pImage = pixmap;
    update();
}

void StateButton::setStateActiveTextColor(eButtonState state, QColor color)
{
     m_StateStyleVector[state].activeColor = color;
    update();
}

void StateButton::setStateInactiveTextColor(eButtonState state, QColor color)
{
     m_StateStyleVector[state].inactiveColor = color;
    update();
}

void StateButton::setValue(unsigned int val)
{
    m_CurrentState =  m_StateValMap.key(val);;
    update();
}

void StateButton::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);
    setFont(font);
}

void StateButton::setClicked()
{
    clickedSlot();
}

void StateButton::clickedSlot()
{
    int tempVal = 0;
    if( m_CurrentState == eButtonState::StateOff)
    {
        tempVal = m_StateValMap.value(eButtonState::StateOn);
    }
    else if( m_CurrentState == eButtonState::StateOn)
    {
        tempVal = m_StateValMap.value(eButtonState::StateOff);
    }
    else
    {
        return;
    }
    setValue(tempVal);
}
