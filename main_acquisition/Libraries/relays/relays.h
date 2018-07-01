/*
 * relays.h
 *
 *  Created on: Jun 30, 2018
 *      Author: laplace
 */

#ifndef RELAYS_RELAYS_H_
#define RELAYS_RELAYS_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

typedef struct {
	uint8_t state;
	uint32_t pin_set;
	uint32_t pin_reset;
	uint32_t pin_feedback;
	uint32_t gpio_port;
	GPIO_TypeDef* port_typedef;
} relay;

void init_relay(relay* r, uint32_t set, uint32_t reset, uint32_t feedback, uint32_t port, GPIO_TypeDef* port_typedef);


#endif /* RELAYS_RELAYS_H_ */
