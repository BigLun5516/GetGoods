#ifndef __BSP_DEBUG_USART_H__
#define __BSP_DEBUG_USART_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "laser.h"
#include "motor.h"
#include <stdio.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define DEBUG_USARTx                                 USART1
#define DEBUG_USARTx_BAUDRATE                        115200
#define DEBUG_USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE()
#define DEBUG_USART_RCC_CLK_DISABLE()                __HAL_RCC_USART1_CLK_DISABLE()

#define DEBUG_USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USARTx_Tx_GPIO_PIN                     GPIO_PIN_9
#define DEBUG_USARTx_Tx_GPIO                         GPIOA
#define DEBUG_USARTx_Rx_GPIO_PIN                     GPIO_PIN_10
#define DEBUG_USARTx_Rx_GPIO                         GPIOA

#define USARTx_IRQHANDLER                            USART1_IRQHandler
#define DEBUG_USART_IRQn                             USART1_IRQn


/*-----------------------------------------------------------*/
#define RS485_USARTx                                 USART3
#define RS485_USARTx_BAUDRATE                        115200
#define RS485_USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART3_CLK_ENABLE()
#define RS485_USART_RCC_CLK_DISABLE()                __HAL_RCC_USART3_CLK_DISABLE()

#define RS485_USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define RS485_USARTx_PORT                            GPIOB
#define RS485_USARTx_Tx_PIN                          GPIO_PIN_10
#define RS485_USARTx_Rx_PIN                          GPIO_PIN_11

#define RS485_IRQHANDLER                            USART3_IRQHandler
#define RS485_USARTX_IRQn                            USART3_IRQn


// ����DMA���
#define RS485_DMAx_CHANNELn                          DMA1_Channel3
#define RS485_RCC_DMAx_CLK_ENABLE()                  __HAL_RCC_DMA1_CLK_ENABLE()
#define RS485_DMAx_CHANNELn_IRQn                     DMA1_Channel3_IRQn
#define RS485_DMAx_CHANNELn_IRQHANDLER               DMA1_Channel3_IRQHandler

/* ��չ���� ------------------------------------------------------------------*/


/* �������� ------------------------------------------------------------------*/



/* ��չ���� ------------------------------------------------------------------*/
extern UART_HandleTypeDef husart_debug;
extern DMA_HandleTypeDef hdma_debug_rx;

extern DMA_HandleTypeDef hdma_rs485_rx;
extern UART_HandleTypeDef husartx_rs485;
/* �������� ------------------------------------------------------------------*/
void MX_DEBUG_USART_Init(void);
void RS485_USARTx_Init(void);


#endif  /* __BSP_DEBUG_USART_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
