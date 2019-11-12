/*
 * UI.h
 *
 *  Created on: 29.05.2019
 *      Author: domin
 */

#ifndef UI_H_
#define UI_H_

#include "main.h"
#include "UI.h"
#include "Bluetooth.h"
#include "Button.h"
#include "LCD.h"
#include "Light.h"
#include "Time.h"

typedef enum {
	START = 0,
	SET_TIME = 1,
	SET_ALARM = 2,
	SET_SUNRISE_DURATION = 3,
	SET_AUDIO = 4,
	SET_BLUETOOTH = 5
} UI_Name;

void initUI();
void setUI(UI_Name UI);

void changeUI(signed int direction);
void doUIUpdate10(); //10ms Called by timer3
void doUIUpdate500(); //500ms Called by timer3
void handleUIUpdate();


#endif /* UI_H_ */
