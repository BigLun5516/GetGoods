#include "laser.h"

UART_HandleTypeDef husart_laser1;
UART_HandleTypeDef husart_laser2;

float distance_laser1;
float distance_laser2;

uint8_t buf_laser1[10];
uint8_t buf_laser2[10];

uint8_t flag1_laser1;
uint8_t flag2_laser1;
uint8_t flag1_laser2;
uint8_t flag2_laser2;

uint8_t count_laser1;
uint8_t count_laser2;

void LASER_Init()
{
    flag1_laser1 = 0;
    flag2_laser1 = 0;
    flag1_laser2 = 0;
    flag2_laser2 = 0;
    count_laser1 = 0;
    count_laser2 = 0;

    // 激光1 串口初始化
    husart_laser1.Instance = LASER1_USARTx;
    husart_laser1.Init.BaudRate = LASER1_USARTx_BAUDRATE;
    husart_laser1.Init.WordLength = UART_WORDLENGTH_8B;
    husart_laser1.Init.StopBits = UART_STOPBITS_1;
    husart_laser1.Init.Parity = UART_PARITY_NONE;
    husart_laser1.Init.Mode = UART_MODE_TX_RX;
    husart_laser1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart_laser1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&husart_laser1);

    // 激光1 配置串口中断并使能
    HAL_NVIC_SetPriority(LASER1_USART_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(LASER1_USART_IRQn);

    // 激光2 串口初始化
    husart_laser2.Instance = LASER2_USARTx;
    husart_laser2.Init.BaudRate = LASER2_USARTx_BAUDRATE;
    husart_laser2.Init.WordLength = UART_WORDLENGTH_8B;
    husart_laser2.Init.StopBits = UART_STOPBITS_1;
    husart_laser2.Init.Parity = UART_PARITY_NONE;
    husart_laser2.Init.Mode = UART_MODE_TX_RX;
    husart_laser2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart_laser2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&husart_laser2);

    // 激光2 配置串口中断并使能
    HAL_NVIC_SetPriority(LASER2_USART_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(LASER2_USART_IRQn);
}

float getDistance(uint8_t LASER)
{
    if (LASER == LASER1)
    {
        return distance_laser1;
    }
    else if (LASER == LASER2)
    {
        return distance_laser2;
    }
    else
    {
        return -1;
    }
}