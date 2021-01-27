#include <bsp_int.h>
#include <lib_def.h>
#include <os.h>
#include "lpc_gpio.h"
#include "lpc_clkpwr.h"
#include "lpc_timer.h"
#include "mbdriver.h"
#include "lpc_Pinsel.h"

#include <LPC407x_8x_177x_8x.h>

/**********************************************************************
 * @brief   UART初始化
 * @param   *Uart   UART
 * @return	none
 *********************************************************************/
uint8_t xMB_UartInit(const sUART_Def *Uart)
{
	uint8_t UartIntID = 0; 
    
	UART_CFG_Type UARTCfg = Uart->UARTCfg;    //Uart串口基本设置
		
	UART_FIFO_CFG_Type UARTFIFOCfg;
	UARTFIFOCfg.FIFO_DMAMode = DISABLE;
	UARTFIFOCfg.FIFO_Level = UART_FIFO_TRGLEV2;
	UARTFIFOCfg.FIFO_ResetRxBuf = ENABLE;
	UARTFIFOCfg.FIFO_ResetTxBuf = ENABLE;
	
	//UART_DE
	GPIO_SetDir(Uart->DE->Port , 1<<Uart->DE->Pin, 1);
	GPIO_SetValue(Uart->DE->Port, 1<<Uart->DE->Pin);
		
	//UART_INV
	GPIO_SetDir(Uart->INV->Port, 1<<Uart->INV->Pin, 1);
	GPIO_ClearValue(Uart->INV->Port, 1<<Uart->INV->Pin);
	
	switch(Uart->ID)
	{
		case UART_0:
			(void)PINSEL_ConfigPin(Uart->Txd->Port, Uart->Txd->Pin, 1);
	        (void)PINSEL_ConfigPin(Uart->Rxd->Port, Uart->Rxd->Pin, 1);
		    UartIntID = BSP_INT_ID_UART0;
	    break;
		
		case UART_1:
		    (void)PINSEL_ConfigPin(Uart->Txd->Port, Uart->Txd->Pin, 2);   //管脚配置必须参考芯片手册，不同的UART的管脚配置不同
	        (void)PINSEL_ConfigPin(Uart->Rxd->Port, Uart->Rxd->Pin, 2);
		    UartIntID = BSP_INT_ID_UART1;
		break;
		
		case UART_2:
		    UartIntID = BSP_INT_ID_UART2;
	    break;
		
		case UART_3:
		    UartIntID = BSP_INT_ID_UART3;
		break;
		
		case UART_4:
		    UartIntID = BSP_INT_ID_UART4;
		break;
		
		default: break;
	}
	
	UART_Init(Uart->ID, &UARTCfg);         					//串口初始化
	UART_FIFOConfig(Uart->ID, &UARTFIFOCfg);  				//FIFO控制寄存器设置
	UART_IntConfig(Uart->ID, UART_INTCFG_RBR, ENABLE); 		//串口中断设置   接收中断
	BSP_IntEn(UartIntID);                                 	//外部中断使能
//  NVIC_SetPriority(UART1_IRQn, ((0x01<<3)|0x01));
//	NVIC_EnableIRQ(UART1_IRQn);
	UART_TxCmd(Uart->ID, ENABLE);                           //UART发送使能

	return 1;
}

/**********************************************************************
 * @brief   485总线接收或者发送
 * @param   *Uart   UART
 * @param   mode    0：接收
 * 					1：发送
 * @return	none
 *********************************************************************/
void MB_SendOrRecive(const sUART_Def *Uart, eUART_EN mode)
{
	GPIO_SetDir(Uart->DE->Port, 1<<Uart->DE->Pin , 1);
   
	if(mode == UART_TX_EN)
	{
		GPIO_SetValue(Uart->DE->Port, 1<<Uart->DE->Pin);
	}
	else if(mode == UART_RX_EN)
	{
		GPIO_ClearValue(Uart->DE->Port, 1<<Uart->DE->Pin);
	}
}

void EnterCriticalSection( void )
{
//	__SETPRIMASK();
	__disable_irq();

}

void ExitCriticalSection( void )
{
//	__RESETPRIMASK();
	__enable_irq();
}
