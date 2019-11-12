/*
 * LCD.h
 *
 *  Created on: 21.05.2019
 *      Author: domin
 */
#ifndef LCD_H_
#define LCD_H_


#include "Light.h"
#include "Bluetooth.h"
#include "stm32f1xx_hal.h"
#include "Time.h"
#include "Button.h"
#include "UI.h"


typedef enum {
	CHAR_ALARM_EMPTY = 0,
	CHAR_ALARM_FILLED = 1,
	CHAR_BLUETOOTH = 2,
	CHAR_SUN_EMPTY = 3,
	CHAR_SUN_FULL = 4,
	CHAR_CLOCK = 5,
	CHAR_TICK_POSITIVE = 6,
	CHAR_MELODY = 7
} Symbols;


void clearLCD();
void printLCD(uint8_t row, uint8_t col, char* text);
void printLCDCustomChar(uint8_t row, uint8_t col, int index);
void sendLCDCmd(uint8_t data);
void sendLCDData(uint8_t data);
void initLCD(HAL_I2C_StateTypeDef *hi2cmain, TIM_HandleTypeDef *htim4main);



void doBacklightOff();
void doBacklightOn();
uint8_t getBacklightState();


#endif /* LCD_H_ */
