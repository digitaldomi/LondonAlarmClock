/*
 * LCD.c
 *
 *  Created on: 21.05.2019
 *      Author: domin
 */
#include "LCD.h"

#define use_display 1

HAL_I2C_StateTypeDef hi2c1;
TIM_HandleTypeDef htimLCD;

uint8_t lcdBusy = 0;
uint8_t lcdBacklight = 1;

uint8_t hour_displayed;
uint8_t minute_displayed;
uint8_t second_displayed;

uint8_t cursorPosition = 0;

void initLCD(HAL_I2C_StateTypeDef *hi2cmain, TIM_HandleTypeDef *htimmain){
	if(use_display){
		lcdBacklight = 1;
		hi2c1 = *hi2cmain;

		HAL_Delay(40); //Wait for display on boot
		//Init LCD
		// 7 DB4
		// 6 DB5
		// 5 DB6
		// 4 DB7
		// 3 Backlight
		// 2 EN
		// 1 R/W
		// 0 RS
		// first the high nibble of the command has to be sent
		// I2C address 0x27 (must be shifted <<1 in HAL function!!)

		sendLCDCmd(0b0011); //function set 8bit
		HAL_Delay(5);
		sendLCDCmd(0b0011); //function set 8bit
		sendLCDCmd(0b0011); //function set 8bit

		HAL_Delay(1);
		sendLCDCmd(0b0010); //0x2
		sendLCDCmd(0b0010); //cursor move dir & shift & x & x
		sendLCDCmd(0b1000); //
		sendLCDCmd(0b0000); //
		sendLCDCmd(0b0001); //
		sendLCDCmd(0b0000); //
		sendLCDCmd(0b0110);
		sendLCDCmd(0b0000); //
		sendLCDCmd(0xC);

		//Define custom Characters Array
		uint8_t customFont[64] = { //stores 8 custom characters
			0x04,0x0E,0x0A,0x0A,0x0A,0x11,0x1F,0x04, 	/* Alarm Symbol not filled*/
			0x04,0x0E,0x0E,0x0E,0x0E,0x1F,0x1F,0x04,  	/* Alarm Symbol filled*/
			0x4,0x6,0x15,0xe,0x6,0xd,0x16,0x4, 			/* Bluetooth Symbol*/
			0x4,0x15,0xa,0x11,0xa,0x15,0x4,0x0,			/* Sun Empty Symbol */
			0x4,0x15,0xe,0x1f,0xe,0x15,0x4,0x0,			/* Sun Full Symbol */
			0x0,0xe,0x15,0x17,0x11,0xe,0x0,0x0,			/* Clock Symbol  */
			0x0,0x1,0x3,0x16,0x1c,0x8,0x0,0x0,			/* Tick Positive Symbol */
			0x2,0x3,0x2,0xe,0x1e,0xc,0x0				/* Melody Symbol Music Note */
		};

		//Store custom characters in LCD CGRAM
		for(int j = 0; j < 8; j++){
			uint8_t command = 0x40 + j*0x08;
			sendLCDCmd((command>>4)&0x0F); 	//Set CGRAM to 0 -> custom char pos, auto increment
			sendLCDCmd(command&0x0F); 		// ^
			for(int i = 0; i < 8; i++){
				sendLCDData(((customFont[j*8+i])>>4) & 0x0F);
				sendLCDData((customFont[j*8+i]) & 0x0F);
				HAL_Delay(5);
			}
		}
		//Print all custom chars
		/*printLCD(0,0,"Chars created:");
		for(int i = 0; i < 8; i++){
			printLCDCustomChar(1, i, i);
		}
		HAL_Delay(500);
		clearLCD();
		 */

		// UI INIT
		htimLCD = *htimmain;
		HAL_TIM_Base_Start_IT(&htimLCD);
		__HAL_TIM_SET_COMPARE(&htimLCD, TIM_CHANNEL_1, 700); //10ms interrupt

		initUI();

	}
}
void clearLCD(){
	if(use_display) {
		sendLCDCmd(0x0);
		sendLCDCmd(0x1);
	}
}
void printLCD(uint8_t row, uint8_t col, char* text){
	if(use_display){
		uint8_t length = strlen(text);
	//80-...   1st Line
	//0C-...   2nd Line
		uint8_t rowData;
		if(row == 0){
			rowData = 0x80 + col;
		}
		else if(row == 1){ //row = 1
			rowData = 0xC0 + col;
		}
		sendLCDCmd(0xF); //Next instruction is position
		sendLCDCmd(0xE); // ^^
		sendLCDCmd((rowData&0xF0)>>4); //upper position nibble
		sendLCDCmd(rowData&0x0F); //lower position nibble

		for(int i = 0; i < length; i++){
			sendLCDData((*(text+i)&0xF0)>>4);
			sendLCDData(*(text+i)&0x0F);
		}
	}
}

void printLCDCustomChar(uint8_t row, uint8_t col, int index){
	if(use_display){
		// INDEX from 0-7 for one of the 8 custom characters
		uint8_t rowData;
		if(row == 0){ //0x80-...   1st Line
			rowData = 0x80 + col;
		}
		else if(row == 1){ //0x0C-...   2nd Line
			rowData = 0xC0 + col;
		}
		sendLCDCmd(0xF); //Next instruction is position
		sendLCDCmd(0xE); // ^^
		sendLCDCmd((rowData&0xF0)>>4); //upper position nibble
		sendLCDCmd(rowData&0x0F); //lower position nibble

		sendLCDData(((index)&0xF0)>>4);
		sendLCDData((index)&0x0F);
	}
}

void sendLCDCmd(uint8_t data){ //Send byte to LCD, as a command (RS pin low)
	if(use_display){
		data = data << 4;

		uint8_t address = (0x27<<1) & 0b11111110; // last bit is R/^W -> 0=Write=what we want
		int I2Ctimeout = 500;

		uint8_t command = 0b00000100 | data | (lcdBacklight << 3) ; //Enable high and backlight according to global variable

		lcdBusy = 1;

		HAL_I2C_Master_Transmit(&hi2c1, address, &command , 1, I2Ctimeout); // 0x2
		delayUS(30);//HAL_Delay(1);
		command = 0b00000000 | data  | (lcdBacklight << 3); //Enable low and backlight according to global variable -> This triggers read in of display
		//same data sent again
		HAL_I2C_Master_Transmit(&hi2c1, address, &command , 1, I2Ctimeout); // 0x2 Set cursor move dir & shift

		lcdBusy = 0;

		delayUS(30);//HAL_Delay(1);
	}
}

void sendLCDData(uint8_t data){  //Send byte to LCD, as data (RS pin high)
	if(use_display){
		data = data << 4;
		uint8_t address = (0x27<<1) & 0b11111110; // last bit is R/^W -> 0=Write=what we want
		int I2Ctimeout = 500;

		lcdBusy = 1;

		uint8_t command = 0b00000101 | data | (lcdBacklight << 3); //Enable high and backlight according to global variable
		HAL_I2C_Master_Transmit(&hi2c1, address, &command , 1, I2Ctimeout); // 0x2
		delayUS(30);//HAL_Delay(1);

		command = 0b00000001 | data | (lcdBacklight << 3); //Enable low and backlight according to global variable -> This triggers read in of display
		//same data sent again
		HAL_I2C_Master_Transmit(&hi2c1, address, &command , 1, I2Ctimeout); // 0x2 Set cursor move dir & shift

		lcdBusy = 0;

		delayUS(30);//HAL_Delay(1);
	}
}



void doBacklightOn(){
	lcdBacklight = 1;
}

void doBacklightOff(){
	lcdBacklight = 0;
}

uint8_t getBacklightState(){
	return lcdBacklight;
}




