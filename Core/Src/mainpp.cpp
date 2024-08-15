/*
 * mainpp.cpp
 *
 *  Created on: Aug 13, 2024
 *      Author: yujungHuang
 */
/*
 * stm1: send count message every second
 * stm2: as soon as receive the message, send count+1 back
 *
 * variables in live expression
 * stm1: t, r, count, rcv_count1, stm1_Ttext, stm1_Rtext
 * stm2: t, r, rcv_count2, stm2_Ttext, stm2_Rtext
 */
#include "mainpp.h"
#include "stm32f4xx_hal.h"
#include "string.h"

extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart3;
int t=0;
int r=0;
float count=0;
float coord1[3]={0};
float coord1_5[3]={0};
float coord2[3]={0};
uint8_t stm1_Ttext[3*sizeof(float)]={0};
uint8_t stm1_Rtext[3*sizeof(float)]={0};
uint8_t stm2_Ttext[3*sizeof(float)]={0};
uint8_t stm2_Rtext[3*sizeof(float)]={0};

void setup(){
	// --- stm1 code --- //
	HAL_UART_Receive_DMA(&huart3, stm1_Rtext, sizeof(float));
	HAL_TIM_Base_Start_IT(&htim6);

	//HAL_UART_Receive_DMA(&huart3, stm2_Rtext, sizeof(float));//stm2
}

void FLOAT_CHAR_Convert(void *dest, void* src){
	for(int i=0;i<3;i++){
		memcpy(dest+sizeof(float)*i, src+sizeof(float)*i,sizeof(float));
	}
}

void main_function(){
	setup();
	while(1){}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==USART3){
		//FLOAT_CHAR_Convert(coord1_5,stm1_Ttext);
		t++;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==USART3){
		// --- stm1 code --- //
		HAL_UART_Receive_DMA(&huart3, stm1_Rtext, sizeof(float));
		FLOAT_CHAR_Convert(coord1, stm1_Rtext);

		// --- stm2 code --- //
		//HAL_UART_Receive_DMA(&huart3, stm2_Rtext, sizeof(float));
		//FLOAT_CHAR_Convert(coord2, stm2_Rtext);
		//FLOAT_CHAR_Convert(stm2_Ttext, coord2);
		//HAL_UART_Transmit_DMA(&huart3, stm2_Ttext, sizeof(float));

		r++;
	}
}

// --- stm1 code --- //
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance==TIM6){
		count++;
		coord1[0]=count+1;
		coord1[1]=count*2;
		coord1[2]=count*0.5;
		FLOAT_CHAR_Convert(stm1_Ttext, coord1);
		HAL_UART_Transmit_DMA(&huart3, stm1_Ttext, sizeof(stm1_Ttext));
	}
}
