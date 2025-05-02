#include "usart.h"
#include "los_hwi.h"
#include "platform.h"

UART_Type* gUARTx_Print;

void Uartx_Init(UART_Type* UARTx, uint32_t rate)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_UART_InitTypeDef UART_InitStruct = {0};

    UART_InitStruct.TransferDirection = LL_UART_DIRECTION_TX_RX;

    switch((uint32_t)UARTx)
    {
        case UART0_BASE:
            //PA13:UART0-RX   PA14:UART0-TX
//            GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
//            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
//            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//            GPIO_InitStruct.Pull = DISABLE;
//            GPIO_InitStruct.RemapPin = DISABLE;
//            LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

            //PA2:UART0-RX   PA3:UART0-TX
            GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = DISABLE;
            GPIO_InitStruct.RemapPin = DISABLE;
            LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

            UART_InitStruct.ClockSrc = LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;
            break;

        case UART1_BASE:
            //PB13:UART1-RX   PB14:UART1-TX
            GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = DISABLE;
            GPIO_InitStruct.RemapPin = DISABLE;
            LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

            //PC2:UART1-RX   PC3:UART1-TX
//            GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
//            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
//            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//            GPIO_InitStruct.Pull = DISABLE;
//            GPIO_InitStruct.RemapPin = DISABLE;
//            LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

            UART_InitStruct.ClockSrc = LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;
            break;

        case UART4_BASE:
            //PB2:UART4-RX   PB3:UART4-TX
            GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = DISABLE;
            GPIO_InitStruct.RemapPin = DISABLE;
            LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

            //PA0:UART4-RX   PA1:UART4-TX
//            GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
//            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
//            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//            GPIO_InitStruct.Pull = DISABLE;
//            GPIO_InitStruct.RemapPin = DISABLE;
//            LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            break;

        case UART5_BASE:
            //PD0:UART5-RX   PD1:UART5-TX
            GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = DISABLE;
            GPIO_InitStruct.RemapPin = DISABLE;
            LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
            // UART_InitStruct.TransferDirection = LL_UART_DIRECTION_TX;

            //PC4:UART5-RX   PC5:UART5-TX
//            GPIO_InitStruct.Pin = LL_GPIO_PIN_4|LL_GPIO_PIN_5;
//            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
//            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//            GPIO_InitStruct.Pull = DISABLE;
//            GPIO_InitStruct.RemapPin = DISABLE;
//            LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
            break;

        default:
            break;
    }
    UART_InitStruct.BaudRate = rate;
    UART_InitStruct.DataWidth = LL_UART_DATAWIDTH_8B;
    UART_InitStruct.StopBits = LL_UART_STOPBITS_1;
    UART_InitStruct.Parity = LL_UART_PARITY_NONE;
//    UART_InitStruct.TransferDirection = LL_UART_DIRECTION_TX_RX;
    UART_InitStruct.InfraredModulation = DISABLE;
    LL_UART_Init(UARTx, &UART_InitStruct);
}

void UartxPrint_Init(UART_Type* UARTx)
{
    gUARTx_Print = UARTx;
    Uartx_Init(UARTx, 115200);
}


int HAL_UART_Transmit(UART_Type *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint16_t i;

    (void)Timeout;

    for (i = 0; i < Size; i++) {
        LL_UART_TransmitData(huart, pData[i]);              // load data to reg
        while(SET != LL_UART_IsActiveFlag_TXSE(huart));     // wait for send
    }
    return 0;
}

int HAL_UART_Receive(UART_Type *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint16_t i;

    (void)Timeout;

    if(!pData)    return 1;

    for(i = 0; i < Size; i++){
        while (SET != LL_UART_IsActiveFlag_RXBF(huart));
        *pData++ =  (int)LL_UART_ReceiveData(huart);
    }
    return 0;
}

int fputc(int ch, FILE *f)
{
    LL_UART_TransmitData(gUARTx_Print, ch);
    while(SET != LL_UART_IsActiveFlag_TXSE(gUARTx_Print));
    return (ch);
}

int fgetc(FILE *f)
{
    while (SET != LL_UART_IsActiveFlag_RXBF(gUARTx_Print));
    return (int)LL_UART_ReceiveData(gUARTx_Print);
}

__attribute__((used)) int _write(int fd, char *ptr, int len)
{
    return HAL_UART_Transmit(gUARTx_Print, (void*)ptr, len, 0);
}

VOID UsartInit(VOID) {
    UartxPrint_Init(UART5);
}

VOID UsartWrite(const CHAR c)
{
    HAL_UART_Transmit(UART5, (UINT8 *)&c, 1, DEFAULT_TIMEOUT);
}

UINT8 UsartRead(VOID)
{
    UINT8 ch;
    (VOID)HAL_UART_Receive(UART5, &ch, sizeof(UINT8), 0);
    return ch;
}

STATIC VOID UartHandler(VOID)
{
    (VOID)uart_getc();
}

INT32 UsartHwi(VOID)
{
    NVIC_DisableIRQ(UART5_IRQn);
    NVIC_SetPriority(UART5_IRQn, 1);
    NVIC_EnableIRQ(UART5_IRQn);
    LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 1, 0, UartHandler, NULL);
    LOS_HwiEnable(NUM_HAL_INTERRUPT_UART);
    return LOS_OK;
}

UartControllerOps g_genericUart = {
    .uartInit = UsartInit,
    .uartWriteChar = UsartWrite,
    .uartReadChar = UsartRead,
    .uartHwiCreate = UsartHwi
};
