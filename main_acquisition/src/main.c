/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.0.1   2018-06-30

The MIT License (MIT)
Copyright (c) 2018 STMicroelectronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
/* homemade libraries includes */
#include "relays.h"
#include "relay_remote.h"
#include "imu10dof.h"
#include "rf_comm.h"
#include "gps.h"

/* Private macro */
#define SET_ACQUISITION GPIO_Pin_5
#define RESET_ACQUISITION GPIO_Pin_6
#define FEEDBACK_ACQUISITION GPIO_Pin_4
#define PORT_ACQUISITION RCC_AHB1Periph_GPIOE
#define PORT_TYPEDEF_ACQUISITION GPIOE

#define SET_DEPLOY0 GPIO_Pin_2
#define RESET_DEPLOY0 GPIO_Pin_1
#define FEEDBACK_DEPLOY0 GPIO_Pin_6
#define PORT_DEPLOY0 RCC_AHB1Periph_GPIOD
#define PORT_TYPEDEF_DEPLOY0 GPIOD

#define SET_DEPLOY1 GPIO_Pin_0
#define RESET_DEPLOY1 GPIO_Pin_1
#define FEEDBACK_DEPLOY1 GPIO_Pin_2
#define PORT_DEPLOY1 RCC_AHB1Periph_GPIOE
#define PORT_TYPEDEF_DEPLOY1 GPIOE

#define SET_PAYLOAD GPIO_Pin_0
#define RESET_PAYLOAD GPIO_Pin_1
#define FEEDBACK_PAYLOAD GPIO_Pin_7
#define PORT_PAYLOAD RCC_AHB1Periph_GPIOD
#define PORT_TYPEDEF_PAYLOAD GPIOD

/* Private variables */
// relay declarations
relay* r_acquisition;
relay* r_deploy0;
relay* r_deploy1;
relay* r_payload;
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void) {
	int i = 0;

	/**
	*  IMPORTANT NOTE!
	*  The symbol VECT_TAB_SRAM needs to be defined when building the project
	*  if code has been located to RAM and interrupts are used.
	*  Otherwise the interrupt table located in flash will be used.
	*  See also the <system_*.c> file and how the SystemInit() function updates
	*  SCB->VTOR register.
	*  E.g.  SCB->VTOR = 0x20000000;
	*/

	/* TODO - Add your application code here */
	// initialize relays
	init_relay(r_acquisition, SET_ACQUISITION, RESET_ACQUISITION, FEEDBACK_ACQUISITION, PORT_ACQUISITION, PORT_TYPEDEF_ACQUISITION);
	init_relay(r_deploy0, SET_DEPLOY0, RESET_DEPLOY0, FEEDBACK_DEPLOY0, PORT_DEPLOY0, PORT_TYPEDEF_DEPLOY0);
	init_relay(r_deploy1, SET_DEPLOY1, RESET_DEPLOY1, FEEDBACK_DEPLOY1, PORT_DEPLOY1, PORT_TYPEDEF_DEPLOY1);
	init_relay(r_payload, SET_PAYLOAD, RESET_PAYLOAD, FEEDBACK_PAYLOAD, PORT_PAYLOAD, PORT_TYPEDEF_PAYLOAD);
	/* Initialize LEDs */
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	/* Turn on LEDs */
	STM_EVAL_LEDOn(LED3);
	STM_EVAL_LEDOn(LED4);
	STM_EVAL_LEDOn(LED5);
	STM_EVAL_LEDOn(LED6);

	/* Infinite loop */
	while (1)
	{
		i++;
	}
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
	/* TODO, implement your code here */
	return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
	/* TODO, implement your code here */
	return -1;
}
