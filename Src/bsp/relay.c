#include "relay.h"

void RELAY_Init(){
    GPIO_InitTypeDef GPIO_InitStruct;

    RELAY_RCC_CLK_ENABLE();

    GPIO_InitStruct.Pin = RELAY1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(RELAY1_GPIO, &GPIO_InitStruct);
    RELAY1_OFF;
    
    GPIO_InitStruct.Pin = RELAY2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(RELAY2_GPIO, &GPIO_InitStruct);
    RELAY2_OFF;
    
    GPIO_InitStruct.Pin = RELAY3_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(RELAY3_GPIO, &GPIO_InitStruct);
    RELAY3_OFF;

    GPIO_InitStruct.Pin = RELAY4_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(RELAY4_GPIO, &GPIO_InitStruct);
    RELAY4_OFF;

    GPIO_InitStruct.Pin = RELAY5_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(RELAY5_GPIO, &GPIO_InitStruct);
    RELAY5_OFF;
}

// relay2 常闭GND
// relay4 常闭24V

// 24 全开：申
void push_rod_extend(){
    RELAY2_ON;
    RELAY4_ON;
    
}

// 24 全关：缩
void push_rod_back(){
    RELAY2_OFF;
    RELAY4_OFF;
}

void push_rod_stop(){
    RELAY2_OFF;
    RELAY4_ON;
}