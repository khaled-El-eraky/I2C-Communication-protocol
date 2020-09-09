/*
 * Microcontroller_config.h
 * <This File Contains ALL ATmega32A Register Address, Memory Map Information and >
 
 * Created: 2/5/2020 2:22:29 PM
 *  Author: Khaled Osama Eraky
 */ 



#ifndef MICROCONTROLLER_CONFIG_H_
#define MICROCONTROLLER_CONFIG_H_

#include "Macros.h"
#include "StdTypes.h"


/******************************* CPU Related Defines *******************************************************/
#define F_CPU		(16000000)

/******************************* CPU Internal Registers <SFRs & GPRs> **************************************/
#define IO_OFFSET	(0x20)

#define ATMEGA32_SREG		((reg8*)(0x3F+IO_OFFSET))
#define ATMEGA32A_MCUCR		((reg8*)(0x35+IO_OFFSET))
#define ATMEGA32_MCUCSR		((reg8*)(0x34+IO_OFFSET))
#define ATMEGA32_GICR		((reg8*)(0x3B+IO_OFFSET))
#define GIFR				((reg8*)(0x3A+IO_OFFSET))

/*****************************************  Attribute Defines FOR AVR Interrupt Handling ************************************************/
#define ISR_NONBLOCK 	__attribute__((interrupt))
#define ISR_BLOCK
#define ISR_NAKED      	__attribute__((naked))
#define ISR_ALIASOF(v) 	__attribute__((alias(__STRINGIFY(v))))
#define _VECTOR(N) 		__vector_ ## N

#define ISR(V,...)	void V (void) __attribute__ ((signal)) __VA_ARGS__;\
void V (void)
/******************************************* Global Interrupt Related Macros **************************************/
/*{} not () as those () will higher the piriority..and () as I will call it as a func*/
#define GLOBAL_INTERRUPT_ENABLE()	{ATMEGA32_SREG->bits.b7=1;}
#define GLOBAL_INTERRUPT_DISABLE()	{ATMEGA32_SREG->bits.b7=0;}

/******************************************* User Types For AVR MCU **************************************/

typedef union
{
	vuint8_t allRegister;
	struct
	{
		vuint8_t b0:1;
		vuint8_t b1:1;
		vuint8_t b2:1;
		vuint8_t b3:1;
		vuint8_t b4:1;
		vuint8_t b5:1;
		vuint8_t b6:1;
		vuint8_t b7:1;
	}bits;
}reg8;

typedef struct
{
	reg8 pin;
	reg8 ddr;
	reg8 port;
}DIO_Struct;


#endif /* MICROCONTROLLER_CONFIG_H_ */