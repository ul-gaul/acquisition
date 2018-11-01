/*
 * rf_comm.c
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#include "rf_comm.h"

void init_rfd900() {
	// enable clock and alternative function on GPIOs
	GPIO_InitTypeDef gpio_init_struct;
	RCC_AHB1PeriphClockCmd(RFD_PORT, ENABLE);
	GPIO_PinAFConfig(RFD_TYPEDEF_PORT, RFD_TX, GPIO_AF_USART3);
	GPIO_PinAFConfig(RFD_TYPEDEF_PORT, RFD_RX, GPIO_AF_USART3);
	gpio_init_struct.GPIO_Pin = RFD_RX | RFD_TX;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(RFD_TYPEDEF_PORT, &gpio_init_struct);
	// initialize USART peripheral
	USART_InitTypeDef USART_InitStruct;
	// enable clock for USART peripheral
	RCC_APB2PeriphClockCmd(RFD_USART_PERIPH, ENABLE);
	// initialize and activate USART peripheral
	USART_InitStruct.USART_BaudRate = RFD_BAUDRATE;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(RFD_USART_PERIPH_TYPEDEF, &USART_InitStruct);
	USART_Cmd(RFD_USART_PERIPH_TYPEDEF, ENABLE);
#if RX_ENABLE
	NVIC_InitTypeDef NVIC_InitStruct;
	// enable RX interrupt
	USART_ITConfig(RFD_USART_PERIPH_TYPEDEF, USART_IT_RXNE, ENABLE);
	// initialize NVIC for RX interrupts
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
#endif /* RX_ENABLE */
}

void rfd900_write(uint8_t* src, uint16_t size) {
	for(uint32_t i = 0; i < size; i++) {
		USART_SendData(USART3, src[i]);
	}
}
