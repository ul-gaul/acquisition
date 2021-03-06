/*
 * rf_comm.c
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#include "rf_comm.h"

void init_rfd900() {
	// enable clock for USART3 and GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// configure GPIOs
	GPIO_InitTypeDef gpio_init_struct;
	gpio_init_struct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RFD_TYPEDEF_PORT, &gpio_init_struct);

	// connect USART pins to their alternate functions
	GPIO_PinAFConfig(RFD_TYPEDEF_PORT, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(RFD_TYPEDEF_PORT, GPIO_PinSource9, GPIO_AF_USART3);

	// initialize USART peripheral
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = RFD_BAUDRATE;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART3, &USART_InitStruct);

	// enable USART
	USART_Cmd(USART3, ENABLE);

	// setup interrupts
	USART_ITConfig(RFD_USART_PERIPH_TYPEDEF, USART_IT_RXNE, ENABLE);

#if RFD_RX_ENABLE
	// initialize NVIC for RX interrupts
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
#endif /* RX_ENABLE */
}

void rfd900_write(uint8_t* src, uint16_t size) {
	for(uint32_t i = 0; i < size; i++) {
		while(!(USART3->SR & USART_FLAG_TXE));
		USART_SendData(USART3, src[i]);
	}
}

void rfd900_send_packet(RocketPacket* pkt) {
	serialize_rocket_packet(pkt, rocket_packet_serialized);
	rfd900_write(rocket_packet_serialized, ROCKET_PACKET_SIZE);
}
