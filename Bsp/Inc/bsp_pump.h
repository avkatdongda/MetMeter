/*
=================================================================================================================
 	模块名称 : 气泵驱动
 	文件名称 : bsp_pump.h
 	版    本 : V1.0.0
 	说    明 : PWM调占空比
=================================================================================================================
*/
#ifndef __BSP_PUMP_H__
#define __BSP_PUMP_H__
#ifndef PUMP_GLOBAL
#define PUMP_EXT	extern
#else
#define PUMP_EXT
#endif

// #include "bsp.h"

/*******************************PUMP DEFINE*************************************/
#define PUMP_HTIM  htim5
#define PUMP_CHAN  TIM_CHANNEL_2

#define PUMP_ON()       PUMP_Start() // 
#define PUMP_OFF()      PUMP_Stop() //

/*******************************PUMP DEFINE*************************************/

PUMP_EXT void PUMP_Int(void); //初始化
PUMP_EXT uint8_t PUMP_SetFrequence(void); //设置频率
PUMP_EXT uint8_t PUMP_SetDutyCycle(uint8_t dutyCycle); //设置占空比
PUMP_EXT void PUMP_Start(void);
PUMP_EXT void PUMP_Stop(void);

#endif  // __BSP_PUMP_H__
