/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器模块
*	文件名称 : bsp_beep.h
*	版    本 : V1.0
*	说    明 : 头文件,驱动蜂鸣器.LED同时亮灭
*
*********************************************************************************************************
*/

#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#define BEEP_HAVE_POWER		/* 定义此行表示有源蜂鸣器，直接通过GPIO驱动, 无需PWM */

#ifdef	BEEP_HAVE_POWER		/* 有源蜂鸣器 */

	/* PB0 */
	#define GPIO_RCC_BEEP   RCC_APB2Periph_GPIOA
	#define GPIO_PORT_BEEP	BEEP_GPIO_Port
	#define GPIO_PIN_BEEP	BEEP_Pin

	#define BEEP_ENABLE()	HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET) /* 使能蜂鸣器鸣叫 */
	#define BEEP_DISABLE()	HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET) /* 禁止蜂鸣器鸣叫 */
#else		/* 无源蜂鸣器 */
	/* PA8/TIM1_CH1 ---> TIM1_CH1 */

	/* 1500表示频率1.5KHz，5000表示50.00%的占空比 */
	#define BEEP_ENABLE()	bsp_SetTIMOutPWM(GPIO_PORT_BEEP, GPIO_PIN_BEEP, TIM1, 1, 1500, 5000);

	/* 禁止蜂鸣器鸣叫 */
	#define BEEP_DISABLE()	bsp_SetTIMOutPWM(GPIO_PORT_BEEP, GPIO_PIN_BEEP, TIM1, 1, 1500, 0);
#endif

typedef struct _BEEP_T
{
	uint8_t ucEnalbe;
	uint8_t ucState;
	uint16_t usBeepTime;
	uint16_t usStopTime;
	uint16_t usCycle;
	uint16_t usCount;
	uint16_t usCycleCount;
}BEEP_T;

/* 供外部调用的函数声明 */
void BEEP_InitHard(void);
void BEEP_Start(uint16_t _usBeepTime, uint16_t _usStopTime, uint16_t _usCycle);
void BEEP_Stop(void);
void BEEP_KeyTone(void);
void BEEP_Pro(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
