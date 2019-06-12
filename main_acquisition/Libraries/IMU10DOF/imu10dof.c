/*
 * imu10dof.c
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#include "imu10dof.h"


#define I2C_TIMEOUT 100


static uint32_t i2c_timeout;
//static uint32_t TM_I2C_INT_Clocks[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0

/* I2C related functions */

uint8_t i2c_wait_busy(void) {
	i2c_timeout = I2C_TIMEOUT;
	while(!(IMU10DOF_I2C->SR1 & I2C_SR1_SB)) {
		if(--i2c_timeout == 0) {
			return 1;
		} else {
			// wait 1 ms
			i2c_delay_ms(1);
		}
	}
	return 0;
}
////////////////CHECK HERE, WHAT DOES i2c start, voir page 23 pdf
uint8_t i2c_start(uint8_t address, uint8_t direction, uint8_t ack) {
	// generate i2c start pulse
	IMU10DOF_I2C->CR1 |= I2C_CR1_START;
	i2c_delay_ms(1);
	// wait for i2c to be busy
	if(i2c_wait_busy()) return 1;
	// enable ack if applicable
	if(ack) IMU10DOF_I2C->CR1 |= I2C_CR1_ACK;
	i2c_delay_ms(1);
	// send write / read bit
	i2c_timeout = I2C_TIMEOUT;
	if(direction == I2C_TRANSMITTER_MODE) {
		IMU10DOF_I2C->DR = address & ~I2C_OAR1_ADD0;
		i2c_delay_ms(1);
		while(!(IMU10DOF_I2C->SR1 & I2C_SR1_ADDR)) {
			if(--i2c_timeout == 0) {
				return 1;
			} else {
				i2c_delay_ms(1);
			}
		}
	} else if(direction == I2C_RECEIVER_MODE) {
		IMU10DOF_I2C->DR = address | I2C_OAR1_ADD0;
		i2c_delay_ms(1);
		while(!I2C_CheckEvent(IMU10DOF_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
			if(--i2c_timeout == 0) {
				return 1;
			} else {
				i2c_delay_ms(1);
			}
		}
	}
	// read status register to clear ADDR flag
	IMU10DOF_I2C->SR2;
	return 0;
}

uint8_t i2c_stop(void) {
	// wait till transmitter is empty
	i2c_timeout = I2C_TIMEOUT;
	// tant que le 7 eme bit de SR1 est a zero OU que le deuxieme bit de SR1 est a zero
	while (((!(IMU10DOF_I2C->SR1 & I2C_SR1_TXE)) || (!(IMU10DOF_I2C->SR1 & I2C_SR1_BTF)))) {
		if(--i2c_timeout == 0) {
			return 1;
		} else {
			i2c_delay_ms(1);
		}
	}
	// generate stop
	IMU10DOF_I2C->CR1 |= I2C_CR1_STOP;
	return 0;
}

uint8_t i2c_read(uint8_t address, uint8_t reg) {
	i2c_start(address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	i2c_write_data(reg);
	i2c_stop();
	i2c_start(address, I2C_RECEIVER_MODE, I2C_ACK_DISABLE);
	return L3GD20_i2c_read_nack();
}

uint8_t i2c_read_no_register(uint8_t address) {
	i2c_start(address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	return i2c_read_nack();
}



uint8_t i2c_read_ack(void) {
	uint8_t data;

	/* Enable ACK */
	IMU10DOF_I2C->CR1 |= I2C_CR1_ACK;

	/* Wait till not received */
	i2c_timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(IMU10DOF_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--i2c_timeout == 0x00) {
			return 1;
		} else {
			i2c_delay_ms(1);
		}
	}

	/* Read data */
	data = IMU10DOF_I2C->DR;

	/* Return data */
	return data;
}

uint8_t i2c_read_nack(void) {
	uint8_t data;

	/* Disable ACK */
	IMU10DOF_I2C->CR1 &= ~I2C_CR1_ACK;

	/* Generate stop */
	IMU10DOF_I2C->CR1 |= I2C_CR1_STOP;

	/* Wait till received */
	i2c_timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(IMU10DOF_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--i2c_timeout == 0x00) {
			return 1;
		} else {
			i2c_delay_ms(1);
		}
	}

	/* Read data */
	data = IMU10DOF_I2C->DR;

	/* Return data */
	return data;
}


uint8_t L3GD20_i2c_read_nack(void) {
	uint8_t data;

	/* Disable ACK */
	IMU10DOF_I2C->CR1 &= ~I2C_CR1_ACK;


	/* Wait till received */
	i2c_timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(IMU10DOF_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		if (--i2c_timeout == 0x00) {
			return 1;
		} else {
			i2c_delay_ms(1);
		}
	}

	/* Read data */
	data = IMU10DOF_I2C->DR;

	/* Generate stop */
	IMU10DOF_I2C->CR1 |= I2C_CR1_STOP;
	/* Return data */
	return data;
}

void i2c_write_data(uint8_t data) {
	/* Wait till I2C is not busy anymore */
	i2c_timeout = I2C_TIMEOUT;
	while (!(IMU10DOF_I2C->SR1 & I2C_SR1_TXE) && i2c_timeout) {
		i2c_timeout--;
		i2c_delay_ms(1);
	}

	/* Send I2C data */
	IMU10DOF_I2C->DR = data;
	i2c_delay_ms(1);
}

void i2c_read_multi(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	i2c_start(address, I2C_TRANSMITTER_MODE, I2C_ACK_ENABLE);
	i2c_write_data(reg);
	// insert delay here
	i2c_start(address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	while (count--) {
		if (!count) {
			/* Last byte */
			*data++ = i2c_read_nack();
		} else {
			*data++ = i2c_read_ack();
		}
	}
}

void i2c_read_multi_no_register(uint8_t address, uint8_t* data, uint16_t count) {
	i2c_start(address, I2C_RECEIVER_MODE, I2C_ACK_ENABLE);
	while(count--) {
		if(!count) {
			*data = i2c_read_nack();
		} else {
			*data = i2c_read_ack();
		}
	}
}

void i2c_write(uint8_t address, uint8_t reg, uint8_t data) {
	i2c_start(address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	i2c_write_data(reg);
	i2c_write_data(data);
	i2c_stop();
}

void i2c_write_multi(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
	i2c_start(address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	i2c_write_data(reg);
	while(count--) {
		i2c_write_data(*data++);
	}
	i2c_stop();
}

void i2c_write_no_register(uint8_t address, uint8_t data) {
	i2c_start(address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	i2c_write_data(data);
	i2c_stop();
}

void i2c_write_multi_no_register(uint8_t address, uint8_t* data, uint16_t count) {
	i2c_start(address, I2C_TRANSMITTER_MODE, I2C_ACK_DISABLE);
	while(count--) {
		i2c_write_data(*data++);
	}
	i2c_stop();
}

/* IMU10DOF related functions */

uint8_t imu10dof_check_i2c_connection(uint8_t address) {
	// i2c_start will return 0 if device is connected
	uint8_t connected = 0;
	if(i2c_start(address, I2C_TRANSMITTER_MODE, 1) == 0) connected = 1;
	i2c_stop();
	return connected;
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

uint8_t imu10dof_init(struct BMP180_struct* bmp180_data, void (*delay_func) (uint32_t)) {
	// init the i2c bus
	imu10dof_i2c_init();
	i2c_delay_ms = delay_func;
	// init all the devices
	bmp180_init((BMP180_struct *) bmp180_data);
	/* Init L3GD20 sensor */
	if (L3GD20_Init(L3GD20_Scale_2000) != L3GD20_Result_Ok) {
		/* Sensor error */
		//while (1);
	}
	lsm303dlhc_init();

	return 0;
}

