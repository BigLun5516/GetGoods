#include "motor.h"
#include "usart/bsp_debug_usart.h"

UART_HandleTypeDef husart_motor;

/* 扩展变量 ------------------------------------------------------------------*/
uint8_t reset_cmd[10] = {0x23, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};         // 复位
uint8_t velocity_mode_cmd[10] = {0x23, 0x01, 0x03, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}; // 选择速度模式
// uint8_t velocity_position_mode_cmd[10] = { 0x23 ,0x01 ,0x05 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 };
uint8_t stop_cmd[10] = {0x23, 0x04, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55}; // 断电 且 速度设为0

// 配置指令
uint8_t receive_info_cmd[10] = {0x23, 0x0A, 0xFF, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}; //第三位是发送的周期，单位为mm，我们这里选最大256
uint8_t stop_receive_info_cmd[10] = {0x23, 0x0A, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
uint8_t check_online_cmd[10] = {0x23, 0x0F, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};

uint8_t set_velocity_cmd[10] = {0x23, 0x04, 0x13, 0x88, 0x01, 0xF4, 0x55, 0x55, 0x55, 0x55}; // 速度设为500

// uint8_t set_velocity_position_cmd[10] = {0x23 ,0x06 ,0x13 ,0x88 ,0x02 ,0xEE ,0x00 ,0x00 ,0x3E ,0x80	};

// 电机复位
void motor_reset()
{
    HAL_UART_Transmit(&husart_motor, reset_cmd, 10, 100);
    HAL_UART_Transmit(&husart_debug, reset_cmd, 10, 1000);//////
}

// 选择速度模式
void motor_entry_velocity_mode()
{
    HAL_UART_Transmit(&husart_motor, velocity_mode_cmd, 10, 100);
    HAL_UART_Transmit(&husart_debug, velocity_mode_cmd, 10, 1000);//////
}

// 设置速度
void motor_set_velocity(int16_t velocity)
{
    set_velocity_cmd[4] = (velocity >> 8) & 0xff; // 防止移位是算数移位 从而导致高8位为1
    set_velocity_cmd[5] = velocity & 0xff;
    HAL_UART_Transmit(&husart_motor, set_velocity_cmd, 10, 100);
    HAL_UART_Transmit(&husart_debug, set_velocity_cmd, 10, 1000);//////
}

// 电机停止
void motor_stop()
{
    // 断电 速度设为0
    HAL_UART_Transmit(&husart_motor, stop_cmd, 10, 100);
    HAL_UART_Transmit(&husart_debug, stop_cmd, 10, 1000);//////
}

// 配置电机接收速度、位置、电流反馈信息
void motor_recieve_info()
{
    HAL_UART_Transmit(&husart_motor, receive_info_cmd, 10, 100);
}

// 电机停止接收反馈信息
void motor_stop_recieve_info()
{
    HAL_UART_Transmit(&husart_motor, stop_receive_info_cmd, 10, 100);
}


/**
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void MOTOR_USARTx_Init(void)
{
    /* 使能串口功能引脚GPIO时钟 */
    MOTOR_USARTx_GPIO_ClK_ENABLE();

    husart_motor.Instance = MOTOR_USARTx;
    husart_motor.Init.BaudRate = MOTOR_USARTx_BAUDRATE;
    husart_motor.Init.WordLength = UART_WORDLENGTH_8B;
    husart_motor.Init.StopBits = UART_STOPBITS_1;
    husart_motor.Init.Parity = UART_PARITY_NONE;
    husart_motor.Init.Mode = UART_MODE_TX_RX;
    husart_motor.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart_motor.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&husart_motor);

    /* 配置串口中断并使能，需要放在HAL_UART_Init函数后执行修改才有效 */
    HAL_NVIC_SetPriority(MOTOR_USARTx_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(MOTOR_USARTx_IRQn);
}
