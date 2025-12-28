/*
=================================================================================================================
 	模块名称 : TCD1254驱动，改自TCD1103，未改函数名
 	文件名称 : TCD1254.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/

#if 1//CCD_TYPE == CCD_TYPE_1254


#ifndef _TCD1254_H_
#define _TCD1254_H_

#ifndef TCD1254_GLOBAL
#define TCD1254_EXT	extern
#else
#define TCD1254_EXT
#endif

#include "bsp.h"

#define TCD1103_BUD 256000 //波特率
#define TCD1103_UART huart1 //串口

#define I_10US      0
#define I_20US      1
#define I_50US      2
#define I_100US     3
#define I_200US     4
#define I_400US     5
#define I_800US     6
#define I_1600US    7
#define I_3200US    8
#define I_6400US    9
#define I_12800US   10
#define I_32000US   11
#define I_64000US   12
#define I_128000US  13

#define TCD1103_DATA_LEN8   5000
#define TCD1103_DATA_LEN16  (TCD1103_DATA_LEN8/2)
#define TCD1103_DATA_SIZE8  5006
#define TCD1103_DATA_SIZE16 (TCD1103_DATA_SIZE8/2)

typedef union
{
    uint8_t Data8[TCD1103_DATA_SIZE8];
    uint16_t Data16[TCD1103_DATA_SIZE16];
}TYPE_UNION_TCD1103_DATA;

TCD1254_EXT TYPE_UNION_TCD1103_DATA TCD1103;
TCD1254_EXT TYPE_UNION_TCD1103_DATA TCD1103_DMABUF;

#define TCD1103_TIME_NOACK 300
TCD1254_EXT uint16_t TCD1103_Timer_noack;

#define TCD1103_STA_NUL     0
#define TCD1103_STA_OK      1
#define TCD1103_STA_ERROR   0xFF
TCD1254_EXT uint8_t TCD1103_sta;

TCD1254_EXT uint8_t TCD1103_Init(void);
TCD1254_EXT uint8_t TCD1103_GetData(void);
TCD1254_EXT void TCD1103_TimerIsr(void);
TCD1254_EXT uint8_t TCD1103_StartGetData(void);

TCD1254_EXT uint8_t TCD1103_RXD_DONE;


#endif

#endif
