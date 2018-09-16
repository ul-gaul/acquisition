/*
 * rf_comm.h
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#ifndef RF_COMM_RF_COMM_H_
#define RF_COMM_RF_COMM_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


#define RFD_PORT RCC_AHB1Periph_GPIOD
#define RFD_TYPEDEF_PORT GPIOD
#define RFD_USART_PERIPH RCC_APB1Periph_USART3
#define RFD_USART_PERIPH_TYPEDEF USART3
#define RFD_TX GPIO_Pin_8
#define RFD_RX GPIO_Pin_9
#define RFD_BAUDRATE 9600

/*
 * Brief:
 *		Initialize the GPIOs and peripherals used by the RFD900.
 */
void init_rfd900();

/*
 * Brief:
 *		Sends a char array over the USART bus.
 * Return:
 *		The number of bytes sent.
 */
uint32_t rfd900_write(char* src, uint32_t size);

#endif /* RF_COMM_RF_COMM_H_ */
