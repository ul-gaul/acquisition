/*
Author:	  laplace
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.0.1   2018-06-30

*/

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
/* homemade libraries includes */
#include "imu10dof.h"
#include "l3gd20.h"
#include "rf_comm.h"
#include "gps.h"
#include "leds.h"
#include "rocket_packet.h"

/* Private macro */

/* Private variables */
// timer counter variable
static __IO uint32_t TIM2_ms_counter;
/* Private function prototypes */
void init_TIM2();
//void enable_TIM2_interrupts();
void TIM2_IRQHandler();
void TIM2_ms_delay_decrement(void);
void delay_ms(__IO uint32_t delay);
void USART3_IRQHANDLER();

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

	// configure Systick to 84000 ticks
	// with 168 000 000 ticks/s it means an interrupt every 0.5 ms
	SysTick_Config(SystemCoreClock / 2000);
	
	// global BMP180, L3GD20 and LSM303 structs are declared here
	BMP180_struct bmp180_data;
	L3GD20_struct l3gd20_data;
	LSM303DLHC_struct lsm303_data;

	init_leds();
	init_TIM2();
	set_led_on(LED1);
	set_led_on(LED2);
	set_led_on(LED3);
	set_led_off(LED4);
	set_led_off(LED5);
	set_led_off(LED6);
	init_rfd900();
	initGps();// activate USART1 on PA9 (TX) and PA10 (RX)

	imu10dof_init((struct BMP180_struct *) &bmp180_data, &delay_ms);
	bmp180_data.delay_func = &delay_ms;


	gpsData gpsDataStruct; //struct used to store GPS data, need to malloc

	/* Infinite loop */
	uint8_t ledstate;
	// writing test rocket data and packet
	RocketData rd;
	rd.timestamp = 0;
	//rd.latitude = 46.779013;
	rd.latitude = 0;
	//rd.longitude = -71.276001;
	rd.longitude = 0;
	//rd.altitude = 10000.1;
	rd.altitude = 0;
	//rd.temperature = 25.7;
	rd.temperature = 0;
	//rd.x_accel = 1.56;
	rd.x_accel = 0;
	//rd.y_accel = 2.45;
	rd.y_accel = 0;
	//rd.z_accel = 3.91;
	rd.z_accel = 0;
	//rd.x_magnet = 11.12;
	rd.x_magnet = 0;
	//rd.y_magnet = 12.34;
	rd.y_magnet = 0;
	//rd.z_magnet = 13.56;
	rd.z_magnet = 0;
	//rd.x_gyro = 21.12;
	rd.x_gyro = 0;
	//rd.y_gyro = 22.34;
	rd.y_gyro = 0;
	//rd.z_gyro = 23.56;
	rd.z_gyro = 0;
	RocketPacket rp;
	rp.start_char = ROCKET_PACKET_START;
	rp.data = rd;
	rp.checksum = 0;

	unsigned int num_bytes;
	uint8_t rp_buffer[ROCKET_PACKET_SIZE];
	num_bytes = serialize_rocket_packet(&rp, rp_buffer);

	while (1) {
	    rfd900_write(rp_buffer, num_bytes);
	    delay_ms(500);
	    set_led_off(LED1);
	    rfd900_write(rp_buffer, num_bytes);
	    delay_ms(500);
	    set_led_on(LED1);
	    //Cette partie transfer les donnee recus dans le buffer ou les donnees sont "stockees"
            while (Read != Write) {                 /* Do it until buffer is empty */
                USART1->DR = UART_Buffer[Read++];   /* Start byte transfer */
                while (!(USART1->SR & USART_SR_TXE));   /* Wait till finished */
                if (Read == UART_BUFFER_SIZE) {     /* Check buffer overflow */
                    Read = 0;
                }
            }
    //fonction qui met a jour les donner dans le gpsDataStruct
    updateGps(&gpsDataStruct);
    rd.timestamp = gpsDataStruct.UTCTime;
	rd.latitude = gpsDataStruct.latitude;
	rd.longitude = gpsDataStruct.longitude;
    // read IMU10DOF devices
    L3GD20_Read(&l3gd20_data);

    lsm303dlhc_read_acceleration(&lsm303_data);
    lsm303dlhc_read_magneticfield(&lsm303_data);
    lsm303dlhc_read_temperature(&lsm303_data);

    bmp180_start_temperature(&bmp180_data);
    bmp180_read_temperature(&bmp180_data);
    bmp180_start_pressure(&bmp180_data, BMP180_Sampling_lowpower);
    bmp180_read_pressure(&bmp180_data);
    // update rocket packet with imu10dof data
    rp.data.altitude = bmp180_data.altitude;
    rp.data.temperature = bmp180_data.temperature;
    }
}


/* Private functions */

void init_TIM2() {
	// init timer 82 Mhz / 42 -> 0.525 microsecond / tick
	// period = 1950 -> 1 interrupt / ms
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInit;
	timerInit.TIM_Prescaler = 42;
	timerInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerInit.TIM_Period = 1950;
	timerInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInit);
	// enable timer
	TIM_Cmd(TIM2, ENABLE);
	// enable timer interrupts
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	// init interrupts
	NVIC_InitTypeDef nvic_struct;
	nvic_struct.NVIC_IRQChannel = TIM2_IRQn;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_struct.NVIC_IRQChannelSubPriority = 1;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_struct);
}

void TIM2_IRQHandler() {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		// decrement ms count value
		TIM2_ms_delay_decrement();
	}
}

void TIM2_ms_delay_decrement(void) {
	if(TIM2_ms_counter > 0) TIM2_ms_counter--;
}

void delay_ms(__IO uint32_t delay) {
	TIM2_ms_counter = delay;
	while(TIM2_ms_counter) {

	}
}

void USART3_IRQHANDLER() {
//	char byte_rx;
//	if(USART_GetITStatus(RFD_USART_PERIPH_TYPEDEF, USART_IT_RXNE)) {
//		byte_rx = USART_ReceiveData(RFD_USART_PERIPH_TYPEDEF);
//	}
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
