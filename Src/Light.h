/*
 * Light.h
 *
 *  Created on: 21.05.2019
 *      Author: domin
 */
#ifndef LIGHT_H_
#define LIGHT_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include "LCD.h"
#include "Bluetooth.h"
#include "Time.h"
#include "Button.h"
#include "UI.h"



//////////////////////////
//   //
//  PWM R    PWM B   PWM G//
//   //
//  PWM R    //
//   //
//   //

void initLight(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2);
void startSunrise(uint8_t sunriseDurationMin);
void stopSunrise();
void setLEDBrightness(int brightness);
void setLEDColor(int r, int g, int b);
void handleSunrise();
uint8_t getSunriseDurationMin();
void setSunriseDurationMin(uint8_t);
void doSunriseUpdate();

void ledTransition(void);
void ledSunset(uint8_t duration);


#endif /* LIGHT_H_ */
