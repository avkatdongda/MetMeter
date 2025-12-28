#ifndef _BSP_H_
#define _BSP_H

#include "stm32F1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "usart.h"
#include "tim.h"
#include "rtc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "my_task.h"

#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_pump.h"
#include "bsp_usart.h"
#include "absQueue.h"

#include "protocol.h"
#include "mbcrc.h"

#include "gui.h"

#include "lcd.h"
#include "Ble.h"
#include "stmflash.h"
#include "W25Qxx.h"

#include "UsrData.h"
#include "filter.h"

#include "SHT20.h"
#include "JXM_O2.h"
#include "WF5803F.h"
// #include "TCD1103.h"
#include "TCD1254.h"

//开关全局中断的宏 
#define ENABLE_INT() __enable_irq() //使能全局中断
#define DISABLE_INT() __disable_irq() //禁止全局中断

//报警蜂鸣器设置，单位10ms
//告警等级1
#define TIME_BEEP_BEEP_LEV1	100 //蜂鸣时长
#define TIME_BEEP_STOP_LEV1 100 //停止时长
//告警等级2
#define TIME_BEEP_BEEP_LEV2	20 //蜂鸣时长
#define TIME_BEEP_STOP_LEV2 20 //停止时长


#define POW5V_ON()       HAL_GPIO_WritePin(OXY_EN_GPIO_Port, OXY_EN_Pin, GPIO_PIN_SET)       // 
#define POW5V_OFF()      HAL_GPIO_WritePin(OXY_EN_GPIO_Port, OXY_EN_Pin, GPIO_PIN_RESET)     //

void bsp_Init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
void delayMs(uint16_t nms);
uint32_t GetBatV(void);
void BSP_GetBatPow(void);
void ALARM_Start(uint8_t lev);
void ALARM_Stop(void);
void BEEP_Beep(uint8_t n);

#endif
