#define WF5803F_GLOBAL
#include "WF5803F.h"

#include <stdio.h>
#include <string.h>
#include <SensorIIC.h>

extern I2C_HandleTypeDef hi2c1;

void WF5803F_Init(void)
{
    WF5803F_Timer_noack = WF5803F_TIME_NOACK;
//    WF5803F_sta = WF5803F_STA_NUL;
}


// void WF5803F_Read(void)
// {
// 	uint8_t sendbuf[64];
// 	uint8_t buf[5];
// 	uint32_t dat;
// 	uint8_t err;
// 	float fDat, Press_Data, Temp_Data;
// 	uint64_t readingTemp64;

// 	// while (1) 
// 	{
// 		buf[0] = 0X08;
// 		if ((err = IIC_WriteNByte(0, 0X30, 0x01, buf)) == 0) {
// 			osDelay(200);
// 			buf[0] = 0X09;
// 			if ((err = IIC_WriteNByte(0, 0X30, 0x01, buf)) == 0) {
// 				osDelay(200);
// 				if ((err = IIC_ReadNByte(0, 0X06, 0x05, buf)) == 0) {
// 					dat = buf[0];
// 					dat = dat << 8;
// 					dat |= buf[1];
// 					dat = dat << 8;
// 					dat |= buf[2];
// 					if (dat > 8388608) {
// 						fDat = (int32_t)(dat - 16777216) / 8388608.0f;
// 					} else {
// 						fDat = dat / 8388608.0f;
// 					}
// 					Press_Data = 180 / 0.81 * (fDat - 0.1) + 30;
// 					//
// 					dat = buf[3];
// 					dat = dat << 8;
// 					dat |= buf[4];
// 					if (dat > 32768) {
// 						Temp_Data = (dat - 65536) / 256.0f;
// 					} else {
// 						Temp_Data = dat / 256.0f;
// 					}
// 					// sprintf((char*) sendbuf, "0x%02x,0x%02x,0x%02x,0x%02x,0x%02x = %5.2fkPa,%5.2f C",
// 					// 		buf[0], buf[1], buf[2], buf[3], buf[4], Press_Data, Temp_Data);
// 					// HAL_UART_Transmit(&huart1, sendbuf, strlen((char*) &sendbuf), 1000);
// 					// sprintf((char*) sendbuf, "P=%5.2fkPa,T=%5.2f",Press_Data, Temp_Data);
//                     WF5803F.Pressure = Press_Data;
//                     WF5803F.Temperature = Temp_Data;
                    
// 				} else {
// 					// sprintf((char*) sendbuf, "error3:0x%02x", err);
// 					// HAL_UART_Transmit(&huart1, sendbuf, strlen((char*) &sendbuf), 1000);
// 				}
// 			} else {
// 				// sprintf((char*) sendbuf, "error2:0x%02x", err);
// 				// HAL_UART_Transmit(&huart1, sendbuf, strlen((char*) &sendbuf), 1000);
// 			}
// 		} else {
// 			// sprintf((char*) sendbuf, "error1:0x%02x", err);
// 			// HAL_UART_Transmit(&huart1, sendbuf, strlen((char*) &sendbuf), 1000);
// 		}
// 		osDelay(1000);
// 	}

// }


#define WF5803_ADDRESS 0xDA

uint8_t WF5803F_Read(void)
{
	uint8_t sendbuf[64];
	uint8_t buf[5];
	uint32_t dat;
	float fDat, Press_Data, Temp_Data;
	uint64_t readingTemp64;

	// while (1) 
	{
		buf[0] = 0x30;
		buf[1] = 0X0A;
		if(HAL_I2C_Master_Transmit(&hi2c1, WF5803_ADDRESS, buf, 2, 100) != HAL_OK){
			return ERR_ERROR;//TRANSMIT_FAIL;
		}

		

		while(buf[0] != 1)
		{
			buf[0] = 0x02;
			if(HAL_I2C_Master_Transmit(&hi2c1, WF5803_ADDRESS, buf, 1, 100) != HAL_OK){
				return 1;//TRANSMIT_FAIL;
			}
			if(HAL_I2C_Master_Receive(&hi2c1, WF5803_ADDRESS, buf, 1, 100) != HAL_OK){
			return ERR_ERROR;//RECEIVE_FAIL;
			}
		}
		
		buf[0] = 0x06;
		if(HAL_I2C_Master_Transmit(&hi2c1, WF5803_ADDRESS, buf, 1, 100) != HAL_OK){
			return ERR_ERROR;//TRANSMIT_FAIL;
		}
		if(HAL_I2C_Master_Receive(&hi2c1, WF5803_ADDRESS, buf, 5, 100) != HAL_OK){
		return ERR_ERROR;//RECEIVE_FAIL;
		}
		

		// if ((err = IIC_WriteNByte(0, 0X30, 0x01, buf)) == 0) 
		{
			// osDelay(1000);
			
			{
				osDelay(10);
				// if ((err = IIC_ReadNByte(0, 0X06, 0x05, buf)) == 0) 
				{
					
					dat = buf[0];
					dat = dat << 8;
					dat |= buf[1];
					dat = dat << 8;
					dat |= buf[2];
					if (dat > 8388608) {
						fDat = (int32_t)(dat - 16777216) / 8388608.0f;
					} else {
						fDat = dat / 8388608.0f;
					}
					Press_Data = 180 / 0.81 * (fDat - 0.1) + 30;
					//
					dat = buf[3];
					dat = dat << 8;
					dat |= buf[4];
					if (dat > 32768) {
						Temp_Data = (dat - 65536) / 256.0f;
					} else {
						Temp_Data = dat / 256.0f;
					}
					// sprintf((char*) sendbuf, "0x%02x,0x%02x,0x%02x,0x%02x,0x%02x = %5.2fkPa,%5.2f C",
					// 		buf[0], buf[1], buf[2], buf[3], buf[4], Press_Data, Temp_Data);
					// HAL_UART_Transmit(&huart1, sendbuf, strlen((char*) &sendbuf), 1000);
					// sprintf((char*) sendbuf, "P=%5.2fkPa,T=%5.2f",Press_Data, Temp_Data);
                    WF5803F.Pressure = Press_Data;
                    WF5803F.Temperature = Temp_Data;
					
					WF5803F.T32 = (uint32_t)(WF5803F.Temperature*100);
					WF5803F.P32 = (uint32_t)(WF5803F.Pressure*1000);

                    return ERR_OK;//
				}
			} 
		osDelay(1000);
		}
	}

}

