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
	/* Initialize system */
	SystemInit();

    /* Init GPIO pins for UART */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_DMA2EN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    (void)RCC->AHB1ENR;
    (void)RCC->APB2ENR;

    /* Set alternate functions */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    /* Init GPIO pins */
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure UART setup */
    USART_StructInit(&USART_InitStruct);
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruct);

    /* Enable global interrupts for USART */
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    /* Enable USART */
    USART_Cmd(USART1, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    /* Enable IDLE line detection for DMA processing */
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

    /* Configure DMA for USART RX, DMA1, Stream5, Channel4 */
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_Channel = DMA_Channel_4;
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)DMA_RX_Buffer;
    DMA_InitStruct.DMA_BufferSize = DMA_RX_BUFFER_SIZE;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Init(DMA2_Stream5, &DMA_InitStruct);

    /* Enable global interrupts for DMA stream */
    NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

    /* Enable transfer complete interrupt */
    DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA2_Stream5, ENABLE);
}

void USART_Parser(unsigned char *uCharArray, unsigned int ArraySize) //modify the received char array/string to something of the form of $XXXXXXXXXXXXXXX\n
{
    unsigned char currentChar = 0;
    unsigned char getOut = 0;
    unsigned int i = 0;
	do
    {
		currentChar = UART_Buffer[i];
		++i;
    }while(currentChar != '$' && i < UART_BUFFER_SIZE);
	--i;
	if(i >= UART_BUFFER_SIZE)
	{
		getOut = 1;
	}
	//currentChar = UART_Buffer[i];
    unsigned char ii = 0;
    while(currentChar != '\n' && ii < ArraySize && getOut == 0 && (i + ii + 1) < UART_BUFFER_SIZE)
    {
    	uCharArray[ii] = currentChar;
    	++ii;
    	currentChar = UART_Buffer[i + ii];
    }
    //do not flush data :
    //GPRMC
    //GPGLL
    //GPGGA
    //possible optimisation :
    //flush anything that isn't one of the previous data
}

unsigned char customSplit() // TODO : implementing a prettier way to split the char array between ','
{
	return 1;
}


/*
 * updateGps is a fonction that update the gpsStruct that it receive with data from the gps
 * it will take the received latitude, longitude, EW and NS indicator from the gps and update it
 * in the gpsData struct that it receive in function arg
 */

//NOTE TO YOURSELF : S'assurer que les subsetlattitude on bien un nombre et non une lettre
//sinon la conversion avec atof se transforme en 0.000
void updateGps(gpsData *gpsStruct)
{
    unsigned char gpsDataString[80]; //important to put the array size from here
    memset(gpsDataString, 0, 80);
	USART_Parser(gpsDataString, 80); // into this function right there
	unsigned char getIn = 0;
	unsigned short utcGetIn = 0;
	unsigned char currentStart = 7; //Cette variable est l'index qui indique la premiere position de la latitude dans le gpsDataString array
	if(gpsDataString[4] == 'G') // check if data is GPGGA ex gpsDataString[0 to 4] = "$GPGG"
	{
		getIn = 1;
		unsigned char i = 0;
		unsigned char GTFO = 0;
		unsigned char subsetCharArray[20];
	    memset(subsetCharArray, 0, 20);
		while(i < 20 && GTFO == 0)
		{
			unsigned char temp = gpsDataString[currentStart+i];
			if(temp == ',')
			{
				currentStart += i + 1; // valeur qui faut starter pour avoir dequoi de pertinent
				// + 1 car je vais a la premiere position de la donner de latitude
				//string ex : "$GPGGA,111636.932,2447.0949,N,12100.5223,E,1,11,0.8,118.2,M,,,,0000*02<CR><LF>"
				GTFO = 1;
			}
			else
			{
				subsetCharArray[i] = temp;
			}
			i++;
		}
		gpsStruct->UTCTime = (double)atof((char *)subsetCharArray);
	}
	else if(gpsDataString[4] == 'M') // check if data is GPGMR
	{
		getIn = 1;
		unsigned char i = 0;
		unsigned char GTFO = 0;
		unsigned char subsetCharArray[20];
	    memset (subsetCharArray, 0, 20);
		while(i < 20 && GTFO == 0)
		{
			unsigned char temp = gpsDataString[currentStart+i];
			if(temp == ',')
			{
				currentStart += i + 3; // valeur qui faut starter pour avoir dequoi de pertinent
				// + 3 car je saute l'octet de status (le "A" dans l'exemple)
				//string ex : "$GPRMC,111636.932,A,2447.0949,N,12100.5223,E,000.0,000.0,030407,,,A*61<CR><LF>"
				GTFO = 1;
			}
			else
			{
				subsetCharArray[i] = temp;
			}
			i++;
		}
		gpsStruct->UTCTime = (double)atof((char *)subsetCharArray);
	}
	else if(gpsDataString[5] == 'L') // check if data is GPGLL
	{
		getIn = 1;
		currentStart = 7; // string ex: "$GPGLL,2447.0944,N,12100.5213,E,112609.932,A,A*57<CR><LF>"
		utcGetIn = 1;
	}
	if(getIn == 1) // String parsing section si dans gpsDataString on a GPRMC GPGLL GPGGA
	{
		///those 4 value are there to check at the end if there was corruption or not
		double latTemp = 0.0f;
		double lonTemp = 0.0f;
		char NSIndicatorTemp;
		char EWIndicatorTemp;
		////

		unsigned char subsetLatitude[20];
	    memset (subsetLatitude, 0, 20);
		unsigned char i = 0;
		unsigned char GTFO = 0;
		while(i < 20 && GTFO == 0)
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
		latTemp = (double)atof((char *)subsetLatitude); //return a double from a char table
		NSIndicatorTemp = gpsDataString[currentStart]; // should be either 'N' or 'S'
		currentStart += 2; // +2 because you move past the comma and get the first position of the longitude
		GTFO = 0;
		i = 0;
	    memset (subsetLatitude, 0, 20); // reuse the same char array as before, but reset all its value just to be sure
		while(i < 20 && GTFO == 0)
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
		lonTemp = (double)atof((char *)subsetLatitude); //return a double from a char table
		EWIndicatorTemp = gpsDataString[currentStart]; // should be either 'E' or 'W'

		if (utcGetIn == 1) // only enter if the type is GPGLL
		{
			//The role of the following is to extract the time from the GPGLL data string/char array
			currentStart += 2;
			GTFO = 0;
			unsigned char i = 0;
			unsigned char subsetCharArray[20];
		    memset(subsetCharArray, 0, 20);
			while(i < 20 && GTFO == 0)
			{
				unsigned char temp = gpsDataString[currentStart+i];
				if(temp == ',')
				{
					currentStart += i + 1;
					//string ex : "$GPGGA,111636.932,2447.0949,N,12100.5223,E,1,11,0.8,118.2,M,,,,0000*02<CR><LF>"
					GTFO = 1;
				}
				else
				{
					subsetCharArray[i] = temp;
				}
				i++;
			}
			gpsStruct->UTCTime = (double)atof((char *)subsetCharArray);
		}

		if ((EWIndicatorTemp == 'E' || EWIndicatorTemp == 'W') && (NSIndicatorTemp == 'N' || NSIndicatorTemp == 'S')) // test if data is corrupt or not
		{
			gpsStruct->latitude = latTemp;
			gpsStruct->NSIndicator = NSIndicatorTemp; // la position retourne sois N ou S
			gpsStruct->longitude = lonTemp;
			gpsStruct->EWIndicator = EWIndicatorTemp; // la position retourne sois E ou W
		}
	}
}
/////////
///Fontion USART
/////////
/**
 * \brief       Global interrupt handler for USART1
 */
void USART1_IRQHandler(void) {
    /* Check for IDLE flag */
    if (USART1->SR & USART_FLAG_IDLE) {         /* We want IDLE flag only */
        /* This part is important */
        /* Clear IDLE flag by reading status register first */
        /* And follow by reading data register */
        volatile uint32_t tmp;                  /* Must be volatile to prevent optimizations */
        tmp = USART1->SR;                       /* Read status register */
        tmp = USART1->DR;                       /* Read data register */
        (void)tmp;                              /* Prevent compiler warnings */
        DMA2_Stream5->CR &= ~DMA_SxCR_EN;       /* Disabling DMA will force transfer complete interrupt if enabled */
    }
}

/**
 * \brief       Global interrupt handler for DMA2 stream5
 * \note        Except memcpy, there is no functions used to
 */
void DMA2_Stream5_IRQHandler(void) {
    size_t len, tocopy;
    uint8_t* ptr;

    /* Check transfer complete flag */
    if (DMA2->HISR & DMA_FLAG_TCIF5) {
        DMA2->HIFCR = DMA_FLAG_TCIF5;           /* Clear transfer complete flag */

        /* Calculate number of bytes actually transfered by DMA so far */
        /**
         * Transfer could be completed by 2 events:
         *  - All data actually transfered (NDTR = 0)
         *  - Stream disabled inside USART IDLE line detected interrupt (NDTR != 0)
         */
        len = DMA_RX_BUFFER_SIZE - DMA2_Stream5->NDTR;
        tocopy = UART_BUFFER_SIZE - Write;      /* Get number of bytes we can copy to the end of buffer */

        /* Check how many bytes to copy */
        if (tocopy > len) {
            tocopy = len;
        }

        /* Write received data for UART main buffer for manipulation later */
        ptr = DMA_RX_Buffer;
        memcpy(&UART_Buffer[Write], ptr, tocopy);   /* Copy first part */

        /* Correct values for remaining data */
        Write += tocopy;
        len -= tocopy;
        ptr += tocopy;

        /* If still data to write for beginning of buffer */
        if (len) {
            memcpy(&UART_Buffer[0], ptr, len);      /* Don't care if we override Read pointer now */
            Write = len;
        }

        /* Prepare DMA for next transfer */
        /* Important! DMA stream won't start if all flags are not cleared first */
        DMA2->HIFCR = DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_TCIF5 | DMA_FLAG_TEIF5;
        DMA2_Stream5->M0AR = (uint32_t)DMA_RX_Buffer;   /* Set memory address for DMA again */
        DMA2_Stream5->NDTR = DMA_RX_BUFFER_SIZE;    /* Set number of bytes to receive */
        DMA2_Stream5->CR |= DMA_SxCR_EN;            /* Start DMA transfer */
    }
}
