/*
 * I2C.c
 *
 * Created: 9/7/2020 12:33:22 AM
 *  Author: Khaled
 */ 

#include "I2C.h"
#include "DIO.h" 
#include "LCD.h"

#define BitRrate_Register		        (72)

#define WriteMode                       (0x00)
#define ReadMode                        (0x01)

/* Status codes */
#define Start_Condition_Code            (0x08)
#define Repeated_Start_Condition_Code   (0x10)
#define SLA_WR_ACK_Code					(0x18)
#define SLA_WR_NOT_ACK_Code				(0x20)
#define Data_WR_ACK_Code				(0x28)
#define SLA_R_ACK_Code					(0x40)
#define SLA_R_NOT_ACK_Code				(0x48)
#define Data_RD_ACK_Code				(0x50)
#define Data_RD_NACK_Code				(0x58)
/*
 *Master
 *SCL Freq = 100 KHz "Normal"
 *Interrupt Disable
 */

/*Static Function to send the Start Condition*/
static void TWI_SendStartCondition(void)
{
	/* Clear stop, start and ACK Enable bits and keep the state of remain bits */
	CLEARBIT(TWCR,TWSTO);
	CLEARBIT(TWCR,TWSTA);
	CLEARBIT(TWCR,TWEA);
	
	/* SET the start bit */
	SETBIT(TWCR,TWSTA);
	
	/* Clear the Interrupt Flag By Setting It */
	SETBIT(TWCR,TWINT);
	
	while (READBIT(TWCR,TWINT) == 0)
	{
		/*Wait Till Stop condition is Received by the addressed slave */
	}
	
} 

/*Static Function to send the Stop Condition*/
static void TWI_SendStopCondition(void)
{
	/* Clear stop,Start and ACK Enable bits */
	CLEARBIT(TWCR,TWSTA);
	CLEARBIT(TWCR,TWSTO);
	CLEARBIT(TWCR,TWEA);
	
	/* SET the Stop bit */
	SETBIT(TWCR,TWSTO);
	
	/* Clear the Interrupt Flag By Setting It */
	SETBIT(TWCR,TWINT);
	
	while (READBIT(TWCR,TWINT) == 0)
	{
		/*Wait Till start condition is transmitted>>to ensure that Start Cond is generated*/
	}
	
}

/*Static Function to write new data to Data Register>>TWDR*/
static void TWI_WriteData(uint8_t data)
{
	/* Clear stop,Start and ACK Enable bits */
	CLEARBIT(TWCR,TWSTA);
	CLEARBIT(TWCR,TWSTO);
	CLEARBIT(TWCR,TWEA);
	
	/* Write Data */
	TWDR = data;
	
	/* Clear the Interrupt Flag By Setting It */
	SETBIT(TWCR,TWINT);
	
	while (READBIT(TWCR,TWINT) == 0)
	{
		/* Wait Till data transmitted */
	}
	
}
/*Static Function to Read data with sending ACK*/
static uint8_t TWI_ReadData_ACK(void)
{
	/* Clear stop,Start and ACK Enable bits */
	CLEARBIT(TWCR,TWSTA);
	CLEARBIT(TWCR,TWSTO);
	CLEARBIT(TWCR,TWEA);
	
	/* Enable the Acknowledge Bit */
	SETBIT(TWCR,TWEA);
	
	/* Clear the Interrupt Flag By Setting It */
	SETBIT(TWCR,TWINT);
	
	while (READBIT(TWCR,TWINT) == 0)
	{
		/* Wait Till data is Received */
	}
	return TWDR;
	
}

/*Static Function to Read data without sending ACK*/
static uint8_t TWI_ReadData_NACK(void)
{
	/* Clear stop,Start and ACK Enable bits */
	CLEARBIT(TWCR,TWSTA);
	CLEARBIT(TWCR,TWSTO);
	CLEARBIT(TWCR,TWEA);
	
	/* Disable the Acknowledge Bit>> Stop operation*/
	
	/* Clear the Interrupt Flag By Setting It */
	SETBIT(TWCR,TWINT);
	
	while (READBIT(TWCR,TWINT) == 0)
	{
		/* Wait Till data is Received */
	}
	return TWDR;
	
}

/* static Function to get the status for the status Register */
static uint8_t TWI_GetStatus(void)
{
	
	uint8_t status = (TWSR & 0xF8);
	return status;
} 


/*Function For initializing the I2C*/
void TWI_Init(void)
{ 
	/*SET the BitRate as F_CPU=16MHz and SCL_F=100KHz
	 *From Equation >> SCL_F=CPU Clock frequency/(16+2(TWBR).4^TWPS)
	 *Where 4^TWPS is the prescaler and = 1 >> TWPS0=0,TWPS1=0
	 *So, TWBR = 72
	 */
	TWBR = BitRrate_Register;
	CLEARBIT(TWSR,TWPS0);
	CLEARBIT(TWSR,TWPS1);
	
	/*Enable I2C*/
	SETBIT(TWCR,TWEN);
}

/*Function for Sending Byte by I2C*/
void TWI_SendByte(uint8_t SlaveAddress , uint8_t InternalReg_Address , uint8_t data)
{
	/*Start Condition*/
	TWI_SendStartCondition();
	
	/* Check the ACK*/
	if (TWI_GetStatus != Start_Condition_Code)
	{
		/*Handel the Error*/
	}
	
	/*Send SLA + WR*/
	TWI_WriteData((SlaveAddress<<1) | WriteMode);
	
	/* Check the ACK*/
	if (TWI_GetStatus != SLA_WR_ACK_Code)
	{
		/*Handel the Error*/
	}
	
	/*Send Register Address*/
	TWI_WriteData(InternalReg_Address);
	
	/* Check the ACK*/
	if (TWI_GetStatus != Data_WR_ACK_Code)
	{
		/*Handel the Error*/
	}
	
	/*Write data to the addressed Register*/
	TWI_WriteData(data);
	
	/* Check the ACK*/
	if (TWI_GetStatus != Data_WR_ACK_Code)
	{
		/*Handel the Error*/
	}
	
	/* Send Stop Condition */
	TWI_SendStopCondition();
}

/*Function for Reading Byte by I2C*/
uint8_t TWI_ReadByte(uint8_t SlaveAddress , uint8_t InternalReg_Address)
{
	uint8_t ReceivedData = 0;
	
	/* Send Start Condition */
	TWI_SendStartCondition();
	/* Check the ACK*/
	if (TWI_GetStatus != Start_Condition_Code)
	{
		/*Handel the Error*/
	}
	
	/* Send SLA + WR */
	TWI_WriteData((SlaveAddress<<1) | WriteMode);
	
	/* Check the ACK*/
	if (TWI_GetStatus != SLA_WR_ACK_Code)
	{
		/*Handel the Error*/
	}
	
	/* Send Register Address */
	TWI_WriteData(InternalReg_Address);
	
	/* Check the ACK*/
	if (TWI_GetStatus != Data_WR_ACK_Code)
	{
		/*Handel the Error*/
	}
	
	/* Send Repeated Start */
	TWI_SendStartCondition();
	/* Check the ACK*/
	if (TWI_GetStatus  != Repeated_Start_Condition_Code)
	{
		/*Handel the Error*/
	}
	
	/* Send SLA + READ*/
	TWI_WriteData((SlaveAddress<<1) | ReadMode);
	
	/* Check the ACK*/
	if (TWI_GetStatus != SLA_R_ACK_Code)
	{
		/*Handel the Error*/
	}
	
	/* Read data form TWDR with NO ACK */
	ReceivedData = TWI_ReadData_NACK();
	if (TWI_GetStatus != Data_RD_NACK_Code)
	{
		/*Handel the Error*/
	}
	
	/* Send Stop Condtion */
	TWI_SendStopCondition();
	
	return ReceivedData;
}