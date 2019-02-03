/*
 * imu10dof.h
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#ifndef IMU10DOF_IMU10DOF_H_
#define IMU10DOF_IMU10DOF_H_

#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"

#include "bmp180.h"
#include "l3gd20.h"
#include "lsm303dlhc.h"


#define IMU10DOF_I2C I2C1
#define IMU10DOF_I2C_GPIO_PORT GPIOB
#define IMU10DOF_I2C_SCL_SOURCE GPIO_PinSource8
#define IMU10DOF_I2C_SDA_SOURCE GPIO_PinSource9
#define IMU10DOF_I2C_SCL GPIO_Pin_8
#define IMU10DOF_I2C_SDA GPIO_Pin_9
#define IMU10DOF_I2C_SPEED 100000

#define IMU10DOF_ERR_NOCONNECT 1000


// forward declaration of structs to prevent circular inclusions
struct BMP180_struct;
struct l3gd20_struct;
struct lsm303_struct;

uint8_t imu10dof_check_i2c_connection(uint8_t address);

void i2c_write_data(uint8_t data);

void i2c_read_multi(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count);

uint8_t i2c_read_nack(void);

void i2c_write(uint8_t address, uint8_t reg, uint8_t data);

uint8_t imu10dof_init(struct BMP180_struct* bmp180_data);

#endif /* IMU10DOF_IMU10DOF_H_ */
