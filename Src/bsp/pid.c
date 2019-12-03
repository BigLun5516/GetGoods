#include "PID.h"

struct PID
{
    float P;
    float I;
    float D;
};
struct PID dis_PID = {8000, 0, 0};
struct PID dis_PID2 = {8000, 0, 0};

//参数：设定值，当前值，清除历史值（1清除，0不清除）
float Motor_PID1(float set, float now, uint8_t clear)
{
    float error = 0;
    float result = 0;
    static float lastError = 0; //上次误差

    if (clear == 0)
    {
        error = set - now;
        result = dis_PID.P * error + dis_PID.D * (error - lastError); //PD控制算法
        lastError = error;                                            //储存历史值
    }
    else
        lastError = 0;

    if (result > MAX1)
        result = MAX1;
    if (result < MIN1)
        result = MIN1;

    return result;
}

float Motor_PID2(float set, float now, uint8_t clear)
{
    float error = 0;
    float result = 0;
    static float lastError = 0; //上次误差

    if (clear == 0)
    {
        error = set - now;
        result = dis_PID2.P * error + dis_PID2.D * (error - lastError); //PD控制算法
        lastError = error;                                            //储存历史值
    }
    else
        lastError = 0;

    if (result > MAX2)
        result = MAX2;
    if (result < MIN2)
        result = MIN2;

    return -result;
}