/*------------------------------------------------------------------------------------

SHT20传感器驱动程序

-----------------------------------------------------------------------------------*/
#ifndef _SHT20_H_
#define _SHT20_H_

#ifndef SHT20_GLOBAL
#define SHT20_EXT	extern
#else
#define SHT20_EXT
#endif

#include "bsp.h"


#define SHT20_HI2C hi2c2
// SHT20_EXT void SHT20_Init(void);
// SHT20_EXT uint8_t SHT20_GetData(void);
// SHT20_EXT void SHT20_TimerIsr(void);

/*SHT20 设备操作相关宏定义，详见手册*/
#define SHT20_ADDRESS  0X40
#define SHT20_Write_Add 0x80
#define SHT20_Read_Add	0x81
#define SHT20_Measurement_RH_HM  0XE5
#define SHT20_Measurement_T_HM  0XE3
#define SHT20_Measurement_RH_NHM  0XF5  //非主机模式地址
#define SHT20_Measurement_T_NHM  0XF3   //非主机模式地址
#define SHT20_READ_REG  0XE7
#define SHT20_WRITE_REG  0XE6
#define SHT20_SOFT_RESET  0XFE
 
typedef struct
{
	float Temprature;
	float Humidity;
	uint32_t T32;
    uint32_t H32;
}TYPE_STRUCT_SHT20;
 
SHT20_EXT TYPE_STRUCT_SHT20 Sht20;
SHT20_EXT uint8_t SHT20_GetValue(void);
 
#endif
