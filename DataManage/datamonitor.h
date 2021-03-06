#ifndef DATAMONITOR_H
#define DATAMONITOR_H
#include <stdint.h>
#include <pthread.h>
#include "device.h"

#include <QObject>
#include <QMap>

#define MONITOR(val, ucDataType) \
       (DataMonitor::getInstance())->monitorRegist(static_cast<void*>(&this->val), ucDataType, static_cast<Device*>(this));

typedef void(QObject::*pvObjEvent)(void* pvArg);

class Monitor : public QObject
{
    Q_OBJECT

public:

    enum DataType{
        Single,
        Boolean,
        Int8t,
        Uint8t,
        Int16t,
        Uint16t,
        Int32t,
        Uint32t,
    };

    explicit  Monitor(void* pvVal, DataType eDataType, uint32_t uiDataId, int32_t iMaxVal = 65535, int32_t iMinVal = -65535);

    friend class DataMonitor;

    void setValRange(int32_t iMaxVal, int32_t iMinVal);
    void setValType(DataType emDataType);
    void setValue(int32_t iVal);

    uint8_t  getValType();
    int32_t  getCurVal();
    uint32_t getDataId();
    void*    getValueAddr();

private:

    int32_t    m_iMaxVal = 0;
    int32_t    m_iMinVal = 0;
    int32_t    m_iDataVal = 0;     //数据值
    uint32_t   m_uiDataId = 0;     //数据ID
    void*      m_pvVal = nullptr;
    DataType   m_DataType = Uint16t;  //数据类型

signals:
    void valChanged(Monitor*);
};

/*typedef struct sMonitorList
{
    Monitor*  pMonitor;

    struct sMonitorList*  pNext;
    struct sMonitorList*  pLast;
}sMonitorList;
*/

typedef struct sMonitorMapList /**/
{
    pthread_t      sMonitorThread;
    QMap<void*, Monitor*> m_MonitorMap;

    struct sMonitorMapList*  pNext;
    struct sMonitorMapList*  pLast;
}sMonitorMapList;


class DataMonitor : public QObject
{
    Q_OBJECT
public:
    static uint16_t g_usMonitorID;
    static sMonitorMapList*  g_psMonitorMapList;

    static Monitor* monitorRegist(void* pvVal, Monitor::DataType emDataType, int32_t iMaxVal = 65535, int32_t iMinVal = -65535);
    static void* monitorPollTask(void *pvArg);
    static DataMonitor* getInstance();

private:
    static QMap<void* , Monitor*> g_Monitors;

    explicit DataMonitor(QObject *parent = nullptr);
    static DataMonitor*  g_pDataMonitor;

};


#endif // DATAMONITOR_H
