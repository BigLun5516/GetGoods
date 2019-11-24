#ifndef _PID_H_
#define _PID_H_

#include "stm32f1xx_hal.h"

#define ratedSpd1 4000     //线性模组 电机 常用速度，r/min
#define ratedSpd2 1000     //取货杆 小电机 常用速度，r/min
#define MAX1 ratedSpd1  //限制最大转速
#define MIN1 -ratedSpd1 //限制最小转速
#define MAX2 ratedSpd2  //限制最大转速
#define MIN2 -ratedSpd2 //限制最小转速

#define crtDistance 0.1 // 安全距离
#define allowedError 0.01

extern float Motor_PID1(float set, float now, uint8_t clear); //线性模组 电机
extern float Motor_PID2(float set, float now, uint8_t clear); //取货杆 小电机

#endif