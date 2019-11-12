/*
 * Button.c
 *
 *  Created on: May 24, 2019
 *      Author: domin
 */
#include "Button.h"

uint8_t buttonUpdateReady = 0;
uint8_t button1State, button2State, button3State, button4State;
uint8_t button1State_old, button2State_old, button3State_old, button4State_old;
uint8_t button1Pressed, button2Pressed, button3Pressed, button4Pressed;

void buttonUpdate(){
	buttonUpdateReady = 1;
}

void initButtons(){
	button1State = 0;
	button2State = 0;
	button3State = 0;
	button4State = 0;

	button1State_old = 0;
	button2State_old = 0;
	button3State_old = 0;
	button4State_old = 0;
}


uint8_t getButtonState(uint8_t number){
	uint8_t state = 0;
	switch(number){
	case 1: state = button1Pressed;
			if(state == 1){button1Pressed = 0;}
			break;
	case 2: state = button2Pressed;
			if(state == 1){button2Pressed = 0;}
			break;
	case 3: state = button3Pressed;
			if(state == 1){button3Pressed = 0;}
			break;
	case 4: state = button4Pressed;
			if(state == 1){button4Pressed = 0;}
			break;
	default: break;
	}
	return state;
}

void handleButtonUpdate(){
	if(buttonUpdateReady){
		buttonUpdateReady = 0;
		button1State = HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin);
		button2State = HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin);
		button3State = HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin);
		button4State = HAL_GPIO_ReadPin(BUTTON4_GPIO_Port, BUTTON4_Pin);
		if((button1State != button1State_old)&&(button1State == 1)){
			button1Pressed = 1;
			doBacklightOn();
			resetBacklightCounter();
		}
		if((button2State != button2State_old)&&(button2State == 1)){
			button2Pressed = 1;
			doBacklightOn();
			resetBacklightCounter();
		}
		if((button3State != button3State_old)&&(button3State == 1)){
			button3Pressed = 1;
			doBacklightOn();
			resetBacklightCounter();
		}
		if((button4State != button4State_old) && (button4State == 1)){
			button4Pressed = 1;
			doBacklightOn();
			resetBacklightCounter();
		}
		button1State_old = button1State;
		button2State_old = button2State;
		button3State_old = button3State;
		button4State_old = button4State;

	}
}
