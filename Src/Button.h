/*
 * Button.h
 *
 *  Created on: May 24, 2019
 *      Author: domin
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"
#include "Light.h"
#include "Bluetooth.h"
#include "stm32f1xx_hal.h"
#include "Time.h"
#include "LCD.h"
#include "UI.h"



void initButtons();
void buttonUpdate();
void handleButtonUpdate();
uint8_t getButtonState(uint8_t number);




#endif /* BUTTON_H_ */
