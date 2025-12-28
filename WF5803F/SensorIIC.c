/*
 * IIC_DEVICE_ADDR.c
 *
 *  Created on: 2020年8月4日
 *      Author: ITry
 */
#include <string.h>
#include "SoftIIc.h"
#include "SensorIIC.h"

/*
 * 连续读N个寄存器的值
 */
uint8_t IIC_ReadNByte(uint8_t Pro_Channel, uint8_t start_addr, uint8_t data_num, uint8_t *readData) {
	uint8_t i = 0;
	uint8_t err = 0;
	if (readData == NULL || data_num == 0) {
		return 1;
	}
	memset(readData, 0, (data_num * sizeof(uint8_t)));
	iic_start();
	do {
		if (iic_send_check(IIC_DEVICE_ADDR | IIC_WRITE)) {
			err = 2;
			break;
		}
		if (iic_send_check(start_addr)) {
			err = 3;
			break;
		}
		iic_start();
		if (iic_send_check(IIC_DEVICE_ADDR | IIC_READ)) {
			err = 4;
			break;
		}
		for (i = 0; i < data_num; i++) {
			readData[i] = iic_readbyte();
			if (i != data_num - 1) {
				iic_ack(); /* 读完非最后一个字节后， 产生应答信号 */
			} else {
				iic_nack(); /* 最后一个字节读完后 产生非应答信号,以提示Slave设备准备接收Stop信号 */
			}
		}
		err = 0;
	} while (0);
	iic_stop();
	return err;
}

/*
 * 连续写N个寄存器的值
 */
uint8_t IIC_WriteNByte(uint8_t Pro_Channel, uint8_t start_addr, uint8_t data_num, const uint8_t *reg_data) {
	uint8_t i = 0;
	uint8_t err = 0;
	if (reg_data == NULL || data_num == 0) {
		return 1;
	}
	iic_start();
	do {
		if (iic_send_check(IIC_DEVICE_ADDR | IIC_WRITE)) {
			err = 2;
			break;
		}
		if (iic_send_check(start_addr)) {
			err = 3;
			break;
		}
		err = 0;
		while (i < data_num) {
			if (iic_send_check(reg_data[i])) {
				err = 4;
				break;
			}
			i++;
		}
	} while (0);
	iic_stop();
	return err;
}
