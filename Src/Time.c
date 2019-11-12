/*
 * Time.c
 *
 *  Created on: May 22, 2019
 *      Author: domin
 */
#include "time.h"

RTC_HandleTypeDef hrtcRTC;
TIM_HandleTypeDef htim4;
RTC_AlarmTypeDef alarmT;
RTC_TimeTypeDef timeT;

uint8_t newSecondEvent = 0;

uint8_t lightAlarmTriggered = 0; //Invisible alarm
uint8_t audioAlarmTriggered = 0; //Visible alarm
uint8_t audioAlarmEnable = 1; //default on
uint8_t alarmDismissed = 1; //1=disabled, 0=Alarm is running, witing for dismission


uint8_t lightAlarmHour = 0;
uint8_t lightAlarmMinute = 0;
uint8_t lightAlarmSecond = 1;

uint8_t audioAlarmHour = 0;
uint8_t audioAlarmMinute = 0;
uint8_t audioAlarmSecond = 1;

extern sunriseDurationMin;

uint8_t alarmSet = 0;
uint8_t firstAlarm = 1;

void setHardware(TIM_HandleTypeDef *htim){
	htim4 = *htim;
}

void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc){
	newSecondEvent = 1;
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
	if(firstAlarm){
		lightAlarmTriggered = 1;
		firstAlarm = 0;
		RTC_AlarmTypeDef localAlarm;
		localAlarm.AlarmTime.Hours = audioAlarmHour;
		localAlarm.AlarmTime.Minutes = audioAlarmMinute;
		localAlarm.AlarmTime.Seconds = audioAlarmSecond;
		localAlarm.Alarm= 1;
		HAL_RTC_SetAlarm_IT(&hrtcRTC, &localAlarm, RTC_FORMAT_BIN);
	}else{
		audioAlarmTriggered = 1;
		alarmDismissed = 0;
		alarmSet = 0;
	}

}

void delayUS(uint32_t us) {
	for(int i = 0; i < us*70; i++){ //~maybe 1us per 70
		if(i == 2){
			i = 2;
		}
	}
}

void setTimeString(char* time){
	int i;
	sscanf(time, "%d", &i);
	uint8_t hour = i / 10000;
	uint8_t minute = (i % 10000) / 100;
	uint8_t second = (i % 100);

	timeT.Hours = hour;
	timeT.Minutes = minute;
	timeT.Seconds = second;
	HAL_RTC_SetTime(&hrtcRTC, &timeT, RTC_FORMAT_BIN);
}

void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds){
	timeT.Hours = hours;
	timeT.Minutes = minutes;
	timeT.Seconds = seconds;
	HAL_RTC_SetTime(&hrtcRTC, &timeT, RTC_FORMAT_BIN);
}

uint8_t getHour(){
	HAL_RTC_GetTime(&hrtcRTC, &timeT, RTC_FORMAT_BIN);
	return timeT.Hours;
}
uint8_t getMinute(){
	HAL_RTC_GetTime(&hrtcRTC, &timeT, RTC_FORMAT_BIN);
	return timeT.Minutes;
}
uint8_t getSecond(){
	HAL_RTC_GetTime(&hrtcRTC, &timeT, RTC_FORMAT_BIN);
	return timeT.Seconds;
}

uint8_t getAlarmHour(){
	return audioAlarmHour;
}
uint8_t getAlarmMinute(){
	return audioAlarmMinute;
}
uint8_t getAlarmSecond(){
	return audioAlarmSecond;
}
uint8_t getAlarmLightHour(){
	return lightAlarmHour;
}
uint8_t getAlarmLightMinute(){
	return lightAlarmMinute;
}
uint8_t getAlarmLightSecond(){
	return lightAlarmSecond;
}

void setAlarm(uint8_t hour, uint8_t minute, uint8_t second, uint8_t sunriseDuration){
	RTC_TimeTypeDef localTime;
	RTC_AlarmTypeDef localAlarm;

	sunriseDurationMin = sunriseDuration;

	if(sunriseDurationMin!=0){ //WITH SUNRISE
		if(minute>=sunriseDurationMin){ //only modify minute
			lightAlarmHour = hour;
			lightAlarmMinute = minute-sunriseDurationMin;
			lightAlarmSecond = 0;
		}else{ // modify hour as well
			if(hour > 0){
				lightAlarmHour = hour-1; //deduct 1 h because of minute underflow
			}else{
				lightAlarmHour = 23; //set to 23
			}
			lightAlarmMinute = 60-(sunriseDurationMin-minute);
			lightAlarmSecond = 0;
		}

		audioAlarmHour = hour;
		audioAlarmMinute = minute;
		audioAlarmSecond = second;

		localTime.Hours= lightAlarmHour;
		localTime.Minutes = lightAlarmMinute;
		localTime.Seconds = lightAlarmSecond;

		localAlarm.Alarm= 1;
		localAlarm.AlarmTime = localTime;
		HAL_RTC_SetAlarm_IT(&hrtcRTC, &localAlarm, RTC_FORMAT_BIN);

		firstAlarm = 1; //1 = light interrupt comes first, 0=no light, directly audio on interrupt
	}else{ //WITHOUT SUNRISE
		audioAlarmHour = hour;
		audioAlarmMinute = minute;
		audioAlarmSecond = second;

		localTime.Hours= audioAlarmHour;
		localTime.Minutes = audioAlarmMinute;
		localTime.Seconds = audioAlarmSecond;

		localAlarm.Alarm= 1;
		localAlarm.AlarmTime = localTime;
		HAL_RTC_SetAlarm_IT(&hrtcRTC, &localAlarm, RTC_FORMAT_BIN);

		firstAlarm = 0; //skip light alarm step in RTC interrupt by setting this to 0
	}

	alarmSet = 1;
	audioAlarmTriggered = 0;
	lightAlarmTriggered = 0;
	alarmDismissed = 1; //nothing to dismiss yet

	char text[20];
	sprintf(text, "Light alarm: %d:%d:%d\n",lightAlarmHour,lightAlarmMinute,lightAlarmSecond);
	printBT(text);
	sprintf(text, "Audio alarm: %d:%d:%d\n",audioAlarmHour,audioAlarmMinute,audioAlarmSecond);
	printBT(text);
}

void resetAlarm(){
	HAL_RTC_DeactivateAlarm(&hrtcRTC, 1);
	audioAlarmTriggered = 0;
	lightAlarmTriggered = 0;
	alarmSet = 0;
	firstAlarm = 1;
	dismissAlarm();
	printBT("Alarm disabled\n");
}

void initRTC(RTC_HandleTypeDef *hrtc){
	hrtcRTC = *hrtc;

	RTC_TimeTypeDef sTime;
	sTime.Hours = 5;
	sTime.Minutes = 6;
	sTime.Seconds = 59;
	HAL_RTC_SetTime(&hrtcRTC, &sTime, RTC_FORMAT_BIN);

	HAL_RTCEx_SetSecond_IT(&hrtcRTC);




}

void handleRTCTimeStep(){
	if(newSecondEvent){
		newSecondEvent = 0;

		RTC_TimeTypeDef sTime;
		HAL_RTC_GetTime(&hrtcRTC, &sTime, RTC_FORMAT_BIN);
		char textBT[10];
		sprintf(&textBT, "%02.2d:%02.2d:%02.2d\n", sTime.Hours, sTime.Minutes, sTime.Seconds);
		printBT(textBT);

		if(audioAlarmTriggered){
			audioAlarmTriggered = 0;
			printBT("Audio Alarm Triggered \n");
			if(melodyEnabled()){
				//play melody
			}else{
				//dont play melody
			}
		}else if(lightAlarmTriggered){
			lightAlarmTriggered = 0;
			printBT("Light Alarm Triggered \n");
			startSunrise(sunriseDurationMin);
		}
	}
}

void drawTimeOnUI(){
	RTC_TimeTypeDef timeT;
	HAL_RTC_GetTime(&hrtcRTC, &timeT, RTC_FORMAT_BIN);
	char textLCD[8];
	sprintf(&textLCD, "%02.2d:%02.2d:%02.2d", timeT.Hours, timeT.Minutes, timeT.Seconds);
	printLCD(0,3, textLCD);
}
void drawAlarmTimeOnUI(){
	char textLCD[5];
	sprintf(textLCD, "%02.2d:%02.2d", getAlarmHour(), getAlarmMinute());
	printLCD(1,10, textLCD);
	printLCDCustomChar(1, 0, CHAR_SUN_EMPTY);
	if(getSunriseDurationMin()>0){
		sprintf(textLCD, "%02.2d:%02.2d", getAlarmLightHour(), getAlarmLightMinute());
		printLCD(1,1, textLCD);
	}else{
		printLCD(1,1, "off");
	}



}

uint8_t getAlarmSetStatus(){
	return alarmSet;
}
uint8_t getAudioAlarmStatus(){
	return audioAlarmTriggered;
}
uint8_t getLightAlarmStatus(){
	return lightAlarmTriggered;
}
uint8_t getDismissedStatus(){
	return alarmDismissed;
}
void dismissAlarm(){
	alarmDismissed = 1;
}
