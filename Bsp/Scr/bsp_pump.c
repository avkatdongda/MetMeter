/*
=================================================================================================================
 	模块名称 : 气泵驱动
 	文件名称 : bsp_pump.c
 	版    本 : V1.0.0
 	说    明 : PWM调占空比
=================================================================================================================
*/
#define PUMP_GLOBAL
#include "bsp.h"


void PUMP_Int(void)
{
    PUMP_SetFrequence(); //设置频率
    PUMP_SetDutyCycle(PUMP_DUTY_CYCLE); //设置占空比
}


#define PUMP_HTIM  htim5
#define PUMP_CHAN TIM_CHANNEL_2
#define PUMP_ARR (1000000/PUMP_FREQUENCE)

uint8_t PUMP_SetFrequence(void)//设置频率
{
     __HAL_TIM_SET_AUTORELOAD(&PUMP_HTIM, PUMP_ARR);
    return ERR_OK;
}

uint8_t PUMP_SetDutyCycle(uint8_t dutyCycle) //设置占空比
{
    uint32_t pulse;
    pulse = PUMP_ARR/100*dutyCycle;
    // pulse = dutyCycle;
     __HAL_TIM_SET_COMPARE(&PUMP_HTIM, PUMP_CHAN, (uint16_t)pulse);
    return ERR_OK;
}

void PUMP_Start(void)
{
    HAL_TIM_Base_Start(&PUMP_HTIM);  //从定时器计数开启
    HAL_TIM_PWM_Start(&PUMP_HTIM, PUMP_CHAN);  //从定时器PWM脉冲输出
}

void PUMP_Stop(void)
{
    HAL_TIM_PWM_Stop(&PUMP_HTIM, PUMP_CHAN);  //从定时器PWM脉冲输出
    HAL_TIM_Base_Stop(&PUMP_HTIM);  //从定时器计数开启
}
