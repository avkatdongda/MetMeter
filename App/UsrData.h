/*
=================================================================================================================
 	模块名称 : 存储用户数据处理
 	文件名称 : UsrData.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#ifndef _USR_DATA_H
#define _USR_DATA_H

#ifndef USR_DATA_GLOBAL

#define USR_DATA_EXT extern
#else
#define USR_DATA_EXT
#endif

#include "my_task.h"

#define USR_DATA_ADD_PARA    0//系统参数保存地址 1个W25QXX_SECTOR_SIZE
#define USR_DATA_ADD_DATA    0 //W25QXX_SECTOR_SIZE//采集数据保存地址 W25QXX_SECTOR_SIZE之后

#define LEN_PARA (sizeof(Saved.Buf)/2)

USR_DATA_EXT uint16_t UsrData_Send(void);
USR_DATA_EXT void UsrData_Clear(void);
USR_DATA_EXT void UsrData_Save(void);
USR_DATA_EXT void SaveFlashData(void);
USR_DATA_EXT void ReadFlashData(void);
USR_DATA_EXT void UsrData_SaveSampleData(uint16_t n);
USR_DATA_EXT uint16_t UsrData_ReadSampleData(uint8_t *Buf);
USR_DATA_EXT uint16_t UsrData_ClearSampleData(void);
USR_DATA_EXT uint16_t UsrData_SendThreshold(void);
USR_DATA_EXT uint16_t UsrData_SandSampleData(void);
USR_DATA_EXT uint16_t UsrData_SendSampleStart(void);
USR_DATA_EXT uint16_t UsrData_BrowseSampleData(uint8_t *Buf, uint16_t n);

#endif
