/**
  ******************************************************************************
  * �ļ�����: bsp_debug_usart.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ���ص��Դ��ڵײ���������Ĭ��ʹ��USART1
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "usart/bsp_debug_usart.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
UART_HandleTypeDef husart_debug;
DMA_HandleTypeDef hdma_debug_rx;

DMA_HandleTypeDef hdma_rs485_rx;
UART_HandleTypeDef husartx_rs485;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ����Ӳ����ʼ������
  * �������: huart�����ھ������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if (huart->Instance == DEBUG_USARTx)
  {
    /* ��������ʱ��ʹ�� */
    DEBUG_USART_RCC_CLK_ENABLE();
    USARTx_RCC_DMAx_CLK_ENABLE();

    /* �������蹦��GPIO���� */
    GPIO_InitStruct.Pin = DEBUG_USARTx_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DEBUG_USARTx_Tx_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DEBUG_USARTx_Rx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DEBUG_USARTx_Rx_GPIO, &GPIO_InitStruct);

    /* ��ʼ��DMA���� */
    hdma_debug_rx.Instance = USARTx_DMAx_CHANNELn;
    hdma_debug_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_debug_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_debug_rx.Init.MemInc = DMA_MINC_DISABLE;
    hdma_debug_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_debug_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_debug_rx.Init.Mode = DMA_CIRCULAR;
    hdma_debug_rx.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_debug_rx);

    __HAL_LINKDMA(huart, hdmarx, hdma_debug_rx);
  }

  // �����ŷ�
  if (huart->Instance == RS485_USARTx)
  {
    /* ��������ʱ��ʹ�� */
    RS485_USARTx_GPIO_ClK_ENABLE();
    RS485_REDE_GPIO_ClK_ENABLE();
    RS485_RCC_DMAx_CLK_ENABLE();

    /* �������蹦��GPIO���� */
    GPIO_InitStruct.Pin = RS485_USARTx_Tx_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RS485_USARTx_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RS485_USARTx_Rx_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(RS485_USARTx_PORT, &GPIO_InitStruct);

    /* SP3485E��������ʹ�ܿ������ų�ʼ�� */
    HAL_GPIO_WritePin(RS485_REDE_PORT, RS485_REDE_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = RS485_REDE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RS485_REDE_PORT, &GPIO_InitStruct);

    /* ��ʼ��DMA���� */
    hdma_rs485_rx.Instance = RS485_DMAx_CHANNELn;
    hdma_rs485_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_rs485_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_rs485_rx.Init.MemInc = DMA_MINC_DISABLE;
    hdma_rs485_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rs485_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_rs485_rx.Init.Mode = DMA_CIRCULAR;
    hdma_rs485_rx.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_rs485_rx);

    __HAL_LINKDMA(huart, hdmarx, hdma_rs485_rx);
  }

  // ����1
  if (huart->Instance == LASER1_USARTx)
  {
    /* ��������ʱ��ʹ�� */
    LASER1_USART_RCC_CLK_ENABLE();
    LASER1_USARTx_GPIO_ClK_ENABLE();

    /* �������蹦��GPIO���� */
    GPIO_InitStruct.Pin = LASER1_USARTx_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LASER1_USARTx_Tx_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LASER1_USARTx_Rx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LASER1_USARTx_Rx_GPIO, &GPIO_InitStruct);
  }

  // ����2
  if (huart->Instance == LASER2_USARTx)
  {
    /* ��������ʱ��ʹ�� */
    LASER2_USART_RCC_CLK_ENABLE();
    LASER2_USARTx_GPIO_ClK_ENABLE();

    /* �������蹦��GPIO���� */
    GPIO_InitStruct.Pin = LASER2_USARTx_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LASER2_USARTx_Tx_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LASER2_USARTx_Rx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LASER2_USARTx_Rx_GPIO, &GPIO_InitStruct);
  }

  // С���������
  if (huart->Instance == MOTOR_USARTx)
  {
    /* ��������ʱ��ʹ�� */
    MOTOR_USART_RCC_CLK_ENABLE();

    /* �������蹦��GPIO���� */
    GPIO_InitStruct.Pin = MOTOR_USARTx_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MOTOR_USARTx_Tx_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MOTOR_USARTx_Rx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MOTOR_USARTx_Rx_GPIO, &GPIO_InitStruct);
  }
}

/**
  * ��������: ����Ӳ������ʼ������
  * �������: huart�����ھ������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{

  if (huart->Instance == DEBUG_USARTx)
  {
    /* ��������ʱ�ӽ��� */
    DEBUG_USART_RCC_CLK_DISABLE();

    /* �������蹦��GPIO���� */
    HAL_GPIO_DeInit(DEBUG_USARTx_Tx_GPIO, DEBUG_USARTx_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(DEBUG_USARTx_Rx_GPIO, DEBUG_USARTx_Rx_GPIO_PIN);

    /* �����жϽ��� */
    HAL_NVIC_DisableIRQ(DEBUG_USART_IRQn);
  }

  if(huart->Instance == LASER1_USARTx)
  {
    /* ��������ʱ�ӽ��� */
    laser_USART_RCC_CLK_DISABLE();
  
    /* �������蹦��GPIO���� */
    HAL_GPIO_DeInit(LASER1_USARTx_Tx_GPIO, LASER1_USARTx_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(LASER1_USARTx_Rx_GPIO, LASER1_USARTx_Rx_GPIO_PIN);
    
    /* �����жϽ��� */
    HAL_NVIC_DisableIRQ(LASER1_USART_IRQn);
  }
  if(huart->Instance == LASER2_USARTx)
  {
    /* ��������ʱ�ӽ��� */
    laser_USART_RCC_CLK_DISABLE();
  
    /* �������蹦��GPIO���� */
    HAL_GPIO_DeInit(LASER2_USARTx_Tx_GPIO, LASER2_USARTx_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(LASER2_USARTx_Rx_GPIO, LASER2_USARTx_Rx_GPIO_PIN);
    
    /* �����жϽ��� */
    HAL_NVIC_DisableIRQ(LASER2_USART_IRQn);
  }
  if(huart->Instance==MOTOR_USARTx)
  {
    /* ��������ʱ�ӽ��� */
    MOTOR_USART_RCC_CLK_DISABLE();
  
    /* �������蹦��GPIO���� */
    HAL_GPIO_DeInit(MOTOR_USARTx_Tx_GPIO, MOTOR_USARTx_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(MOTOR_USARTx_Rx_GPIO, MOTOR_USARTx_Rx_GPIO_PIN);
    
    /* �����жϽ��� */
    HAL_NVIC_DisableIRQ(MOTOR_USARTx_IRQn);
  }
}

/**
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_DEBUG_USART_Init(void)
{
  /* ʹ�ܴ��ڹ�������GPIOʱ�� */
  DEBUG_USARTx_GPIO_ClK_ENABLE();

  husart_debug.Instance = DEBUG_USARTx;
  husart_debug.Init.BaudRate = DEBUG_USARTx_BAUDRATE;
  husart_debug.Init.WordLength = UART_WORDLENGTH_8B;
  husart_debug.Init.StopBits = UART_STOPBITS_2;
  husart_debug.Init.Parity = UART_PARITY_NONE;
  husart_debug.Init.Mode = UART_MODE_TX_RX;
  husart_debug.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart_debug.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husart_debug);

  HAL_NVIC_SetPriority(DEBUG_USART_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQn);
  HAL_NVIC_SetPriority(USARTx_DMAx_CHANNELn_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(USARTx_DMAx_CHANNELn_IRQn);
}

/**
  * ��������: �ض���c�⺯��printf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&husart_debug, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * ��������: �ض���c�⺯��getchar,scanf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&husart_debug, &ch, 1, 0xffff);
  return ch;
}

/**
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void RS485_USARTx_Init(void)
{
  /* RS485ʱ��ʹ�� */
  RS485_USART_RCC_CLK_ENABLE();

  husartx_rs485.Instance = RS485_USARTx;
  husartx_rs485.Init.BaudRate = RS485_USARTx_BAUDRATE;
  husartx_rs485.Init.WordLength = UART_WORDLENGTH_8B;
  husartx_rs485.Init.StopBits = UART_STOPBITS_2;
  husartx_rs485.Init.Parity = UART_PARITY_NONE;
  husartx_rs485.Init.Mode = UART_MODE_TX_RX;
  husartx_rs485.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husartx_rs485.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husartx_rs485);

  HAL_NVIC_SetPriority(RS485_USARTX_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(RS485_USARTX_IRQn);
  /* DMA interrupt configuration */
  HAL_NVIC_SetPriority(RS485_DMAx_CHANNELn_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(RS485_DMAx_CHANNELn_IRQn);
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
