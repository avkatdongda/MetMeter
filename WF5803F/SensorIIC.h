/*
 * SensorIIC.h
 *
 *  Created on: 2020年8月4日
 *      Author: ITry
 */

#ifndef INC_SENSORIIC_H_
#define INC_SENSORIIC_H_

#define IIC_DEVICE_ADDR		0xDA

uint8_t IIC_ReadNByte(uint8_t Pro_Channel, uint8_t start_addr, uint8_t data_num, uint8_t *readData);
uint8_t IIC_WriteNByte(uint8_t Pro_Channel, uint8_t start_addr, uint8_t data_num, const uint8_t *reg_data);

#endif /* INC_SENSORIIC_H_ */
