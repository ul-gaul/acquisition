/*
 * bmp180.h
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#ifndef IMU10DOF_BMP180_H_
#define IMU10DOF_BMP180_H_

#include "stm32f4xx.h"
#include "imu10dof.h"
#include "math.h"

#define BMP180_I2C_ADDRW 0xEE
#define BMP180_I2C_ADDRR 0xEF

/* Registers */
#define	 BMP180_REGISTER_CONTROL 0xF4
#define	 BMP180_REGISTER_RESULT 0xF6
#define BMP180_REGISTER_EEPROM 0xAA

/* Commands */
#define	BMP180_COMMAND_TEMPERATURE 0x2E
#define	BMP180_COMMAND_PRESSURE_0 0x34
#define	BMP180_COMMAND_PRESSURE_1 0x74
#define	BMP180_COMMAND_PRESSURE_2 0xB4
#define	BMP180_COMMAND_PRESSURE_3 0xF4

/* Minimum waiting delay, in milliseconds */
#define BMP180_TEMPERATURE_DELAY	5
#define BMP180_PRESSURE_0_DELAY		5
#define BMP180_PRESSURE_1_DELAY		8
#define BMP180_PRESSURE_2_DELAY		13
#define BMP180_PRESSURE_3_DELAY		25

/* whether to wait or not after temperature/pressure start functions */
#define BMP180_START_DELAY 0

typedef enum {
	BMP180_Res_OK = 0x00,
	BMP180_Res_NoConnect,
	BMP180_Res_LibNotInit
} BMP180_Results;

typedef enum {
	BMP180_Sampling_lowpower = 0x00,
	BMP180_Sampling_standard = 0x01,
	BMP180_Sampling_highres = 0x02,
	BMP180_Sampling_ultrahighres = 0x03
} BMP180_Sampling;

typedef struct {
	float altitude;
	uint32_t pressure;
	float temperature;
	uint16_t delay;
	void (* delay_func)(uint32_t delay_ms);
	BMP180_Sampling sampling;
} BMP180_struct;

/*
 * Check if the BMP180 is connected.
 */
BMP180_Results bmp180_is_connected(BMP180_struct* data);

/*
 * Initialise the BMP180 pressure sensor.
 */
BMP180_Results bmp180_init(BMP180_struct* data);

/*
 * Start the temperature sensor on the BMP180.
 */
BMP180_Results bmp180_start_temperature(BMP180_struct* data);

/*
 * Read the temperature from the BMP180.
 */
BMP180_Results bmp180_read_temperature(BMP180_struct* data);

/*
 * Start the pressure reading on the BMP180.
 */
BMP180_Results bmp180_start_pressure(BMP180_struct* data, BMP180_Sampling sampling);

/*
 * Read the pressure from the BMP180.
 */
BMP180_Results bmp180_read_pressure(BMP180_struct* data);

/*
 * Calculate pressure above sea level in pascals. This is good, if 
 * you read pressure from sensor at known altitude, not altitude
 * provided from sensor. Altitude from sensor is calculated in fact,
 * that pressure above the sea is 101325 Pascals. So, if you know
 * your pressure, and you use calculated altitude, you will not get
 * real pressure above the sea.
 */
uint32_t bmp180_get_pressure_at_sea_level(uint32_t pressure, float altitude);

#endif /* IMU10DOF_BMP180_H_ */
