/*------------------------------------------------------------------------------------

WF5803F传感器驱动程序

-----------------------------------------------------------------------------------*/
#ifndef _WF5803F_H_
#define _WF5803F_H_

#ifndef WF5803F_GLOBAL
#define WF5803F_EXT	extern
#else
#define WF5803F_EXT
#endif

#include "bsp.h"

typedef struct
{
	float Pressure;
    float Temperature;

    uint32_t P32;
    uint32_t T32;

}TYPE_STRUCT_WF5803;

WF5803F_EXT TYPE_STRUCT_WF5803 WF5803F;

#define WF5803F_TIME_NOACK 300
WF5803F_EXT uint16_t WF5803F_Timer_noack;

// #define WF5803F_STA_NUL     0
// #define WF5803F_STA_OK      1
// #define WF5803F_STA_ERROR   0xFF
// WF5803F_EXT uint8_t WF5803F_sta;

WF5803F_EXT void WF5803F_Init(void);
// WF5803F_EXT void WF5803F_Read(void);
WF5803F_EXT void WF5803F_TimerIsr(void);
WF5803F_EXT uint8_t WF5803F_Read(void);

#endif