#ifndef __KEY_H
#define __KEY_H

#include "stm32f1xx_hal.h"

#define KEY_RCC_CLK_ENABLE()          __HAL_RCC_GPIOC_CLK_ENABLE()

#define KEY_GPIO1 GPIOC
#define KEY_GPIO_PIN1 GPIO_PIN_0

#define KEY_GPIO2 GPIOC
#define KEY_GPIO_PIN2 GPIO_PIN_5

#define KEY_ON_LEVEL   GPIO_PIN_SET
#define KEY_LEVEL   HAL_GPIO_ReadPin(KEY_GPIO1, KEY_GPIO_PIN1)

void key_init();
#endif
