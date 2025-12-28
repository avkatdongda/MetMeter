/*
=================================================================================================================
 	模块名称 : TCD1103驱动
 	文件名称 : TCD1103.c
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#if 0//CCD_TYPE == CCD_TYPE_1103

#define TCD1103_GLOBAL
#include "TCD1103.h"


#define TCD1103_P_LEN   TCD1103_DATA_SIZE8

#define TCD1103_Q_12    0xA1 //请求12位数据指令，返回值：（数据总数为 3000 个，1500 个像素点，低八位数据在前，高八位数据在后）
#define TCD1103_Q_8     0xA2 //请求8 位数据指令，返回值：（数据总数为 1500 个，1500 个像素点）
#define TCD1103_Q_V     0xA3 //请求最大值、最小值、平均值（10-1490 像素点的平均）指令 
                             // 返回值格式：引导码，最大点坐标高位，最大点坐标低位，最小点坐标高位，最小点
                             // 坐标低位，光强平均值高位，光强平均值低位，最大点亮度值高位，最大点亮度值低位，
                             // 最小点亮度值高位，最小点亮度值低位，校验码。 
                             // 例如：0xfe,0x01,0x02, 0x03,0x04, 0x05,0x06, 0x07,0x08, 0x09,0x0A, 0x37 
#define TCD1103_P_HEAD  0xFE //引导码

void TCD1103_SendData(uint8_t *cmd)
{
    TCD1103_RXD_DONE = 0;
    comSendBuf( &TCD1103_UART, cmd, 1);
}

void TCD1103_Q12(void)
{
	uint8_t cmd;
	cmd = TCD1103_Q_12;
    // comSendBuf( &TCD1103_UART, &cmd, 1);
    HAL_UART_Transmit(&TCD1103_UART, &cmd, 1,0xffff);
    TCD1103_RXD_DONE = 0;
    // HAL_UART_Transmit_DMA(&TCD1103_UART, &cmd, 1);    //DMA发送数据
}


void TCD1103_SetItime(void)
{
	uint8_t cmd;
	cmd = TCD1103_I_TIME;
    HAL_UART_Transmit(&TCD1103_UART, &cmd, 1,0xffff);
}

void TCD1103_QuryData(uint8_t *cmd)
{
//    uint8_t cmd;
	*cmd = TCD1103_Q_12;
    HAL_UART_Transmit(&TCD1103_UART, cmd, 1,0xffff);
}

uint8_t TCD1103_StartGetData(void)
{
    HAL_UART_Receive_DMA(&huart1, &TCD1103_DMABUF.Data8[0], 3000); // DMA方式接收
    return ERR_OK;
}

uint8_t TCD1103_ComGetData(UART_HandleTypeDef * huart, uint8_t *DATA)
{
    uint8_t uartIndex;
	uint16_t i, len;
    uint8_t temC;
    uint16_t timeout;
    
    i = 0;
    temC = 0;
	timeout = 0;

//    uartIndex = TCD1103_INDEX;
	
    while(1)
    {
        len = getQueueElemNum(&uartRcvFifo[uartIndex]);
        if(len>TCD1103_DATA_LEN-1)
        {
            break;
        }
        osDelay(100);
	    timeout++;
        if(timeout > 20) 
        {
            break;
            // return 0;
        }
    }

    timeout = 0;
    
    while( timeout<200)
    {
		if(GetElemFromQueue(&uartRcvFifo[uartIndex], &temC))
    	{
			timeout = 0;
            
            DATA[i++] = temC;
            // if(i >= TCD1103_DATA_LEN)
            // {
            //     //校验
            //     return i;
            // }
    	}
		else
		{
		    osDelay(1);
		    timeout++;
		}
    }
    return i;
	// if(len>TCD1103_DATA_LEN-1)
    // {
    //   	while( timeout<200)
    //     {
	// 		if(GetElemFromQueue(&uartRcvFifo[uartIndex], &temC))
    //     	{
	// 			timeout = 0;
        	    
    //             DATA[i++] = temC;
    //             if(i >= TCD1103_DATA_LEN)
    //             {
    //                 //校验
    //                 return i;
    //             }
    //     	}
	// 		else
	// 		{
	// 		    osDelay(1);
	// 		    timeout++;
	// 		}
    //     }
    // }
	
	// return 0;
}

void TCD1103_TimerIsr(void)
{
    if(TCD1103_Timer_noack) TCD1103_Timer_noack--;
}

uint8_t TCD1103_GetData(void)
{
    uint8_t err, timerNoAck;

    TCD1103_Q12();
    while (TCD1103_RXD_DONE == 0)
    {
        osDelay(10);
        timerNoAck++;
        if(timerNoAck > 200) return ERR_TIMEOUT;
    }
    memcpy(&TCD1103.Data8[0], &TCD1103_DMABUF.Data8[0], TCD1103_DATA_SIZE8);
    // osDelay(2000);
    
    return ERR_OK;

}


uint8_t TCD1103_Init(void)
{
    TCD1103_Timer_noack = TCD1103_TIME_NOACK;
    TCD1103_sta = TCD1103_STA_NUL;
    TCD1103_SetItime();
    return ERR_OK;
}

#endif