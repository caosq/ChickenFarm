#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>
#include "mbdriver.h"
#include "mb.h"
#include "mb_m.h"

class Modbus : public QObject
{
public:

    typedef enum {
        None 	= 0,					/*!< No parity */
        Even, 							/*!< Even parity */
        Odd,	 						/*!< Odd parity */
        Sp_1, 							/*!< Forced "1" stick parity */
        Sp_0 							/*!< Forced "0" stick parity */
    }eParityType;


    void uartConfig(uint32_t usBaudRate, uint8_t ucDataBit, uint8_t ucStopBit, uint8_t pcParity);

    bool masterInit(eMBMode eMode, const char* pcPortName, uint8_t ucMinAddr, uint8_t ucMaxAddr, bool bDTUEnable);

    bool slaveInit(eMBMode eMode, const char* pcPortName, uint8_t ucSlaveAddr);


    sMBMasterInfo* getMBMasterInfo();

    static Modbus* getInstance();

    explicit Modbus(eMBType eMBType, QObject *parent = nullptr);

private:
    eMBType         m_MBType;
    sUART_Def       m_Uart;
    sMBMasterInfo   m_MasterInfo;  //主栈接口
    sMBSlaveInfo*   m_SlaveInfo;  //从栈接口

    static Modbus *g_pModbus;
};


#endif // MODBUS_H
