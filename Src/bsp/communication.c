#include "communication.h"

uint8_t cmd_buf[100];
uint8_t cmd_count = 0;
uint8_t flag_heigth = 0;
uint8_t flag_width = 0;
uint8_t flag_get = 0;
uint8_t flag_put = 0;
uint8_t flag_throw = 0;
double height;
double width;

char *end_; // 用于函数 strtod(char*, char**);

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

void cmmu_receive_cmd_get(UART_HandleTypeDef *husart)
{
    HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
    cmd_count++;
    if (cmd_count == 1 && cmd_buf[0] != 'C')
    {
        cmd_count = 0;
    }
    else if (cmd_count == 2 && cmd_buf[1] != 'G')
    {
        cmd_count = 0;
    }
    else if (cmd_count == 3 && cmd_buf[2] != 'e')
    {
        cmd_count = 0;
    }
    else if (cmd_count == 4 && cmd_buf[3] != 't')
    {
        cmd_count = 0;
    }
    else if (cmd_count == 5 && cmd_buf[4] != '@')
    {
        cmd_count = 0;
    }
    else if (cmd_count == 5)
    {
        cmd_buf[cmd_count] = '\0';
        while (laser_send_cmd(husart, cmd_buf, "OK", 100))
            ;
        cmd_count = 0;
        flag_get = 1;
    }
}

void cmmu_receive_cmd_put(UART_HandleTypeDef* husart){
    HAL_UART_Receive(husart, &cmd_buf[cmd_count], 1, 10);
            cmd_count++;
            if (cmd_count == 1 && cmd_buf[0] != 'C')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 2 && cmd_buf[1] != 'P')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 3 && cmd_buf[2] != 'u')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 4 && cmd_buf[3] != 't')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 5 && cmd_buf[4] != '@')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 5)
            {
                cmd_buf[cmd_count] = '\0';
                while (laser_send_cmd(husart, cmd_buf, "OK", 100))
                    ;
                cmd_count = 0;
                flag_put = 1;
            }
}

void cmmu_receive_cmd_throw(UART_HandleTypeDef* husart){
    HAL_UART_Receive(&husart_debug, &cmd_buf[cmd_count], 1, 10);
            cmd_count++;
            if (cmd_count == 1 && cmd_buf[0] != 'C')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 2 && cmd_buf[1] != 'T')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 3 && cmd_buf[2] != 'h')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 4 && cmd_buf[3] != 'w')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 5 && cmd_buf[4] != '@')
            {
                cmd_count = 0;
            }
            else if (cmd_count == 5)
            {
                cmd_buf[cmd_count] = '\0';
                while (laser_send_cmd(&husart_debug, cmd_buf, "OK", 100))
                    ;
                cmd_count = 0;
                flag_throw = 1;
            }
}