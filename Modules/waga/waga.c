/*
 * waga.c
 *
 *  Created on: Sep 1, 2024
 *      Author: ddd
 */

#include "waga/hx711.h"
#include "waga/waga.h"

void init_weight(hx711_t *hx711){
//  	char buffer[128] = {0};


  	/* Initialize the hx711 sensors */
  	hx711_init(hx711, GPIOA, GPIO_PIN_9, GPIOA, GPIO_PIN_10);

  	/* Configure gain for each channel (see datasheet for details) */
  	set_gain(hx711, 64);

  	/* Set HX711 scaling factor (see README for procedure) */
  	set_scale(hx711, 410.45);

  	/* Tare weight */
  	tare_all(hx711, 1);

  }

  /**
   * @brief  Weight Measuring Function
   * @retval the weight measured for each associated channel
   */

  float measure_weight(hx711_t* hx711){
  	float weightA = 0;

//  	char buffer[128] = {0};
  	// Measure the weight for channel A
  	weightA = get_weight(hx711, 1, CHANNEL_A);
//  	liveWeight = weightA;

  	return weightA;
  }


