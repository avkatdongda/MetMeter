/*
=================================================================================================================
 	模块名称 : LED驱动，PWM方式
 	文件名称 : bap_led.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#include "bsp_led.h"

/*
********************************************************************************
* @function : void bsp_led_Int(void)
* @brief    : 
* @param    : none
* @return   : none
********************************************************************************
*/

#define LED_ARR (1000000/LED_FREQUENCE)

void bsp_led_Int(void)
{
    LED_SetFrequence(LED_FREQUENCE); //设置频率
    LED_SetDutyCycle(LED_DUTY_CYCLE); //设置占空比
}

uint32_t LED_CHANNEL[3] ={
    LED1_CHAN, LED2_CHAN, LED3_CHAN
};


uint8_t LED_SetCompare(uint8_t n, uint16_t pulse)
{
     __HAL_TIM_SET_COMPARE(&LED_HTIM, LED_CHANNEL[n], pulse);
    return ERR_OK;
}

//1M/reload frequence
uint8_t LED_SetReload(uint16_t reload) //设置频率
{
     __HAL_TIM_SET_AUTORELOAD(&LED_HTIM, reload);
   
    return ERR_OK;
}

uint8_t LED_SetFrequence(uint16_t fre)//设置频率
{
     __HAL_TIM_SET_AUTORELOAD(&LED_HTIM, LED_ARR);

    return ERR_OK;
}

uint8_t LED_SetDutyCycle(uint8_t dutyCycle) //设置占空比
{
    uint32_t pulse;
    pulse = LED_ARR*dutyCycle/100;
    __HAL_TIM_SET_COMPARE(&LED_HTIM, LED_CHANNEL[LED_USE1], (uint16_t)pulse);
    __HAL_TIM_SET_COMPARE(&LED_HTIM, LED_CHANNEL[LED_USE2], (uint16_t)pulse);

    return ERR_OK;
}

void LED_Start(void) //uint32_t Channel
{
    HAL_TIM_Base_Start(&LED_HTIM);  //从定时器计数开启
    HAL_TIM_PWM_Start(&LED_HTIM, LED_CHANNEL[LED_USE1]);  //从定时器PWM脉冲输出
    HAL_TIM_PWM_Start(&LED_HTIM, LED_CHANNEL[LED_USE2]);  //从定时器PWM脉冲输出
}

void LED_Stop(void)
{
    HAL_TIM_PWM_Stop(&LED_HTIM, LED_CHANNEL[LED_USE1]);  //从定时器PWM脉冲输出
    HAL_TIM_PWM_Stop(&LED_HTIM, LED_CHANNEL[LED_USE2]);  //从定时器PWM脉冲输出
    HAL_TIM_Base_Stop(&LED_HTIM);  //从定时器计数开启
}
