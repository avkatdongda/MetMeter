/*------------------------------------------------------------------------------------

JXMO2gen Sensor 传感器驱动程序

-----------------------------------------------------------------------------------*/
#ifndef _JXMO2_H_
#define _JXMO2_H_

#ifndef JXMO2_GLOBAL
#define JXMO2_EXT	extern
#else
#define JXMO2_EXT
#endif

#include "bsp.h"

#define JXMO2_UARTINDEX UART2INDEX
#define JXMO2_UART huart2
#define JXMO2_BUD 9600

typedef struct //info
{
    uint8_t resolution; //分辨率
    uint16_t O2;   // 氧气浓度 单位：%
    float Oxygen;
}TYPE_STRUCT_JXMO2_DATA; 

typedef struct //info
{
    uint16_t TimerNoAck;
    TYPE_STRUCT_JXMO2_DATA Data;
}TYPE_STRUCT_JXMO2;

JXMO2_EXT TYPE_STRUCT_JXMO2_DATA JXMO2;

#define JXMO2_STA_AUTO    0
#define JXMO2_STA_MANUAL  1
#define JXMO2_STA_STOP    1
#define JXMO2_STA_ERROR   0xFF

#define JXMO2_TIME_NOACK 300
JXMO2_EXT uint16_t JXMO2_Timer_noack;
JXMO2_EXT uint8_t JXMO2_sta;

JXMO2_EXT uint8_t JXMO2_Init(void);
JXMO2_EXT uint8_t JXMO2_GetData(void);

JXMO2_EXT void JXMO2_AutoStar(void);
JXMO2_EXT void JXMO2_AutoStop(void);
JXMO2_EXT void JXMO2_TimerIsr(void);
JXMO2_EXT void JXMO2_SetAutoMode(void);
JXMO2_EXT void JXMO2_SetManualMode(void);
#endif