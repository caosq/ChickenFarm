#ifndef DATAMONITOR_H
#define DATAMONITOR_H
#include <stdint.h>
#include <pthread.h>
#include "device.h"

#include <QObject>

#define MONITOR_DATA_MAX_NUM        200     //最大可监控点位数，根据实际情况调整

#define MONITOR(val, ucDataType) \
       (DataMonitor::getInstance())->monitorRegist(static_cast<void*>(&this->val), ucDataType, static_cast<Device*>(this));

typedef void(QObject::*pvObjEvent)(void* pvArg);

enum eDataType{
    Single,
    Boolean,
    Int8t,
    Uint8t,
    Int16t,
    Uint16t,
    Int32t,
    Uint32t,
};

class Monitor : public QObject
{
    Q_OBJECT
public:
    explicit  Monitor();

    friend class DataMonitor;

    void setValRange(int32_t iMaxVal, int32_t iMinVal);
    void setValType(eDataType emDataType);
    void setValue(uint32_t uiVal);
    uint32_t getCurVal();

private:
    void*      m_pvVal;
    uint32_t   m_uiDataVal;   //数据值
    int32_t    m_iMaxVal;
    int32_t    m_iMinVal;
    uint16_t   m_usDataId;    //全局标示
    eDataType  m_eDataType;  //数据类型

signals:
    void valChanged(uint32_t);
};

typedef struct sMonitorList /**/
{
    Monitor*  pMonitor;

    struct sMonitorList*  pNext;
    struct sMonitorList*  pLast;
}sMonitorList;

typedef struct sMonitorDataList /**/
{
    pthread_t      sMonitorThread;
    sMonitorList*  psMonitorList;

    struct sMonitorDataList*  pNext;
    struct sMonitorDataList*  pLast;
}sMonitorDataList;


class DataMonitor : public QObject
{
    Q_OBJECT
public:
    static uint16_t g_usMonitorID;
    static sMonitorDataList*  g_psMonitorDataList;

    static Monitor* monitorRegist(void* pvVal, eDataType  emDataType, int32_t iMaxVal = 65535, int32_t iMinVal = -65535);
    static void* monitorPollTask(void *pvArg);

    static DataMonitor* getInstance();

private:
    explicit DataMonitor();

    static DataMonitor*       g_pDataMonitor;

};




#endif // DATAMONITOR_H
