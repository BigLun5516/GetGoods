/* ����ͷ�ļ� ----------------------------------------------------------------*/
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

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
__IO uint8_t Rx_Buf[50]; //�������ݻ���

uint8_t aRxBuffer_debug;
uint8_t aRxBuffer_laser1;
uint8_t aRxBuffer_laser2;
uint8_t aRxBuffer_motor;

char *end; // ���ں��� strtod(char*, char**);

#define filter 0.2
float setSpd = 0;

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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // �̵�������
    RELAY_Init();
    // 24 ȫ�أ���
    // 24 ȫ������
    // DCT_ON;
    // push_rod_extend();
    // push_rod_back();
    // // DCT_OFF;
    // while(1);

    // /* ��ʼ�����ڲ����ô����ж����ȼ� */
    MX_DEBUG_USART_Init();

    //////////////////////
    // ���ز���
    key_init();
    // while(KEY_LEVEL != KEY_ON_LEVEL);
    // RELAY5_ON;
    // while(1);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //�������
    LASER_Init();
    HAL_UART_Receive_IT(&husart_laser1, &aRxBuffer_laser1, 1); // ����1�Ĵ��� ���ж�
    HAL_UART_Receive_IT(&husart_laser2, &aRxBuffer_laser2, 1); // ����2�Ĵ��� ���ж�
    HAL_Delay(500);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���� ����ģ��
    RS485_USARTx_Init();

    HAL_UART_Receive_DMA(&husartx_rs485, &test, 1); // Data Direction: 485 --> USART1

    /* Disable the Half transfer complete interrupt */
    __HAL_DMA_DISABLE_IT(&hdma_rs485_rx, DMA_IT_HT);
    __HAL_DMA_DISABLE_IT(&hdma_rs485_rx, DMA_IT_TE);

    /* ��ʼ��ASDA-B2����,����Ϊ�ٶ�ģʽ*/
    ASDAB2_Init();
    /* ����SP3�ٶ�ֵΪ0*0.1r/min */
    SetSpeed(REG_SP3, 0);
    /* �����ŷ� */
    StartServo();
    HAL_Delay(2000); // ���������

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // С�������
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

    /* ����ѭ�� */
    while (1)
    {
        // ���� ��Ʒ�߶�
        while (!flag_height)
        {
            cmmu_receive_data_height(&husart_debug);
        }

        // ���� ���̿��
        while (!flag_width)
        {
            cmmu_receive_data_width(&husart_debug);
        }

        // ���� ��ʼȡ��������
        while (!flag_get)
        {
            cmmu_receive_cmd_get(&husart_debug);
        }

        // �ƶ��� ��Ʒ�߶�
        static uint8_t flag_height_speed = 0; ////
        while (1)
        {
            distance_filter_laser1 = distance_laser1 * (1 - filter) + last_distance_laser1 * filter;
            if ((distance_filter_laser1 - height) > crtDistance) //���Ʋ���ѡ��
            {
                setSpd = -ratedSpd1;
                flag_height_speed = 1;
            }
            else if ((distance_filter_laser1 - height) < (-1 * crtDistance)) //���Ʋ���ѡ��
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
                (fabs(last_distance_laser1 - height) < allowedError)) //������֡������ΧҪ�������һ��
            {
                SetSpeed(REG_SP3, 0); //ֹͣ���
                setSpd = 0;
                Motor_PID1(0, 0, 1); //�����ʷֵ
                break;
            }
            HAL_Delay(20);
        }

        // ȡ��  ���ȡ����
        while (1)
        {
            distance_filter_laser2 = distance_laser2 * (1 - filter) + last_distance_laser2 * filter;
            if ((distance_filter_laser2 - (init_distance_laser2 - width / 2)) > crtDistance) //���Ʋ���ѡ��
                setSpd = -1 * ratedSpd2;
            else if ((distance_filter_laser2 - (init_distance_laser2 - width / 2)) < (-1 * crtDistance)) //���Ʋ���ѡ��
                setSpd = ratedSpd2;
            else if (fabs(distance_filter_laser2 - (init_distance_laser2 - width / 2)) <= crtDistance)
                setSpd = Motor_PID2(init_distance_laser2 - width / 2, distance_filter_laser2, 0);
            motor_set_velocity(setSpd);

            if ((fabs(distance_laser2 - (init_distance_laser2 - width / 2)) < allowedError2) &&
                (fabs(last_distance_laser2 - (init_distance_laser2 - width / 2)) < allowedError2)) //������֡������ΧҪ�������һ��
            {
                motor_stop(); //ֹͣ���
                setSpd = 0;
                Motor_PID2(0, 0, 1); //�����ʷֵ
                break;
            }
            HAL_Delay(30);
        }

        // �������
        DCT_ON;
        HAL_Delay(400);

        // ̧4cm
        height = height + 0.04;
        while (1)
        {
            distance_filter_laser1 = distance_laser1 * (1 - filter) + last_distance_laser1 * filter;
            if ((distance_filter_laser1 - height) > crtDistance) //���Ʋ���ѡ��
                setSpd = -ratedSpd1;
            else if ((distance_filter_laser1 - height) < (-1 * crtDistance)) //���Ʋ���ѡ��
                setSpd = 1 * ratedSpd1;
            else if (fabs(distance_filter_laser1 - height) <= crtDistance)
                setSpd = Motor_PID1(height, distance_filter_laser1, 0);
            SetSpeed(REG_SP3, setSpd);

            if ((fabs(distance_laser1 - height) < allowedError) &&
                (fabs(last_distance_laser1 - height) < allowedError)) //������֡������ΧҪ�������һ��
            {
                SetSpeed(REG_SP3, 0); //ֹͣ���
                setSpd = 0;
                Motor_PID1(0, 0, 1); //�����ʷֵ
                break;
            }
            HAL_Delay(20);
        }

        // �ջ�ȡ����
        motor_set_velocity(ratedSpd2);
        // �ȴ��г̿��ذ���
        while (KEY_LEVEL != KEY_ON_LEVEL)
            ;
        motor_stop();

        // ȡ��� ������Ϣ
        while (laser_send_cmd(&husart_debug, "CGetOK@", "CGetOK@", 100))
            ;

        // ���� ж��������
        while (!flag_put && !flag_lod)
        {
            cmmu_receive_cmd_put_or_lod(&husart_debug);
        }

        if (flag_put)
        {
            // ж��
            // �ƶ��� ж���߶�
            height = 1;
            while (1)
            {
                distance_filter_laser1 = distance_laser1 * (1 - filter) + last_distance_laser1 * filter;
                if ((distance_filter_laser1 - height) > crtDistance) //���Ʋ���ѡ��
                    setSpd = -ratedSpd1;
                else if ((distance_filter_laser1 - height) < (-1 * crtDistance)) //���Ʋ���ѡ��
                    setSpd = 1 * ratedSpd1;
                else if (fabs(distance_filter_laser1 - height) <= crtDistance)
                    setSpd = Motor_PID1(height, distance_filter_laser1, 0);
                SetSpeed(REG_SP3, setSpd);

                if ((fabs(distance_laser1 - height) < allowedError) &&
                    (fabs(last_distance_laser1 - height) < allowedError) &&
                    (fabs(last_last_distance_laser1 - height) < allowedError)) //������֡������ΧҪ�������һ��
                {
                    SetSpeed(REG_SP3, 0); //ֹͣ���
                    setSpd = 0;
                    Motor_PID1(0, 0, 1); //�����ʷֵ
                    break;
                }
                HAL_Delay(20);
            }
            push_rod_extend();
            HAL_Delay(1000); // �ȴ��Ƹ��Ƶ���
            HAL_Delay(1000); // �ȴ��Ƹ��Ƶ���
            HAL_Delay(1000); // �ȴ��Ƹ��Ƶ���
            push_rod_back();

            // ж��� ������Ϣ
            while (laser_send_cmd(&husart_debug, "CPutOK@", "CPutOK@", 100))
                ;

            // ���� �ӵ����̵�����
            while (!flag_throw)
            {
                cmmu_receive_cmd_throw(&husart_debug);
            }

            // �ӵ�����
            DCT_OFF; // �������

            // ����� ������Ϣ
            while (laser_send_cmd(&husart_debug, "CThwOK@", "CThwOK@", 100))
                ;
        }
        else
        {
            // �ϻ�
            flag_height = 0;
            cmmu_receive_data_height(&husart_debug);

            height += 0.04; //ȡ������̧һ���

            motor_set_velocity(-1 * ratedSpd2); // ���ȡ����

            HAL_Delay(500);

            motor_stop();

            // �رյ����
            DCT_OFF;

            // ȡ�����½�һЩ
            height -= 0.06;
            while (1)
            {
                distance_filter_laser1 = distance_laser1 * (1 - filter) + last_distance_laser1 * filter;
                if ((distance_filter_laser1 - height) > crtDistance) //���Ʋ���ѡ��
                    setSpd = -ratedSpd1;
                else if ((distance_filter_laser1 - height) < (-1 * crtDistance)) //���Ʋ���ѡ��
                    setSpd = 1 * ratedSpd1;
                else if (fabs(distance_filter_laser1 - height) <= crtDistance)
                    setSpd = Motor_PID1(height, distance_filter_laser1, 0);
                SetSpeed(REG_SP3, setSpd);

                if ((fabs(distance_laser1 - height) < allowedError) &&
                    (fabs(last_distance_laser1 - height) < allowedError) &&
                    (fabs(last_last_distance_laser1 - height) < allowedError)) //������֡������ΧҪ�������һ��
                {
                    SetSpeed(REG_SP3, 0); //ֹͣ���
                    setSpd = 0;
                    Motor_PID1(0, 0, 1); //�����ʷֵ
                    break;
                }
                HAL_Delay(20);
            }

            // �ջ�ȡ����
            motor_set_velocity(ratedSpd2);
            while (KEY_LEVEL != KEY_ON_LEVEL) // �ȴ��г̿��ذ���
                ;
            motor_stop();

            // ��֪�����ϻ����
            while (laser_send_cmd(&husart_debug, "CLodOK@", "CLodOK@", 100))
                ;
        }
        cmmu_reset();
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

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
