#include "key.h"

void key_init(){
    GPIO_InitTypeDef GPIO_InitStruct;

    KEY_RCC_CLK_ENABLE();

    GPIO_InitStruct.Pin = KEY_GPIO_PIN1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(KEY_GPIO1, &GPIO_InitStruct);
}