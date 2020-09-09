/*
 * DIO.c
 *
 * Created: 8/24/2020 1:25:45 AM
 *  Author: Khaled Osama Eraky
 */ 

#include "DIO.h"

void	DIO_pinConfigure(DIO_Struct * Dio, pinNumber pinNum, pinMode Mode)
{
	switch(Mode)
	{
		case INPUT_FLOAT:
		CLEARBIT(Dio->ddr.allRegister,pinNum);
		CLEARBIT(Dio->port.allRegister,pinNum);
		break;
		case INPUT_PULLUP:
		CLEARBIT(Dio->ddr.allRegister,pinNum);
		SETBIT(Dio->port.allRegister,pinNum);
		break;
		case OUTPUT:
		SETBIT(Dio->ddr.allRegister,pinNum);
		break;
		default:
		break;
	}
}

void	DIO_pinWrite(DIO_Struct * Dio, pinNumber pinNum, digitalState State)
{
	switch(State)
	{
		case HIGH:
		SETBIT(Dio->port.allRegister,pinNum);
		break;
		case LOW:
		CLEARBIT(Dio->port.allRegister,pinNum);
		break;
		default:
		break;
	}
}

digitalState DIO_pinRead(DIO_Struct * Dio, pinNumber pinNum)
{
	digitalState returnvar = LOW;
	
	switch(READBIT(Dio->pin.allRegister,pinNum))
	{
		case LOW:
		returnvar = LOW;
		break;
		case HIGH:
		returnvar = HIGH;
		break;
		default:
		break;
	}
	return returnvar;
}


void	DIO_portConfigure(DIO_Struct * Dio,vuint8_t ConfigByte)
{
	Dio->ddr.allRegister=ConfigByte;
}	

void	DIO_portWrite(DIO_Struct * Dio, vuint8_t Data)
{
	Dio->port.allRegister=Data;
}

vuint8_t DIO_portRead(DIO_Struct * Dio)
{
	return  Dio->pin.allRegister;
}
