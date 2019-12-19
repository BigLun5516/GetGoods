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

void cmmu_reset()
{
    cmd_count = 0;
    flag_heigth = 0;
    flag_width = 0;
    flag_get = 0;
    flag_put = 0;
    flag_throw = 0;
}

// DH *.***** !
void cmmu_receive_data_height(UART_HandleTypeDef *husart)
{
    while (flag_heigth != 1)
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
            HAL_UART_Transmit(husart, cmd_buf, cmd_count, 10); //////
            HAL_Delay(200);
            HAL_UART_Transmit(husart, cmd_buf, cmd_count, 10); //////
            HAL_Delay(200);
            HAL_UART_Transmit(husart, cmd_buf, cmd_count, 10); //////
            HAL_Delay(200);
            cmd_count = 0;
            flag_heigth = 1;
        }
        else if (cmd_count == CMMU_BUF_SIZE)
        {
            cmd_count = 0;
        }
    }
}

// DW *.***** !
void cmmu_receive_data_width(UART_HandleTypeDef *husart)
{
    while (flag_width != 1)
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
            HAL_UART_Transmit(husart, cmd_buf, cmd_count, 10);
            HAL_Delay(200);
            HAL_UART_Transmit(husart, cmd_buf, cmd_count, 10);
            HAL_Delay(200);
            HAL_UART_Transmit(husart, cmd_buf, cmd_count, 10);
            HAL_Delay(200);
            cmd_count = 0;
            flag_width = 1;
        }
        else if (cmd_count == CMMU_BUF_SIZE)
        {
            cmd_count = 0;
        }
    }
}

// CGet@
void cmmu_receive_cmd_get(UART_HandleTypeDef *husart)
{
    memset(cmd_buf, 48, CMMU_BUF_SIZE);
    while (flag_get != 1)
    {
        HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
        cmd_count++;
        if (strstr(cmd_buf, "CGet@"))
        {
            HAL_UART_Transmit(husart, "CGet@", 5, 1000);
            HAL_UART_Transmit(husart, "CGet@", 5, 1000);
            HAL_UART_Transmit(husart, "CGet@", 5, 1000);
            cmd_count = 0;
            flag_get = 1;
        }
        else if (cmd_count >= CMMU_BUF_SIZE)
        {
            cmd_count = 0;
            memset(cmd_buf, 48, CMMU_BUF_SIZE);
        }
    }
}

void cmmu_receive_cmd_put(UART_HandleTypeDef *husart)
{
    memset(cmd_buf, 48, CMMU_BUF_SIZE);
    while (flag_put != 1)
    {
        HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
        cmd_count++;
        if (strstr(cmd_buf, "CPut@"))
        {
            HAL_UART_Transmit(husart, "CPut@", 5, 1000);
            HAL_UART_Transmit(husart, "CPut@", 5, 1000);
            HAL_UART_Transmit(husart, "CPut@", 5, 1000);
            cmd_count = 0;
            flag_put = 1;
        }
        else if (cmd_count >= CMMU_BUF_SIZE)
        {
            memset(cmd_buf, 48, CMMU_BUF_SIZE);
            cmd_count = 0;
        }
    }
}

void cmmu_receive_cmd_throw(UART_HandleTypeDef *husart)
{
    memset(cmd_buf, 48, CMMU_BUF_SIZE);
    while (flag_throw != 1)
    {
        HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
        cmd_count++;
        if (strstr(cmd_buf, "CThw@"))
        {
            cmd_buf[cmd_count] = '\0';
            HAL_UART_Transmit(husart, "CThw@", 5, 1000);
            HAL_UART_Transmit(husart, "CThw@", 5, 1000);
            HAL_UART_Transmit(husart, "CThw@", 5, 1000);
            cmd_count = 0;
            flag_throw = 1;
        }
        else if (cmd_count >= CMMU_BUF_SIZE)
        {
            memset(cmd_buf, 48, CMMU_BUF_SIZE);
            cmd_count = 0;
        }
    }
}