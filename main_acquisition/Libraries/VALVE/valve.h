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
 */
uint8_t get_valve_state();

#endif /* VALVE_VALVE_H_ */
