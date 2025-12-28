/*
=================================================================================================================
 	模块名称 : 串口驱动
 	文件名称 : bsp_usart.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#ifndef __BSP_USART_H__
#define __BSP_USART_H__


#ifndef BSP_USART_GLOBAL
#define BSP_USART_EXT	extern
#else
#define BSP_USART_EXT
#endif

#include "bsp.h"

#include "absQueue.h"


#define DEBUG_EN

#define DEBUG_DISABLE 	0
#define DEBUG_ENABLE 	1
#define DEBUG_COM huart5
#define  DEBUG_INDEX UART5INDEX


BSP_USART_EXT uint8_t DEBUG_TEM[100];

//485收发控制
#define RS485_1_TX() 	;//HAL_GPIO_WritePin(RS485_1_EN_GPIO_Port, RS485_1_EN_Pin, GPIO_PIN_SET)
#define RS485_1_RX() 	;//HAL_GPIO_WritePin(RS485_1_EN_GPIO_Port, RS485_1_EN_Pin, GPIO_PIN_RESET)
#define RS485_2_TX() 	;//HAL_GPIO_WritePin(RS485_2_EN_GPIO_Port, RS485_2_EN_Pin, GPIO_PIN_SET)
#define RS485_2_RX() 	;//HAL_GPIO_WritePin(RS485_2_EN_GPIO_Port, RS485_2_EN_Pin, GPIO_PIN_RESET)


#define UART_IT_EN

#define UART_NUM           3	//串口数量
#define USART_REC_BUF_LEN  128 	//接收缓存大小
#define USART_TXD_BUF_LEN  128  //发送缓存大小
enum _UartIndex 	//串口索引号
{
  	// UART1INDEX = 0,
  	UART2INDEX=0,
	UART4INDEX,
	UART5INDEX,
	UARTEND
};

extern uint8_t uartBuf[UART_NUM];
extern uint8_t uartRcvBuf[UART_NUM][USART_REC_BUF_LEN];
extern QUEUE_ABS uartRcvFifo[UART_NUM];		//

extern uint8_t uartSBuf[UART_NUM];
extern uint8_t uartSndBuf[UART_NUM][USART_TXD_BUF_LEN];
extern QUEUE_ABS uartSndFifo[UART_NUM];												

#define TD_REC_BUF_LEN 3100
extern uint8_t TDRcvBuf[TD_REC_BUF_LEN];

BSP_USART_EXT void bsp_usart_int(void);
BSP_USART_EXT void comSendBuf(UART_HandleTypeDef *huart, uint8_t *data, uint16_t Size);
BSP_USART_EXT uint8_t comGetALine(UART_HandleTypeDef * huart, char *TEM);
BSP_USART_EXT void DebugPrintStr(char *data);
BSP_USART_EXT void DebugSet(uint8_t lg);
BSP_USART_EXT void DebugPrint(uint8_t *data, uint16_t size);
BSP_USART_EXT void DebugPrintStrHex(uint8_t *data, uint16_t len);
BSP_USART_EXT void DebugPrintU16StrHex(uint16_t *data, uint16_t len);

#endif  // __BSP_LED_H__
