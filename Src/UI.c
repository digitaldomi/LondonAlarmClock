/*
 * UI.c
 *
 *  Created on: 29.05.2019
 *      Author: domin
 */
#include "UI.h"

uint8_t interval_500ms_UI = 0; //set by timer4 called function
uint8_t interval_10ms_UI = 0; //set by timer4 called function

UI_Name current_UI = START;
uint8_t const UIcount = 6; //adjust according to enum in h file
uint8_t UIsubCount = 0; //stores position of time picker

uint8_t timePicker_hourTens = 0;
uint8_t timePicker_hourOnes = 0;
uint8_t timePicker_minuteTens = 0;
uint8_t timePicker_minuteOnes = 0;
uint8_t newTimePickerValue = 0;
uint8_t currentNumberInput = 0;



void initUI(){
	UIsubCount = 0; //stores position of time picker
	timePicker_hourTens = 0;
	timePicker_hourOnes = 0;
	timePicker_minuteTens = 0;
	timePicker_minuteOnes = 0;
	newTimePickerValue = 0;
	currentNumberInput = 0;
	setUI(START);
}

void setUI(UI_Name UI){
	current_UI = UI;

	timePicker_hourTens = 0;//getHour()/10;
	timePicker_hourOnes = 0;//getHour()%10;
	timePicker_minuteTens = 0;//getMinute()/10;
	timePicker_minuteOnes = 0;//getMinute()%10;
	UIsubCount = 0; //stores position of time picker
	newTimePickerValue = 0;
	currentNumberInput = 0;

	switch(UI){
		case START:
			clearLCD();

			break;
		case SET_TIME:
			clearLCD();
			printLCD(0,0,"      Time     >");
			char text[15];
			sprintf(text,"     %02.2d:%02.2d",getHour(), getMinute());
			printLCD(1,0,text);
			printLCDCustomChar(0, 5, CHAR_CLOCK);
			break;
		case SET_ALARM:
			clearLCD();
			printLCD(0,0,"      Alarm    >");
			text[15];
			if(getAlarmSetStatus()){
				sprintf(text,"     %02.2d:%02.2d",getAlarmHour(), getAlarmMinute());
				printLCD(1,0,text);
			}else{
				sprintf(text,"      off     ");
				printLCD(1,0,text);
			}
			printLCDCustomChar(0,5, CHAR_ALARM_FILLED);
			break;
		case SET_AUDIO:
			clearLCD();
			printLCD(0,0,"     Melody    >");
			if(melodyEnabled()){
				char text[16];
				sprintf(text,"     %01.1d", getMelody());
				printLCD(1,2,text);
				printLCDCustomChar(0, 4, CHAR_MELODY);
			}else{
				printLCD(1,0,"      off       ");
				printLCD(0,4, " ");
			}

			break;
		case SET_SUNRISE_DURATION:
			clearLCD();
			printLCD(0,0,"     Sunrise   >");
			text[15];
			if(getSunriseDurationMin() != 0){
				sprintf(text,"     %02.2d min",getSunriseDurationMin());
				printLCD(1,0,text);
			}else{
				sprintf(text,"      off     ");
				printLCD(1,0,text);
			}
			printLCDCustomChar(0,4,CHAR_SUN_FULL);
			break;
		case SET_BLUETOOTH:
			clearLCD();
			printLCD(0,0,"    Bluetooth  >");
			printLCD(1,6,"ON");
			printLCDCustomChar(0, 3, CHAR_BLUETOOTH);
			break;
		default:
			clearLCD();
			printLCD(0,0,"error");
			printLCD(1,0,"UI ID unknown");
			break;
	}

}


void handleUIUpdate(){
	//----500ms------------------------------------------------//
	if(interval_500ms_UI){
		interval_500ms_UI = 0;
		switch(current_UI){
			case START:
				drawTimeOnUI();
				if(getAlarmSetStatus()){
					drawAlarmTimeOnUI();
					printLCDCustomChar(1, 15, CHAR_ALARM_FILLED);
				}else if(getDismissedStatus() == 0){ //not yet dismissed
					//backlight on
					doBacklightOn();
					resetBacklightCounter();

					static int local_blink;
					if(local_blink > 0){
						local_blink = 0;
						printLCD(1,0,"                ");
					}else{
						local_blink++;
						printLCDCustomChar(1, 1, CHAR_ALARM_FILLED);
						printLCDCustomChar(1, 3, CHAR_ALARM_FILLED);
						printLCDCustomChar(1, 5, CHAR_ALARM_FILLED);
						printLCDCustomChar(1, 7, CHAR_ALARM_FILLED);
						printLCDCustomChar(1, 9, CHAR_ALARM_FILLED);
						printLCDCustomChar(1, 11, CHAR_ALARM_FILLED);
						printLCDCustomChar(1, 13, CHAR_ALARM_FILLED);
						printLCDCustomChar(1, 15, CHAR_ALARM_FILLED);
					}

				}else{
					printLCD(1,0,"   Alarm off    "); //Override texts
					printLCDCustomChar(1, 15, CHAR_ALARM_EMPTY);
				}
				if(getAlarmSetStatus() == 1 && getLightAlarmStatus() == 0){
					printLCDCustomChar(0, 0, CHAR_ALARM_EMPTY); //empty alarm
				}else if(getLightAlarmStatus() && getAudioAlarmStatus() == 0){
					printLCDCustomChar(0, 0, CHAR_SUN_EMPTY); //full sun
				}else if(getDismissedStatus()==0){
					printLCDCustomChar(0,0,CHAR_ALARM_FILLED);//alarm symbol filled
				}
				//Print Bluetooth Connection state
				if(HAL_GPIO_ReadPin(BT_STATE_GPIO_Port, BT_STATE_Pin)){
					printLCDCustomChar(0,15,CHAR_BLUETOOTH);
				}else{
					printLCD(0,15," ");
				}
				break;
			case SET_TIME:

				break;
			case SET_ALARM:
				if(getAlarmSetStatus()){
					printLCDCustomChar(0,5, CHAR_ALARM_FILLED);
				}else if(UIsubCount == 0){
					printLCDCustomChar(0,5, CHAR_ALARM_EMPTY);
					printLCD(1,5," off  ");
				}
				break;
			case SET_SUNRISE_DURATION:
				if(getSunriseDurationMin() != 0){
					printLCDCustomChar(0,4, CHAR_SUN_FULL);
				}else if(UIsubCount == 0){
					printLCDCustomChar(0,4, CHAR_SUN_EMPTY);
					printLCD(1,5," off  ");
				}
				break;
			case SET_AUDIO:

				break;
			default: break;
		}
	}//interval500ms
	//----500ms------------------------------------------------//
	//----10ms-------------------------------------------------//
	if(interval_10ms_UI){
		//----BUTTONS------------------------------------------//
		interval_10ms_UI = 0;
		// ----- ^^^^^^^^^^^^^^^^ ---------------------------------------------------------------------
		if(getButtonState(1)){ // UP BUTTON PRESSED
			printBT("UP PRESSED!\n");
			if(getDismissedStatus() == 0){ //not dismissed yet
				dismissAlarmOnUI();
			}else{
				if(current_UI == START){

				}else if(current_UI == SET_TIME){
					if(UIsubCount > 0 && UIsubCount < 5){
						newTimePickerValue = 1;
						int max_value;
						switch(UIsubCount){
							case 1: max_value = 2; break;
							case 2: if(timePicker_hourTens > 1){max_value = 3;}else{max_value = 9;}break;
							case 3: max_value = 5; break;
							case 4: max_value = 9; break;
							default: max_value = 9; break;
						}
						if(currentNumberInput < max_value){
							currentNumberInput++;
						}else{
							currentNumberInput = 0;
						}
					}else{
						UIsubCount = 0;
					}
				}
				else if(current_UI == SET_ALARM){
					if(UIsubCount > 0 && UIsubCount < 5){
						newTimePickerValue = 1;
						int max_value;
						switch(UIsubCount){
							case 1: max_value = 2; break;
							case 2: if(timePicker_hourTens > 1){max_value = 3;}else{max_value = 9;}break;
							case 3: max_value = 5; break;
							case 4: max_value = 9; break;
							default: max_value = 9; break;
						}
						if(currentNumberInput < max_value){
							currentNumberInput++;
						}else{
							currentNumberInput = 0;
						}
					}else{
						UIsubCount = 0;
					}
					if(UIsubCount == 0 && getAlarmSetStatus()){//turn off alarm
						resetAlarm();
						stopSunrise();
						setUI(SET_ALARM);
					}
				}else if(current_UI == SET_SUNRISE_DURATION){
					if(UIsubCount > 0 && UIsubCount < 3){
						newTimePickerValue = 1;
						int max_value;
						switch(UIsubCount){
							case 1: max_value = 5; break;
							case 2: max_value = 9;break;
							default: max_value = 9; break;
						}
						if(currentNumberInput < max_value){
							currentNumberInput++;
						}else{
							currentNumberInput = 0;
						}
					}else{
						UIsubCount = 0;
					}
					if(UIsubCount == 0 && getSunriseDurationMin() != 0){//turn off sunrise
						stopSunrise();
						setSunriseDurationMin(0);
						if(getAlarmSetStatus()){ //if alarm set, reset with 0 sunrise
							setAlarm(getAlarmHour(),getAlarmMinute(),getAlarmSecond(),0);
						}
						setUI(SET_SUNRISE_DURATION);
					}
				}else if(current_UI == SET_AUDIO){
					if(UIsubCount > 0 && UIsubCount < 2){
						newTimePickerValue = 1;
						int max_value = 9;
						if(currentNumberInput < max_value){
							currentNumberInput++;
						}else{
							currentNumberInput = 0;
						}
					}else{
						UIsubCount = 0;
					}
					if(UIsubCount == 0 && melodyEnabled()){//turn off alarm
						disableMelody();
						setUI(SET_AUDIO);
					}
				}
			}
		}
		// ----- <<<<<<<<<<<<<<---------------------------------------------------------------------
		else if(getButtonState(2)){ // LEFT BUTTON PRESSED
			printBT("LEFT PRESSED!\n");
			if(getDismissedStatus() == 0){ //not dismissed yet
				dismissAlarmOnUI();
			}else{
				if(current_UI == START){

				}else if(current_UI == SET_TIME){
					if(UIsubCount < 5){
						UIsubCount++;
						newTimePickerValue = 1;
						currentNumberInput = 0;
						char text[15];
						sprintf(text, "SubUI: %d\n", UIsubCount);
						printBT(text);
					}else{
						UIsubCount = 0;
						setUI(SET_TIME);
					}
				}else if(current_UI == SET_ALARM){
					if(UIsubCount < 5){
						UIsubCount++;
						newTimePickerValue = 1;
						currentNumberInput = 0;
						char text[15];
						sprintf(text, "SubUI: %d\n", UIsubCount);
						printBT(text);
					}else{
						UIsubCount = 0;
						setUI(SET_ALARM);
					}
				}else if(current_UI == SET_SUNRISE_DURATION){
					if(UIsubCount < 3){
						UIsubCount++;
						newTimePickerValue = 1;
						int max_value;
						switch(UIsubCount){
							case 1: max_value = 5; break;
							case 2: max_value = 9; break;
							default: max_value = 9; break;
						}
						if(currentNumberInput < max_value){
							currentNumberInput++;
						}else{
							currentNumberInput = 0;
						}
					}else{
						UIsubCount=0;
					}
				}else if(current_UI == SET_AUDIO){
					if(UIsubCount < 2){
						UIsubCount++;
						newTimePickerValue = 1;
						int max_value = 9;
						if(currentNumberInput < max_value){
							currentNumberInput++;
						}else{
							currentNumberInput = 0;
						}
					}else{
						UIsubCount = 0;
					}
				}
			}
		}
		// ----- >>>>>>>>>>>>>>>>>>>>---------------------------------------------------------------------
		else if(getButtonState(3)){ // RIGHT BUTTON PRESSED
			printBT("RIGHT PRESSED!\n");
			if(getDismissedStatus() == 0){ //not dismissed yet
				dismissAlarmOnUI();
			}else{
				changeUI(+1);
			}

		}else if(getButtonState(4)){
			printBT("DOWN PRESSED!\n");
			if(getDismissedStatus() == 0){ //not dismissed yet
				dismissAlarmOnUI();
			}else{
				//CODE HERE
			}

		}
		//----BUTTONS------------------------------------------//
		//----Fast UI------------------------------------------//
		if(current_UI == SET_TIME){
			if(UIsubCount == 0){
				//nothing, wait for ui change or value modification
			}else if(UIsubCount == 1 && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_hourTens = currentNumberInput;
				//enter hour tens
				printLCD(1,4,">");
				printLCD(1,6,".:..");
				char text[2];
				sprintf(text, "%d", timePicker_hourTens);
				printLCD(1,5,text);
			}else if(UIsubCount == 2  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_hourOnes = currentNumberInput;
				//enter hour ones
				printLCD(1,4,">");
				printLCD(1,7,":..");
				char text[2];
				sprintf(text, "%d", timePicker_hourOnes);
				printLCD(1,6,text);
			}else if(UIsubCount == 3  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_minuteTens = currentNumberInput;
				//enter minute tens
				printLCD(1,4,">");
				printLCD(1,9,".");
				char text[2];
				sprintf(text, "%d", timePicker_minuteTens);
				printLCD(1,8,text);
			}else if(UIsubCount == 4  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_minuteOnes = currentNumberInput;
				//enter minute ones
				printLCD(1,4,">");
				//printLCD(1,10," ");//nothing to override
				char text[2];
				sprintf(text, "%d", timePicker_minuteOnes);
				printLCD(1,9,text);
			}else if(UIsubCount == 5  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				UIsubCount = 0;
				setTime(timePicker_hourTens*10 + timePicker_hourOnes, timePicker_minuteTens*10+timePicker_minuteOnes, 0);
				setUI(SET_TIME);


			}
		}else if(current_UI == SET_ALARM){
			if(UIsubCount == 0){
				//nothing, wait for ui change or value modification
			}else if(UIsubCount == 1 && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_hourTens = currentNumberInput;
				//enter hour tens
				printLCD(1,4,">");
				printLCD(1,6,".:..");
				char text[2];
				sprintf(text, "%d", timePicker_hourTens);
				printLCD(1,5,text);
			}else if(UIsubCount == 2  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_hourOnes = currentNumberInput;
				//enter hour ones
				printLCD(1,4,">");
				printLCD(1,7,":..");
				char text[2];
				sprintf(text, "%d", timePicker_hourOnes);
				printLCD(1,6,text);
			}else if(UIsubCount == 3  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_minuteTens = currentNumberInput;
				//enter minute tens
				printLCD(1,4,">");
				printLCD(1,9,".");
				char text[2];
				sprintf(text, "%d", timePicker_minuteTens);
				printLCD(1,8,text);
			}else if(UIsubCount == 4  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_minuteOnes = currentNumberInput;
				//enter minute ones
				printLCD(1,4,">");
				//printLCD(1,10," ");//nothing to override
				char text[2];
				sprintf(text, "%d", timePicker_minuteOnes);
				printLCD(1,9,text);
			}else if(UIsubCount == 5  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				UIsubCount = 0;
				setAlarm(timePicker_hourTens*10 + timePicker_hourOnes, timePicker_minuteTens*10+timePicker_minuteOnes, 0, getSunriseDurationMin()); //TODO duration
				setUI(SET_ALARM);
			}
		}else if(current_UI == SET_SUNRISE_DURATION){
			if(UIsubCount == 0){
				//nothing, wait for ui change or value modification
			}else if(UIsubCount == 1 && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_hourTens = currentNumberInput;
				//enter hour tens
				printLCD(1,4,">    ");
				printLCD(1,6,".");
				char text[2];
				sprintf(text, "%d", timePicker_hourTens);
				printLCD(1,5,text);
			}else if(UIsubCount == 2  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_hourOnes = currentNumberInput;
				//enter hour ones
				//printLCD(1,4,">");
				char text[2];
				sprintf(text, "%d", timePicker_hourOnes);
				printLCD(1,6,text);
			}else if(UIsubCount == 3  && newTimePickerValue == 1){
				newTimePickerValue = 0;
				UIsubCount = 0;
				setSunriseDurationMin(timePicker_hourTens * 10 + timePicker_hourOnes);
				if(getAlarmSetStatus()){ //reset current alarm with new sunrise duration
					setAlarm(getAlarmHour(),getAlarmMinute(),getAlarmSecond(),getSunriseDurationMin());
				}
				setUI(SET_SUNRISE_DURATION);
			}
		}else if(current_UI == SET_AUDIO){
			if(UIsubCount == 0){

				//nothing, wait for ui change or value modification
			}else if(UIsubCount == 1 && newTimePickerValue == 1){
				newTimePickerValue = 0;
				timePicker_hourOnes = currentNumberInput;
				//enter hour tens
				printLCD(1,6,">  ");
				char text[1];
				sprintf(text,"%01.1d",timePicker_hourOnes);
				printLCD(1,7,text);
			}else if(UIsubCount == 2  && newTimePickerValue == 1){
				newTimePickerValue = 0;

				//TODO SET ALARM MELODY FUCKKKKEERRRRR
				printBT("New melody set\n");

				enableMelody();
				setMelody(timePicker_hourOnes);

				setUI(SET_AUDIO);
			}
		}
		//----Fast UI------------------------------------------//

	}//interval10ms
	//----10ms-------------------------------------------------//
}//function closer

void changeUI(signed int direction){
	if(direction == 1){
		if(current_UI < UIcount-1){
			current_UI+=1;
			setUI(current_UI);
		}else{
			setUI(START); //endless loop scrolling
		}
	}else if(direction == -1){

	}
}

void doUIUpdate10(){ //10ms Called by timer4
	interval_10ms_UI = 1;
}
void doUIUpdate500(){ //500ms Called by timer4
	interval_500ms_UI = 1;
}

void dismissAlarmOnUI(){
	dismissAlarm();
	stopSunrise();
	setLEDColor(0,0,0);
	setAlarm(getAlarmHour(), getAlarmMinute(), getAlarmSecond(), getSunriseDurationMin());
	printBT("Alarm manually dismissed\n");
	printLCD(1,0,"                ");
}
