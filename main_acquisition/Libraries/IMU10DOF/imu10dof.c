/*
 * imu10dof.c
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#include "imu10dof.h"


uint8_t imu10dof_check_i2c_connection(uint8_t address) {

	return 0;
}

void imu10dof_i2c_init(void) {
	I2C_InitTypeDef i2c_init_struct;
	GPIO_InitTypeDef gpio_init_struct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_PinAFConfig(IMU10DOF_I2C_GPIO_PORT, 
			IMU10DOF_I2C_SCL_SOURCE, 
			GPIO_AF_I2C1);
	GPIO_PinAFConfig(IMU10DOF_I2C_GPIO_PORT,
			IMU10DOF_I2C_SDA_SOURCE,
			GPIO_AF_I2C1);

	// configure and init the gpios
	gpio_init_struct.GPIO_Pin = IMU10DOF_I2C_SCL | IMU10DOF_I2C_SDA;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_struct.GPIO_OType = GPIO_OType_OD;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(IMU10DOF_I2C_GPIO_PORT, &gpio_init_struct);

	// configure and init the i2c bus
	i2c_init_struct.I2C_ClockSpeed = IMU10DOF_I2C_SPEED;
	i2c_init_struct.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_init_struct.I2C_Mode = I2C_Mode_I2C;
	i2c_init_struct.I2C_Ack = I2C_Ack_Disable;
	i2c_init_struct.I2C_OwnAddress1 = 0x45;
	i2c_init_struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(IMU10DOF_I2C, &i2c_init_struct);
}

uint8_t imu10dof_init(struct BMP180_struct* bmp180_data) {
	// init the i2c bus
	imu10dof_i2c_init();
	// init all the devices
	bmp180_init((BMP180_struct *) bmp180_data);
	l3gd20_init();
	lsm303dlhc_init();

	return 0;
}

