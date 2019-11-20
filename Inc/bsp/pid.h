#ifndef _PID_H_
#define _PID_H_

#include "stm32f1xx_hal.h"

#define ratedSpd 4000     //常用速度，r/min
#define MAX ratedSpd  //限制最大转速
#define MIN -ratedSpd //限制最小转速

extern float Motor_PID(float set, float now, uint8_t clear);

#endif