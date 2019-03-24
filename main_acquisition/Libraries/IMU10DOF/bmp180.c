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
#define BMP180_1_P_SEA ((float) 0.00001) // BMP180_1_P_SEA is equal to 1 over pressure at sea level

/* EEPROM values */
static int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
static uint16_t AC4, AC5, AC6, uncomp_temperature;
/* OK */
static int32_t X1, X2, X3, B3, B5, B6, T, p;
static uint32_t B4, B7, uncomp_pressure;
static uint8_t lib_initialized = 0;


/* BMP180 related functions */

BMP180_Results bmp180_is_connected(BMP180_struct* data) {

	return BMP180_Res_OK;
}

BMP180_Results bmp180_init(BMP180_struct* data) {
	uint8_t raw_data[22];
	uint8_t i = 0;

	// test if BMP180 is connected
	if(imu10dof_check_i2c_connection(BMP180_I2C_ADDRW) == 0) {
		return BMP180_Res_NoConnect;
	}
	//	get default values from EEPROM, EEPROM starts at 0xAA address, read 22 bytes
	i2c_read_multi(BMP180_I2C_ADDRR, BMP180_REGISTER_EEPROM, raw_data, 22);

	/* Set configuration values */
	AC1 = (int16_t) (((int16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	AC2 = (int16_t) (((int16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	AC3 = (int16_t) (((int16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	AC4 = (uint16_t) (((uint16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	AC5 = (uint16_t) (((uint16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	AC6 = (uint16_t) (((uint16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	B1 = (int16_t) (((int16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	B2 = (int16_t) (((int16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	MB = (int16_t) (((int16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	MC = (int16_t) (((int16_t) raw_data[i]) << 8 | raw_data[i + 1]); i += 2;
	MD = (int16_t) (((int16_t) raw_data[i]) << 8 | raw_data[i + 1]);

	/* Initialized OK */
	lib_initialized = 1;

	return BMP180_Res_OK;
}

BMP180_Results bmp180_start_temperature(BMP180_struct* data) {
	// check that library was initialized first
	if(!lib_initialized) return BMP180_Res_LibNotInit;
	// write to device
	i2c_write(BMP180_I2C_ADDRW, BMP180_REGISTER_CONTROL,
		BMP180_COMMAND_TEMPERATURE);
	// set minimum delay
	data->delay = BMP180_TEMPERATURE_DELAY;
	data->delay_func(data->delay);
	return BMP180_Res_OK;
}

BMP180_Results bmp180_read_temperature(BMP180_struct* data) {
	uint8_t raw_data[2];
	// check that library was initialized first
	if(!lib_initialized) return BMP180_Res_LibNotInit;
	// read multi bytes first
	i2c_read_multi(BMP180_I2C_ADDRR, BMP180_REGISTER_RESULT,
		raw_data, 2);
	// read uncompensated temperature
	uint16_t msb = raw_data[0];
	uncomp_temperature = (msb << 8) + (uint16_t) raw_data[1];
	// compensate temperature (get actual value)
	X1 = (uncomp_temperature - AC6) * AC5 * BMP180_1_32768;
	X2 = ((int32_t) MC * 2048) / (X1 + MD);
	B5 = X1 + X2;
	// convert to degrees and store in data struct
//	int32_t tmp;
//	tmp = (B5 + 8) / ((float) 160.0);
	data->temperature = (B5 + 8) / ((float) 160.0);
	return BMP180_Res_OK;
}

BMP180_Results bmp180_start_pressure(BMP180_struct* data, BMP180_Sampling sampling) {
	uint8_t command;
	if(!lib_initialized) return BMP180_Res_LibNotInit;
	// set command and delay according to sampling
	switch(sampling) {
		case BMP180_Sampling_lowpower:
			command = BMP180_COMMAND_PRESSURE_0;
			data->delay = BMP180_PRESSURE_0_DELAY;
			break;
		case BMP180_Sampling_standard:
			command = BMP180_COMMAND_PRESSURE_1;
			data->delay = BMP180_PRESSURE_1_DELAY;
			break;
		case BMP180_Sampling_highres:
			command = BMP180_COMMAND_PRESSURE_2;
			data->delay = BMP180_PRESSURE_2_DELAY;
			break;
		case BMP180_Sampling_ultrahighres:
			command = BMP180_COMMAND_PRESSURE_3;
			data->delay = BMP180_PRESSURE_3_DELAY;
			break;
	}
	// write to device
	i2c_write(BMP180_I2C_ADDRW, BMP180_REGISTER_CONTROL, command);
	// wait for device
	data->delay_func(data->delay);
	// save selected sampling
	data->sampling = sampling;
	return BMP180_Res_OK;
}

BMP180_Results bmp180_read_pressure(BMP180_struct* data) {
	uint8_t raw_data[3];
	if(!lib_initialized) return BMP180_Res_LibNotInit;
	// read multi bytes from i2c
	i2c_read_multi(BMP180_I2C_ADDRR, BMP180_REGISTER_RESULT, raw_data, 3);
	// read uncompensated pressure value

	uncomp_pressure = ((((uint32_t) raw_data[0]) << 16) | ((uint32_t) raw_data[1] << 8) | ((uint32_t) raw_data[2])) >> (8 - (uint8_t) data->sampling);
	// compensated pressure (get actual value)
	B6 = B5 - 4000;
	X1 = (B2 * (B6 * B6 * BMP180_1_4096)) * BMP180_1_2048;
	X2 = AC2 * B6 * BMP180_1_2048;
	X3 = X1 + X2;
	B3 = (((AC1 * 4 + X3) << (uint8_t) data->sampling) + 2) * 0.25;
	X1 = AC3 * B6 * BMP180_1_8192;
	X2 = (B1 * (B6 * B6 * BMP180_1_4096)) * BMP180_1_65536;
	X3 = ((X1 + X2) + 2) * 0.25;
	B4 = AC4 * (uint32_t) (X3 + 32768) * BMP180_1_32768;
	B7 = ((uint32_t) uncomp_pressure - B3) * (50000 >> (uint8_t) data->sampling);
	if (B7 < 0x80000000) {
		p = (B7 * 2) / B4;
	} else {
		p = (B7 / B4) * 2;
	}
	X1 = ((float) p * BMP180_1_256) * ((float) p * BMP180_1_256);
	X1 = (X1 * 3038) * BMP180_1_65536;
	X2 = (-7357 * p) * BMP180_1_65536;
	p = p + (X1 + X2 + 3791) * BMP180_1_16;
	// save pressure
	data->pressure = p;
	// calculate altitude
	data->altitude = 44330.0 * (1.0 - (float) pow(((float) p) * BMP180_1_P_SEA, 0.19029495));
	return BMP180_Res_OK;
}

uint32_t bmp180_get_pressure_at_sea_level(uint32_t pressure, float altitude) {
	return ((float) pressure / ((float) pow(1 - (float) altitude / (float) 44330, 5.255)));
}





