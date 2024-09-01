/*
 * waga.h
 *
 *  Created on: Sep 1, 2024
 *      Author: ddd
 */

#ifndef WAGA_WAGA_H_
#define WAGA_WAGA_H_

extern hx711_t loadcell;

void init_weight(hx711_t *hx711);
float measure_weight(hx711_t* hx711);



#endif /* WAGA_WAGA_H_ */
