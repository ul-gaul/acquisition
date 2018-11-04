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
#include "imu10dof.h"
#include "rf_comm.h"
#include "gps.h"
#include "leds.h"
#include "valve.h"
#include "rocket_packet.h"

/* Private macro */

/* Private variables */
// timer counter variable
static __IO uint32_t tim2_ms_counter;
/* Private function prototypes */
void init_tim2();
void enable_tim2_interrupts();
void TIM2_IRQHandler();
void TIM2_ms_delay_decrement(void);
void delay_ms(__IO uint32_t delay);

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void) {

	/**
	*  IMPORTANT NOTE!
	*  The symbol VECT_TAB_SRAM needs to be defined when building the project
	*  if code has been located to RAM and interrupts are used.
	*  Otherwise the interrupt table located in flash will be used.
	*  See also the <system_*.c> file and how the SystemInit() function updates
	*  SCB->VTOR register.
	*  E.g.  SCB->VTOR = 0x20000000;
	*/

	/* Application code goes here */
	init_leds();
	init_valve();
	set_led_on(LED1);
	set_led_on(LED2);
	set_led_on(LED3);
	set_led_off(LED4);
	set_led_off(LED5);
	set_led_off(LED6);

	/* Infinite loop */
	uint16_t ledstate;
	unsigned char test_str[4] = "test";
	while (1) {
		set_valve_on();
		get_valve_state();
		set_valve_off();
		get_valve_state();
		rfd900_write(test_str, 4);
		set_led_off(LED1);
		rfd900_write(test_str, 4);
		set_led_on(LED1);
	}
}

/* Private functions */

void init_tim2() {
	RCC_AHB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInit;
	timerInit.TIM_Prescaler = 40000;
	timerInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerInit.TIM_Period = 500;
	timerInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInit);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void enable_tim2_interrupts() {
	NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

void TIM2_IRQHandler() {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		// decrement ms count value
		TIM2_ms_delay_decrement();
	}
}

void TIM2_ms_delay_decrement(void) {
	if(tim2_ms_counter > 0) tim2_ms_counter--;
}

void delay_ms(__IO uint32_t delay) {
	tim2_ms_counter = delay;
	while(tim2_ms_counter);
}

/*
 * Callback used by stm32f4_discovery_audio_codec.c.RCC_APB1RCC_AHB1PeriphClockCmd
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
