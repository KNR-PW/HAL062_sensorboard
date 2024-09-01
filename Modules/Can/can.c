/**
 ******************************************************************************
 * @file           : can.c
 * @author         : K. Czechowicz, A. Rybojad, J. Prokopczuk, D. Mucha
 * @brief          : Communication threw CAN
 ******************************************************************************
 */

#include "Can/can.h"
#include "leds/leds.h"
#include "waga/timer.h"
#include "waga/hx711.h"
#include "waga/waga.h"

uint8_t initWeight = 0;
uint8_t initTim = 0;


static uint32_t CAN_TxMailbox;
static uint8_t CAN_RxMsg[8];

hx711_t loadcell;

CAN_RxHeaderTypeDef CAN_RxHeader;
CAN_TxHeaderTypeDef CAN_TxHeader;


CAN_HandleTypeDef hcan1;

void CAN_Init(void) {
	CAN_FilterTypeDef sFilterConfig;

	hcan1.Instance = CAN1;
	hcan1.Init.Prescaler = 40u;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_9TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_8TQ;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.AutoRetransmission = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan1) != HAL_OK) {
		volatile uint32_t b = 0;
		b++;
	}
	sFilterConfig.FilterBank = 0u;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
	sFilterConfig.FilterIdHigh = 0xFFFF;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0xFFFF;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 0;
	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
		volatile uint32_t b = 0;
		b++;
	}

	if (HAL_CAN_Start(&hcan1) != HAL_OK) {
		volatile uint32_t b = 0;
		b++;
	}

	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING)
			!= HAL_OK) {
		volatile uint32_t b = 0;
		b++;
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {

	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &CAN_RxHeader, CAN_RxMsg);
	if(CAN_RxHeader.StdId==50)
	{
		if(CAN_RxMsg[0]==1)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		}
		if(CAN_RxMsg[1]==1)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		}
		if(CAN_RxMsg[2]==1)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		}
	}


	// TODO ARBITRALNE WARTOSCI DO RAMKI, USTALIC STRUKTURE
	// flaga do kalibracji wagi
	else if (CAN_RxHeader.StdId == 61) {
		if(CAN_RxMsg[0]==1){	// wlacz zegar posylajacy odczyty wagi
			initTim++;
		}
		if(CAN_RxMsg[1]==1){	// resetuj/kalibruj wage
			initWeight++;
		}
	Leds_toggleLed(LED3);
	}

}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {

	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO1, &CAN_RxHeader, CAN_RxMsg);
//	Leds_toggleLed(LED3);
}

void Can_sendMessage(uint8_t* msg, uint8_t ID)
{
	CAN_TxHeader.StdId = ID;
	CAN_TxHeader.ExtId = 0;
	CAN_TxHeader.IDE = CAN_ID_STD;
	CAN_TxHeader.RTR = CAN_RTR_DATA;
	CAN_TxHeader.DLC = 8;
	uint8_t dane[8];
	for(uint8_t i = 0; i<8; i++)
	{
		dane[i] = msg[i];
	}

	HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeader, dane, &CAN_TxMailbox);
	Leds_toggleLed(LED4);
}

void Can_testMessage(void) {
	CAN_TxHeader.StdId = 20;
	CAN_TxHeader.ExtId = 20;
	CAN_TxHeader.IDE = CAN_ID_STD;
	CAN_TxHeader.RTR = CAN_RTR_DATA;
	CAN_TxHeader.DLC = 8;
	uint8_t dane[8];
	dane[0] = 0xAAu;
	dane[1] = 0xAAu;
	for (int i = 2; i < 8; i++) {
		dane[i] = 0;
	}
	HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeader, dane, &CAN_TxMailbox);
	Leds_toggleLed(LED4);
}

