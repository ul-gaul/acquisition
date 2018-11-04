/*
 * valve.h
 *
 *  Created on: Nov 1, 2018
 *      Author: laplace
 *
 *  Brief:
 *  	Controls the motor's tank valve
 */

#ifndef VALVE_VALVE_H_
#define VALVE_VALVE_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#define VALVE_PORT RCC_AHB1Periph_GPIOE
#define VALVE_TYPEDEF_PORT GPIOE
#define VALVE_GPIO GPIO_Pin_9
#define VALVE_GPIO_PIN_NUMBER 9
#define VALVE_ON_BYTE 0xA0
#define VALVE_OFF_BYTE 0xB0

/*
 * Brief:
 * 		Initialize the valve GPIO on the acquisition
 * 		board.
 */
void init_valve();

/*
 * Brief:
 * 		Sets the valve's enable signal to high.
 */
void set_valve_on();

/*
 * Brief:
 * 		Sets the valve's enable signal to low.
 */
void set_valve_off();

/*
 * Brief:
 * 		Reads the valve's state
 *
 * Return:
 * 		0 if the valve is shut, 1 if it's open.
 */
uint8_t get_valve_state();

#endif /* VALVE_VALVE_H_ */
