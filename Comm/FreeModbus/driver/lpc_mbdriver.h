#ifndef _LPC_MB_DRIVER_H_
#define _LPC_MB_DRIVER_H_

#include "lpc_pinsel.h"
#include "lpc_uart.h"
#include "lpc_clkpwr.h"
#include "md_io.h"

typedef enum
{
	UART_RX_EN = 0,       //接收使能
	UART_TX_EN = 1        //发送使能
}eUART_EN;

typedef struct
{
    const IODef*        Rxd;      //Rxd  
	const IODef*        Txd;      //Txd
	const IODef*        DE;       //DE
	const IODef*        INV;      //INV
	const UART_ID_Type  ID; 
	UART_CFG_Type       UARTCfg; 
}sUART_Def;

void MB_UartInit(const sUART_Def *Uart);
void MB_SendOrRecive(const sUART_Def *Uart,eUART_EN mode);

uint16_t MB_CRCCalc(const uint8_t *Data,uint8_t StartIndex, uint16_t DataSize);
uint16_t usMBCRC16( uint8_t * pucFrame, uint16_t usLen );

#endif
