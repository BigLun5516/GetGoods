/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "usart/bsp_debug_usart.h"
#include "string.h"
#include "ASDA_B2/bsp_ASDA_B2.h"
#include "relay.h"
#include "stdlib.h"
#include "pid.h"
#include <math.h>
#include "communication.h"
#include "key.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
__IO uint8_t Rx_Buf[50]; //接收数据缓存

uint8_t aRxBuffer_debug;
uint8_t aRxBuffer_laser1;
uint8_t aRxBuffer_laser2;
uint8_t aRxBuffer_motor;

char *end; // 用于函数 strtod(char*, char**);

#define filter 0.2
float setSpd = 0;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 系统时钟配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE; // 外部晶振，8MHz
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9; // 9倍频，得到72MHz主时钟
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // 系统时钟：72MHz
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        // AHB时钟：72MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;         // APB1时钟：36MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         // APB2时钟：72MHz
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

    // HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
    // HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
    // HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000); // 配置并启动系统滴答定时器
    /* 系统滴答定时器时钟源 */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    /* 系统滴答定时器中断优先级配置 */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{
    uint8_t i = 0, test = 0;
    int8_t dir = 1;
    /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
    HAL_Init();
    /* 配置系统时钟 */
    SystemClock_Config();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 继电器测试
    RELAY_Init();
    // 24 全关：缩
    // 24 全开：申
    // DCT_ON;
    // push_rod_extend();
    // push_rod_back();
    // // DCT_OFF;
    // while(1);

    // /* 初始化串口并配置串口中断优先级 */
    MX_DEBUG_USART_Init();

    //////////////////////
    // 开关测试
    key_init();
    // while(KEY_LEVEL != KEY_ON_LEVEL);
    // RELAY5_ON;
    // while(1);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //激光测试
    LASER_Init();
    HAL_UART_Receive_IT(&husart_laser1, &aRxBuffer_laser1, 1); // 激光1的串口 开中断
    HAL_UART_Receive_IT(&husart_laser2, &aRxBuffer_laser2, 1); // 激光2的串口 开中断
    HAL_Delay(500);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 大电机 线性模组
    RS485_USARTx_Init();

    HAL_UART_Receive_DMA(&husartx_rs485, &test, 1); // Data Direction: 485 --> USART1

    /* Disable the Half transfer complete interrupt */
    __HAL_DMA_DISABLE_IT(&hdma_rs485_rx, DMA_IT_HT);
    __HAL_DMA_DISABLE_IT(&hdma_rs485_rx, DMA_IT_TE);

    /* 初始化ASDA-B2参数,配置为速度模式*/
    ASDAB2_Init();
    /* 设置SP3速度值为0*0.1r/min */
    SetSpeed(REG_SP3, 0);
    /* 启动伺服 */
    StartServo();
    HAL_Delay(2000); // 两秒后启动

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 小电机测试
    MOTOR_USARTx_Init();
    HAL_Delay(1000);
    motor_reset();
    HAL_Delay(500);
    motor_reset();
    HAL_Delay(500);
    motor_reset();
    HAL_Delay(500);
    motor_entry_velocity_mode();
    HAL_Delay(500);

    while (laser_send_cmd(&husart_debug, "CRdyOK@", "CRdyOK@", 100))
        ;

    /* 无限循环 */
    while (1)
    {
        // 接收 物品高度
        while (!flag_height)
        {
            cmmu_receive_data_height(&husart_debug);
        }

        // 接收 托盘宽度
        while (!flag_width)
        {
            cmmu_receive_data_width(&husart_debug);
        }

        // 接收 开始取货的命令
        while (!flag_get)
        {
            cmmu_receive_cmd_get(&husart_debug);
        }

        // 移动到 物品高度
        static uint8_t flag_height_speed = 0; ////
        while (1)
        {
            distance_filter_laser1 = distance_laser1 * (1 - filter) + last_distance_laser1 * filter;
            if ((distance_filter_laser1 - height) > crtDistance) //控制策略选择
            {
                setSpd = -ratedSpd1;
                flag_height_speed = 1;
            }
            else if ((distance_filter_laser1 - height) < (-1 * crtDistance)) //控制策略选择
            {
                setSpd = 1 * ratedSpd1;
                flag_height_speed = 2;
            }
            else if (fabs(distance_filter_laser1 - height) <= crtDistance)
            {
                setSpd = Motor_PID1(height, distance_filter_laser1, 0);
                flag_height_speed = 3;
            }
            SetSpeed(REG_SP3, setSpd);

            if ((fabs(distance_laser1 - height) < allowedError) &&
                (fabs(last_distance_laser1 - height) < allowedError)) //连续三帧满足误差范围要求进行下一步
            {
                SetSpeed(REG_SP3, 0); //停止电机
                setSpd = 0;
                Motor_PID1(0, 0, 1); //清除历史值
                break;
            }
            HAL_Delay(20);
        }

        // 取货  伸出取货杆
        while (1)
        {
            distance_filter_laser2 = distance_laser2 * (1 - filter) + last_distance_laser2 * filter;
            if ((distance_filter_laser2 - (init_distance_laser2 - width / 2)) > crtDistance) //控制策略选择
                setSpd = -1 * ratedSpd2;
            else if ((distance_filter_laser2 - (init_distance_laser2 - width / 2)) < (-1 * crtDistance)) //控制策略选择
                setSpd = ratedSpd2;
            else if (fabs(distance_filter_laser2 - (init_distance_laser2 - width / 2)) <= crtDistance)
                setSpd = Motor_PID2(init_distance_laser2 - width / 2, distance_filter_laser2, 0);
            motor_set_velocity(setSpd);

            if ((fabs(distance_laser2 - (init_distance_laser2 - width / 2)) < allowedError2) &&
                (fabs(last_distance_laser2 - (init_distance_laser2 - width / 2)) < allowedError2)) //连续三帧满足误差范围要求进行下一步
            {
                motor_stop(); //停止电机
                setSpd = 0;
                Motor_PID2(0, 0, 1); //清除历史值
                break;
            }
            HAL_Delay(30);
        }

        // 电磁铁开
        DCT_ON;
        HAL_Delay(400);

        // 抬4cm
        height = height + 0.04;
        while (1)
        {
            distance_filter_laser1 = distance_laser1 * (1 - filter) + last_distance_laser1 * filter;
            if ((distance_filter_laser1 - height) > crtDistance) //控制策略选择
                setSpd = -ratedSpd1;
            else if ((distance_filter_laser1 - height) < (-1 * crtDistance)) //控制策略选择
                setSpd = 1 * ratedSpd1;
            else if (fabs(distance_filter_laser1 - height) <= crtDistance)
                setSpd = Motor_PID1(height, distance_filter_laser1, 0);
            SetSpeed(REG_SP3, setSpd);

            if ((fabs(distance_laser1 - height) < allowedError) &&
                (fabs(last_distance_laser1 - height) < allowedError)) //连续三帧满足误差范围要求进行下一步
            {
                SetSpeed(REG_SP3, 0); //停止电机
                setSpd = 0;
                Motor_PID1(0, 0, 1); //清除历史值
                break;
            }
            HAL_Delay(20);
        }

        // 收回取货杆
        motor_set_velocity(ratedSpd2);
        // 等待行程开关按下
        while (KEY_LEVEL != KEY_ON_LEVEL)
            ;
        motor_stop();

        // 取完货 发个消息
        while (laser_send_cmd(&husart_debug, "CGetOK@", "CGetOK@", 100))
            ;

        // 接收 卸货的命令
        while (!flag_put && !flag_lod)
        {
            cmmu_receive_cmd_put_or_lod(&husart_debug);
        }

        if (flag_put)
        {
            // 卸货
            // 移动到 卸货高度
            height = 1;
            while (1)
            {
                distance_filter_laser1 = distance_laser1 * (1 - filter) + last_distance_laser1 * filter;
                if ((distance_filter_laser1 - height) > crtDistance) //控制策略选择
                    setSpd = -ratedSpd1;
                else if ((distance_filter_laser1 - height) < (-1 * crtDistance)) //控制策略选择
                    setSpd = 1 * ratedSpd1;
                else if (fabs(distance_filter_laser1 - height) <= crtDistance)
                    setSpd = Motor_PID1(height, distance_filter_laser1, 0);
                SetSpeed(REG_SP3, setSpd);

                if ((fabs(distance_laser1 - height) < allowedError) &&
                    (fabs(last_distance_laser1 - height) < allowedError) &&
                    (fabs(last_last_distance_laser1 - height) < allowedError)) //连续三帧满足误差范围要求进行下一步
                {
                    SetSpeed(REG_SP3, 0); //停止电机
                    setSpd = 0;
                    Motor_PID1(0, 0, 1); //清除历史值
                    break;
                }
                HAL_Delay(20);
            }
            push_rod_extend();
            HAL_Delay(1000); // 等待推杆推到底
            HAL_Delay(1000); // 等待推杆推到底
            HAL_Delay(1000); // 等待推杆推到底
            push_rod_back();

            // 卸完货 发个消息
            while (laser_send_cmd(&husart_debug, "CPutOK@", "CPutOK@", 100))
                ;

            // 接收 扔掉托盘的命令
            while (!flag_throw)
            {
                cmmu_receive_cmd_throw(&husart_debug);
            }

            // 扔掉托盘
            DCT_OFF; // 电磁铁关

            // 扔完货 发个消息
            while (laser_send_cmd(&husart_debug, "CThwOK@", "CThwOK@", 100))
                ;
        }
        else
        {
            // 上货
            flag_height = 0;
            cmmu_receive_data_height(&husart_debug);

            height += 0.04; //取货杆上抬一点点

            motor_set_velocity(-1 * ratedSpd2); // 伸出取货杆

            HAL_Delay(500);

            motor_stop();

            // 关闭电磁铁
            DCT_OFF;

            // 取货杆下降一些
            height -= 0.06;
            while (1)
            {
                distance_filter_laser1 = distance_laser1 * (1 - filter) + last_distance_laser1 * filter;
                if ((distance_filter_laser1 - height) > crtDistance) //控制策略选择
                    setSpd = -ratedSpd1;
                else if ((distance_filter_laser1 - height) < (-1 * crtDistance)) //控制策略选择
                    setSpd = 1 * ratedSpd1;
                else if (fabs(distance_filter_laser1 - height) <= crtDistance)
                    setSpd = Motor_PID1(height, distance_filter_laser1, 0);
                SetSpeed(REG_SP3, setSpd);

                if ((fabs(distance_laser1 - height) < allowedError) &&
                    (fabs(last_distance_laser1 - height) < allowedError) &&
                    (fabs(last_last_distance_laser1 - height) < allowedError)) //连续三帧满足误差范围要求进行下一步
                {
                    SetSpeed(REG_SP3, 0); //停止电机
                    setSpd = 0;
                    Motor_PID1(0, 0, 1); //清除历史值
                    break;
                }
                HAL_Delay(20);
            }

            // 收回取货杆
            motor_set_velocity(ratedSpd2);
            while (KEY_LEVEL != KEY_ON_LEVEL) // 等待行程开关按下
                ;
            motor_stop();

            // 告知主控上货完成
            while (laser_send_cmd(&husart_debug, "CLodOK@", "CLodOK@", 100))
                ;
        }
        cmmu_reset();
    }
}

/**
  * 函数功能: 串口接收回调函数
  * 输入参数: UARTHandle:串口句柄
  * 返 回 值: 无
  * 说    明: 接收到从机的反馈数据之后,分包并存放到Rx_Buf里面,一次只能接收一帧数据
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    static uint8_t i = 0;
    if (UartHandle->Instance == DEBUG_USARTx)
    {
        HAL_UART_Transmit(&husart_debug, &aRxBuffer_debug, 1, 0);
        HAL_UART_Receive_IT(&husart_debug, &aRxBuffer_debug, 1);
    }
    if (UartHandle->Instance == RS485_USARTx)
    {
        Rx_Buf[i] = husartx_rs485.Instance->DR;
        if (Rx_Buf[0] == ':')
        {
            i++;
            if (Rx_Buf[i - 1] == 0x0A)
            {
                if (Rx_Buf[i - 2] == 0x0D)
                {
                    Rx_Buf[i] = '\0';   // 手动添加结束符
                    UsartState = Ready; // 接收完成,通讯待机
                    i = 0;
                }
            }
        }
        else
            i = 0;
    }

    // 激光1
    if (UartHandle->Instance == LASER1_USARTx)
    {
        if (flag1_laser1 == 0)
        {
            if (aRxBuffer_laser1 == 'D')
            {
                flag1_laser1 = 1;
            }
        }
        else if (flag1_laser1 == 1 && flag2_laser1 == 0)
        {
            if (aRxBuffer_laser1 == '=')
            {
                flag2_laser1 = 1;
            }
            else
            {
                flag1_laser1 = 0;
            }
        }
        else if (flag1_laser1 == 1 && flag2_laser1 == 1 && aRxBuffer_laser1 == 'm')
        {
            flag1_laser1 = flag2_laser1 = 0;

            // // 发给串口1 测试用
            // HAL_UART_Transmit(&husart_debug, buf_laser1, count_laser1, 1000);
            // HAL_UART_Transmit(&husart_debug, "\r\n", 2, 1000);

            last_last_distance_laser1 = last_distance_laser1;
            last_distance_laser1 = distance_laser1;
            distance_laser1 = strtod(buf_laser1, &end);
            count_laser1 = 0;
        }
        else if (count_laser1 > 5)
        {
            count_laser1 = flag1_laser1 = flag2_laser1 = 0;
        }
        else
        {
            buf_laser1[count_laser1++] = aRxBuffer_laser1;
        }
        HAL_UART_Receive_IT(&husart_laser1, &aRxBuffer_laser1, 1);
    }

    // 激光2
    if (UartHandle->Instance == LASER2_USARTx)
    {
        if (flag1_laser2 == 0)
        {
            if (aRxBuffer_laser2 == 'D')
            {
                flag1_laser2 = 1;
            }
        }
        else if (flag1_laser2 == 1 && flag2_laser2 == 0)
        {
            if (aRxBuffer_laser2 == '=')
            {
                flag2_laser2 = 1;
            }
            else
            {
                flag1_laser2 = 0;
            }
        }
        else if (flag1_laser2 == 1 && flag2_laser2 == 1 && aRxBuffer_laser2 == 'm')
        {
            flag1_laser2 = flag2_laser2 = 0;

            // // 发给串口1 测试用
            // HAL_UART_Transmit(&husart_debug, buf_laser2, count_laser2, 1000);
            // HAL_UART_Transmit(&husart_debug, "\r\n", 2, 1000);

            last_last_distance_laser2 = last_distance_laser2;
            last_distance_laser2 = distance_laser2;
            distance_laser2 = strtod(buf_laser2, &end);
            count_laser2 = 0;
        }
        else if (count_laser2 > 5)
        {
            count_laser2 = flag1_laser2 = flag2_laser2 = 0;
        }
        else
        {
            buf_laser2[count_laser2++] = aRxBuffer_laser2;
        }
        HAL_UART_Receive_IT(&husart_laser2, &aRxBuffer_laser2, 1);
    }

    if (UartHandle->Instance == MOTOR_USARTx)
    {
        HAL_UART_Transmit(&husart_motor, &aRxBuffer_motor, 1, 0);
        HAL_UART_Receive_IT(&husart_motor, &aRxBuffer_motor, 1);
    }
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
