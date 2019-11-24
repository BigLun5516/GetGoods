#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "stm32f1xx_hal.h"

extern uint8_t cmd_buf[100]; 
extern uint8_t cmd_count;
extern uint8_t flag_heigth;
extern uint8_t flag_width;
extern uint8_t flag_get;
extern uint8_t flag_put;
extern uint8_t flag_throw;
extern double height;  
extern double width;    

void cmmu_receive_data_height(UART_HandleTypeDef* husart);
void cmmu_receive_data_width(UART_HandleTypeDef* husart);
void cmmu_receive_cmd_get(UART_HandleTypeDef* husart);
void cmmu_receive_cmd_put(UART_HandleTypeDef* husart);
void cmmu_receive_cmd_throw(UART_HandleTypeDef* husart);

#endif