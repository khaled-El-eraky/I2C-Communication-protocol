/*
 * I2C_Application.c
 *
 * Created: 9/6/2020 3:27:39 PM
 *  Author: Khaled
 */ 


#include "LCD.h"
#include "ADXL345_Accelerometer.h"
#include <util/delay.h>

int main(void)
{
	unsigned short X = 0;
	LCD_init();
	LCD_DisplayString("ADxL345");
	_delay_ms(2000);
	LCD_Clear();
	ADXL345_Init();
	LCD_DisplayString("X=");
	LCD_DisplayInt(X);

	
	
    while(1)
    {
	    X = ADXL345_ReadAxis(X_AXIS);
    }
}