/*
 * LCD.h
 *
 * Created: 12/13/2017 1:46:01 AM
 *  Author: Hamdy
 *
 ******************************* internal Structure **************************
 *
 *	The LCD Contains Two 8-bit Registers ( Instruction Register for Storing Instructions ) (Data Register That Stores incoming Data).
 *	Character Generator ROM (CGROM)   ( Contains Predefined 5 * 8 (208 Character) dot or 5 * 10 (32 Char) dot character patterns.
 *  Character Display RAM (DDRAM)   (In the character generator RAM, the user can rewrite character patterns by program. For 5 * 8 dots.)
 *
 *
 ******************************* Operating Modes *****************************
 *
 * (8-bit Mode)			all eight bus lines (DB0 to DB7) are used.
 * (4-bit Mode)			four bus lines (DB4 to DB7) are used for transfer to transfer data, the four high order bits of Data, are transferred before the four low order bits.
 *
 *
 ****************************** pinout Functions *****************************
 *
 *										* Control PINS *
 *	RS					Select  0 for Instruction Register ,  1 for Data Register 
 *	RW				    Select  0 for Write , 1 For Read
 *  E				    Starts Data read/Write ( Transaction From High To Low When Writing , Transaction From Low to High when Reading Data will be Available as long as the signal is High)
 *
 *										* Data PINS *
 *  DB4 to DB7			Four high order bidirectional pins. Used for data transfer and receive between the MPU and the LCD.
 *	DB0 to DB3			Four Lower order bidirectional pins. Used for data transfer and receive between the MPU and the LCD.
 *  DB7					Can Be Used As state indicator Flag
 *
 *										* POWER PINS *
 *	VSS					Power Supply pin should be connected to GND
 *  VDD					Power Supply pin should be connected to VCC ( 3.3 ~ 9V)
 *  VO					Power Supply pin controls the LCD Contrast 
 *	A					The Anode Terminal of the back LCD (Should be Driven High 3V)
 *	K					The cathode Terminal of The Back of the LCD ( GND )
 *
 *
 ******************************** Function Description **************************
 * 
 * Busy Flag (BF)   When the busy flag is 1 The LCD is Busy And Cannot Take Instructions You Can Check The Busy Flag When RS=0,RW=1 @ DB7
 *
 *								Operation Selection
 *				RS				R/W				Operation
 *				0				0			    IR write as an internal operation (display clear, etc.)
 *				0				1				Read busy flag (DB7) and address counter (DB0 to DB6)
 *				1				0				Write To LCD
 *				1				1				Read The Contents of The LCD
 *
 *
 ******************************** Instructions ************************************
 *
 *	Clear display							Clears entire display and sets DDRAM address 0 in address counter.
 *	Return home				 (1.52ms)		Sets DDRAM address 0 in address counter. Also returns display from being shifted to original position DDRAM contents remain unchanged.
 *	Entry mode set			 (37 us )		Sets cursor move direction	and specifies display shift.
 *	Display on/off control	 (37 us )		Controls LCD , Cursor , Blinking.
 *  Cursor or display shift	 (37 us )		shifts LCD without changing it's Content.
 *  Function set			 (37 us )		Sets interface data length (DL),number of display lines(N),and character font (F).
 *	Read busy flag & address (0 us )	    Reads busy flag (BF)
 *	Set DDRAM address		 (37 us )
 *	Set CGRAM address		 (37 us )
 *
 *
 *
 *
 */ 



	#include "LCD.h"
	#include <stdlib.h>
	#include <util/delay.h>
	#include <stdlib.h>

		#if ( _5x10_Dot && Two_lines )
				#pragma GCC error "You cannot Have Two Lines LCD with 5 * 10"
		#endif

		/* List Of Command Codes */
#define Clear_display 0x01
#define Return_Home	  0x02
#define Display_off   0x08
#define Display_on   (0x0C | (CURSOR<<1) | (Blink))
#define CURSOR_Shift_Right	 0x14
#define CURSOR_Shift_left	 0x10
#define Display_shift_left    0x18
#define Display_shift_Right	  0x1C
#define Data_Set (0x20 |(_5x10_Dot<<2)|(Two_lines<<3)| (eight_bitdata<<4))
#define DDRAM_CONST 0x80
#define CGRAM_CONST 0x40

#define LCD_1ST_BASE 0x00
#define LCD_2ND_BASE 0x40
#define LCD_3RD_BASE 0x10
#define LCD_4TH_BASE 0x50

#define LCD_LEFT 1
#define LCD_RIGHT 0


	static void Send_instruction(uint8_t instruction);

	void LCD_init(void)
	{
		/* Set The Control Pins Direction to OUtput */
		LCD_CONTROL_IO->ddr.allRegister |=(1<<E) | (1<<RW) | (1<<RS);
		
		#if eight_bitdata
			/* Set the Direction of the 8 Data pins to Output */
			LCD_CONTROL_IO->ddr.allRegister =0xFF;
		#else
			
			#if Upper_Data_port
				/* Set The Direction of The Upper Pins to OP*/
			LCD_Data_IO->ddr.allRegister |=(0xF0);
			#else
				/* Set The Direction of The LOWER Pins to OP*/
			LCD_Data_IO->ddr.allRegister|=(0x0F);
			#endif
			Send_instruction(0x02);
		#endif
		
		Send_instruction(Data_Set);
		Send_instruction(Display_on);
		Send_instruction(Clear_display);
	}
	
	void Send_instruction(uint8_t instruction)
	{

		/* RW, RS Should be 0 when Writing instructions to LCD */
		CLEARBIT(LCD_CONTROL_IO->port.allRegister,RS);
		CLEARBIT(LCD_CONTROL_IO->port.allRegister,RW);
		/* Delay of Address Set 60ns minimum */
		_delay_ms(1);
		
		/* Enable LCD E=1 */
		SETBIT(LCD_CONTROL_IO->port.allRegister,E);
		/* delay for processing PW min 450ns */
		_delay_ms(1); 
		
		#if eight_bitdata		
			LCD_Data_IO->port.allRegister = instruction;
			/* Data set-up time delay  Tdsw = 195 ns */
			_delay_ms(1); 
			/* disable LCD E=0 */
			CLEARBIT(LCD_CONTROL_IO->port.allRegister,E);
			/* Data Hold delay Th = 10ns */ 
			_delay_ms(1); 
		#else 
			
			#if Upper_Data_port
				/* clear the Prev Data */
				LCD_Data_IO->port.allRegister &=0x0F;
				LCD_Data_IO->port.allRegister |= (instruction & 0xF0);
			
			#else	/* Lower Port */
				/* clear the Prev Data */
				LCD_Data_IO->port.allRegister &=0xF0;
				LCD_Data_IO->port.allRegister |= ((instruction >> 4) );
			#endif
			/* Data set-up time delay  Tdsw = 195 ns */
			_delay_ms(1);
		
			/* disable LCD E=0 */
			CLEARBIT(LCD_CONTROL_IO->port.allRegister,E);
			/* Data Hold delay Th = 10ns */
			_delay_ms(1);
			
			/* Enable LCD E=1 */
			SETBIT(LCD_CONTROL_IO->port.allRegister,E);
			/* delay for processing PW min 450ns */
			_delay_ms(1);
			#if Upper_Data_port
				/* clear the Prev Data */
				LCD_Data_IO->port.allRegister &=0x0F;
				LCD_Data_IO->port.allRegister |= ((instruction <<4) );
			
			#else	/* Lower Port */
				/* clear the Prev Data */
				LCD_Data_IO->port.allRegister &=0xF0;
				LCD_Data_IO->port.allRegister |= ((instruction) & 0x0f);
			#endif
				/* Data set-up time delay  Tdsw = 195 ns */
				_delay_ms(1);
				/* disable LCD E=0 */
				CLEARBIT(LCD_CONTROL_IO->port.allRegister,E);
				/* Data Hold delay Th = 10ns */
				_delay_ms(1);
		#endif
	}
	
	
	void LCD_DisplayChar(uint8_t Data)
	{
		/* RW  Should be 0  and RS should be 1 when Writing Data to LCD */
		SETBIT(LCD_CONTROL_IO->port.allRegister,RS);
		CLEARBIT(LCD_CONTROL_IO->port.allRegister,RW);
		/* Delay of Address Set 60ns minimum */
		_delay_ms(1);
		/* Enable LCD E=1 */
		SETBIT(LCD_CONTROL_IO->port.allRegister,E);
		/* delay for processing PW min 450ns */
		_delay_ms(1);
		#if eight_bitdata
			LCD_Data_IO->port.allRegister=Data;
		#else
			#if Upper_Data_port
				/* clear the Prev Data */
				LCD_Data_IO->port.allRegister &=0x0F;
				/* Send MSB (4-bits )*/
				LCD_Data_IO->port.allRegister |= (Data & 0xF0);
				
			#else /* Lower Port */
				/* clear the Prev Data */
				LCD_Data_IO->port.allRegister &=0xF0;
				/* Send MSB (4-bits )*/
				LCD_Data_IO->port.allRegister |= ((Data >> 4) & 0x0f);
			#endif
				
			/* Data set-up time delay  Tdsw = 195 ns */
			_delay_ms(1);
			/* disable LCD E=0 */
			CLEARBIT(LCD_CONTROL_IO->port.allRegister,E);
			/* Data Hold delay Th = 10ns */
			_delay_ms(1);
			/* Enable LCD E=1 */
			SETBIT(LCD_CONTROL_IO->port.allRegister,E);
				
			#if Upper_Data_port
				/* clear the Prev Data */
				LCD_Data_IO->port.allRegister &=0x0F;
				LCD_Data_IO->port.allRegister |= ((Data <<4) & 0xF0);
				/* delay for processing PW min 450ns */
				_delay_ms(1);
			#else	/* Lower Port */
				/* clear the Prev Data */
				LCD_Data_IO->port.allRegister &=0xF0;
				LCD_Data_IO->port.allRegister |= ((Data) & 0x0f);
			#endif
			
			#endif
		/* Data set-up time delay  Tdsw = 195 ns */
		_delay_ms(1);
		/* disable LCD E=0 */
		CLEARBIT(LCD_CONTROL_IO->port.allRegister,E);
		/* Data Hold delay Th = 10ns */
		_delay_ms(1);
		
	}
	
	void LCD_Clear(void)
	{
		/* Clear Screen */
		Send_instruction(Clear_display); 
	}
	
	
	void LCD_DisplayString(char * ptr)
	{
		while(*ptr != '\0')
		{
			LCD_DisplayChar(*ptr);
			ptr++;
		}
	}
	
	void LCD_Select_RowCol(uint8_t Row , uint8_t col)
	{
		uint8_t Address=0;
		
		switch(Row)
		{
			case 0:
					/* When Writing To LCD 1st Col The Base Address is 0x00 */
						Address=col + LCD_1ST_BASE;
						break;
			case 1:
					/* When Writing To LCD 2nd Col The Base Address is 0x40 */
						Address=col + LCD_2ND_BASE;
						break;
			case 2:
					/* When Writing To LCD 1st Col The Base Address is 0x10 */
						Address = col + LCD_3RD_BASE;
						break;
			case 3:
					/* When Writing To LCD 1st Col The Base Address is 0x50 */
						Address=col + LCD_4TH_BASE;
						break;		
			default:
						break;
		}
		
		Send_instruction(DDRAM_CONST|Address);
	}
	
	
	void LCD_DisplayInt(unsigned short data)
	{
		/* String to hold the ascii result */
		char Result[17];
		/* 10 for decimal */
		itoa(data,Result,10);
		/* Display The Result */
		LCD_DisplayString(Result);
	}