/*
 * ADXL345_Accelerometer.h
 *
 * Created: 9/8/2020 2:33:12 AM
 *  Author: Khaled
 */ 


#ifndef ADXL345_ACCELEROMETER_H_
#define ADXL345_ACCELEROMETER_H_

typedef enum
{
	NO_TAB,
	SINGLE_TAB,
	DOUBLE_TAB
}ADXL345_TabChaeck;

typedef enum
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
}ADXL345_SelectAxis;

void ADXL345_Init(void);
ADXL345_TabChaeck ADXL345_ChaeckTab(void);
uint8_t ADXL345_ReadAxis(ADXL345_SelectAxis Axis);


#endif /* ADXL345_ACCELEROMETER_H_ */