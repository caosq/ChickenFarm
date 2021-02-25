#ifndef _MB_DRIVER_H_
#define _MB_DRIVER_H_

#include <stdint.h>
#include "mbconfig.h"

#if MB_UCOSIII_ENABLED

#include "lpc_pinsel.h"
#include "lpc_uart.h"
#include "md_io.h"

typedef struct
{
    const IODef*        Rxd;      //Rxd
    const IODef*        Txd;      //Txd
    const IODef*        DE;       //DE
    const IODef*        INV;      //INV
    const UART_ID_Type  ID;
    UART_CFG_Type       UARTCfg;
}sUART_Def;

#elif MB_LINUX_ENABLED

typedef struct
{
    int fd;

    /* Parity: "even", "odd", "none" */
    uint8_t parity;

    uint32_t baud;
    /* Data bit */
    uint8_t data_bit;
    /* Stop bit */
    uint8_t stop_bit;

}sUART_Def;

#endif

typedef enum
{
    UART_RX_EN = 0,       //接收使能
    UART_TX_EN = 1        //发送使能
}eUART_EN;

uint8_t xMB_UartInit(const sUART_Def *Uart);
void MB_SendOrRecive(const sUART_Def *Uart,eUART_EN mode);

#endif
