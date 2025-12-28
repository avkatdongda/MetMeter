#define SHT20_GLOBAL
#include "SHT20.h"



// void SHT20_Init(void)
// {
//     SHT20_Timer_noack = SHT20_TIME_NOACK;
//     SHT20_sta = SHT20_STA_NUL;
// }

//头文件
#include "stm32f1xx_hal.h"
#include "sht20.h"
#include "i2c.h"
 
const int16_t POLYNOMIAL = 0x131;
// TYPE_STRUCT_SHT20 Sht20;
 
 
//	函数功能：	SHT20复位
void SHT20_reset(void)
{	
    HAL_I2C_Mem_Write(&SHT20_HI2C, SHT20_Write_Add, SHT20_SOFT_RESET, I2C_MEMADD_SIZE_8BIT,(void*)0, 1, 1000);
    osDelay(15);	
}
 
//函数功能：	SHT20读取用户寄存器
unsigned char  SHT20_read_user_reg(void)
{	
    unsigned char val = 0;	
    HAL_I2C_Mem_Read(&SHT20_HI2C, SHT20_Read_Add, SHT20_READ_REG, I2C_MEMADD_SIZE_8BIT,&val, 1, 1000);	
    return val;	
}
 
//函数功能：	检查数据正确性
char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum)
{	
    char crc = 0;
    char bit = 0;
    char byteCtr = 0;	
    //calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }	
    if(crc != checksum)
		return 1;
    else
		return 0;	
}
 
//*	函数功能：	温度计算
float SHT2x_CalcTemperatureC(unsigned short u16sT)
{
	
    float temperatureC = 0;            // variable for result
    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [℃] --
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; //T= -46.85 + 175.72 * ST/2^16	
    return temperatureC;	
}
 
//	函数功能：	湿度计算
float SHT2x_CalcRH(unsigned short u16sRH)
{
    float humidityRH = 0;              // variable for result
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    //humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
    humidityRH = ((float)u16sRH * 0.00190735) - 6;	
    return humidityRH;
}
 
//函数功能：	测量温湿度
float SHT2x_MeasureHM(unsigned char cmd, unsigned short *pMeasurand)
{
    char  checksum = 0;  //checksum
    char  data[2];    //data array for checksum verification
//	unsigned char addr = 0;
    unsigned short tmp = 0;
    float t = 0;
	HAL_I2C_Mem_Read(&SHT20_HI2C, SHT20_Read_Add, cmd, I2C_MEMADD_SIZE_8BIT,data,2, 0xFFFF);
	SHT2x_CheckCrc(data, 2, checksum);
    tmp = (data[0] << 8) + data[1];
    if(cmd == SHT20_Measurement_T_HM)
    {
        t = SHT2x_CalcTemperatureC(tmp);
    }
    else
    {
        t = SHT2x_CalcRH(tmp);
    }	
    if(pMeasurand)
    {
        *pMeasurand = (unsigned short)t;
    }	
    return t;	
}
//函数功能：	获取温湿度数据
uint8_t SHT20_GetValue(void)
{
	Sht20.Temprature = SHT2x_MeasureHM(SHT20_Measurement_T_HM, (void *)0);
	osDelay(70);	
	Sht20.Humidity = SHT2x_MeasureHM(SHT20_Measurement_RH_HM, (void *)0);
	osDelay(25);
	SHT20_reset();

    Sht20.T32 = (uint32_t)(Sht20.Temprature*100);
    Sht20.H32 = (uint32_t)(Sht20.Humidity*100);

     return ERR_OK;//
}


//   while (1)
//   {
// 		SHT20_GetValue();
// 		printf( "Temperature:%0.1fC,  Humidity:%0.1f%%\r\n", Sht20.tempreture,                 
//         Sht20.humidity);
// 		osDelay(1000);	  
	  
//     /* USER CODE END WHILE */
 
//     /* USER CODE BEGIN 3 */
//   }
