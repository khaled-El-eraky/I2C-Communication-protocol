/*
 * ADXL345_Accelerometer.c
 *
 * Created: 9/8/2020 2:33:26 AM
 *  Author: Khaled
 */ 

#include "Microcontroller_config.h"
#include "I2C.h"
#include "ADXL345_Accelerometer.h"
#include <util/delay.h>

/*
 * when ALT Address is connected to High
 * ADXL345 Address is 0x1D 
 * when ALT Address is connected to Low
 * ADXL345 Address is 0x53    
 */

#define ADXL345_MAIN_ADDRESS       (0X1D) 
#define ADXL345_ALTERNATE_ADDRESS  (0x53)

#define POWERCTRL_REGISTER         (0X2D)
#define TABCHECK_REGISTER		   (0X2B)
#define X0_AXIS_REGISTER		   (0X32)
#define X1_AXIS_REGISTER		   (0X33)
#define Y0_AXIS_REGISTER		   (0X34)
#define Y1_AXIS_REGISTER		   (0X35)
#define Z0_AXIS_REGISTER           (0X36)
#define Z1_AXIS_REGISTER           (0X37)

#define MEASUREMODE_CODE           (0X04) 

void ADXL345_Init(void)
{
	TWI_Init();
	TWI_SendByte(ADXL345_MAIN_ADDRESS,POWERCTRL_REGISTER,MEASUREMODE_CODE);
	_delay_ms(10);  	
}

ADXL345_TabChaeck ADXL345_ChaeckTab(void)
{
	uint8_t TAB =0;
	uint8_t Tab = TWI_ReadByte(ADXL345_MAIN_ADDRESS,TABCHECK_REGISTER);
	if ((Tab & 0x07) != 0)
	{
		TAB = SINGLE_TAB;
		/*Handel the double tab*/
	}
	return TAB;
}

 
uint8_t ADXL345_ReadAxis(ADXL345_SelectAxis Axis)
{
	unsigned short Result = 0;
	switch(Axis)
	{
		case X_AXIS:
		/* Read LSB */
		Result = TWI_ReadByte(ADXL345_MAIN_ADDRESS,X0_AXIS_REGISTER);
		_delay_ms(1);
		/* Read MSB */
		Result |= (TWI_ReadByte(ADXL345_MAIN_ADDRESS,X1_AXIS_REGISTER) << 8);
		break;	
		case Y_AXIS:
		/* Read LSB */
		Result = TWI_ReadByte(ADXL345_MAIN_ADDRESS,Y0_AXIS_REGISTER);
		_delay_ms(1);
		/* Read MSB */
		Result |= (TWI_ReadByte(ADXL345_MAIN_ADDRESS,Y1_AXIS_REGISTER) << 8);
		break;
		case Z_AXIS:
		/* Read LSB */
		Result = TWI_ReadByte(ADXL345_MAIN_ADDRESS,Z0_AXIS_REGISTER);
		_delay_ms(1);
		/* Read MSB */
		Result |= (TWI_ReadByte(ADXL345_MAIN_ADDRESS,Z1_AXIS_REGISTER) << 8);
		break;	
		default:
		break;
	}
	/*Get Two's compliment of Result*/ 
	Result = ~Result + 1;
	return Result;
}