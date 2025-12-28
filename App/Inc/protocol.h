/*
=================================================================================================================
 	模块名称 : BLE，wifi通信协议解析
 	文件名称 : protocol.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/

#ifndef __P_H
#define __P_H

#ifndef P_GLOBAL
#define P_EXT	extern
#else
#define P_EXT
#endif

#define P_HEAD  0x5B //0xFA
#define P_TAIL  0x5D //0x5F
#define P_LEN   18
#define P_ACK   0x80
#define P_CMD_NOTE 0x01 //主动上报采集数据

#define P_Q_INFO 0x01 // 查询采集数据	0x01
// 查询设备信息回应	0x81
#define P_S_TIME 0x02 // 同步时间	0x02
// 同步时间回应	0x82

P_EXT uint8_t check_sum( uint8_t *s, uint8_t len );
P_EXT uint8_t P_GetMsg(UART_HandleTypeDef * huart, uint8_t *TEM);
P_EXT uint8_t P_Pares( uint8_t *msg, uint8_t len );

#endif
