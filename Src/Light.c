/*
 * Light.c
 *
 *  Created on: 21.05.2019
 *      Author: domin
 */
#include "Light.h"

TIM_HandleTypeDef htimLED1, htimLED2;


#define timer_period 10000
#define timer_period_maxused 2000


uint8_t sunriseUpdateReady = 0;
uint8_t sunriseStarted;
int sunrise100msCounter = 0;

int sunriseLightState = 0;

uint8_t sunriseDurationMin = 1;
int r,g,b;

void initLight(TIM_HandleTypeDef *mainhtim1, TIM_HandleTypeDef *mainhtim2){
	htimLED1 = *mainhtim1;
	htimLED2 = *mainhtim2;

	HAL_TIM_Base_Start(&htimLED1);
	HAL_TIM_PWM_Start(&htimLED1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htimLED1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htimLED1,TIM_CHANNEL_3);

	HAL_TIM_Base_Start(&htimLED2);
	HAL_TIM_PWM_Start(&htimLED2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htimLED2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htimLED2,TIM_CHANNEL_4);

	sunriseStarted = 0;
	setLEDColor(2000, 2000, 2000);
	HAL_Delay(500);
	setLEDColor(0,0,0);
}

void startSunrise(uint8_t dur){
	sunriseDurationMin = dur;
	sunrise100msCounter = 0;
	//light10msStepValue = timer_period_maxused / (dur * 60 * 100);
	r=1;
	g=1;
	b=1;
	sunriseStarted = 1;
}
void stopSunrise(){
	sunriseStarted = 0;
	sunrise100msCounter = 0;
	//setLEDColor(2000,2000,2000); //leave the lights on
}

void setLEDColor(int r, int g, int b){
	__HAL_TIM_SET_COMPARE(&htimLED1, TIM_CHANNEL_1, g);
	__HAL_TIM_SET_COMPARE(&htimLED1, TIM_CHANNEL_2, r);
	__HAL_TIM_SET_COMPARE(&htimLED1, TIM_CHANNEL_3, b);

	__HAL_TIM_SET_COMPARE(&htimLED2, TIM_CHANNEL_2, g);
	__HAL_TIM_SET_COMPARE(&htimLED2, TIM_CHANNEL_3, r);
	__HAL_TIM_SET_COMPARE(&htimLED2, TIM_CHANNEL_4, b);
}

void handleSunrise(){
	if(sunriseStarted){
		if(sunriseUpdateReady){
			sunriseUpdateReady = 0;

			int nbrof100ms = sunriseDurationMin *60 * 10;
			int stepValue = timer_period_maxused / (nbrof100ms) * 8;
			if(stepValue < 1){ stepValue = 1;}
			//dark to strong blue
			if(sunrise100msCounter < (nbrof100ms * 1/9)){
				r = 0;
				g = 0;
				b = b + 2 * stepValue;
			}
			//strong blue delay
			else if(sunrise100msCounter < (nbrof100ms * 2/9)){

			}
			//middle blue to bright blue
			else if(sunrise100msCounter < (nbrof100ms * 3/9)){
				r = 0;
				g = g + stepValue;
				b = b - stepValue;
			}
			//bright blue delay
			else if(sunrise100msCounter < (nbrof100ms * 4/9)){
				r=r;
				g=g;
				b=b;
			}
			//bright blue to strong red
			else if(sunrise100msCounter < (nbrof100ms * 5/9)){
				r = r + 2 * stepValue;
				g = g - stepValue;
				b = b - stepValue;
			}
			//strong red to middle orange
			else if(sunrise100msCounter < (nbrof100ms * 6/9)){
				r = r - stepValue;
				g = g + 2 * stepValue;
				b = b;
			}
			//middle orange to strong orange
			else if(sunrise100msCounter < (nbrof100ms * 7/9)){
				r = r + 2 * stepValue;
				g = g + 1 * stepValue;
				b = b;
			}
			//strong orange to white
			else if(sunrise100msCounter < (nbrof100ms * 8/9)){
				r = r;
				g = g;
				b = b +2 * stepValue;
			}
			//white to stronger white
			else if(sunrise100msCounter < (nbrof100ms * 9/9)){
				r = r + stepValue;
				g = g + stepValue;
				b = b + stepValue;
			}else{
				stopSunrise();
			}
			//char text[20];
			//sprintf(text,"R:%d,G:%d,B:%d\n", r,g,b);
			//printBT(text);

			if(r < 0){
				r = 0;
			}
			if(g < 0){
				g = 0;
			}
			if(b < 0){
				b = 0;
			}
			setLEDColor(r,g,b);

		}
	}
}

int getSunriseValue(){
	return r;
}

uint8_t getSunriseDurationMin(){
	return sunriseDurationMin;
}

void setSunriseDurationMin(uint8_t value){
	if(value < 59){
		sunriseDurationMin = value;
	}else{
		value = 0;
	}

}

void doSunriseUpdate(){
	sunriseUpdateReady = 1;
	sunrise100msCounter++;
}


//--------MAXIMS SUNRISE MAGIC-------------------------------------------------
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim1;


/******************************************************************************/
/* Make a smooth transition between the colours
 * It only goes until 60% of the power of the led
 */
/******************************************************************************/
/*void ledTransition(void)
{
	static int increment = 0;
	static int pwmSet[6] = {0, 200, 400, 0, 0, 0};

	if(increment >= 10)
	{
		for(int i = 0; i <= 2; i++)
		{
			if(pwmSet[i] == 600)
			{
				pwmSet[i+3] = 1;
			}
			if(pwmSet[i] == 0)
			{
				pwmSet[i+3] = 0;
			}
			if(pwmSet[i+3] == 1)
			{
				pwmSet[i] -= 2;
			}
			if(pwmSet[i+3] == 0)
			{
				pwmSet[i] += 2;
			}
		}

		increment = 0;

		//First led
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwmSet[1]);
		//Second led
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pwmSet[2]);
		//Third led
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pwmSet[0]);

		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwmSet[1]);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwmSet[2]);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwmSet[0]);

	}
	increment ++;
}
*/

/******************************************************************************/
/* Make a sunrise
 * It only goes until 60% of the power of the led
 * Led1= blue, led2= red, led3= green
 * Simulation of a sunset: Blue, bright blue, red, orange yellow, white
 */
/******************************************************************************/
void ledSunset(uint8_t duration)
{
	static int increment = 0;
	static uint8_t state = 0;

	//pwm led0, pwm led1, pwm led2, select led0, select led1, select led2
	//state 0
	static uint16_t pwmSet[4] = {0, 0, 0, 0};
	//state 1
	//static int pwmSet[6] = {300, 0, 0, 0};
	//state 2
	//static int pwmSet[6] = {300, 0, 0, 100};
	//state 3
	//static int pwmSet[6] = {150, 0, 150, 100};
	//state 4
	//static int pwmSet[6] = {150, 0, 150, 250};
	//state 5
	//static int pwmSet[6] = {0, 300, 0, 250;
	//state 6
	//static int pwmSet[6] = {0, 150, 150, 250};
	//state 7
	//static int pwmSet[6] = {0, 400, 400, 250};


	if(increment >= 1*duration)
	{
		switch (state)
		{
			//case 0: dark to strong blue
			case 0:
				//With duration = 0 it last about 21s to go from 0 to 300
				//End: led0: 300
				//When only one step -> 31s
				if(pwmSet[0] < 300)
				{
					if(pwmSet[0] > 200)
					{
						pwmSet[0] +=2; //blue
					}
					pwmSet[0] ++;
				}
				else state ++;
				break;

			//case 1: delay
			//End: led0: 300
			case 1:

				if(pwmSet[3] < 100)
				{
					pwmSet[3] ++;
				}
				else state ++;
				break;

			//case 2: middle blue to bright blue
			//End: led0: 150, led1: 150
			case 2:

				if(pwmSet[2] < 150 && pwmSet[0] > 150)
				{
					pwmSet[2] ++; //green
					pwmSet[0] --; //blue
				}
				else state ++;
				break;

			//case3:bright blue delay
			//End: led0: 150, led2: 150
			case 3:

				if(pwmSet[3] < 250)
				{
					pwmSet[3] ++;
				}
				else state ++;
				break;

			//case4: bright blue to strong red
			//End: led1: 300
			case 4:

				if(pwmSet[2] > 0 && pwmSet[0] > 0)
				{
					pwmSet[0] --; //blue
					pwmSet[2] --; //green
				}

				if(pwmSet[1] < 300)
				{
					if(pwmSet[1] > 200)
					{
						pwmSet[1] ++; //red
					}
					pwmSet[1] ++;
				}
				else state ++;
				break;

				//case5: strong red to middle orange
				//End: led1: 150, led2: 150
			case 5:
				if(pwmSet[1] > 150 && pwmSet[2] < 150)
				{
					pwmSet[2] +=2; //green
					pwmSet[1] -=2; //red
				}
				else state ++;
				break;

				//case6: middle orange to stronger orange
				//End: led1: 400, led2: 400
			case 6:
				if(pwmSet[1] < 400 )
				{
					pwmSet[2] +=2; //green
					pwmSet[1] +=2; //red
				}
				else state ++;
				break;

				//case7: strong orange to white
				//End: led1: 400, led2: 400, led0: 400
			case 7:
				if(pwmSet[0] < 400 )
				{
					pwmSet[0] +=2; //blue
				}
				else state ++;
				break;

				//case8: white to stronger white
				//End: led1: 600, led2: 600, led0: 600
			case 8:
				if(pwmSet[0] < 600 )
				{
					pwmSet[0] ++; //all
					pwmSet[2] ++;
					pwmSet[1] ++;
				}
				else state ++;
				break;

				//case8: white to stronger white
				//End: led1: 600, led2: 600, led0: 600
			case 9:
				//if(int function = 1)
				//{
					pwmSet[0] = 0;
					pwmSet[2] = 0;
					pwmSet[1] = 0;
				//}
				break;

		}

		increment = 0;

		//First led
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwmSet[0]);
		//Second led
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pwmSet[1]);
		//Third led
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pwmSet[2]);

		//Other leds
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwmSet[1]);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwmSet[2]);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwmSet[0]);
	}
	increment ++;
}

