/*
 * gps.h
 *
 *  Created on: November 1, 2018
 *      Author: Guy-Philippe Nadon
 */

#ifndef GPS_GPS_H_
#define GPS_GPS_H_



#define RX_ENABLE 1


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <stm32f4xx_usart.h>
#include <stdlib.h>
#include <string.h>
//mettre des noms qui ont de l'allure icite
#define RFD_PORTgps RCC_AHB1Periph_GPIOA
#define RFD_TYPEDEF_PORTgps GPIOA
#define RFD_USART_PERIPHgps RCC_APB2Periph_USART1
#define RFD_USART_PERIPH_TYPEDEFgps USART1
#define RFD_TXgps GPIO_Pin_9
#define RFD_RXgps GPIO_Pin_10
#define RFD_BAUDRATEgps 115200

/* Receive buffer for DMA */
#define DMA_RX_BUFFER_SIZE          128
uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];

/* Buffer after received data */
#define UART_BUFFER_SIZE            256
uint8_t UART_Buffer[UART_BUFFER_SIZE];
size_t Write, Read;

USART_InitTypeDef USART_InitStruct;
DMA_InitTypeDef DMA_InitStruct;
GPIO_InitTypeDef GPIO_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

typedef struct {
	// GPS values
	float latitude;
	float longitude;
	char NSIndicator;
	char EWIndicator;
} gpsData;
void DMA1_Stream5_IRQHandler(void);
void USART1_IRQHandler(void);
void initGps(void);
unsigned char customSplit();
void updateGps(gpsData* gpsStruct);

#endif /* GPS_GPS_H_ */
