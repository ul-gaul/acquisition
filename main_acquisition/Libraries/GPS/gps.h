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

typedef struct {
	// GPS values
	float latitude;
	float longitude;
	char NSIndicator;
	char EWIndicator;
} gpsData;

void initGps(void);
uint8_t Serial_GetByte(USART_TypeDef *USARTx);
unsigned char customSplit();
void updateGps(gpsData* gpsStruct);

#endif /* GPS_GPS_H_ */
