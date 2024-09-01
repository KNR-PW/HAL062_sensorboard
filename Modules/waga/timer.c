/*
 ******************************************************************************
 * @file           : timers.c
 * @author         : D. Mucha
 * @brief          : Timers configuration
 ******************************************************************************
 */

#include <stdint.h>
#include "timer.h"
#include "waga/hx711.h"
#include "waga/waga.h"
#include "Can/can.h"
#include "leds/leds.h"

TIM_HandleTypeDef htim7;
extern hx711_t loadcell;
float liveWeight;
extern uint8_t initTim;

void TIM7_Init(void) {
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 80000-1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 10000 - 1;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		// TODO: error handling needed
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig)
			!= HAL_OK) {
		// TODO: error handling needed
	}

	HAL_TIM_Base_Start_IT(&htim7);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base) {
	if (htim_base->Instance == TIM7) {
		/* Peripheral clock enable */
		__HAL_RCC_TIM7_CLK_ENABLE();
		/* TIM7 interrupt Init */
		HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
	}

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim) {
//	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
}

void TIM7_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim7);
}

// TODO ARBITRALNIE WYBRANA RAMKA NA WYSYLKE POMIAROW WAGI
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM7){
		Leds_toggleLed(LED4);
		float weightA = measure_weight(&loadcell);
		liveWeight = weightA;
		uint8_t bytes[8];
		bytes[0] = ((long)weightA & 0xFF000000) >> 24;
		bytes[1] = ((long)weightA & 0x00FF0000) >> 16;
		bytes[2] = ((long)weightA & 0x0000FF00) >> 8;
		bytes[3] = ((long)weightA & 0x000000FF);
		for(uint8_t i = 4; i<8; i++){
				bytes[i] = 0;
			}
		if (initTim > 0)
		{

		Can_sendMessage(bytes, 60);
		}
	}

}


