/*
=================================================================================================================
 	模块名称 : 串口驱动
 	文件名称 : bsp_usart.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#define BSP_USART_GLOBAL

#include "bsp.h"


uint8_t uartBuf[UART_NUM];													
uint8_t uartRcvBuf[UART_NUM][USART_REC_BUF_LEN];                               
QUEUE_ABS uartRcvFifo[UART_NUM];

uint8_t uartSBuf[UART_NUM];														
uint8_t uartSndBuf[UART_NUM][USART_TXD_BUF_LEN];                               
QUEUE_ABS uartSndFifo[UART_NUM];												

uint8_t TDRcvBuf[TD_REC_BUF_LEN];

//串口缓存队列初始化
void bsp_usart_int(void)
{
	queueInit(&uartRcvFifo[UART2INDEX], &(uartRcvBuf[UART2INDEX][0]), sizeof(uint8_t),USART_REC_BUF_LEN);
	uartBuf[UART2INDEX] = 0;
	queueInit(&uartSndFifo[UART2INDEX], &(uartSndBuf[UART2INDEX][0]), sizeof(uint8_t),USART_TXD_BUF_LEN);
	uartSBuf[UART2INDEX] = 0;

	queueInit(&uartRcvFifo[UART4INDEX], &(uartRcvBuf[UART4INDEX][0]), sizeof(uint8_t),USART_REC_BUF_LEN);
	uartBuf[UART4INDEX] = 0;
	queueInit(&uartSndFifo[UART4INDEX], &(uartSndBuf[UART4INDEX][0]), sizeof(uint8_t),USART_TXD_BUF_LEN);
	uartSBuf[UART4INDEX] = 0;
	
	queueInit(&uartRcvFifo[UART5INDEX], &(uartRcvBuf[UART5INDEX][0]), sizeof(uint8_t),USART_REC_BUF_LEN);
	uartBuf[UART5INDEX] = 0;
	queueInit(&uartSndFifo[UART5INDEX], &(uartSndBuf[UART5INDEX][0]), sizeof(uint8_t),USART_TXD_BUF_LEN);
	uartSBuf[UART5INDEX] = 0;
	

  	// HAL_UART_Receive_IT( &huart1, &uartBuf[UART1INDEX], 1);
	HAL_UART_Receive_IT( &huart2, &uartBuf[UART2INDEX], 1);
	HAL_UART_Receive_IT( &huart4, &uartBuf[UART4INDEX], 1);
	HAL_UART_Receive_IT( &huart5, &uartBuf[UART5INDEX], 1);
	
}

//串口接收回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		TCD1103_RXD_DONE = 1;
		TCD1103_StartGetData();
	}else
    if(huart->Instance == USART2){
		PushElemToQueue(&uartRcvFifo[UART2INDEX], (void *)&uartBuf[UART2INDEX]);
		HAL_UART_Receive_IT(&huart2,(uint8_t *)&uartBuf[UART2INDEX],1);
	}else
    if(huart->Instance == UART4){
		PushElemToQueue(&uartRcvFifo[UART4INDEX], (void *)&uartBuf[UART4INDEX]);
		HAL_UART_Receive_IT(&huart4,(uint8_t *)&uartBuf[UART4INDEX],1);
	}else
    if(huart->Instance == UART5){
		PushElemToQueue(&uartRcvFifo[UART5INDEX], (void *)&uartBuf[UART5INDEX]);
		HAL_UART_Receive_IT(&huart5,(uint8_t *)&uartBuf[UART5INDEX],1);
	}
}



#ifdef UART_IT_EN

void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart) {
   
	#ifdef UART_IT_EN //

    // if(huart->Instance == USART1){
	// 	if(GetElemFromQueue(&uartSndFifo[UART1INDEX], &uartSBuf[UART1INDEX])){
	// 		HAL_UART_Transmit_IT(&huart1,(uint8_t *)&uartSBuf[UART1INDEX],1);
	// 	}
	// }else
	if(huart->Instance == USART2){
		if(GetElemFromQueue(&uartSndFifo[UART2INDEX], &uartSBuf[UART2INDEX])){
			HAL_UART_Transmit_IT(&huart2,(uint8_t *)&uartSBuf[UART2INDEX],1);
		}
	}else
	if(huart->Instance == UART4){
		if(GetElemFromQueue(&uartSndFifo[UART4INDEX], &uartSBuf[UART4INDEX])){
			HAL_UART_Transmit_IT(&huart4,(uint8_t *)&uartSBuf[UART4INDEX],1);
		}
	}else
	if(huart->Instance == UART5){
		if(GetElemFromQueue(&uartSndFifo[UART5INDEX], &uartSBuf[UART5INDEX])){
			HAL_UART_Transmit_IT(&huart5,(uint8_t *)&uartSBuf[UART5INDEX],1);
		}
	}

	#endif
}

#endif

void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart){

	// overrun
	 if (huart->ErrorCode == HAL_UART_ERROR_ORE) {
		__HAL_UART_CLEAR_OREFLAG(huart);
		__HAL_UART_CLEAR_NEFLAG(huart);
		__HAL_UART_CLEAR_FEFLAG(huart);
		/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
		__HAL_UART_DISABLE_IT(huart, UART_IT_ERR);
	 }
	
	
	//The most important thing when UART framing error occur/any error is restart the RX process 
	// if(huart->Instance == USART1){
	// //Restarting the RX, .. 1 byte. .. u8DATUartShortRxBuffer is My own rx buffer  
	// 	__HAL_UNLOCK(huart);
	// 	HAL_UART_Receive_IT(&huart1,(uint8_t *)&uartBuf[UART1INDEX],1);
	// }else 
	if(huart->Instance == USART2){
	//Restarting the RX, .. 1 byte. 
		__HAL_UNLOCK(huart);
		HAL_UART_Receive_IT(&huart2,(uint8_t *)&uartBuf[UART2INDEX],1);
	}else 
	if(huart->Instance == UART4){
	//Restarting the RX, .. 1 byte. 
		__HAL_UNLOCK(huart);
		HAL_UART_Receive_IT(&huart4,(uint8_t *)&uartBuf[UART4INDEX],1);
	}else 
	if(huart->Instance == UART5){
	//Restarting the RX, .. 1 byte. 
		__HAL_UNLOCK(huart);
		HAL_UART_Receive_IT(&huart5,(uint8_t *)&uartBuf[UART5INDEX],1);
	}
	
}

void comSendBuf(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
	uint16_t i;

#ifdef UART_IT_EN //发送中断使能

// 	if(huart->Instance == USART1)
// 	{
// //		if(osMutexWait(myMutexU1TxdHandle,osWaitForever)==osOK)
// 		{
// 			for(i=0 ; i<size; i++)
// 			{
// 				PushElemToQueue(&uartSndFifo[UART1INDEX], &data[i]);
// 			}
// 			osDelay(5);
// 			// GetElemFromQueue(&uartSndFifo[UART1INDEX], &uartSBuf[UART1INDEX]);
// 			// HAL_UART_Transmit_IT(huart,(uint8_t *)&uartSBuf[UART1INDEX],1);
			
// 			if(GetElemFromQueue(&uartSndFifo[UART1INDEX], &uartSBuf[UART1INDEX]))
// 			{
// 				HAL_UART_Transmit_IT(huart,(uint8_t *)&uartSBuf[UART1INDEX],1);
// 			}

// //			osMutexRelease(myMutexU1TxdHandle);
// 		}
// 	}else
	if(huart->Instance == USART2)
	{
		for(i=0 ; i<size; i++)
		{
			PushElemToQueue(&uartSndFifo[UART2INDEX], &data[i]);
		}
		// HAL_UART_Transmit_IT(huart, data, 1);
		osDelay(5);
		// GetElemFromQueue(&uartSndFifo[UART2INDEX], &uartSBuf[UART2INDEX]);
		// HAL_UART_Transmit_IT(huart,(uint8_t *)&uartSBuf[UART2INDEX],1);

		if(GetElemFromQueue(&uartSndFifo[UART2INDEX], &uartSBuf[UART2INDEX]))
		{
			HAL_UART_Transmit_IT(huart,(uint8_t *)&uartSBuf[UART2INDEX],1);
		}
	}else
	if(huart->Instance == UART4)
	{
		for(i=0 ; i<size; i++)
		{
			PushElemToQueue(&uartSndFifo[UART4INDEX], &data[i]);
		}
		osDelay(5);

		if(GetElemFromQueue(&uartSndFifo[UART4INDEX], &uartSBuf[UART4INDEX]))
		{
			HAL_UART_Transmit_IT(huart,(uint8_t *)&uartSBuf[UART4INDEX],1);
		}
	}else
	if(huart->Instance == UART5)
	{
		for(i=0 ; i<size; i++)
		{
			PushElemToQueue(&uartSndFifo[UART5INDEX], &data[i]);
		}
		osDelay(5);
		if(GetElemFromQueue(&uartSndFifo[UART5INDEX], &uartSBuf[UART5INDEX]))
		{
			HAL_UART_Transmit_IT(huart,(uint8_t *)&uartSBuf[UART5INDEX],1);
		}
	}
	
#else
	HAL_UART_Transmit(huart, data, size, 1000); //阻�?�发送模�??
	while(HAL_UART_GetState(&huart) == HAL_UART_STATE_BUSY_TX);//检测UART发送结�??

#endif

}

uint8_t comGetALine(UART_HandleTypeDef * huart, char *TEM)
{
    uint8_t uartIndex;
	uint16_t i, len;
    uint8_t temC;//, TEM[200];
    uint16_t timeout;
    
    i = 0;
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
	}else
	if(huart->Instance == UART4)
	{
		uartIndex = UART4INDEX;
	}else
	if(huart->Instance == UART5)
	{
		uartIndex = UART5INDEX;
	}
	
	len = getQueueElemNum(&uartRcvFifo[uartIndex]);
	if(len>2)
    {
      	while( timeout<200)//
        {
			if(GetElemFromQueue(&uartRcvFifo[uartIndex], &temC))
        	{
				timeout = 0;
        	    TEM[i++] = temC;
        	    if(temC==0x0A)
                {
					if( i>2 )
					{
						// if( TEM[i-2] == 0x0D )//检测到AT指令
						{
                            TEM[i++] = 0;
                            return i-1;
                        }
                    }else
					{
						return 0;
					}
                }
        	}
			else
			{
			    osDelay(1);//vTaskDelay(1);
			    timeout++;
			}
        }
    }
	
	return 0;
}

//Debug 输出函数------------------------------------------------------------------------------

uint8_t deugEn; 
//Debug使能控制
void DebugSet(uint8_t lg)
{
	deugEn = lg;
}

void DebugPrint(uint8_t *data, uint16_t size)
{
	#ifdef DEBUG_EN

	if( deugEn )
	{
		comSendBuf(&DEBUG_COM, data, size);
	}

	#endif
}

void DebugPrintStr(char *data)
{
	#ifdef DEBUG_EN
	
	if( deugEn )
	{
		comSendBuf(&DEBUG_COM, (uint8_t*)data, strlen(data));

	}
	
	#endif
}

uint8_t HEX[]="0123456789ABCDEF";
void DebugPrintStrHex(uint8_t *data, uint16_t len)
{
	#ifdef DEBUG_EN
	uint8_t tem8;
	uint16_t i;
	if( deugEn )
	{
		for(i=0; i<len; i++)
		{
			tem8 = *(data+i);
			
			comSendBuf(&DEBUG_COM, &HEX[(tem8>>4)&0x0F], 1);
			comSendBuf(&DEBUG_COM, &HEX[tem8&0x0F], 1);
			comSendBuf(&DEBUG_COM, " ", 1);
		}
	}

	#endif
}

void DebugPrintU16StrHex(uint16_t *data, uint16_t len)
{
	#ifdef DEBUG_EN
	uint8_t tem8, temH, temL;
	uint16_t tem16, i;
	
	if( deugEn )
	{
		for(i=0; i<len; i++)
		{
			tem16 = *(data+i);
			temL = (uint8_t)(tem16&0x00FF);
			temH = (uint8_t)((tem16>>8)&0x00FF);
			
			comSendBuf(&DEBUG_COM, &HEX[(temH>>4)&0x0F], 1);
			comSendBuf(&DEBUG_COM, &HEX[temH&0x0F], 1);
			comSendBuf(&DEBUG_COM, &HEX[(temL>>4)&0x0F], 1);
			comSendBuf(&DEBUG_COM, &HEX[temL&0x0F], 1);
			comSendBuf(&DEBUG_COM, " ", 1);
		}
	}

	#endif
}

