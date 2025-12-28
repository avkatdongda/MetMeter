
#define JXMO2_GLOBAL
#include "JXM_O2.h"

#define JXMO2_P_HEAD  0xFF
#define JXMO2_P_LEN   10

uint8_t STR_SET_MANUAL_MODE[] = {"\xFF\x01\x03\x02\x00\x00\x00\x00\x05"};
uint8_t STR_SET_AUTO_MODE[] =   {"\xFF\x01\x03\x01\x00\x00\x00\x00\x04"};
uint8_t STR_NOTICE[] =          {"\xFF\x01\x07\x01\x00\x00\x00\x00\x00\x3A"};

uint8_t STR_QUERY[] =           {"\xFF\x01\x07\x00\x00\x00\x00\x00\x07"};

void JXMO2_SendData(uint8_t *cmd)
{
    comSendBuf( &JXMO2_UART, cmd, 9);
}

void JXMO2_AutoStar(void)
{
    uint8_t i;
    for(i=0; i<3; i++)
    {
        JXMO2_SendData(STR_SET_AUTO_MODE);
        osDelay(100);

    }
}

void JXMO2_AutoStop(void)
{
    uint8_t i;
    for(i=0; i<3; i++)
    {
        JXMO2_SendData(STR_SET_MANUAL_MODE);
        osDelay(100);

    }
    JXMO2_sta = JXMO2_STA_STOP;
}

void JXMO2_SetAutoMode(void)
{
    uint8_t i;
    for(i=0; i<3; i++)
    {
        JXMO2_SendData(STR_SET_AUTO_MODE);
        osDelay(100);

    }
}

void JXMO2_SetManualMode(void)
{
    uint8_t i;
    for(i=0; i<3; i++)
    {
        JXMO2_SendData(STR_SET_MANUAL_MODE);
        osDelay(100);

    }
    JXMO2_sta = JXMO2_STA_STOP;
}

uint8_t JXMO2_ComGetData(UART_HandleTypeDef * huart, char *TEM)
{
    uint8_t err, uartIndex;
	uint16_t i, len;
    uint8_t temC;
    uint16_t timeout;
    
    i = 0;
    temC = 0;
	timeout = 0;
	uartIndex = JXMO2_UARTINDEX;

	// if(huart->Instance == USART1)
	// {
	// 	uartIndex = UART1INDEX;
	// }else
	// if(huart->Instance == USART2)
	// {
	// 	uartIndex = UART2INDEX;
	// }
	
	len = getQueueElemNum(&uartRcvFifo[uartIndex]);
	if(len>3)
    {
      	while( timeout<200)//
        {
			if(GetElemFromQueue(&uartRcvFifo[uartIndex], &temC))
        	{
				timeout = 0;
        	    
                if(i == 0)
                {
                    if(temC == JXMO2_P_HEAD) //帧头
                    {
                        TEM[i++] = temC;
                    }
                }else
                {
                    TEM[i++] = temC;
                    if(i >= JXMO2_P_LEN)
                    {
                        //校验
                        err = check_sum(TEM+1, JXMO2_P_LEN-2)-1;
                        // err = ((~err)+1);
                        if(temC == err)
                        {
                            return i;
                        }
                        else
                        {
                            TEM[4] = 0;
                            TEM[5] = 0;
                            return 0;
                        }
                    }
                }
        	}
			else
			{
			    osDelay(1);
			    timeout++;
			}
        }
    }
	
	return 0;
}


void JXMO2_TimerIsr(void)
{
    if(JXMO2_Timer_noack) JXMO2_Timer_noack--;
}

uint8_t JXMO2_GetData(void)
{
    uint8_t err, TEM[20];

    // if(JXMO2_sta != JXMO2_STA_AUTO) JXMO2_AutoStar();

    JXMO2_SendData(STR_QUERY);

    err = JXMO2_ComGetData(&JXMO2_UART, TEM);
    if(err)
    {
        // DebugPrintStrHex(TEM,10);
        if(TEM[2] == 0x07)
        {
            
            JXMO2_sta = JXMO2_STA_AUTO;
            JXMO2_Timer_noack = JXMO2_TIME_NOACK;

            
            JXMO2.resolution  =  TEM[3];
            JXMO2.O2  = (((uint16_t)TEM[4])<<8) + TEM[5];
            JXMO2.Oxygen = (float)(JXMO2.O2/10);
            return ERR_OK;
        }
    }else
    {
        // DebugPrintStr("JXMO2_GetData error\r\n");
    }

    if(JXMO2_Timer_noack == 0)
    {
        if(JXMO2_sta == JXMO2_STA_AUTO) return ERR_TIMEOUT;
    }
    
    return ERR_IDEL;
}


uint8_t JXMO2_Init(void)
{
   uint8_t err;
    
    JXMO2_SetManualMode();
    JXMO2_sta = JXMO2_STA_MANUAL;

    return err;
}
