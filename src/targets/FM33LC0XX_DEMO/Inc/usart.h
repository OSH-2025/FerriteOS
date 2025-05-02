#ifndef __USER_UART_H__
#define __USER_UART_H__

#include "main.h"
#include "los_typedef.h"
#include "uart.h"

typedef struct{
	uint8_t *TxBuf;	// Tx BUffer
	uint8_t TxLen;	// Tx Lenght
	uint8_t TxOpc;	// Tx Opcodes
	uint8_t *RxBuf;	// Rx BUffer
	uint8_t RxLen;	// Tx Lenght
	uint8_t RxOpc;	// Tx Opcodes
}tUARTx_OPT;

extern UART_Type* gUARTx_Print;
extern UartControllerOps g_genericUart;

extern void Uartx_Init(UART_Type* UARTx, uint32_t rate);
extern void UartxPrint_Init(UART_Type* UARTx);
extern int HAL_UART_Transmit(UART_Type *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
extern int HAL_UART_Receive(UART_Type *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif
