/*
=================================================================================================================
 	模块名称 : LED驱动，PWM方式
 	文件名称 : bap_led.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/

#ifndef __BSP_LED_H__
#define __BSP_LED_H__
#ifndef LED_GLOBAL
#define LED_EXT	extern
#else
#define LED_EXT
#endif

#include "bsp.h"

/*******************************LED DEFINE*************************************/
#define LED_ALARM_ON()                   HAL_GPIO_WritePin(LED_ALARM_GPIO_Port, LED_ALARM_Pin, GPIO_PIN_SET)     // 
#define LED_ALARM_OFF()                  HAL_GPIO_WritePin(LED_ALARM_GPIO_Port, LED_ALARM_Pin, GPIO_PIN_RESET)       // 
#define LED_ALARM_TOGGLE()               HAL_GPIO_TogglePin(LED_ALARM_GPIO_Port, LED_ALARM_Pin)                    // 

#define LED_HTIM  htim3
#define LED1_HTIM htim3
#define LED1_CHAN TIM_CHANNEL_1
#define LED2_HTIM htim3
#define LED2_CHAN TIM_CHANNEL_2
#define LED3_HTIM htim3
#define LED3_CHAN TIM_CHANNEL_3

/*******************************LED DEFINE*************************************/
LED_EXT void bsp_led_Int(void);
// LED_EXT uint8_t LED_SetCompare(uint16_t pulse);
LED_EXT uint8_t LED_SetFrequence(uint16_t fre); //设置频率
LED_EXT uint8_t LED_SetDutyCycle(uint8_t dutyCycle); //设置占空比
LED_EXT void LED_Start(void);
LED_EXT void LED_Stop(void);

#endif  // __BSP_LED_H__
