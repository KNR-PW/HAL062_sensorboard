/*
 ******************************************************************************
 * @file           : timers.h
 * @author         : D. Mucha
 * @brief          : Timers configuration
 ******************************************************************************
 */

#ifndef WAGA_TIMER_H_
#define WAGA_TIMER_H

#include <stm32f4xx_hal.h>
#include "can/can.h"

extern TIM_HandleTypeDef htim7; // measuring speed - TIM14

void TIM7_Init(void);


#endif /* MOTORS_TIMERS_H_ */
