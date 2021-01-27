#include "settingbutton.h"

SettingButton::SettingButton()
{

    connect(this,SIGNAL(clicked()),this,SLOT(clickedSlot()));
}

bool SettingButton::setMonitorData(void* pvVal, uint8_t ucDataType)
{
    //m_pMonitor = DataMonitor::monitorRegist(pvVal, ucDataType);
    //return m_pMonitor == nullptr ? false:true;
}
