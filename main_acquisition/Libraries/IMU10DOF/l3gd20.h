/*
 * l3gd20.h
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#ifndef IMU10DOF_L3GD20_H_
#define IMU10DOF_L3GD20_H_


/* slave addresses for I2C operation
 * define SDO when the SDO pin is HIGH
 */
#define SDO 1
#if !SDO
#define L3GD20_ADDRESS_R (0xD5)        // 11010101
#define L3GD20_ADDRESS_W (0xD4)        // 11010100
#else
#define L3GD20_ADDRESS_R (0xD7)        // 11010111
#define L3GD20_ADDRESS_W (0xD6)        // 11010110
#endif

#define L3GD20_ADDRESS (0x6B) // 1101011
#define L3GD20_POLL_TIMEOUT (100) // Maximum number of read attempts

// Sensitivity values from the mechanical characteristics in the datasheet.
#define GYRO_SENSITIVITY_250DPS (0.00875F)
#define GYRO_SENSITIVITY_500DPS (0.0175F)
#define GYRO_SENSITIVITY_2000DPS (0.070F)

/*
 * Registers addess map
 * NAME ADDR // default value (if applicable) type
 */

#define GYRO_REGISTER_WHO_AM_I 0x0F // 11010100 r
#define GYRO_REGISTER_CTRL_REG1 0x20 // 00000111 rw
#define GYRO_REGISTER_CTRL_REG2 0x21 // 00000000 rw
#define GYRO_REGISTER_CTRL_REG3 0x22 // 00000000 rw
#define GYRO_REGISTER_CTRL_REG4 0x23 // 00000000 rw
#define GYRO_REGISTER_CTRL_REG5 0x24 // 00000000 rw
#define GYRO_REGISTER_REFERENCE 0x25 // 00000000 rw
#define GYRO_REGISTER_OUT_TEMP 0x26 // r
#define GYRO_REGISTER_STATUS_REG 0x27 // r
#define GYRO_REGISTER_OUT_X_L 0x28 // r
#define GYRO_REGISTER_OUT_X_H 0x29 // r
#define GYRO_REGISTER_OUT_Y_L 0x2A // r
#define GYRO_REGISTER_OUT_Y_H 0x2B // r
#define GYRO_REGISTER_OUT_Z_L 0x2C // r
#define GYRO_REGISTER_OUT_Z_H 0x2D // r
#define GYRO_REGISTER_FIFO_CTRL_REG 0x2E // 00000000 rw
#define GYRO_REGISTER_FIFO_SRC_REG 0x2F // r
#define GYRO_REGISTER_INT1_CFG 0x30 // 00000000 rw
#define GYRO_REGISTER_INT1_SRC 0x31 // r
#define GYRO_REGISTER_TSH_XH 0x32 // 00000000 rw
#define GYRO_REGISTER_TSH_XL 0x33 // 00000000 rw
#define GYRO_REGISTER_TSH_YH 0x34 // 00000000 rw
#define GYRO_REGISTER_TSH_YL 0x35 // 00000000 rw
#define GYRO_REGISTER_TSH_ZH 0x36 // 00000000 rw
#define GYRO_REGISTER_TSH_ZL 0x37 // 00000000 rw
#define GYRO_REGISTER_INT1_DURATION 0x38 // 00000000   rw

void l3gd20_init(void);

#endif /* IMU10DOF_L3GD20_H_ */
