/*
 * DIO.h
 *
 * -> file  < Digital Input output Related API      >
 * -> brief < Contains API to Handle DIO of AVR MCU >
 *
 *		DIO_pinConfigure
 < Function >   Use This Function To Configure Direction of a DIO pin
 *						  return		 void
 *						  Parameters
 *										 1- DIO Port			< DIOA , DIOB .. etc>
 *										 2- DIO Pin Number		<0 .. 7>
 *										 3- DIO Pin Mode		< OUTPUT , INPUT_PULLUP , INPUT_FLOAT >
 *
 *		 DIO_pinWrite
 < Function >   Use This Function To Write Output pins Level
 *						  return		 void
 *						  Parameters
 *										 1- DIO Port			 < DIOA , DIOB .. etc>
 *										 2- DIO Pin Number	 <0 .. 7>
 *										 3- DIO Pin State	 < LOW , HIGH >
 *
 *
 *		 DIO_pinRead
 < Function >   Use This Function To Read Input pins Level
 *					     return         Pin Read <Low , HIGH>
 *					     Parameters
 *										1- DIO Port			 < DIOA , DIOB .. etc>
 *										2- DIO Pin Number	 <0 .. 7>
 *
 *
 * Created: 8/24/2020 1:25:31 AM
 *  Author: Khaled Osama Eraky
 */ 


#ifndef DIO_H_
#define DIO_H_

	#include "Microcontroller_config.h"
	#include "Macros.h"
	#include "StdTypes.h"
	
	/******************************************* Defines for Port  **************************************/

	#define	DIOA		((DIO_Struct *)(0x19+IO_OFFSET))
	#define DIOB		((DIO_Struct *)(0x16+IO_OFFSET))
	#define DIOC		((DIO_Struct *)(0x13+IO_OFFSET))
	#define DIOD		((DIO_Struct *)(0x10+IO_OFFSET))

	/******************************************* User Types For AVR Input/Output MCU **************************************/
	
	typedef enum
	{
		INPUT_FLOAT,
		INPUT_PULLUP,
		OUTPUT,
	}pinMode;
	

	/******************************************* IO Functions Prototype ***************************************************/

	void	     DIO_pinConfigure(DIO_Struct * Dio, pinNumber pinNum, pinMode Mode);
	void		 DIO_pinWrite(DIO_Struct * Dio, pinNumber pinNum, digitalState State);
	digitalState DIO_pinRead(DIO_Struct * Dio, pinNumber pinNum);
	void		 DIO_portConfigure(DIO_Struct * Dio, vuint8_t ConfigByte);
	void		 DIO_portWrite(DIO_Struct * Dio, vuint8_t Data);
	vuint8_t     DIO_portRead(DIO_Struct * Dio);




#endif /* DIO_H_ */