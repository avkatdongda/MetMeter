/*
=================================================================================================================
 	模块名称 : BLE，wifi通信协议解析
 	文件名称 : protocol.c
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/

#define P_GLOBAL

#include "bsp.h"

#define CMD_N_MAX 10


uint8_t check_sum( uint8_t *s, uint8_t len )
{
    uint8_t sum = 0, i = 0;

    for( i=0; i<len; i++ )
    {
        sum += *( s + i );
    }

    return sum;
}

uint8_t check_crc( uint8_t *s, uint8_t len )
{
    uint8_t sum = 0, i = 0;

    for( i=0; i<len; i++ )
    {
        sum ^= *( s + i );
    }

    return sum;
}

uint8_t P_GetMsg(UART_HandleTypeDef * huart, uint8_t *TEM)
{
    uint8_t cmdType, cmdLen, devId;
    uint8_t i, uartIndex;
    uint8_t temC;//, TEM[200];
    uint16_t timeout;
    
    i = 0;
    cmdLen = 0;

    temC = 0;
	timeout = 0;
	uartIndex = 0;

//	if(huart->Instance == USART1)
//	{
//		uartIndex = UART1INDEX;
//	}else
	if(huart->Instance == USART2)
	{
		uartIndex = UART2INDEX;
	}
	
	if(getQueueElemNum(&uartRcvFifo[uartIndex]))
    {
      	while( timeout<1000)//( comGetLen(_ucPort) )
        {
			if(GetElemFromQueue(&uartRcvFifo[uartIndex], &temC))
        	{
                if(i == 0)
                {
                    if(temC == P_HEAD)
                    {
                        TEM[i++] = temC;
                    }
                }else
                if(i == 2)
                {
                    devId = temC;
                    TEM[i++] = temC;
                }else
                if(i == 3)
                {
                    cmdType = temC;
                    TEM[i++] = temC;
                    switch(cmdType)
                    {
                        case 0x01:
                            cmdLen = 23;
                            break;
                        case 0x11:
                            cmdLen = 23;
                            break;
                        default:
                            break;
                    }
                }else
                if(i < cmdLen)
        	    {
                    TEM[i++] = temC;
                }else
                {
                    if(temC == P_TAIL)
                    {
                        return i;
                    }else
                    {
                        return 0;
                    }
                }
        	}
			else
			{
			    vTaskDelay(1);
			    timeout++;
			}
        }
    }
	
	return 0;
}

#define CHK_SUM_EN      0
#define CHK_DEVID_EN    0

uint8_t P_Pares( uint8_t *msg, uint8_t len )
{
    uint8_t i, k, errCode, cmdType, devNum;
    uint8_t temH, temL;

    RTC_DateTypeDef DataTem;
    RTC_TimeTypeDef TimeTem;

   i = 0;
   k = 0;

//   uint8_t tem8;

    #if CHK_SUM_EN == 1 //

    i = check_sum( msg+1, len-2 );
    if( i!= *(msg+len-1 ) ) return 0;

    #endif

    if(*( msg + k++ ) != P_HEAD) return 0;
    // pType = *( msg + k++ ); //

    devNum = *( msg + k++ ); //

    #if CHK_DEVID_EN == 1 //

    //    if( teamNum != Team.teamNum ) return 0; //
    //    if(( devNum != 0xFF )&&( devNum > MAX_DEV_NUM )) return 0; //

    #endif

    cmdType = *( msg + k++ ); //

    //本设备命令
   switch( cmdType )
   {
       case P_Q_INFO : //通知上传采集数据
           lg_SendData = LG_SEND_SAMPLEDATA; //上传采集数据
           return 0;

       case P_S_TIME : //同步时间
            DataTem.Year= *( msg + k++ ); //
            DataTem.Month= *( msg + k++ ); //
            DataTem.Date= *( msg + k++ ); //
            TimeTem.Hours = *( msg + k++ ); //
            TimeTem.Minutes = *( msg + k++ ); //
            TimeTem.Seconds = *( msg + k++ ); //

            HAL_RTC_SetTime(&hrtc, &TimeTem, RTC_FORMAT_BIN);
            HAL_RTC_SetDate(&hrtc, &DataTem, RTC_FORMAT_BIN);

            if(devNum == 0xFF) //广播命令
            {
                if(Dev.id == 0) //主设备
                {
                    return len; //本机设置完后，转发命令
                }//else 从设备收到广播命令不回应
            }else //非广播命令
            {
                //回应设置成功
                k = 3;
                *( msg + k++ ) = ERR_OK; //
            }
            break;
       default :
           k = 0;
           break;
   }

   if(k) //
   {
       i = k;
       *( msg + 2 ) |= 0x80; //回应命令高位置1 
       *( msg + k++ ) = check_sum( msg+1, i ); //
       *( msg + k++ ) = P_TAIL; //
   }
    
   return k;

}

/******************************************************* END OF FILE ************************************************************/
