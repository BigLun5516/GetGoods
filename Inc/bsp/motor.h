#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"

#define MOTOR_USARTx                                 USART3
#define MOTOR_USARTx_BAUDRATE                        115200
#define MOTOR_USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART3_CLK_ENABLE()
#define MOTOR_USART_RCC_CLK_DISABLE()                __HAL_RCC_USART3_CLK_DISABLE()

#define MOTOR_USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define MOTOR_USARTx_Tx_GPIO_PIN                     GPIO_PIN_10
#define MOTOR_USARTx_Tx_GPIO                         GPIOB
#define MOTOR_USARTx_Rx_GPIO_PIN                     GPIO_PIN_11
#define MOTOR_USARTx_Rx_GPIO                         GPIOB

#define MOTOR_USARTx_IRQHANDLER                      USART3_IRQHandler
#define MOTOR_USARTx_IRQn                            USART3_IRQn


/* 扩展变量 ------------------------------------------------------------------*/
extern UART_HandleTypeDef husart_motor;

extern uint8_t reset_cmd[10];
extern uint8_t velocity_mode_cmd[10];
extern uint8_t stop_cmd[10];
extern uint8_t receive_info_cmd[10];
extern uint8_t stop_receive_info_cmd[10];
extern uint8_t check_online_cmd[10];
extern uint8_t set_velocity_cmd[10];

/* 函数声明 ------------------------------------------------------------------*/
void MOTOR_USARTx_Init(void);

void motor_reset();
void motor_entry_velocity_mode();
void motor_set_velocity(int16_t);
void motor_stop();
void motor_recieve_info();
void motor_stop_recieve_info();
 

#endif