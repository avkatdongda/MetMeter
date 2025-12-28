/*
 * SoftIIC.h
 *
 *  Created on: 2020年8月4日
 *      Author: ITry
 */

#ifndef INC_SOFTIIC_H_
#define INC_SOFTIIC_H_
#include "stm32f1xx_hal.h"

#define IIC_WRITE 	0
#define IIC_READ 	1

void iic_delay(void);
void iic_start(void);
void iic_stop(void);
void iic_ack(void);
void iic_nack(void);
void iic_sendbyte(uint8_t byte);
uint8_t iic_wait_ack(void);
uint8_t iic_readbyte(void);
uint8_t iic_send_check(uint8_t addr);

#endif /* INC_SOFTIIC_H_ */
