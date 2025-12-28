#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "bsp.h"
#include "lcd.h"
#include "spi.h"

#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#define LCD_W 320
#define LCD_H 240



//-----------------LCD端口---------------- 
#define LCDT_SCL_PORT   GPIOB
#define LCDT_SCL_PIN    GPIO_PIN_13

#define LCDT_SDA_PORT   GPIOB
#define LCDT_SDA_PIN    GPIO_PIN_15

#define LCDT_RES_PORT   GPIOC
#define LCDT_RES_PIN    GPIO_PIN_7

#define LCDT_DC_PORT   GPIOB
#define LCDT_DC_PIN    GPIO_PIN_14

#define LCDT_CS_PORT   GPIOB
#define LCDT_CS_PIN    GPIO_PIN_12

#define LCDT_BLK_PORT   GPIOC
#define LCDT_BLK_PIN    GPIO_PIN_6

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(LCDT_SCL_PORT,LCDT_SCL_PIN, GPIO_PIN_RESET)//SCL=SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(LCDT_SCL_PORT,LCDT_SCL_PIN, GPIO_PIN_SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(LCDT_SDA_PORT,LCDT_SDA_PIN, GPIO_PIN_RESET)//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(LCDT_SDA_PORT,LCDT_SDA_PIN, GPIO_PIN_SET)

#define LCD_RES_Clr()  HAL_GPIO_WritePin(LCDT_RES_PORT,LCDT_RES_PIN, GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(LCDT_RES_PORT,LCDT_RES_PIN, GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(LCDT_DC_PORT,LCDT_DC_PIN, GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(LCDT_DC_PORT,LCDT_DC_PIN, GPIO_PIN_SET)
 		     
#define LCD_CS_Clr()   HAL_GPIO_WritePin(LCDT_CS_PORT,LCDT_CS_PIN, GPIO_PIN_RESET)//CS
#define LCD_CS_Set()   HAL_GPIO_WritePin(LCDT_CS_PORT,LCDT_CS_PIN, GPIO_PIN_SET)

#define LCD_BLK_Clr()  HAL_GPIO_WritePin(LCDT_BLK_PORT,LCDT_BLK_PIN, GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(LCDT_BLK_PORT,LCDT_BLK_PIN, GPIO_PIN_SET)


#define LCD_BLK_OFF()  LCD_BLK_Clr()
#define LCD_BLK_ON()   LCD_BLK_Set()


void LCD_GPIO_Init(void);//��ʼ��GPIO
void LCD_Writ_Bus(uint8_t dat);//ģ��SPIʱ��
void LCD_WR_DATA8(uint8_t dat);//д��һ���ֽ�
void LCD_WR_DATA(uint16_t dat);//д�������ֽ�
void LCD_WR_REG(uint8_t dat);//д��һ��ָ��
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//�������꺯��
void LCD_Init(void);//LCD��ʼ��
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Sleep(void);
void LCD_Wake(void);

#endif




