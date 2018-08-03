/*
 * relay_remote.c
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#include "relay_remote.h"

void init_relay_remote_uart(uint32_t gpio_port, GPIO_TypeDef* gpio_typedef,
		uint16_t pin_source_tx, uint16_t pin_source_rx, uint8_t gpio_af,
		uint32_t gpio_tx, uint32_t gpio_rx) {
	// enable clock and alternative function on gpios
	GPIO_InitTypeDef gpio_init_struct;
	RCC_AHB1PeriphClockCmd(gpio_port, ENABLE);
	GPIO_PinAFConfig(gpio_typedef, pin_source_tx, gpio_af);
	GPIO_PinAFConfig(gpio_typedef, pin_source_rx, gpio_af);
	gpio_init_struct.GPIO_Pin = gpio_tx | gpio_rx;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(gpio_typedef, &gpio_init_struct);

	// initialize USART peripheral
}

