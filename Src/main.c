/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "usart/bsp_debug_usart.h"
#include "string.h"
#include "ASDA_B2/bsp_ASDA_B2.h"
#include "relay.h"
#include "stdlib.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
__IO uint8_t Rx_Buf[50]; //�������ݻ���

uint8_t aRxBuffer_debug;
uint8_t aRxBuffer_laser1;
uint8_t aRxBuffer_laser2;
uint8_t aRxBuffer_motor;

char *end; // ���ں��� strtod(char*, char**);
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ϵͳʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE; // �ⲿ����8MHz
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9; // 9��Ƶ���õ�72MHz��ʱ��
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // ϵͳʱ�ӣ�72MHz
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        // AHBʱ�ӣ�72MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;         // APB1ʱ�ӣ�36MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         // APB2ʱ�ӣ�72MHz
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

    // HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
    // HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
    // HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000); // ���ò�����ϵͳ�δ�ʱ��
    /* ϵͳ�δ�ʱ��ʱ��Դ */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    /* ϵͳ�δ�ʱ���ж����ȼ����� */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{
    uint8_t i = 0, test = 0;
    int8_t dir = 1;
    /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
    HAL_Init();
    /* ����ϵͳʱ�� */
    SystemClock_Config();

    // /* ��ʼ�����ڲ����ô����ж����ȼ� */
    MX_DEBUG_USART_Init();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���� ����ģ��
    // RS485_USARTx_Init();

    // HAL_UART_Receive_DMA(&husartx_rs485,&test, 1);// Data Direction: 485 --> USART1

    // /* Disable the Half transfer complete interrupt */
    // __HAL_DMA_DISABLE_IT(&hdma_rs485_rx, DMA_IT_HT);
    // __HAL_DMA_DISABLE_IT(&hdma_rs485_rx, DMA_IT_TE);

    // /* ��ʼ��ASDA-B2����,����Ϊ�ٶ�ģʽ*/
    // ASDAB2_Init();
    // /* ����SP3�ٶ�ֵΪ600*0.1r/min  60r/min */
    // SetSpeed(REG_SP3,100);
    // /* �����ŷ� */
    // StartServo();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // �������
    LASER_Init();
    HAL_UART_Transmit(&husart_debug, "start1\r\n", 8, 1000);
    laser_start_measure(LASER1, 2);
    

    // laser_start_measure(LASER2);
    HAL_UART_Receive_IT(&husart_laser1, &aRxBuffer_laser1, 1); // ����1�Ĵ��� ���ж�
    // HAL_UART_Receive_IT(&husart_laser2, &aRxBuffer_laser2, 1); // ����2�Ĵ��� ���ж�
    HAL_Delay(500);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // С�������
    // MOTOR_USARTx_Init();
    // HAL_UART_Receive_IT(&husart_debug, &aRxBuffer_debug, 1);
    // HAL_UART_Receive_IT(&husart_motor, &aRxBuffer_motor, 1);

    // motor_reset();
    // motor_entry_velocity_mode();
    // motor_set_velocity(500);
    // motor_recieve_info();
    // HAL_Delay(3000);
    // motor_stop_recieve_info();
    // HAL_Delay(3000);
    // // motor_set_velocity(-500);
    // motor_stop();
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // �̵�������
    // RELAY_Init();
    // RELAY1_ON;
    // RELAY1_OFF;
    // RELAY2_ON;
    // RELAY2_OFF;
    // RELAY3_ON;
    // RELAY3_OFF;
    // RELAY4_ON;
    // RELAY4_OFF;
    // RELAY5_ON;
    // RELAY5_OFF;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* ����ѭ�� */
    static char dis[10]; // ������ ���ڽ�floatתΪstr ==================================
    while (1)
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // ����
        // uint16_t j = 100;
        // for(i=0; i<20;i++)
        // {
        //   HAL_Delay(500);
        //   SetSpeed(REG_SP3,(j+=500)*dir);   // ����Ϊ��ת
        // }
        // for(i=0; i<20;i++)
        // {
        //   HAL_Delay(500);
        //   SetSpeed(REG_SP3,(j-=500)*dir);   // ����Ϊ��ת
        // }
        // StopServo();              // ֹͣ�ŷ�
        // HAL_Delay(2000);
        // dir = -dir;
        // SetSpeed(REG_SP3,j*dir);
        // StartServo();             // ���������ŷ����
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // ����
        sprintf(dis, "%.3f", distance_laser1);
        // sprintf(dis, "%.3f", distance_laser2);
        HAL_UART_Transmit(&husart_debug, dis, 5, 1000);
        HAL_UART_Transmit(&husart_debug, "\r\n", 2, 1000);
        HAL_Delay(20);
    }
}

/**
  * ��������: ���ڽ��ջص�����
  * �������: UARTHandle:���ھ��
  * �� �� ֵ: ��
  * ˵    ��: ���յ��ӻ��ķ�������֮��,�ְ�����ŵ�Rx_Buf����,һ��ֻ�ܽ���һ֡����
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
                    Rx_Buf[i] = '\0';   // �ֶ���ӽ�����
                    UsartState = Ready; // �������,ͨѶ����
                    i = 0;
                }
            }
        }
        else
            i = 0;
    }

    // ����1
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

            // // ��������1 ������
            // HAL_UART_Transmit(&husart_debug, buf_laser1, count_laser1, 1000);
            // HAL_UART_Transmit(&husart_debug, "\r\n", 2, 1000);

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

    // ����2
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

            // // ��������1 ������
            // HAL_UART_Transmit(&husart_debug, buf_laser2, count_laser2, 1000);
            // HAL_UART_Transmit(&husart_debug, "\r\n", 2, 1000);

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
        HAL_UART_Transmit(&husart_debug, &aRxBuffer_motor, 1, 0);
        HAL_UART_Receive_IT(&husart_motor, &aRxBuffer_motor, 1);
    }
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
