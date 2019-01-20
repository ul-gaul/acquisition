/*
 * bmp180.c
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */


#include "bmp180.h"


/* Multiple is faster than divide */
#define BMP180_1_16     ((float) 0.0625)
#define BMP180_1_256    ((float) 0.00390625)
#define BMP180_1_2048   ((float) 0.00048828125)
#define BMP180_1_4096   ((float) 0.000244140625)
#define BMP180_1_8192   ((float) 0.0001220703125)
#define BMP180_1_32768  ((float) 0.000030517578125)
#define BMP180_1_65536  ((float) 0.0000152587890625)
#define BMP180_1_101325 ((float) 0.00000986923266726)

/* EEPROM values */
int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6, UT;
/* OK */
int32_t X1, X2, X3, B3, B5, B6, T, p;
uint32_t B4, B7, UP;
uint8_t lib_initialized = 0;


/* BMP180 related functions */

BMP180_Results bmp180_is_connected(BMP180_struct* data) {

	return BMP180_Res_OK;
}

BMP180_Results bmp180_init(BMP180_struct* data) {
	uint8_t raw_data[22];
	uint8_t i = 0;

	// test if BMP180 is connected
	if(!imu10dof_check_i2c_connection(BMP180_I2C_ADDRW)) {
		return BMP180_Res_NoConnect;
	}
	//	get default values from EEPROM, EEPROM starts at 0xAA address, read 22 bytes
	i2c_read_multi(BMP180_I2C_ADDRR, BMP180_REGISTER_EEPROM, raw_data, 22);

	/* Set configuration values */
	AC1 = (int16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	AC2 = (int16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	AC3 = (int16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	AC4 = (uint16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	AC5 = (uint16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	AC6 = (uint16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	B1 = (int16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	B2 = (int16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	MB = (int16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	MC = (int16_t) (raw_data[i] << 8 | raw_data[i + 1]); i += 2;
	MD = (int16_t) (raw_data[i] << 8 | raw_data[i + 1]);

	/* Initialized OK */
	lib_initialized = 1;

	return BMP180_Res_OK;
}
