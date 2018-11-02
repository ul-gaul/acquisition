/*
 * valve.c
 *
 *  Created on: Nov 1, 2018
 *      Author: laplace
 */

#include "valve.h"

void init_valve() {
	// enable the clock on the GPIO port
	RCC_AHB1PeriphClockCmd(VALVE_PORT, ENABLE);
	// configure GPIOs as output
	GPIO_InitTypeDef GPIO_InitOut;
	GPIO_InitOut.GPIO_Pin = VALVE_GPIO;
	GPIO_InitOut.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitOut.GPIO_OType = GPIO_OType_PP;
	GPIO_InitOut.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitOut.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(VALVE_TYPEDEF_PORT, &GPIO_InitOut);
}

void set_valve_on() {
	GPIO_SetBits(VALVE_TYPEDEF_PORT, VALVE_GPIO);
}

void set_valve_off() {
	GPIO_ResetBits(VALVE_TYPEDEF_PORT, VALVE_GPIO);
}

uint8_t get_valve_state() {
	uint16_t a;
	a = VALVE_TYPEDEF_PORT->ODR;
	return 0;
}
