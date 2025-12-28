/*
 * SoftIIC.c
 *
 *  Created on: Aug 3, 2020
 *      Author: ITry
 */
#include "SoftIIC.h"

//SDA的IO方向(输入/输出)
//  #define SoftI2CSdaIn()	{GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=(uint32_t)8<<4;}
//  #define SoftI2CSdaOut()	{GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=(uint32_t)3<<4;}

#define SoftI2CSdaIn()	{GPIOB->CRH|=(uint32_t)8<<4;}
 #define SoftI2CSdaOut()	{GPIOB->CRH|=(uint32_t)3<<4;}

//设置高低电平
#define SetSoftI2CSDA() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)
#define ClrSoftI2CSDA() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
#define SetSoftI2CSCL() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)
#define ClrSoftI2CSCL() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)
//读取高低电平
#define SoftI2C_SDA_H() (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET)
#define SoftI2C_SDA_L() (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET)
#define SoftI2C_SCL_H() (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_SET)

/**
 * 用于模拟IIC时的简单延时
 */
void iic_delay(void) {
	uint8_t i = 0;
	uint8_t delay = 15;

	while (delay--) {
		i = 10;
		while (i--)
			;
	}
}

/**
 *iic启动:当SCL处于高电平状态时，SDA出现一个下降沿,即产生IIC启动信号
 */
void iic_start(void) {
	SoftI2CSdaOut();
	SetSoftI2CSCL();
	SetSoftI2CSDA();
	iic_delay();
	ClrSoftI2CSDA();
	iic_delay();
	ClrSoftI2CSCL(); /*拉低准备发送数据 */
	iic_delay();
}

/**
 *iic停止:当SCL处于高电平状态时，SDA出现一个上升沿,即产生IIC停止信号
 */
void iic_stop(void) {
	SoftI2CSdaOut();
	ClrSoftI2CSCL();
	//iic_delay();
	ClrSoftI2CSDA();
	iic_delay();
	SetSoftI2CSCL();
	//iic_delay();
	SetSoftI2CSDA();
	iic_delay();
}

/**
 *IIC等待应答信号
 *ack_status： 应答状态，0表示应答，1表示设备无响应
 */
uint8_t iic_wait_ack(void) {
	uint8_t ack_status = 0;
	uint8_t err_time = 0;
	SoftI2CSdaIn();
	/** 在等待应答信号之前，要释放总线，即将SDA置位 */
	SetSoftI2CSDA();
	iic_delay();
	SetSoftI2CSCL();
	iic_delay();
	while (SoftI2C_SDA_H()) {
		if (err_time++ > 250) {
			break;
		}
	}
	if (err_time > 250) {
		ack_status = 1;
	//	iic_stop();

	} else {
		ack_status = 0;
		ClrSoftI2CSCL();
		iic_delay();
	}
	return ack_status;
}

/**
 *主机（主控制器）产生应答信号
 */
void iic_ack(void) {
	SoftI2CSdaOut();
	ClrSoftI2CSDA();
	iic_delay();
	SetSoftI2CSCL();
	iic_delay();
	ClrSoftI2CSCL();
	iic_delay();
	SetSoftI2CSDA();
}

/**
 * 主机（主控制器）产生不应答信号
 */
void iic_nack(void) {
	SoftI2CSdaOut();
	SetSoftI2CSDA();
	iic_delay();
	SetSoftI2CSCL();
	iic_delay();
	ClrSoftI2CSCL();
	iic_delay();
}

/**
 *IIC发送1byte数据
 */
void iic_sendbyte(uint8_t byte) {
	uint8_t i;
	SoftI2CSdaOut();
	/** 发送一个字节的高7位 */
	for (i = 0; i < 8; i++) {
		if (byte & 0x80) {
			SetSoftI2CSDA();
		} else {
			ClrSoftI2CSDA();
		}
		iic_delay();
		SetSoftI2CSCL();
		iic_delay();
		ClrSoftI2CSCL();
		byte <<= 1;
		iic_delay();
	}
}

/**
 *IIC读取1byte数据
 */
uint8_t iic_readbyte(void) {
	uint8_t i;
	uint8_t recv_value = 0;
	SetSoftI2CSDA();
	iic_delay();
	SoftI2CSdaIn();
	for (i = 0; i < 8; i++) {
		SetSoftI2CSCL();
		iic_delay();
		recv_value <<= 1;
		if (SoftI2C_SDA_H()) {
			recv_value |= 0x01;
		} else {
			recv_value &= ~0x01;
		}
		iic_delay();
		ClrSoftI2CSCL();
	}
	return recv_value;
}

/**
 *检测IIC总线上的设备状态
 *ack_status： 0 （正常）or 1（异常）
 *主机发送设备地址等待从机应答，若有从机正确的应答信号,则表明IIC总线上挂接了设备，否则表示IIC总线上未检测到 设备
 */
uint8_t iic_check_device_status(uint8_t device_addr) {
	uint8_t ack_status;
	if (SoftI2C_SCL_H() && SoftI2C_SDA_H()) {
		iic_start();
		iic_sendbyte(device_addr | IIC_WRITE);
		ack_status = iic_wait_ack();
		iic_stop();
		return ack_status;
	}
	return 1;
}

/*
 * ack_status： 0 （正常）or 1（异常）
 */
uint8_t iic_send_check(uint8_t dat) {
	uint8_t ack_status = 1;
	iic_sendbyte(dat);
	ack_status = iic_wait_ack();
	return ack_status;
}
