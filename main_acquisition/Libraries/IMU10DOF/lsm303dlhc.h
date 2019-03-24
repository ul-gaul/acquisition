/*
 * lsm303dlhc.h
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#include "imu10dof.h"

#ifndef IMU10DOF_LSM303DLHC_H_
#define IMU10DOF_LSM303DLHC_H_


#define LSM_ACC_ADDRESS_R 0x33
#define LSM_ACC_ADDRESS_W 0x32

#define LSM_MAG_ADDRESS_R 0x3D
#define LSM_MAG_ADDRESS_W 0x3C
/////////////////SECTION ABOUT ACCELEROMETER
#define LSM_CTRL_REG1_A 0x20 // 0x77 to set to = Normal / low-power mode (400 Hz), 0x97 set to = Normal (1.344 kHz) / low-power mode (5.376 kHz)
#define LSM_CTRL_REG2_A 0x21 // 0x00 basically = filter is bypassed, otherwise we could put 0x0F
#define LSM_CTRL_REG3_A 0x22 // 0x00 to disable interrupt fonctionnality, 0xFF to enable them all
#define LSM_CTRL_REG4_A 0x23 // 0x88 +-2g, 0xB8 +-16g enable data integrity + high resolution mode 0x80 to disable high resolution
#define LSM_CTRL_REG5_A 0x24 // 0x80 this will erase memory content on reboot 0x00 could do
#define LSM_CTRL_REG6_A 0x25 // 0x10 to erase memory content 0x00 could do

#define LSM_REFERENCE_A 0x26

#define LSM_STATUS_REG_A 0x27

#define LSM_OUT_X_L_A 0x28
#define LSM_OUT_X_H_A 0x29
#define LSM_OUT_Y_L_A 0x2A
#define LSM_OUT_Y_H_A 0x2B
#define LSM_OUT_Z_L_A 0x2C
#define LSM_OUT_Z_H_A 0x2D

#define LSM_FIFO_CTRL_REG_A 0x2E //0x00 disable fifo
#define LSM_FIFO_SRC_REG_A 0x2F

#define LSM_INT1_CFG_A 0x30 // 0x00 ????? things about interrupts
#define LSM_INT1_SRC_A 0x31 // ???? interrupts things
#define LSM_INT1_THS_A 0x32
#define LSM_INT1_DURATION_A 0x33
#define LSM_INT2_CFG_A 0x34
#define LSM_INT2_SRC_A 0x35
#define LSM_INT2_THS_A 0x36
#define LSM_INT2_DURATION_A 0x37
#define LSM_CLICK_CFG_A 0x38
#define LSM_CLICK_SRC_A 0x39
#define LSM_CLICK_THS_A 0x3A
#define LSM_TIME_LIMIT_A 0x3B
#define LSM_TIME_LATENCY_A 0x3C
#define LSM_TIME_WINDOW_A 0x3D
////////END SECTION ABOUT ACCELEROMETER
////////SECTION ABOUT MAGNETOMETER
#define LSM_CRA_REG_M 0x00 // 0x9C -> Enable temperature, set the rate at which data is written to all three data output registers to 220Hz
#define LSM_CRB_REG_M 0x01 // 0x20 Sensor input field range [Gauss] : +- 1.3; Gain X, Y, and Z [LSB/Gauss] : 1100, Gain Z [LSB/Gauss] : 980
#define LSM_MR_REG_M 0x02 // 0x00 -> Continuous-conversion mode
#define LSM_OUT_X_H_M 0x03
#define LSM_OUT_X_L_M 0x04
#define LSM_OUT_Z_H_M 0x05
#define LSM_OUT_Z_L_M 0x06
#define LSM_OUT_Y_H_M 0x07
#define LSM_OUT_Y_L_M 0x08
#define LSM_SR_REG_M 0x09 // if LSM_SR_REG_M | 0x01 == 1 This bit is when a new set of measurements is available
#define LSM_IRA_REG_M 0x0A // ??
#define LSM_IRB_REG_M 0x0B // ??
#define LSM_IRC_REG_M 0x0C // ??

#define LSM_TEMP_OUT_H_M 0x31 // 8 bits
#define LSM_TEMP_OUT_L_M 0x32 // 0bxxxx---- where "x" is either 0 or 1 and - is garbage

/* Acceleration sensor constants */
#define LSM_ACC_RES 0.06103515625
#define LSM_ACC_G_GAIN_2g 0x0
#define LSM_ACC_G_GAIN_4g 0x1
#define LSM_ACC_G_GAIN_8g 0x2
#define LSM_ACC_G_GAIN_16g 0x3
#define LSM_ACC_G_GAIN LSM_ACC_G_GAIN_16g

#if LSM_ACC_G_GAIN == LSM_ACC_G_GAIN_2g
#define LSM_ACC_BIT_RES 1
#elif LSM_ACC_G_GAIN == LSM_ACC_G_GAIN_4g
#define LSM_ACC_BIT_RES 2
#elif LSM_ACC_G_GAIN == LSM_ACC_G_GAIN_8g
#define LSM_ACC_BIT_RES 4
#elif LSM_ACC_G_GAIN == LSM_ACC_G_GAIN_16g
#define LSM_ACC_BIT_RES 12
#endif



/**
 * @brief
 * 	LSM303DLHC main struct
 * 	Contains XYZ accelerations and XYZ magnetic field measures
 */
typedef struct {
	uint16_t acc_x_uncomp;
	uint16_t acc_y_uncomp;
	uint16_t acc_z_uncomp;
    float acc_x;
    float acc_y;
    float acc_z;
    uint16_t mag_x;
    uint16_t mag_y;
    uint16_t mag_z;
    int16_t temperature;
} LSM303DLHC_struct;

/**
 * @brief
 * 	LSM303DLHC Results enumerations
 */
typedef enum {
    LSM303DLHC_Result_Ok,
    LSM303DLHC_Result_Error
} LSM303DLHC_result;

/**
 * @brief Initializes the sensor
 */
LSM303DLHC_result lsm303dlhc_init(void);

/**
 * @brief Read the acceleration measures
 */
LSM303DLHC_result lsm303dlhc_read_acceleration(LSM303DLHC_struct* data);


/**
 * @brief Read the magnetic field measures
 */
LSM303DLHC_result lsm303dlhc_read_magneticfield(LSM303DLHC_struct* data);

/**
 * @brief Read the temperature
 */
LSM303DLHC_result lsm303dlhc_read_temperature(LSM303DLHC_struct* data);

#endif /* IMU10DOF_LSM303DLHC_H_ */
