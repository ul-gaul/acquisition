/*
 * relays.c
 *
 *  Created on: Jun 30, 2018
 *      Author: laplace
 */


#include "relays.h"

void init_relay(relay* r, uint32_t set, uint32_t reset, uint32_t feedback, uint32_t port, GPIO_TypeDef* port_typedef) {
	// enable clock on GPIO port
	RCC_AHB1PeriphClockCmd(port, ENABLE);
	// configure output GPIOs
	GPIO_InitTypeDef GPIO_InitOut;
	GPIO_InitOut.GPIO_Pin = set | reset;
	GPIO_InitOut.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitOut.GPIO_OType = GPIO_OType_PP;
	GPIO_InitOut.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitOut.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(port_typedef, &GPIO_InitOut);
	// configure input GPIOs
	GPIO_InitTypeDef GPIO_InitIn;
	GPIO_InitIn.GPIO_Pin = feedback;
	GPIO_InitIn.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitIn.GPIO_OType = GPIO_OType_PP;
	GPIO_InitIn.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitIn.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(port_typedef, &GPIO_InitIn);
	// create relay structure and return it once initialized
	r->state = 0;
	r->pin_set = set;
	r->pin_reset = reset;
	r->pin_feedback = feedback;
	r->gpio_port = port;
	r->port_typedef = port_typedef;
	// initialize output pins to RESET state
	r->port_typedef->BSRRH = r->pin_set;
	r->port_typedef->BSRRH = r->pin_reset;
}

uint8_t set_relay(relay* r) {
	r->port_typedef->BSRRL = r->pin_set;
	// TODO wait 50 ms

	r->port_typedef->BSRRH = r->pin_set;
	return 0;
}

uint8_t reset_relay(relay* r) {
	r->port_typedef->BSRRL = r->pin_reset;
	// TODO wait 50 ms

	r->port_typedef->BSRRH = r->pin_reset;
	return 0;
}

uint8_t get_state_relay(relay* r) {
	if((r->port_typedef->IDR & r->pin_feedback) != (uint32_t) Bit_RESET) {
		return Bit_SET;
	} else {
		return Bit_RESET;
	}
}
