/*
 * I2C.h
 *
 * Created: 9/7/2020 12:33:04 AM
 *  Author: Khaled
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "Macros.h"
#include "StdTypes.h"

/***TWI Bit Rate Register***/
#define TWBR    (*(vuint8_t*)0x20)

/***TWI Control Register***/
#define TWCR    (*(vuint8_t*)0x36)
#define TWIE    0
#define TWEN    2
#define TWWC    3
#define TWSTO   4
#define TWSTA   5
#define TWEA    6
#define TWINT   7

/***TWI Status Register***/
#define TWSR    (*(vuint8_t*)0x21)
#define TWPS0   0
#define TWPS1   1
#define TWS3    3
#define TWS4    4
#define TWS5    5
#define TWS6    6
#define TWS7    7

/***TWI Data Register***/
#define TWAR    (*(vuint8_t*)0x22)

/***TWI (slave) Address Register***/
#define TWDR    (*(vuint8_t*)0x23)

/********main Functions Prototypes******/
void TWI_Init(void);
void TWI_SendByte(uint8_t SlaveAddress , uint8_t InternalReg_Address , uint8_t data);
uint8_t TWI_ReadByte(uint8_t SlaveAddress , uint8_t InternalReg_Address);


#endif /* I2C_H_ */