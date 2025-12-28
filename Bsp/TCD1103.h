/*
=================================================================================================================
 	模块名称 : TCD1103驱动
 	文件名称 : TCD1103.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/

#if 0//CCD_TYPE == CCD_TYPE_1103

#ifndef _TCD1103_H_
#define _TCD1103_H_

#ifndef TCD1103_GLOBAL
#define TCD1103_EXT	extern
#else
#define TCD1103_EXT
#endif

#include "bsp.h"

#define TCD1103_BUD 921600
#define TCD1103_UART huart1
//#define TCD1103_INDEX UART1INDEX


//0xA1///请求数据指令，
//0xB1///积分时间  10 μs */
//0xB2///积分时间  20 μs */
//0xB3///积分时间  50 μs */
//0xB4///积分时间  60 μs */
//0xB5///积分时间  75 μs */
//0xB6///积分时间  100 μs */
//0xB7///积分时间  500 μs */
//0xB8///积分时间  1.25 ms */
//0xB9///积分时间  2.5 ms */	
//0xBA///积分时间  7.5 ms */
#define I_10US  0xB1
#define I_20US  0xB2
#define I_50US  0xB3
#define I_60US  0xB4
#define I_75US  0xB5
#define I_100US 0xB6
#define I_500US 0xB7
#define I_1250US 0xB8
#define I_2500US 0xB9
#define I_7500US 0xBA


#define TCD1103_DATA_LEN    3000
#define TCD1103_DATA_SIZE8  3100
#define TCD1103_DATA_SIZE16 (TCD1103_DATA_SIZE8/2)

// TCD1103_EXT uint8_t TCD1103_RXD_BUF[TCD1103_DATA_SIZE8+20];
typedef union
{
    uint8_t Data8[TCD1103_DATA_SIZE8];
    uint16_t Data16[TCD1103_DATA_SIZE16];
}TYPE_UNION_TCD1103_DATA;

TCD1103_EXT TYPE_UNION_TCD1103_DATA TCD1103;
TCD1103_EXT TYPE_UNION_TCD1103_DATA TCD1103_DMABUF;

#define TCD1103_TIME_NOACK 300
TCD1103_EXT uint16_t TCD1103_Timer_noack;

#define TCD1103_STA_NUL     0
#define TCD1103_STA_OK      1
#define TCD1103_STA_ERROR   0xFF
TCD1103_EXT uint8_t TCD1103_sta;

TCD1103_EXT uint8_t TCD1103_Init(void);
TCD1103_EXT uint8_t TCD1103_GetData(void);
TCD1103_EXT void TCD1103_TimerIsr(void);
TCD1103_EXT uint8_t TCD1103_StartGetData(void);

TCD1103_EXT uint8_t TCD1103_RXD_DONE;

#endif

#endif