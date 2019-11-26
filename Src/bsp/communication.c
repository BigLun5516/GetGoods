#include "communication.h"
#include <stdlib.h>
#include <string.h>

uint8_t cmd_buf[CMMU_BUF_SIZE];
uint8_t cmd_count = 0;
uint8_t flag_heigth = 0;
uint8_t flag_width = 0;
uint8_t flag_get = 0;
uint8_t flag_put = 0;
uint8_t flag_throw = 0;
double height;
double width;

char *end_; // 用于函数 strtod(char*, char**);


// DH *.***** !
void cmmu_receive_data_height(UART_HandleTypeDef *husart)
{
    HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
    cmd_count++;
    if (cmd_count == 1 && cmd_buf[0] != 'D')
    {
        cmd_count = 0;
    }
    else if (cmd_count == 2 && cmd_buf[1] != 'H')
    {
        cmd_count = 0;
    }
    else if (cmd_buf[cmd_count - 1] == '!')
    {
        height = strtod(cmd_buf + 2, &end_);
        cmd_buf[cmd_count] = '\0';
        while (laser_send_cmd(husart, cmd_buf, "OK", 100))
            ;
        cmd_count = 0;
        flag_heigth = 1;
    }
}

// DW *.***** !
void cmmu_receive_data_width(UART_HandleTypeDef *husart)
{
    HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
    cmd_count++;
    if (cmd_count == 1 && cmd_buf[0] != 'D')
    {
        cmd_count = 0;
    }
    else if (cmd_count == 2 && cmd_buf[1] != 'W')
    {
        cmd_count = 0;
    }
    else if (cmd_buf[cmd_count - 1] == '!')
    {
        width = strtod(cmd_buf + 2, &end_);
        cmd_buf[cmd_count] = '\0';
        while (laser_send_cmd(husart, cmd_buf, "OK", 100))
            ;
        cmd_count = 0;
        flag_width = 1;
    }
}

// CGet@
void cmmu_receive_cmd_get(UART_HandleTypeDef *husart)
{
    HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
    cmd_count++;
    if(strstr(cmd_buf, "CGet@")){
        while (laser_send_cmd(husart, "CGet@", "OK", 100))
            ;
        cmd_count = 0;
        flag_get = 1;
    }else if(cmd_count >= CMMU_BUF_SIZE){
        cmd_count = 0;
    }
}

void cmmu_receive_cmd_put(UART_HandleTypeDef *husart)
{
    HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
    cmd_count++;
    if(strstr(cmd_buf, "CPut@")){
        while (laser_send_cmd(husart, "CPut@", "OK", 100))
            ;
        cmd_count = 0;
        flag_put = 1;
    }else if(cmd_count >= CMMU_BUF_SIZE){
        cmd_count = 0;
    }
}

void cmmu_receive_cmd_throw(UART_HandleTypeDef *husart)
{
    HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
    cmd_count++;
    if(strstr(cmd_buf, "CThw@")){
        cmd_buf[cmd_count] = '\0';
        while (laser_send_cmd(husart, "CThw@", "OK", 100))
            ;
        cmd_count = 0;
        flag_throw = 1;
    }else if(cmd_count >= CMMU_BUF_SIZE){
        cmd_count = 0;
    }
}