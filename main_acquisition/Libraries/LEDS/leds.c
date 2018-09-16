/*
 * leds.c
 *
 *  Created on: Sep 16, 2018
 *      Author: laplace
 */

#include "leds.h"

void init_leds() {
	// enable the clock on the GPIO port
	RCC_AHB1PeriphClockCmd(LED_PORT, ENABLE);
	// configure GPIOs as output
	GPIO_InitTypeDef GPIO_InitOut;
	GPIO_InitOut.GPIO_Pin = LED1 | LED2 | LED3 | LED4 | LED5 | LED6;
	GPIO_InitOut.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitOut.GPIO_OType = GPIO_OType_PP;
	GPIO_InitOut.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitOut.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_TYPEDEF_PORT, &GPIO_InitOut);
}

