/*
 * Bluetooth.c
 *
 *  Created on: 21.05.2019
 *      Author: domin
 */

#include "Bluetooth.h"

#define bt_command_length 30
#define bt_buffer_size 50

char bluetooth_data[bt_command_length];
//uint8_t valid_cmd_index = 0xFF;

UART_HandleTypeDef huartBT;
char command[bt_command_length] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char command_ready = 0;

void initBluetooth(UART_HandleTypeDef* huartmain){
	huartBT = *huartmain;
}

void bluetoothCmdParse(){
	int separator = 0;
	int terminator = 0;
	command_ready = 0;
	for(int i = 0; (i < bt_command_length) && (separator == 0 || terminator == 0); i++){
		if(command[i] == '='){
			separator = i;
		}else if(command[i] == ';'){
			terminator = i;
		}
	}
	char instruction[separator], parameter[terminator-separator];
	if((separator != 0) && (terminator != 0) && (separator < terminator)){
		for(int i = 0; i < separator-1; i++){
			instruction[i] = command[i+1];
		}
		instruction[separator-1] = '\0';
		for(int i = separator+1; i < terminator; i++){
			parameter[i-separator-1] = command[i];
		}
		parameter[terminator-separator-1] = '\0';
		for(int i = 0; i < bt_command_length; i++){
			command[i] = 0;
		}
		printBT(">>"); printBT(instruction); printBT(", "); printBT(parameter); printBT("\n");
	}
	char setTimeCommand[8] = "SetTime";
	setTimeCommand[7] = '\0';
	//if(strcmp(instruction,setTime) == 0){
		setTimeString(parameter);
	//}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART3){
	  command_ready = 1;
  }
}

void printBT(char* text){
	int length = strlen(text);
	HAL_UART_Transmit(&huartBT, text, length, 200);
}

void handleBluetoothCmd(){
	if(command_ready){ //BLUETOOTH ECHO
		for(int i = 0; i < bt_command_length; i++){
		  command[i] = bluetooth_data[i];
		}
		//HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, bluetooth_data, command, bt_command_length); //Transfer data to command memory to allow new command to arrive
		HAL_UART_Receive_DMA(&huartBT, bluetooth_data, bt_command_length);
		command_ready = 0;
		bluetoothCmdParse();
	}
}
void startBluetoothListener(){
	HAL_UART_Receive_DMA(&huartBT, bluetooth_data, bt_command_length); //TODO CHANGE BACK TO &BLUETOOTH_DATA?
}
