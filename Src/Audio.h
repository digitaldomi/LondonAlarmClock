/*
 * Audio.h
 *
 *  Created on: 06.06.2019
 *      Author: domin
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include "Bluetooth.h"
#include "Button.h"
#include "LCD.h"
#include "Light.h"
#include "Time.h"
#include "UI.h"

void initAudio();
void setMelody(uint8_t newMelody);
uint8_t getMelody();
void enableMelody();
void disableMelody();
uint8_t melodyEnabled();

#endif /* AUDIO_H_ */
