/*
=================================================================================================================
 	模块名称 : TCD1254驱动，改自TCD1103，未改函数名
 	文件名称 : TCD1254.c
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#if 1//CCD_TYPE == CCD_TYPE_1254

#define TCD1254_GLOBAL
#include "TCD1254.h"

#define TCD1254_CMD_Q "@c0080#@" //查询命令
#define TCD1254_CMD_I "@c0000#@" //设置曝光时间积分命令

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
    HAL_UART_Transmit(&TCD1103_UART, &TCD1254_CMD_Q, strlen(TCD1254_CMD_Q),0xffff);
    TCD1103_RXD_DONE = 0;
}

void TCD1103_SetItime(void)
{
    HAL_UART_Transmit(&TCD1103_UART, &TCD1254_CMD_I, strlen(TCD1254_CMD_I),0xffff);
}

void TCD1103_QuryData(uint8_t *cmd)
{
    HAL_UART_Transmit(&TCD1103_UART, TCD1254_CMD_Q, strlen(TCD1254_CMD_Q), 0xffff);
}

uint8_t TCD1103_StartGetData(void)
{
    HAL_UART_Receive_DMA(&huart1, &TCD1103_DMABUF.Data8[0], TCD1103_DATA_SIZE8); // DMA方式接收
    return ERR_OK;
}

void TCD1103_TimerIsr(void)
{
    if(TCD1103_Timer_noack) TCD1103_Timer_noack--;
}

uint8_t TCD1103_GetData(void)
{
    uint8_t timerNoAck;
    uint16_t i;

    TCD1103_Q12();
    while (TCD1103_RXD_DONE == 0)
    {
        osDelay(10);
        timerNoAck++;
        if(timerNoAck > 200) return ERR_TIMEOUT;
    }

    for(i=0; i<TCD1103_DATA_LEN16; i++)
    {
        TCD1103.Data8[2*i] = TCD1103_DMABUF.Data8[2*i+5];
        TCD1103.Data8[2*i+1] = TCD1103_DMABUF.Data8[2*i+4];
    }
    
    return ERR_OK;

}


uint8_t TCD1103_Init(void)
{
    TCD1103_Timer_noack = TCD1103_TIME_NOACK;
    TCD1103_sta = TCD1103_STA_NUL;
    return ERR_OK;
}

#endif
