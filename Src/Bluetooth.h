/*
 * Bluetooth.h
 *
 *  Created on: 21.05.2019
 *      Author: domin
 */
#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "stm32f1xx_hal.h"
#include "LCD.h"
#include "Light.h"
#include "string.h"
#include "Time.h"
#include "Button.h"
#include "UI.h"

void initBluetooth();
void bluetoothCmdParse();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void printBT(char* text);
void handleBluetoothCmd();
void startBluetoothListener();

#endif /* BLUETOOTH_H_ */
