/*
 * l3gd20.c
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#include "l3gd20.h"

void l3gd20_init(void) {

}

/* Private variables */
L3GD20_scale L3GD20_INT_Scale;

/* Private functions */
extern void L3GD20_INT_InitPins(void);
extern uint8_t L3GD20_INT_ReadSPI(uint8_t address);
extern void L3GD20_INT_WriteSPI(uint8_t address, uint8_t data);

/* Public */
L3GD20_result L3GD20_Init(L3GD20_scale scale) {
	/* Init CS pin */
	//3GD20_INT_InitPins();
	/* Init SPI */
	//SPI_Init(L3GD20_SPI, L3GD20_SPI_PINSPACK);
	/* Check if sensor is L3GD20 */
	// GYRO_WHO_AM_I = value of the register at the address GYRO_REGISTER_WHO_AM_I
	unsigned char resultData[1];
	i2c_read_multi(L3GD20H_I2c_SLAVE_ADDRESS, GYRO_REGISTER_WHO_AM_I, resultData, 1);
	if (resultData[0] != GYRO_WHO_AM_I) {
		/* Sensor connected is not L3GD20 */
		return L3GD20_Result_Error;
	}

	/* Enable L3GD20 Power bit */
	i2c_write_no_register(GYRO_REGISTER_CTRL_REG1, 0xFF);

	/* Set L3GD20 scale */
	if (scale == L3GD20_Scale_250) {
		i2c_write_no_register(GYRO_REGISTER_CTRL_REG4, 0x00);
	} else if (scale == L3GD20_Scale_500) {
		i2c_write_no_register(GYRO_REGISTER_CTRL_REG4, 0x10);
	} else if (scale == L3GD20_Scale_2000) {
		i2c_write_no_register(GYRO_REGISTER_CTRL_REG4, 0x20);
	}

	/* Save scale */
	L3GD20_INT_Scale = scale;

	/* Set high-pass filter settings */
	i2c_write_no_register(GYRO_REGISTER_CTRL_REG2, 0x00);

	/* Enable high-pass filter */
	i2c_write_no_register(GYRO_REGISTER_CTRL_REG5, 0x10);

	/* Everything OK */
	return L3GD20_Result_Ok;
}

L3GD20_result L3GD20_Read(L3GD20_struct* L3DG20_Data) {
	float temp, s;

	unsigned char resultData[1];
	/* Read X axis */
	i2c_read_multi(L3GD20H_I2c_SLAVE_ADDRESS, GYRO_REGISTER_OUT_X_L, resultData, 1);
	L3DG20_Data->X = resultData[0];
	i2c_read_multi(L3GD20H_I2c_SLAVE_ADDRESS, GYRO_REGISTER_OUT_X_H, resultData, 1);
	L3DG20_Data->X |= resultData[0] << 8;

	/* Read Y axis */
	i2c_read_multi(L3GD20H_I2c_SLAVE_ADDRESS, GYRO_REGISTER_OUT_Y_L, resultData, 1);
	L3DG20_Data->Y = resultData[0];
	i2c_read_multi(L3GD20H_I2c_SLAVE_ADDRESS, GYRO_REGISTER_OUT_Y_H, resultData, 1);
	L3DG20_Data->Y |= resultData[0] << 8;

	/* Read Z axis */
	i2c_read_multi(L3GD20H_I2c_SLAVE_ADDRESS, GYRO_REGISTER_OUT_Z_L, resultData, 1);
	L3DG20_Data->Z = resultData[0];
	i2c_read_multi(L3GD20H_I2c_SLAVE_ADDRESS, GYRO_REGISTER_OUT_Z_H, resultData, 1);
	L3DG20_Data->Z |= resultData[0] << 8;

	/* Set sensitivity scale correction */
	if (L3GD20_INT_Scale == L3GD20_Scale_250) {
		/* Sensitivity at 250 range = 8.75 mdps/digit */
		s = GYRO_SENSITIVITY_250DPS * 0.001;
	} else if (L3GD20_INT_Scale == L3GD20_Scale_500) {
		/* Sensitivity at 500 range = 17.5 mdps/digit */
		s = GYRO_SENSITIVITY_500DPS * 0.001;
	} else {
		/* Sensitivity at 2000 range = 70 mdps/digit */
		s = GYRO_SENSITIVITY_2000DPS * 0.001;
	}

	temp = (float)L3DG20_Data->X * s;
	L3DG20_Data->X = (int16_t) temp;
	temp = (float)L3DG20_Data->Y * s;
	L3DG20_Data->Y = (int16_t) temp;
	temp = (float)L3DG20_Data->Z * s;
	L3DG20_Data->Z = (int16_t) temp;

	/* Return OK */
	return L3GD20_Result_Ok;
}


