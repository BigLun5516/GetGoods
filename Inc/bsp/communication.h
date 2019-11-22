#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "stm32f1xx_hal.h"

extern uint8_t cmd_buf[100]; 
extern uint8_t cmd_count = 0;
extern uint8_t flag_heigth = 0;
extern uint8_t flag_width = 0;
extern uint8_t flag_get = 0;
extern uint8_t flag_put = 0;
extern uint8_t flag_throw = 0;
extern double height;  
extern double width;    

void cmmu_receive_data_height(UART_HandleTypeDef* husart);
void cmmu_receive_data_width(UART_HandleTypeDef* husart);
void cmmu_receive_cmd_get(UART_HandleTypeDef* husart);
void cmmu_receive_cmd_put(UART_HandleTypeDef* husart);
void cmmu_receive_cmd_throw(UART_HandleTypeDef* husart);

#endif