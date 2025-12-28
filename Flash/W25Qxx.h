/* 文件： w25flash.h
 * 功能描述： Flash 存储器W25Q128的驱动程序
 * W25Q128 芯片参数： 16M字节，24位地址线
 * 分为256个Block，每个Block 64K字节
 * 一个Block又分为16个Sector，共4096个Sector，每个Sector 4K字节
 * 一个Sector又分为16个Page，共65536个Page，每个Page 256字节
 * 写数据操作的基本单元是Page，一次连续写入操作不能超过一个Page的范围。写的Page必须是擦除过的。
 */
 
#ifndef _W25QXX_H
#define _W25QXX_H
 
#include 	"stm32f1xx_hal.h"
#include	"spi.h"		//使用其中的变量 hspi1，表示SPI1接口

// Flash参数
#define W25QXX_SECTOR_SIZE             4096  // 4KB扇区
#define W25QXX_PAGE_SIZE               256   // 256字节页

/*  W25Q128硬件接口相关的部分：CS引脚和SPI接口 ，若电路不同，更改这部分配置即可   */
// Flash_CS -->PA15, 片选信号CS操作的宏定义函数
#define CS_PORT		FLASH_CS_GPIO_Port
#define	CS_PIN		FLASH_CS_Pin
#define	SPI_HANDLE  hspi3		//SPI接口对象，使用spi.h中的变量 hspi1
 
#define	__Select_Flash()		HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET)	//CS=0
#define	__Deselect_Flash()		HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET)	//CS=1

#define	W25QXX_HSPI  hspi3		//SPI接口对象，使用spi.h中的变量 hspi1
#define	W25QXX_CS_LOW		HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET)	//CS=0
#define	W25QXX_CS_HIGH		HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET)	//CS=1

#define READ_ID_CMD                 0x90
#define DUAL_READ_ID_CMD            0x92
#define QUAD_READ_ID_CMD            0x94
#define READ_JEDEC_ID_CMD           0x9F

#define READ_CMD                    0x03
#define FAST_READ_CMD               0x0B
#define DUAL_OUT_FAST_READ_CMD      0x3B
#define DUAL_INOUT_FAST_READ_CMD    0xBB
#define QUAD_OUT_FAST_READ_CMD      0x6B
#define QUAD_INOUT_FAST_READ_CMD    0xEB

#define WRITE_ENABLE_CMD            0x06
#define WRITE_DISABLE_CMD           0x04

#define READ_STATUS_REG1_CMD        0x05
#define READ_STATUS_REG2_CMD        0x35
#define READ_STATUS_REG3_CMD        0x15

#define WRITE_STATUS_REG1_CMD       0x01
#define WRITE_STATUS_REG2_CMD       0x31
#define WRITE_STATUS_REG3_CMD       0x11

#define PAGE_PROG_CMD               0x02
#define QUAD_INPUT_PAGE_PROG_CMD    0x32

#define SECTOR_ERASE_CMD            0x20
#define CHIP_ERASE_CMD              0xC7
#define PROG_ERASE_RESUME_CMD       0x7A
#define PROG_ERASE_SUSPEND_CMD      0x75

uint16_t W25Qxx_ReadID(void);
void W25Qxx_Wait_Busy(void);
void W25Qxx_Write_Enable(void);
void W25Qxx_Write_Disable(void);
uint8_t W25Qx_Write(uint32_t WriteAddr, uint8_t* pData, uint32_t Size);
uint8_t W25Qx_Read(uint32_t ReadAddr, uint8_t* pData, uint32_t Size);
uint8_t W25Qxx_Save(uint32_t address, uint8_t *data, uint16_t length);

uint8_t Erase_Sector(uint32_t sector_addr);
void W25xx_Demo(void);

#endif
