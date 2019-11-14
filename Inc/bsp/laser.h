#ifndef __LASER1_H
#define __LASER1_H

#include "stm32f1xx_hal.h"

#define LASER1 1
#define LASER2 2

// 激光1
#define LASER1_USARTx                                 USART2
#define LASER1_USARTx_BAUDRATE                        115200
#define LASER1_USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART2_CLK_ENABLE()
#define LASER1_USART_RCC_CLK_DISABLE()                __HAL_RCC_USART2_CLK_DISABLE()

#define LASER1_USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define LASER1_USARTx_Tx_GPIO_PIN                     GPIO_PIN_2
#define LASER1_USARTx_Tx_GPIO                         GPIOA
#define LASER1_USARTx_Rx_GPIO_PIN                     GPIO_PIN_3
#define LASER1_USARTx_Rx_GPIO                         GPIOA

#define LASER1_USARTx_IRQHANDLER                      USART2_IRQHandler
#define LASER1_USART_IRQn                             USART2_IRQn

// 激光2
#define LASER2_USARTx                                 UART4
#define LASER2_USARTx_BAUDRATE                        115200
#define LASER2_USART_RCC_CLK_ENABLE()                 __HAL_RCC_UART4_CLK_ENABLE()
#define LASER2_USART_RCC_CLK_DISABLE()                __HAL_RCC_UART4_CLK_DISABLE()

#define LASER2_USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define LASER2_USARTx_Tx_GPIO_PIN                     GPIO_PIN_10
#define LASER2_USARTx_Tx_GPIO                         GPIOC
#define LASER2_USARTx_Rx_GPIO_PIN                     GPIO_PIN_11
#define LASER2_USARTx_Rx_GPIO                         GPIOC

#define LASER2_USARTx_IRQHANDLER                      UART4_IRQHandler
#define LASER2_USART_IRQn                             UART4_IRQn




extern UART_HandleTypeDef husart_laser1; // 串口2
extern UART_HandleTypeDef husart_laser2; // 串口4

extern float distance_laser1;
extern float distance_laser2;

extern uint8_t buf_laser1[10];
extern uint8_t buf_laser2[10];

extern uint8_t flag1_laser1;
extern uint8_t flag2_laser1;
extern uint8_t flag1_laser2;
extern uint8_t flag2_laser2;

extern uint8_t count_laser1;
extern uint8_t count_laser2;

void LASER_Init();

float getDistance(uint8_t LASER);

#endif