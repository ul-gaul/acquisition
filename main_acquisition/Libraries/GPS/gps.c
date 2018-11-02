/*
 * gps.c
 *
 *  Created on: November 1, 2018
 *      Author: Guy-Philippe Nadond
 */

#include "gps.h"


//how to use :
//call the initGps() Function once
//call updateGps() with a gpsStruct as parameter, it will modify this struct based on the gps data


void initGps(void)
{
	// enable clock and alternative function on GPIOs
	GPIO_InitTypeDef gpio_init_struct;
	RCC_AHB1PeriphClockCmd(RFD_PORTgps, ENABLE);
	GPIO_PinAFConfig(RFD_TYPEDEF_PORTgps, RFD_TXgps, GPIO_AF_USART1);
	GPIO_PinAFConfig(RFD_TYPEDEF_PORTgps, RFD_RXgps, GPIO_AF_USART1);
	gpio_init_struct.GPIO_Pin = RFD_RXgps | RFD_TXgps;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(RFD_TYPEDEF_PORTgps, &gpio_init_struct);
	// initialize USART peripheral
	USART_InitTypeDef USART_InitStruct;
	// enable clock for USART peripheral
	RCC_APB2PeriphClockCmd(RFD_USART_PERIPHgps, ENABLE);
	// initialize and activate USART peripheral
	USART_InitStruct.USART_BaudRate = RFD_BAUDRATEgps;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(RFD_USART_PERIPH_TYPEDEFgps, &USART_InitStruct);
	USART_Cmd(RFD_USART_PERIPH_TYPEDEFgps, ENABLE);
#if RX_ENABLE
	NVIC_InitTypeDef NVIC_InitStruct;
	// enable RX interrupt
	USART_ITConfig(RFD_USART_PERIPH_TYPEDEFgps, USART_IT_RXNE, ENABLE);
	// initialize NVIC for RX interrupts
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
#endif /* RX_ENABLE */
}

/**
**===========================================================================
**
**  Abstract: Get_Byte (blocking mode)
**
**===========================================================================
*/

uint8_t Serial_GetByte(USART_TypeDef *USARTx)
{
    while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
    return USART_ReceiveData(USARTx);
}


void PA10_Serial_ReadLine(unsigned char *uCharArray, unsigned int ArraySize) //modify the received char array/string to something of the form of $XXXXXXXXXXXXXXX\n
{
    unsigned char currentChar;
	do
    {
		currentChar = Serial_GetByte(USART1);
    }while(currentChar != '$');
    unsigned char ii = 0;
    while(currentChar != '\n' || ii < ArraySize)
    {
    	uCharArray[ii] = currentChar;
    	ii++;
    	currentChar = Serial_GetByte(USART1);
    }
    //do not flush data :
    //GPRMC
    //GPGLL
    //GPGGA
    //possible optimisation :
    //flush anything that isn't one of the previous data
}

void updateGps(gpsData *gpsStruct)
{
    unsigned char gpsDataString[80] = {0}; //important to put the array size from here
	PA10_Serial_ReadLine(gpsDataString, 80); // into this function right there
	if(gpsDataString[5] == 'L') // String parsing section si GPGLL
	{
		unsigned char subsetLatitude[20] = {0};
		unsigned char currentStart = 7;
		unsigned char i = 0;
		unsigned char GTFO = 0;
		while(i < 20 || GTFO == 0)
		{
			unsigned char temp = gpsDataString[currentStart+i];
			if(temp != ',')
			{
				subsetLatitude[i] = temp;
			}
			else // rentre ici lorsqu'il frappe la virgule
			{
				currentStart += i + 1; // valeur qui faut starter pour avoir dequoi de pertinent
				GTFO = 1;
			}
			i++;
		}
		gpsStruct->latitude = (float)(atof(((char)subsetLatitude)));
		gpsStruct->NSIndicator = gpsDataString[currentStart]; // la position retourne sois N ou S
		currentStart += 2; // +2 because you move past the comma and get the first position of the longitude
		GTFO = 0;
		i = 0;
		while(i < 20 || GTFO == 0)
		{
			unsigned char temp = gpsDataString[currentStart+i];
			if(temp != ',')
			{
				subsetLatitude[i] = temp; // can reuse the same variable because the longitude is usually longer, if something break, create a new subset variable instead
			}
			else // rentre ici lorsqu'il frappe la virgule
			{
				currentStart += i + 1; // valeur qui faut starter pour avoir dequoi de pertinent
				GTFO = 1;
			}
			i++;
		}
		gpsStruct->longitude = (float)atof(((char)subsetLatitude));
		gpsStruct->EWIndicator = gpsDataString[currentStart]; // la position retourne sois E ou W
	}
}
