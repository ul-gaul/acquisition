/*
 * leds.h
 *
 *  Created on: Sep 16, 2018
 *      Author: laplace
 */

#ifndef LEDS_LEDS_H_
#define LEDS_LEDS_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// LED port and number defines
#define LED_PORT RCC_AHB1Periph_GPIOE
#define LED_TYPEDEF_PORT GPIOE
#define LED1 GPIO_Pin_10
#define LED2 GPIO_Pin_11
#define LED3 GPIO_Pin_12
#define LED4 GPIO_Pin_13
#define LED5 GPIO_Pin_14
#define LED6 GPIO_Pin_15

/*
 * Brief:
 * 		Initialize the LEDs on the acquisition board.
 */
void init_leds();

#endif /* LEDS_LEDS_H_ */
