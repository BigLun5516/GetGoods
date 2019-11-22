#include "relay.h"

void RELAY_Init(){
    GPIO_InitTypeDef GPIO_InitStruct;

    RELAY_RCC_CLK_ENABLE();

    GPIO_InitStruct.Pin = RELAY1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(RELAY1_GPIO, &GPIO_InitStruct);
    RELAY1_OFF;
    
    GPIO_InitStruct.Pin = RELAY2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(RELAY2_GPIO, &GPIO_InitStruct);
    RELAY2_OFF;
    
    GPIO_InitStruct.Pin = RELAY3_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(RELAY3_GPIO, &GPIO_InitStruct);
    RELAY3_OFF;

    GPIO_InitStruct.Pin = RELAY4_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(RELAY4_GPIO, &GPIO_InitStruct);
    RELAY4_OFF;

    GPIO_InitStruct.Pin = RELAY5_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(RELAY5_GPIO, &GPIO_InitStruct);
    RELAY5_OFF;
}

void push_rod_extend(){
    RELAY1_OFF;
    RELAY2_OFF;
    RELAY3_OFF;
    RELAY4_OFF;
    HAL_Delay(500);
    RELAY1_ON;
    RELAY4_ON;
}

void push_rod_back(){
    RELAY1_OFF;
    RELAY2_OFF;
    RELAY3_OFF;
    RELAY4_OFF;
    HAL_Delay(500);
    RELAY2_ON;
    RELAY3_ON;
}

void push_rod_stop(){
    RELAY1_OFF;
    RELAY2_OFF;
    RELAY3_OFF;
    RELAY4_OFF;
    HAL_Delay(500);
    RELAY2_ON;
    RELAY4_ON;
}