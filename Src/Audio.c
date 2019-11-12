/*
 * Audio.c
 *
 *  Created on: 06.06.2019
 *      Author: domin
 */

#include "Audio.h"

uint8_t melody = 0;
uint8_t melodyEnable = 1;

void initAudio(){

	enableMelody();
}

void setMelody(uint8_t newMelody){
	melody = newMelody;
}

uint8_t getMelody(){
	return melody;
}

void enableMelody(){
	melodyEnable = 1;
}
void disableMelody(){
	melodyEnable = 0;
}
uint8_t melodyEnabled(){
	return melodyEnable;
}
