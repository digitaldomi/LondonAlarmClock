/*
 * Time.h
 *
 *  Created on: May 22, 2019
 *      Author: domin
 */

#ifndef TIME_H_
#define TIME_H_
#include "stm32f1xx_hal.h"
#include "Bluetooth.h"
#include "LCD.h"
#include "Light.h"
#include "Button.h"
#include "UI.h"



void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc);
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);

void delayUS(uint32_t us);
void setTimeString(char* time);
void setTime(uint8_t hour, uint8_t minute, uint8_t second);
uint8_t getHour();
uint8_t getMinute();
uint8_t getSecond();

uint8_t getAlarmHour();
uint8_t getAlarmMinute();
uint8_t getAlarmSecond();
uint8_t getAlarmLightHour();
uint8_t getAlarmLightMinute();
uint8_t getAlarmLightSecond();

void initRTC(RTC_HandleTypeDef *hrtc);
void setAlarm(uint8_t hour, uint8_t minute, uint8_t second, uint8_t sunriseDuration);
void resetAlarm();
void handleRTCTimeStep();
void drawTimeOnUI();
void drawAlarmTimeOnUI();
uint8_t getAlarmSetStatus();
uint8_t getAudioAlarmStatus();
uint8_t getLightAlarmStatus();
uint8_t getDismissedStatus();
void dismissAlarm();



#endif /* TIME_H_ */
